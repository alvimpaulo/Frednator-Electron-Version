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
  } else if (e.data[0] === "YellowDetectorDebugImagesSize") {
    let radioHtml = "";

    if (e.data[1] == 0) {
      //if debugImages is empty
      radioHtml = String.raw`
      <label>
        <input name="yellowDetectorDebugImageIndex" type="radio" />
        <span>0</span>
      </label>`;
    } else {
      for (let index = 0; index < e.data[1]; index++) {
        radioHtml += String.raw` <label>
        <input name="yellowDetectorDebugImageIndex" type="radio" />
        <span>${index}</span>
        </label> `;
      }
    }
    $(
      "#function-selector-div>ul>li.active>.collapsible-body>div>.debug-image-index-div"
    ).html(radioHtml);
    return;
  } else if (e.data[0] === "YellowDetectorParameters") {
    let parametersHtml = "";

    for (let parameter in e.data[1]) {
      let parameterType = typeof e.data[1][parameter];
      if (parameterType === "boolean") {
        parametersHtml += String.raw`<div class="col s12">
        <p>
          <label for="${parameter}">
            <input
              type="checkbox"
              name="${parameter}"
              id="${parameter}"
            />
            <span>${parameter}</span>
          </label>
        </p>
      </div>`;
      } else if (parameterType === "number") {
        parametersHtml += String.raw`
        <div class="input-field col s12">
                      <input value="${
                        e.data[1][parameter]
                      }" id="${parameter}" type="text" />
                      <label for="${parameter}">${parameter}</label>
                    </div>`;
      }
    }
    $("#function-selector-div>ul>li.active form").html(parametersHtml);
    M.updateTextFields();
    return;
  }

  document
    .getElementById("video-canvas")
    .getContext("2d")
    .drawImage(e.data, 0, 0);
  e.data.close();
  let t2 = performance.now();
  // console.log('mudei a src da img ' + t2)

  requestAnimationFrame(timestamp => {
    //get next frame

    let className = "";
    // let t3 = performance.now()
    // console.log('requisitei um novo frame ' + t3)

    document.getElementById("heap-usage").innerHTML = getMemory();
    // let t4 = performance.now()
    // console.log('catei o uso da memoria ' + t4)

    imgCreationWorker.postMessage([
      "animation frame at " + timestamp,
      document.getElementById("capture-param").value,
      getSelectedIndex(),
      $("li.active>div.collapsible-header").text()
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
  let className = "";
  if ($("#capture-param-btn").text() == "stop") {
    imgCreationWorker.postMessage("stop");
  } else {
    $("#capture-param").prop("disabled", true);
    $("#capture-param-btn").toggleClass("red");
    $("#capture-param-btn").text("stop");
    imgCreationWorker.postMessage([
      "Sent from " + e.target.id,
      document.getElementById("capture-param").value,
      getSelectedIndex(),
      $("li.active>div.collapsible-header").text()
    ]);
    if (
      (className = $("#function-selector-div>ul>li.active")
        .children()
        .first()
        .text())
    ) {
      if (className.includes("Yellow Detector")) {
        imgCreationWorker.postMessage("getYellowDetectorDebugImagesSize");
      }
    }
  }
}

function detectorClicked(event) {
  if (
    event.currentTarget.parentElement.classList["toString"]().includes("active")
  ) {
    event.currentTarget.nextElementSibling.innerHTML = String.raw`<div class="row">
    <div class="col s12 debug-image-index-div">
      <label>
        <input name="group1" type="radio" checked />
        <span>0</span>
      </label>
    </div>
    <div class="col s12 parameters-div">
      <form></form>
    </div>
  </div>`;
  }
  let className = event.target.innerText;
  if (className.includes("Yellow Detector")) {
    imgCreationWorker.postMessage([
      "detectorStarted",
      document.getElementById("capture-param").value,
      $("li.active>.collapsible-body>div>div>ul>li>a.active").text(),
      $("li.active>div.collapsible-header").text()
    ]);
    imgCreationWorker.postMessage("getYellowDetectorDebugImagesSize");
    imgCreationWorker.postMessage("getYellowDetectorParameters");
  }
  console.log(event.target);
}

function getSelectedIndex() {
  let indexSelected = "";
  $(
    "#function-selector-div>ul>li.active>.collapsible-body>div>div>label>input"
  ).each(function(index, element) {
    if (element.checked)
      indexSelected = element.nextSibling.nextSibling.textContent;
  });
  return indexSelected;
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

    $(".function-selection>ul>li>.collapsible-header").on("click", e => {
      detectorClicked(e);
    });
    // -------------------------------------------------------

    /* requestAnimationFrame(timestamp => {
      imgCreationWorker.postMessage('animation frame at ' + timestamp)
    }) */
  });
});
