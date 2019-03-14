importScripts("imgCreation.js");

onmessage = function(e) {
  let t0 = performance.now();
  // console.log('I recieved ' + e.data)
  if (e.data == "stop") {
    closeCap(cap);
    postMessage("capture closed");
    console.log("capture closed");
  } else if (e.data == "getYellowDetectorDebugImagesSize") {
    postMessage([
      "YellowDetectorDebugImagesSize",
      yellowDetectorUtil.getDebugImageSize(yellowDetectorUtil.detector)
    ]);
  } else if (e.data == "getYellowDetectorParameters") {
    postMessage([
      "YellowDetectorParameters",
      yellowDetectorUtil.getParameters(yellowDetectorUtil.detector)
    ]);
  } else if (
    !(
      (e.data[0].startsWith("animation frame") ||
        e.data[0].startsWith("detectorStarted")) &&
      typeof cap == "boolean"
    )
  ) {
    //if cap is false and it have been requested by an animation frame, get out
    createMyImageBitmap(e.data[1], e.data[2], e.data[3]).then(
      function(imgBitmap) {
        postMessage(imgBitmap, [imgBitmap]);
      },
      function(rejectedInfo) {
        postMessage(rejectedInfo);
        console.log("createMyImageBitmap rejected");
      }
    );
  }
};
