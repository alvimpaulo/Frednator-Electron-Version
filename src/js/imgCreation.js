const videoUtil = require("../../build/Release/videoUtil");

let cap;

function openCap(path) {
  let capIndex = parseInt(path);
  if (!isNaN(capIndex)) {
    return videoUtil.videoOpener(capIndex);
  } else {
    return videoUtil.videoOpener(path);
  }
}

function closeCap(capToClose) {
  let closingResponse = videoUtil.videoCloser(capToClose);
  if (closingResponse == false) {
    console.log("couldn't close cap");
  } else if (closingResponse == true) {
    console.log("closed cap");
    cap = false;
  }
}

function createMyImageBitmap(captureParam = 0) {
  return new Promise((resolve, reject) => {
    let imgBitmap;
    if (!cap) cap = openCap(captureParam); // if capture isn't already open
    if (typeof cap === "boolean") {
      // if it was returned false
      reject("capture is boolean");
      return;
    }
    // test for time and heap
    // let t0 = performance.now()
    // let m0 = process.memoryUsage()
    let img = videoUtil.imgFromVideo(cap);
    if (typeof img === "boolean") {
      cap = false;
      reject("Img is boolean");
      return;
    }

    img = videoUtil.yellowDetectorRun(img);

    // test for time and heap
    /* let t1 = performance.now()
    let m1 = process.memoryUsage()
    console.log(
      'gerar cvmat da captura = ' +
        Math.round(t1 - t0) +
        ' milissegundos, ' +
        Math.round(m1.heapUsed - m0.heapUsed)
    ) */
    let newimgarray = videoUtil.typedArrayFromCvMat(img);

    // test for time and heap
    /* let t2 = performance.now()
    let m2 = process.memoryUsage()
    console.log(
      'gerar array = ' +
        Math.round(t2 - t1) +
        ' milissegundos, ' +
        Math.round(m2.heapUsed - m1.heapUsed)
    )
    */
    let imageDataFromArray = new ImageData(
      new Uint8ClampedArray(newimgarray),
      640
    );

    /*  let t3 = performance.now()
    let m3 = process.memoryUsage()
    console.log(
      'escrever img = ' +
        Math.round(t3 - t0) +
        ' milissegundos, ' +
        Math.round(m3.heapUsed - m0.heapUsed)
    ) */

    createImageBitmap(imageDataFromArray).then(imgBitmapCreated => {
      resolve(imgBitmapCreated);
    });
    newimgarray = null;
    // console.log(imgStream.read())
  });
}
