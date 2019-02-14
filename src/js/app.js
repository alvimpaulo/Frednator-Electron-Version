let Readable = require('stream').Readable

const { webFrame } = require('electron')

class mainImageCanvas {
  constructor (width, height) {
    this.canvas = new OffscreenCanvas(width, height)
    this.ctx = this.canvas.getContext('2d', { alpha: false })
    this.idata = this.ctx.createImageData(width, height)
  }
}

let imgCreationWorker = new Worker('./js/imgCreationWorker.js')
imgCreationWorker.onmessage = function (e) {
  console.log(e)
  let t0 = performance.now()
  // console.log('recebi msg do worker ' + t0)

  // document.getElementById('main-img').src = URL.createObjectURL(e.data)
  document
    .getElementById('canvas-ex')
    .getContext('2d')
    .drawImage(e.data, 0, 0)
  e.data.close()
  let t2 = performance.now()
  // console.log('mudei a src da img ' + t2)

  requestAnimationFrame(timestamp => {
    let t3 = performance.now()
    // console.log('requisitei um novo frame ' + t3)

    document.getElementById('heap-usage').innerHTML = getMemory()
    let t4 = performance.now()
    // console.log('catei o uso da memoria ' + t4)

    imgCreationWorker.postMessage('animation frame at ' + timestamp)
    let t5 = performance.now()
    // console.log('mandei msg nova pro worker ' + t5)
  })
}

function logBytes (x) {
  return x[0] + x[1] / (1000.0 * 1000) + ' MB <br>'
}

function getMemory () {
  return Object.entries(process.memoryUsage())
    .map(logBytes)
    .toString()
}

$(function () {
  $(document).ready(function () {
    // Access the DOM elements here...
    console.log('appjs')
    $('.dropdown-trigger').dropdown()

    requestAnimationFrame(timestamp => {
      imgCreationWorker.postMessage('animation frame at ' + timestamp)
    })
  })
})
