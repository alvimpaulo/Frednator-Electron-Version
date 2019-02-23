const videoUtil = require("../../build/Debug/videoUtil");

let cap;
let pastCap;

function openCap(path) {
  let capIndex = parseInt(path);
  if (!isNaN(capIndex)) {
    return videoUtil.videoOpener(capIndex);
  } else {
    return videoUtil.videoOpener(path);
  }
}
cap = openCap("../blobteste2.avi");
let img = videoUtil.imgFromVideo(cap);
while (cap && img) {
  img = videoUtil.imgFromVideo(cap);
  console.log(cap);
  console.log(img);
  // let newimgarray = videoUtil.typedArrayFromCvMat(img)
}
