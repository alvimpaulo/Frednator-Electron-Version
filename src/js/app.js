class mainImageCanvas {
  constructor (width, height) {
    this.canvas = document.createElement('canvas')
    this.ctx = this.canvas.getContext('2d', { alpha: false })
    this.canvas.width = width
    this.canvas.height = height
    this.idata = this.ctx.createImageData(width, height)
    this.image = new Image()
    // let self = this
    // this.image.onload = function () {
    //   let imgElement = document.getElementById('main-img')
    //   imgElement.getContext('2d')
    //   // .drawImage(this, 0, 0, imgElement.width, imgElement.height)
    //   self.image.src = null
    // }
  }
}

let Readable = require('stream').Readable
const addon = require('../build/Release/module')
const { webFrame } = require('electron')

let cap = addon.cvMatSenderReciever()
let imgStream = new Readable()
imgStream._read = () => {}

function printImg (customCanvas, imgArray) {
  // set our buffer as source
  customCanvas.idata.data.set(imgArray)

  // update canvas with new data
  customCanvas.ctx.putImageData(customCanvas.idata, 0, 0)

  let dataUri = customCanvas.canvas.toDataURL()

  document.getElementById('main-img').src = dataUri

  dataUri = null
}

function logBytes (x) {
  return x[0] + x[1] / (1000.0 * 1000) + ' MB <br>'
}

function getMemory () {
  return Object.entries(process.memoryUsage())
    .map(logBytes)
    .toString()
}

function mainLoop (customCanvas) {
  let t0 = performance.now()
  let m0 = process.memoryUsage()
  let img = addon.cvMatSenderReciever(cap)
  let t1 = performance.now()
  let m1 = process.memoryUsage()
  /* console.log(
    'gerar cvmat da captura = ' +
      Math.round(t1 - t0) +
      ' milissegundos, ' +
      Math.round(m1.heapUsed - m0.heapUsed)
  ) */
  let newimgarray = addon.typedArrayFromCvMat(img)
  let t2 = performance.now()
  let m2 = process.memoryUsage()
  // imgStream.push(newimgarray)
  /* console.log(
    'gerar array = ' +
      Math.round(t2 - t1) +
      ' milissegundos, ' +
      Math.round(m2.heapUsed - m1.heapUsed)
  ) */
  printImg(customCanvas, newimgarray)
  let t3 = performance.now()
  let m3 = process.memoryUsage()
  /* console.log(
    'escrever img = ' +
      Math.round(t3 - t2) +
      ' milissegundos, ' +
      Math.round(m3.heapUsed - m2.heapUsed)
  ) */
  newimgarray = null
  // console.log(imgStream.read())
  document.getElementById('heap-usage').innerHTML = getMemory()
  requestAnimationFrame(timestamp => {
    mainLoop(customCanvas)
  })
}

$(function () {
  $(document).ready(function () {
    // Access the DOM elements here...
    console.log('appjs')
    $('.dropdown-trigger').dropdown()
    let customCanvas = new mainImageCanvas(320, 240)

    requestAnimationFrame(timestamp => {
      mainLoop(customCanvas)
    })
    setInterval(() => {
      webFrame.clearCache()
    }, 10000)
  })
})
