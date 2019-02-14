importScripts('imgCreation.js')

onmessage = function (e) {
  console.log('I recieved ' + e.data)
  createImageBlob(e.data).then(function (blob) {
    postMessage(blob)
  })
  // console.log(imgDataUri)
}
