importScripts("imgCreation.js");

onmessage = function(e) {
  let t0 = performance.now();
  // console.log('I recieved ' + e.data)
  if (e.data == "stop") {
    closeCap(cap);
    postMessage("capture closed");
    console.log("capture closed");
  } else if (e.data[0] === "yellowDetector") {
    if (e.data[1] === "getDebugImageSize") {
      postMessage([
        "yellowDetector",
        "getDebugImageSize",
        yellowDetectorUtil.getDebugImageSize(yellowDetectorUtil.detector)
      ]);
    } else if (e.data[1] === "getParameters") {
      postMessage([
        "yellowDetector",
        "getParameters",
        yellowDetectorUtil.getParameters(yellowDetectorUtil.detector)
      ]);
    } else if (e.data[1] === "setParameters") {
      let keysToSet = e.data[2];
      let valuesToSet = e.data[3];
      let paramObj = {};
      if (typeof keysToSet === "string") {
        // single parameter
        paramObj[keysToSet] = valuesToSet;
      } else if (typeof keysToSet === "object") {
        //multiple parameters
        for (const [index, key] of keysToSet.entries()) {
          paramObj[key] = valuesToSet[index]; //create the object for the parameters to be set
        }
      }
      postMessage([
        "yellowDetector",
        "setParameters",
        yellowDetectorUtil.setParameters(yellowDetectorUtil.detector, paramObj)
      ]);
    }
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
