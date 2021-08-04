//################################################
//Webpage Code  jst impport header file and pass that value to id 
//###############################################
String webpageCode = R"***(
<!DOCTYPE html>
<head>
  <title>Lcs Controls ESP32 Web Server </title>
<html>
<!---------------------------------css----------------------------->
<style>
  body { background-color: rgb(3, 68, 132)}
  h4{font-family: arail; text-align; color: white;}
  .card
  {
    max-width:  480px;
    min-height: 150px;
    background: rgba(255, 0, 0, 0.521);
    padding: 20px;
    font-weight: bold;
    font:25px calibri;
    text-align: center;
    box-sizing: border-box;
    color: blue;
    margin: 20px;
    box-shadow: 0px 2px 15px rgba(0,0,0,0.75);
  }
  </style>
  <!-----------------------------html------------------>
  <body>
    <div class="card">
    <h1><span style="background-color-white">Lcs controls Esp32 Web Server</span></h1>
    <h2>
      <Hall Sernsor Current Data: <span style="color:yellow" id="hall">0</span>
    <h2>
   </div>
  <!------------------------JavaScript-------------------------------->
  <script>
      setInterval(function()
      {
        GetHallSensorReading();
      },2000);
      //function----------------------------

      function GetHallSensorReading()
      {
        var HallValueRequest=new XMLHttpRequest();
        HallValueRequest.onreadystatechange = function()
        {
          if(this.readystate == 4 && this.status == 200)
          {
            document.getElementById("hall").innerHtml=this.responseText;
          }
        };

        HallValueRequest.open("GET", readhall", true);
        HallValueRequest.send();
      }
  </script>
      
   
