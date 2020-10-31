(function (CanvasKit) {
  CanvasKit._extraInitializations = CanvasKit._extraInitializations || [];
  CanvasKit._extraInitializations.push(function () {
    CanvasKit.PPEngineDrawPath = function (canvas) {
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
      var svg = "M34.63 100.63C44.38 88.57 59.87 82.86 74.88 81.2"
      "C97.4 78.5 120.27 83.25 140.87 92.37L127.12 127.14C113.55 121.16 99.04 115.9 83.98 116.56"
      "C78.86 116.75 72.88 118.54 70.71 123.69C68.62 128.43 71.52 133.68 75.58 136.27"
      "C91.49 146.66 110.67 151.38 125.46 163.6C132.35 169.11 137.33 176.9 139.36 185.49"
      "C142.55 199.14 140.94 214.31 133.13 226.17C126.23 236.96 114.82 244.16 102.75 247.89"
      "C87.95 252.51 72.16 252.21 56.88 250.78C45.54 249.72 34.64 246.05 24.32 241.36"
      "L24.25 201.1C38.23 208.15 53.37 213.15 68.98 214.75C75.42 215.25 82.17 215.63 88.31 213.27"
      "C92.84 211.53 96.4 206.93 95.86 201.93C95.64 196.77 91.1 193.38 87.03 190.99"
      "C71.96 182.67 54.94 177.66 41.5 166.57C33.19 159.73 27.51 149.8 26.1 139.11"
      "C24.09 125.88 25.91 111.25 34.63 100.63";
      var surface = this.drawSVG(imageInfo, pixelPtr, width * 4, svg);
      var skcanvas = surface.getCanvas()

      var pixels = new Uint8ClampedArray(CanvasKit.HEAPU8.buffer, pixelPtr, pixelLen);
      var imageData = new ImageData(pixels, width, height);

      // flush
      canvas.getContext('2d').putImageData(imageData, 0, 0);
    }
  });
})(Module); // When this file is loaded in, the high level object is "Module";
