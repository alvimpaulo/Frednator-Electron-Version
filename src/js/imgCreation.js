const cvMatUtil = require('../../build/Release/cvMatUtil')
const videoUtil = require('../../build/Release/videoUtil')

class mainImageCanvas {
  constructor (width, height) {
    this.canvas = new OffscreenCanvas(width, height)
    this.ctx = this.canvas.getContext('2d', { alpha: false })
    this.idata = this.ctx.createImageData(width, height)
  }
}

let cap
let pastCap

function openCap () {
  return videoUtil.videoOpener()
}

function generateBlob (customCanvas, imgArray) {
  console.log('inicio do generate blob = ' + performance.now())
  // set our buffer as source
  customCanvas.idata.data.set(imgArray)
  console.log('setado o img array = ' + performance.now())

  // update canvas with new data
  customCanvas.ctx.putImageData(customCanvas.idata, 0, 0)
  console.log('put image data = ' + performance.now())

  let dataBlob = customCanvas.canvas.convertToBlob()
  console.log('convert to blob' + performance.now())

  return dataBlob
}

function createImageBlob () {
  let customCanvas = new mainImageCanvas(320, 240)
  if (!cap) cap = openCap()
  // test for time and heap
  let t0 = performance.now()
  let m0 = process.memoryUsage()
  let img = videoUtil.imgFromVideo(cap)

  // test for time and heap
  let t1 = performance.now()
  let m1 = process.memoryUsage()
  console.log(
    'gerar cvmat da captura = ' +
      Math.round(t1 - t0) +
      ' milissegundos, ' +
      Math.round(m1.heapUsed - m0.heapUsed)
  )
  let newimgarray = cvMatUtil.typedArrayFromCvMat(img)

  // test for time and heap
  let t2 = performance.now()
  let m2 = process.memoryUsage()
  // imgStream.push(newimgarray)
  console.log(
    'gerar array = ' +
      Math.round(t2 - t1) +
      ' milissegundos, ' +
      Math.round(m2.heapUsed - m1.heapUsed)
  )
  let dataBlob = generateBlob(customCanvas, newimgarray)

  let t3 = performance.now()
  let m3 = process.memoryUsage()
  console.log(
    'escrever img = ' +
      Math.round(t3 - t2) +
      ' milissegundos, ' +
      Math.round(m3.heapUsed - m2.heapUsed)
  )
  newimgarray = null
  pastCap = cap
  // console.log(imgStream.read())
  return dataBlob
}
