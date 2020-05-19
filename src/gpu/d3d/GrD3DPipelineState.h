/*
 * Copyright 2020 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrD3DPipelineState_DEFINED
#define GrD3DPipelineState_DEFINED

#include "include/core/SkRefCnt.h"
#include "include/gpu/GrTypes.h"
#include "include/gpu/d3d/GrD3DTypes.h"
#include "src/gpu/GrManagedResource.h"
#include "src/gpu/d3d/GrD3DPipelineStateDataManager.h"
#include "src/gpu/glsl/GrGLSLProgramBuilder.h"

class GrD3DGpu;
class GrD3DRootSignature;
class GrProgramInfo;

class GrD3DPipelineState : public GrManagedResource {
public:
    using UniformInfoArray = GrD3DPipelineStateDataManager::UniformInfoArray;

    GrD3DPipelineState(gr_cp<ID3D12PipelineState> pipelineState,
                       const GrGLSLBuiltinUniformHandles& builtinUniformHandles,
                       const UniformInfoArray& uniforms,
                       uint32_t uniformSize,
                       std::unique_ptr<GrGLSLPrimitiveProcessor> geometryProcessor,
                       std::unique_ptr<GrGLSLXferProcessor> xferProcessor,
                       std::unique_ptr<std::unique_ptr<GrGLSLFragmentProcessor>[]> fragProcessors,
                       int fragmentProcessorCnt);

#ifdef SK_TRACE_MANAGED_RESOURCES
    /** Output a human-readable dump of this resource's information
    */
    void dumpInfo() const override {
        SkDebugf("GrD3DPipelineState: %p (%d refs)\n", fPipelineState.get(), this->getRefCnt());
    }
#endif

    // This will be called right before this class is destroyed and there is no reason to explicitly
    // release the fPipelineState cause the gr_cp will handle that in the dtor.
    void freeGPUData() const override {}

    ID3D12PipelineState* pipelineState() const { return fPipelineState.get(); }

    void setData(const GrRenderTarget* renderTarget, const GrProgramInfo& programInfo);

private:
    /**
     * We use the RT's size and origin to adjust from Skia device space to d3d normalized device
     * space and to make device space positions have the correct origin for processors that require
     * them.
     */
    struct RenderTargetState {
        SkISize         fRenderTargetSize;
        GrSurfaceOrigin fRenderTargetOrigin;

        RenderTargetState() { this->invalidate(); }
        void invalidate() {
            fRenderTargetSize.fWidth = -1;
            fRenderTargetSize.fHeight = -1;
            fRenderTargetOrigin = (GrSurfaceOrigin)-1;
        }

        /**
        * Gets a float4 that adjusts the position from Skia device coords to D3D's normalized device
        * coords. Assuming the transformed position, pos, is a homogeneous float3, the vec, v, is
        * applied as such:
        * pos.x = dot(v.xy, pos.xz)
        * pos.y = dot(v.zw, pos.yz)
        */
        void getRTAdjustmentVec(float* destVec) {
            destVec[0] = 2.f / fRenderTargetSize.fWidth;
            destVec[1] = -1.f;
            if (kBottomLeft_GrSurfaceOrigin == fRenderTargetOrigin) {
                destVec[2] = -2.f / fRenderTargetSize.fHeight;
                destVec[3] = 1.f;
            } else {
                destVec[2] = 2.f / fRenderTargetSize.fHeight;
                destVec[3] = -1.f;
            }
        }
    };

    // Helper for setData() that sets the view matrix and loads the render target height uniform
    void setRenderTargetState(const GrRenderTarget*, GrSurfaceOrigin);

    gr_cp<ID3D12PipelineState> fPipelineState;

    // Tracks the current render target uniforms stored in the vertex buffer.
    RenderTargetState fRenderTargetState;
    GrGLSLBuiltinUniformHandles fBuiltinUniformHandles;

    // Processors in the GrD3DPipelineState
    std::unique_ptr<GrGLSLPrimitiveProcessor> fGeometryProcessor;
    std::unique_ptr<GrGLSLXferProcessor> fXferProcessor;
    std::unique_ptr<std::unique_ptr<GrGLSLFragmentProcessor>[]> fFragmentProcessors;
    int fFragmentProcessorCnt;

    GrD3DPipelineStateDataManager fDataManager;
};

#endif