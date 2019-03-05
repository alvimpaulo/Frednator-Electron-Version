let Readable = require("stream").Readable;

const { webFrame } = require("electron");

let imgCreationWorker = new Worker("./js/imgCreationWorker.js");
imgCreationWorker.onmessage = function(e) {
  // console.log(e)
  // let t0 = performance.now()
  // console.log('recebi msg do worker ' + t0)

  if (
    e.data == "capture is boolean" ||
    e.data == "capture closed" ||
    e.data == "Img is boolean"
  ) {
    // couldn't get image from worker
    document
      .getElementById("video-canvas")
      .getContext("2d")
      .clearRect(0, 0, 640, 480);
    $("#capture-param").prop("disabled", false);
    $("#capture-param-btn").toggleClass("red");
    $("#capture-param-btn").text("start");
    return false;
  }

  // document.getElementById('main-img').src = URL.createObjectURL(e.data)
  document
    .getElementById("video-canvas")
    .getContext("2d")
    .drawImage(e.data, 0, 0);
  e.data.close();
  let t2 = performance.now();
  // console.log('mudei a src da img ' + t2)

  requestAnimationFrame(timestamp => {
    // let t3 = performance.now()
    // console.log('requisitei um novo frame ' + t3)

    document.getElementById("heap-usage").innerHTML = getMemory();
    // let t4 = performance.now()
    // console.log('catei o uso da memoria ' + t4)

    imgCreationWorker.postMessage([
      "animation frame at " + timestamp,
      document.getElementById("capture-param").value
    ]);
    // let t5 = performance.now()
    // console.log('mandei msg nova pro worker ' + t5)
  });
};

function logBytes(x) {
  return x[0] + x[1] / (1000.0 * 1000) + " MB <br>";
}

function getMemory() {
  return Object.entries(process.memoryUsage())
    .map(logBytes)
    .toString();
}

function videoTriedToBeStartedOrStopped(e) {
  if ($("#capture-param-btn").text() == "stop") {
    imgCreationWorker.postMessage("stop");
  } else {
    $("#capture-param").prop("disabled", true);
    $("#capture-param-btn").toggleClass("red");
    $("#capture-param-btn").text("stop");

    imgCreationWorker.postMessage([
      "Sent from " + e.target.id,
      document.getElementById("capture-param").value
    ]);
  }
}

$(function() {
  $(document).ready(function() {
    // Access the DOM elements here...
    console.log("appjs");
    M.AutoInit();

    // -----------------bindings---------------------------
    $("#capture-param").on("keyup", function(e) {
      if (e.keyCode == 13) {
        videoTriedToBeStartedOrStopped(e);
      }
    });
    $("#capture-param-btn").on("click", function(e) {
      videoTriedToBeStartedOrStopped(e);
    });
    // -------------------------------------------------------

    /* requestAnimationFrame(timestamp => {
      imgCreationWorker.postMessage('animation frame at ' + timestamp)
    }) */
  });
});
