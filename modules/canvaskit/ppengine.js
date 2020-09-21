(function (CanvasKit) {
  CanvasKit._extraInitializations = CanvasKit._extraInitializations || [];
  CanvasKit._extraInitializations.push(function () {
    CanvasKit.PPEngineDrawSVG = function (canvas) {
      var width = canvas.width;
      var height = canvas.height;
      var imageInfo = {
        'width':  width,
        'height': height,
        'colorType': CanvasKit.ColorType.RGBA_8888,
        'alphaType': CanvasKit.AlphaType.Unpremul,
        'colorSpace': CanvasKit.SkColorSpace.SRGB,
      }

      var pixelLen = width * height * 4; // it's 8888, so 4 bytes per pixel
      var pixelPtr = CanvasKit._malloc(pixelLen);
      var surface = this.drawPath(imageInfo, pixelPtr, width * 4);
      var skcanvas = surface.getCanvas()

      var pixels = new Uint8ClampedArray(CanvasKit.HEAPU8.buffer, pixelPtr, pixelLen);
      var imageData = new ImageData(pixels, width, height);

      // flush
      canvas.getContext('2d').putImageData(imageData, 0, 0);
    }
  });
})(Module); // When this file is loaded in, the high level object is "Module";
