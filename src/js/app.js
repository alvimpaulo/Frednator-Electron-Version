let Readable = require('stream').Readable

const { webFrame } = require('electron')

let imgCreationWorker = new Worker('./js/imgCreationWorker.js')
imgCreationWorker.onmessage = function (e) {
  console.log('recebi msg do worker ' + performance.now())

  let fr = new FileReader()
  fr.onload = function (e) {
    console.log('carreguei o arquivo ' + performance.now())

    document.getElementById('main-img').src = e.target.result
    console.log('mudei a src da img ' + performance.now())

    requestAnimationFrame(timestamp => {
      console.log('requisitei um novo frame ' + performance.now())

      document.getElementById('heap-usage').innerHTML = getMemory()
      console.log('catei o uso da memoria ' + performance.now())

      imgCreationWorker.postMessage('animation frame at ' + timestamp)
      console.log('mandei msg nova pro worker ' + performance.now())
    })
  }
  fr.readAsDataURL(e.data)
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
