const cvMatUtil = require('../../build/Release/cvMatUtil')
const videoUtil = require('../../build/Release/videoUtil')

let cap
let pastCap

function openCap () {
  return videoUtil.videoOpener()
}

function createMyImageBitmap () {
  return new Promise((resolve, reject) => {
    let imgBitmap
    if (!cap) cap = openCap()
    // test for time and heap
    let t0 = performance.now()
    let m0 = process.memoryUsage()
    let img = videoUtil.imgFromVideo(cap)

    // test for time and heap
    let t1 = performance.now()
    let m1 = process.memoryUsage()
    /* console.log(
      'gerar cvmat da captura = ' +
        Math.round(t1 - t0) +
        ' milissegundos, ' +
        Math.round(m1.heapUsed - m0.heapUsed)
    ) */
    let newimgarray = cvMatUtil.typedArrayFromCvMat(img)

    // test for time and heap
    let t2 = performance.now()
    let m2 = process.memoryUsage()
    // imgStream.push(newimgarray)
    /* console.log(
      'gerar array = ' +
        Math.round(t2 - t1) +
        ' milissegundos, ' +
        Math.round(m2.heapUsed - m1.heapUsed)
    ) */
    let imageDataFromArray = new ImageData(
      new Uint8ClampedArray(newimgarray),
      640
    )

    let t3 = performance.now()
    let m3 = process.memoryUsage()
    /* console.log(
      'escrever img = ' +
        Math.round(t3 - t0) +
        ' milissegundos, ' +
        Math.round(m3.heapUsed - m0.heapUsed)
    ) */

    createImageBitmap(imageDataFromArray).then(imgBitmapCreated => {
      resolve(imgBitmapCreated)
    })
    newimgarray = null
    pastCap = cap
    // console.log(imgStream.read())
  })
}
