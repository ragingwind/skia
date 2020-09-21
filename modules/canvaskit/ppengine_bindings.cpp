#include <stdio.h>
#include <iostream>
#include <string>

#include <emscripten.h>
#include <emscripten/bind.h>

#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "include/core/SkPath.h"

using namespace emscripten;

struct SimpleImageInfo {
    int width;
    int height;
    SkColorType colorType;
    SkAlphaType alphaType;
    sk_sp<SkColorSpace> colorSpace;
};

SkImageInfo toSkImageInfo2(const SimpleImageInfo& sii) {
    return SkImageInfo::Make(sii.width, sii.height, sii.colorType, sii.alphaType, sii.colorSpace);
}


sk_sp<SkSurface> drawPath(const SimpleImageInfo ii, uintptr_t pPtr, size_t rowBytes) {
  uint8_t* pixels = reinterpret_cast<uint8_t*>(pPtr);
  
  SkImageInfo imageInfo = toSkImageInfo2(ii);
  sk_sp<SkSurface> surface = SkSurface::MakeRasterDirect(imageInfo, pixels, rowBytes, nullptr);
  SkCanvas* canvas = surface->getCanvas();

  // creating a path to be drawn
  SkPath path;
  path.moveTo(10.0f, 10.0f);
  path.lineTo(100.0f, 0.0f);
  path.lineTo(100.0f, 100.0f);
  path.lineTo(0.0f, 100.0f);
  path.lineTo(50.0f, 50.0f);
  path.close();

  // creating a paint to draw with
  SkPaint p;
  p.setAntiAlias(true);

  // clear out which may be was drawn before and draw the path
  canvas->clear(SK_ColorWHITE);
  canvas->drawPath(path, p);

  return surface;
}


EMSCRIPTEN_BINDINGS(PPEngine) {
  function("drawPath", &drawPath, allow_raw_pointers());
}
