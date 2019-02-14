importScripts('imgCreation.js')

onmessage = function (e) {
  let t0 = performance.now()
  console.log('I recieved ' + e.data)
  createMyImageBitmap().then(function (imgBitmap) {
    postMessage(imgBitmap, [imgBitmap])
  })
  // console.log(imgDataUri)
}
