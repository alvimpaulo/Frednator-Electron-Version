/* Patterns info:
lowercaseDetector  = detectorDetector
spacedDetector = Detector Detector
 */

//$start imgCreation.js:requires:#lowercaseDetector#$
const #lowercaseDetector#Util = require("../../build/Release/#lowercaseDetector#Util");
//$end imgCreation.js:requires:#lowercaseDetector#$

 //$start imgCreation.js:createMyImageBitmap:#lowercaseDetector#$
 if (classSelected === "#spacedDetector#") {
    runDetector(#lowercaseDetector#Util);
  }
  //$end imgCreation.js:createMyImageBitmap:#lowercaseDetector#$