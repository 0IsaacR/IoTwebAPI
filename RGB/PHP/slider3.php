<?php require ("./readTxt.php"); ?>  


<!DOCTYPE html>
<html>
    <head>

    <!--<title>IoT Course</title>-->
    <script>
/***************************    Put Request Function      ***************************************/
        async function updateResource(newData) {
            const url = `./put.php`; // Replace with my API location
            try {
                const response = await fetch(url, {
                    method: 'PUT',
                    headers: {
                        'Content-Type': 'application/json'
                    },
                    body: JSON.stringify(newData)
                });
        
                if (!response.ok) {
                    throw new Error(`HTTP error! status: ${response.status}`);
                }
        
                const result = await response.json();
                console.log(result);
                // document.getElementById('result').innerHTML = result.htmlText;
                 
            } catch (error) {
                console.error('Error updating resource:', error);
            }
        }
 /***************************    Timer Function       ***************************************/
        let myVariable = 0;
        let timeoutId;
        const STABILIZATION_DELAY = 1000; // Milliseconds to wait for stability
        
        function executeAfterStableValue(id) {
            console.log(`${myName} value has stabilized to: ${myVariable}`);
            // Your main logic goes here
            // {\"Node1\":{\"RGB1\":\"10\"}}
            // updatedData= `{"Node1":{"${myName}":"${myVariable}"}}`;
                
            if (id == 1) {
                updatedData= `{"Node1":{"RGB1":"${myVariable}", "RGB2":"${rgb2ValSpan.innerHTML}","RGB3":"${rgb3ValSpan.innerHTML}"} }`;
            }   
            if (id == 2) {
                updatedData= `{"Node1":{"RGB1":"${rgb1ValSpan.innerHTML}", "RGB2":"${myVariable}", "RGB3":"${rgb3ValSpan.innerHTML}"} }`;
            }
            if (id == 3) {
                updatedData= `{"Node1":{"RGB1":"${rgb1ValSpan.innerHTML}", "RGB2":"${rgb2ValSpan.innerHTML}", "RGB3":"${myVariable}"} }`;
            }
                
            updateResource(updatedData);     // save data to txt after stabilized
        }
        
/***************************    Execute Timer Funciton with new variable present   *******************/        
        function updateVariable(name, newValue, id) {
            myVariable = newValue;
            myName = name;
            console.log(`${myName} updated to ${myVariable}, waiting for stability...`);
        
            // Clear the previous timeout if the value changes again
            if (timeoutId) {
                clearTimeout(timeoutId);
            }
        
            // Set a new timeout
            timeoutId = setTimeout(() => {
                executeAfterStableValue(id);
            }, STABILIZATION_DELAY);
        }
    </script>
    
    <body>  
        <h1 style="color:green;"> IoT Course</h1>
        <h3 style="color:green;"> Isaac R</h3>
        <style>
              .slider-container {
                width: 80%;
                margin: 20px auto;
              }
              .slider {
                width: 100%;
                height: 15px;
                color: white;
                outline: none;
                opacity: 0.7;
                transition: opacity .2s;
              }
            </style>
            </head>
            <body>
            
            <script>
                var jsString = "<?php echo ($content); ?>";
                var txtData = JSON.parse(jsString);
                var initVal = txtData['Node1']['RGB1'];
                var initVal2 = txtData['Node1']['RGB2'];
                var initVal3 = txtData['Node1']['RGB3'];
                console.log(initVal3);

            </script>
            
            <div class=slider-container >
              <label for="rgb1">RGB 1:</label>
              <input type="range" id="rgb1" name="rgb1" min="0" max="255" value=0 class=slider style="background:red;">
              <p>Current value: <span id="rgb1Val"> </span></p>
            </div>
            <div class=slider-container >
              <label for="rgb2">RGB 2:</label>
              <input type="range" id="rgb2" name="rgb2" min="0" max="255" value=0 class=slider style="background:red;">
              <p>Current value: <span id="rgb2Val"> </span></p>
            </div>
            <div class=slider-container >
              <label for="rgb3">RGB 3:</label>
              <input type="range" id="rgb3" name="rgb3" min="0" max="255" value=0 class=slider style="background:red;">
              <p>Current value: <span id="rgb3Val"> </span></p>
            </div>
            
            <script>
              const myInput = document.getElementById("rgb1");
              myInput.value = initVal;
              
              const rgb1Slider = document.getElementById('rgb1');
              const rgb1ValSpan = document.getElementById('rgb1Val');
              document.getElementById('rgb1Val').textContent = initVal;
              
              rgb1Slider.oninput = function() {
                rgb1ValSpan.innerHTML = this.value;
                updateVariable('RGB1', rgb1ValSpan.innerHTML, 1);
              };
            </script>
            <script>
              const myInput2 = document.getElementById("rgb2");
              myInput2.value = initVal2;
              
              const rgb2Slider = document.getElementById('rgb2');
              const rgb2ValSpan = document.getElementById('rgb2Val');
              document.getElementById('rgb2Val').textContent = initVal2;
              
              rgb2Slider.oninput = function() {
                rgb2ValSpan.innerHTML = this.value;
                updateVariable('RGB2', rgb2ValSpan.innerHTML, 2);
              };
            </script>
            <script>
              const myInput3 = document.getElementById("rgb3");
              myInput3.value = initVal3;
              
              const rgb3Slider = document.getElementById('rgb3');
              const rgb3ValSpan = document.getElementById('rgb3Val');
              document.getElementById('rgb3Val').textContent = initVal3;
              
              rgb3Slider.oninput = function() {
                rgb3ValSpan.innerHTML = this.value;
                updateVariable('RGB3', rgb3ValSpan.innerHTML, 3);
              };
            </script>
    

        <p>Check the file: <a href="https://isaacr.org/api_assignment/rgbControl/status.txt" target="_blank">status.txt</a></p>

    </body>
</html>
