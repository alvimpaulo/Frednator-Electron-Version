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
  } else if (e.data[0] === "yellowDetector") {
    if (e.data[1] === "getDebugImageSize") {
      let radioHtml = "";

      if (e.data[2] == 0) {
        //if debugImages is empty
        radioHtml = String.raw`
      <label>
        <input name="yellowDetectorDebugImageIndex" type="radio" />
        <span>0</span>
      </label>`;
      } else {
        for (let index = 0; index < e.data[2]; index++) {
          radioHtml += String.raw`
            <label>
              <input name="yellowDetectorDebugImageIndex" type="radio" />
              <span>${index}</span>
            </label> `;
        }
      }
      $("#function-selector-div>ul>li.active .debug-image-index-div").html(
        radioHtml
      );
      return;
    } else if (e.data[1] === "getParameters") {
      let parametersHtml = "";

      for (let parameter in e.data[2]) {
        //create boolean checkboxes
        let parameterType = typeof e.data[2][parameter];
        if (parameterType === "boolean") {
          parametersHtml += String.raw`
          <div>
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
        <div class="input-field">
          <input value="${
            e.data[2][parameter]
          }" id="${parameter}" type="text" />
          <label for="${parameter}">${parameter}</label>
        </div>`;
        }
      }
      $("#function-selector-div>ul>li.active form").html(parametersHtml);
      M.updateTextFields();
      for (let parameter in e.data[2]) {
        let parameterType = typeof e.data[2][parameter];
        if (parameterType === "boolean") {
          // atualize boolean checkboxes values
          $("#" + parameter).prop("checked", e.data[2][parameter]);
          //change detector parameter
          $("#" + parameter).on("change", checkBoxEvent => {
            imgCreationWorker.postMessage([
              "yellowDetector",
              "setParameters",
              parameter,
              checkBoxEvent.currentTarget.checked
            ]);
          });
        }
        if (parameterType === "number") {
          $("#" + parameter).on("keyup", keyupEvent => {
            if (keyupEvent.key === "Enter") {
              imgCreationWorker.postMessage([
                "yellowDetector",
                "setParameters",
                parameter,
                keyupEvent.target.value
              ]);
            }
          });
        }
      }
    } else if (e.data[1] === "setParameters") {
      for (parameter in e.data[2]) {
        //atualize values
        let parameterType = typeof e.data[2][parameter];

        if (parameterType === "number") {
          $("#" + parameter).val(e.data[2][parameter]);
        } else if (parameterType === "boolean") {
          $("#" + parameter).prop("checked", e.data[2][parameter]);
        }
      }
    }
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
        imgCreationWorker.postMessage(["yellowDetector", "getDebugImageSize"]);
      }
    }
  }
}

function detectorClicked(event) {
  if (
    event.currentTarget.parentElement.classList["toString"]().includes("active")
  ) {
    event.currentTarget.nextElementSibling.innerHTML = String.raw`
    <div class="collapsible-body-grid">
      <div class="debug-image-index-div">
        <label>
          <input name="group1" type="radio" checked />
          <span>0</span>
        </label>
      </div>
      <div class="parameters-div">
        <form></form>
      </div>
    </div>`;
  }
  let className = event.target.innerText;
  if (className.includes("Yellow Detector")) {
    imgCreationWorker.postMessage([
      "detectorStarted",
      document.getElementById("capture-param").value,
      getSelectedIndex(),
      "Yellow Detector"
    ]);
    imgCreationWorker.postMessage(["yellowDetector", "getDebugImageSize"]);
    imgCreationWorker.postMessage(["yellowDetector", "getParameters"]);
  }
}

function getSelectedIndex() {
  let indexSelected = "";
  $(".debug-image-index-div input").each(function(index, element) {
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
    //bind pressing of enter inside the capture parameter text input
    $("#capture-param").on("keyup", function(e) {
      if (e.keyCode == 13) {
        videoTriedToBeStartedOrStopped(e);
      }
    });

    //bind video start/stop button
    $("#capture-param-btn").on("click", function(e) {
      videoTriedToBeStartedOrStopped(e);
    });

    //bind detector selection
    $(".function-selection>ul>li>.collapsible-header").on("click", e => {
      detectorClicked(e);
    });

    //collapsible size once opened
    let detectorSelectionCollapsibleheight = $("#function-selector-div>ul")
      .first()
      .height();
    $("#function-selector-div>.collapsible").prop(
      "M_Collapsible"
    ).options.onOpenEnd = li => {
      li.lastElementChild.style["height"] =
        parseInt(
          document.documentElement.clientHeight -
            detectorSelectionCollapsibleheight -
            parseInt(window.getComputedStyle(li).marginTop) -
            parseInt(window.getComputedStyle(li).marginBottom)
        ).toString() + "px";
      li.lastElementChild.style["overflow-y"] = "auto";
    };
    // -------------------------------------------------------

    /* requestAnimationFrame(timestamp => {
      imgCreationWorker.postMessage('animation frame at ' + timestamp)
    }) */
  });
});
