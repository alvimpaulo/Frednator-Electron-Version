/* Patterns info:
lowercaseDetector  = detectorDetector
 */

//$start imgCreationWorker.js:onmessage:#lowercaseDetector#$
else if (e.data[0] === "#lowercaseDetector#") {
    if (e.data[1] === "getDebugImageSize") {
      getDebugImageSizeUtil(#lowercaseDetector#Util, "#lowercaseDetector#");
    } else if (e.data[1] === "getParameters") {
      getParametersUtil(#lowercaseDetector#Util, "#lowercaseDetector#");
    } else if (e.data[1] === "setParameters") {
      setParametersUtil(e, #lowercaseDetector#Util, "#lowercaseDetector#");
    }
  }
  //$end imgCreationWorker.js:onmessage:#lowercaseDetector#$