const videoUtil = require("../../build/Release/videoUtil");

//$start imgCreation.js:requires$
const yellowDetectorUtil = require("../../build/Release/yellowDetectorUtil");
//$end imgCreation.js:requires$

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

function createMyImageBitmap(
  captureParam = 0,
  debugImageVectorIndex = 0,
  classSelected
) {
  return new Promise((resolve, reject) => {
    let imgBitmap;
    if (!cap) cap = openCap(captureParam); // if capture isn't already open
    if (typeof cap === "boolean") {
      // if it was returned false
      reject("capture is boolean");
      return;
    }
    // test for time and heap
    let t0 = performance.now();
    let m0 = process.memoryUsage();
    let img = videoUtil.imgFromVideo(cap);
    if (typeof img === "boolean") {
      cap = false;
      reject("Img is boolean");
      return;
    }

    function runDetector(util) {
      img = util.run(
        img,
        util.detector,
        util.perceptionData,
        parseInt(debugImageVectorIndex)
      );
    }

    //! Start of code used and generated by an external program, be careful when changing it

    //$start imgCreation.js:createMyImageBitmap$

    //$start imgCreation.js:createMyImageBitmap:yellowDetector$
    if (classSelected === "Yellow Detector") {
      runDetector(yellowDetectorUtil);
    }
    //$end imgCreation.js:createMyImageBitmap:yellowDetector$

    //$start imgCreation.js:createMyImageBitmap:lineDetector$
    if (classSelected === "Line Detector") {
      runDetector(lineDetectorUtil);
    }
    //$end imgCreation.js:createMyImageBitmap:lineDetector$

    //$end imgCreation.js:createMyImageBitmap$

    //! End of code used and generated by an external program, be careful when changing it
    // test for time and heap
    // let t1 = performance.now();
    // let m1 = process.memoryUsage();
    /* console.log(
      'gerar cvmat da captura = ' +
        Math.round(t1 - t0) +
        ' milissegundos, ' +
        Math.round(m1.heapUsed - m0.heapUsed)
    ) */
    // console.log("rss used = " + (m1.rss - m0.rss));
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
