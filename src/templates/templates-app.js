/* Patterns info:
lowercaseDetector  = detectorDetector
uppercaseDetector = DetectorDetector
spacedDetector = Detector Detector
 */

//$start app.js:receiveMessage:#lowercaseDetector#$
else if (e.data[0] === "#lowercaseDetector#") {
    if (e.data[1] === "getDebugImageSize") {
      getDebugImageSize(e, "#lowercaseDetector#");
      return;
    } else if (e.data[1] === "getParameters") {
      getParameters(e, "#lowercaseDetector#");
    } else if (e.data[1] === "setParameters") {
      setParameters(e);
    }
    return;
  }
  //$end app.js:receiveMessage:#lowercaseDetector#$

  //$start app.js:videoTriedToBeStartedOrStopped:#lowercaseDetector#$
  if (className.includes("#spacedDetector#")) {
    imgCreationWorker.postMessage(["#lowercaseDetector#", "getDebugImageSize"]);
  }
  //$end app.js:videoTriedToBeStartedOrStopped:#lowercaseDetector#$

  //$start app.js:detectorClicked:#lowercaseDetector#$
  if (className.includes("#spacedDetector#")) {
    imgCreationWorker.postMessage([
      "detectorStarted",
      document.getElementById("capture-param").value,
      getSelectedIndex(),
      "#spacedDetector#"
    ]);
    imgCreationWorker.postMessage(["#lowercaseDetector#", "getDebugImageSize"]);
    imgCreationWorker.postMessage(["#lowercaseDetector#", "getParameters"]);
  }
  //$end app.js:detectorClicked:#lowercaseDetector#$