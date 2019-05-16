importScripts("imgCreation.js");

/**
 * Function to return, through postMessage, debugImage size from an util.
 *
 * @param {*} util Detector util object.
 * @param {*} detectorName String containing lowercaseDetector.
 */
function getDebugImageSizeUtil(util, detectorName) {
  postMessage([
    detectorName,
    "getDebugImageSize",
    util.getDebugImageSize(util.detector)
  ]);
}

/**
 * Function to return, through postMessage, parameters from an util.
 *
 * @param {*} util Detector util object.
 * @param {*} detectorName String containing lowercaseDetector.
 */
function getParametersUtil(util, detectorName) {
  postMessage([
    detectorName,
    "getParameters",
    util.getParameters(util.detector)
  ]);
}

/**
 * Function to set detector parameters.
 *
 * @param {*} e Event.
 * @param {*} util Detector Util object.
 * @param {*} detectorName String containing lowercaseDetector.
 */
function setParametersUtil(e, util, detectorName) {
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
    detectorName,
    "setParameters",
    util.setParameters(util.detector, paramObj)
  ]);
}

onmessage = function(e) {
  //Function called when the worker receive a message.

  if (e.data == "stop") {
    closeCap(cap);
    postMessage("capture closed");
    console.log("capture closed");
  }
  //! Start of code used and generated by an external program, be careful when changing it

  //$start imgCreationWorker.js:onmessage$

  //$start imgCreationWorker.js:onmessage:yellowDetector$
  else if (e.data[0] === "yellowDetector") {
    if (e.data[1] === "getDebugImageSize") {
      getDebugImageSizeUtil(yellowDetectorUtil, "yellowDetector");
    } else if (e.data[1] === "getParameters") {
      getParametersUtil(yellowDetectorUtil, "yellowDetector");
    } else if (e.data[1] === "setParameters") {
      setParametersUtil(e, yellowDetectorUtil, "yellowDetector");
    }
  }
  //$end imgCreationWorker.js:onmessage:yellowDetector$

  //$start imgCreationWorker.js:onmessage:lineDetector$
  else if (e.data[0] === "lineDetector") {
    if (e.data[1] === "getDebugImageSize") {
      getDebugImageSizeUtil(lineDetectorUtil, "lineDetector");
    } else if (e.data[1] === "getParameters") {
      getParametersUtil(lineDetectorUtil, "lineDetector");
    } else if (e.data[1] === "setParameters") {
      setParametersUtil(e, lineDetectorUtil, "lineDetector");
    }
  }
  //$end imgCreationWorker.js:onmessage:lineDetector$

  //$start imgCreationWorker.js:onmessage:fieldDetector2$
  else if (e.data[0] === "fieldDetector2") {
    if (e.data[1] === "getDebugImageSize") {
      getDebugImageSizeUtil(fieldDetector2Util, "fieldDetector2");
    } else if (e.data[1] === "getParameters") {
      getParametersUtil(fieldDetector2Util, "fieldDetector2");
    } else if (e.data[1] === "setParameters") {
      setParametersUtil(e, fieldDetector2Util, "fieldDetector2");
    }
  }
  //$end imgCreationWorker.js:onmessage:fieldDetector2$

  //$end imgCreationWorker.js:onmessage$

  //! End of code used and generated by an external program, be careful when changing it
  else if (
    !(
      (e.data[0].startsWith("animation frame") ||
        e.data[0].startsWith("detectorStarted")) &&
      (typeof cap == "boolean" && cap == false)
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
