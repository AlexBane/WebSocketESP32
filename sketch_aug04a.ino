#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with network credentials
const char* ssid = "TestSoftAP";
const char* password = "Password123";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

//adding timing for delay 
unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

int value;

//to read the sensor value
void getSensorReadings(){
  value = hallRead();
}

String processor(const String& var){
  getSensorReadings();
  //Serial.println(var);
  if(var == "HALL_SENSOR"){
    return String(value);
  }
}

// Initialize WiFi
void initWiFi() {
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server established");
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta http-equiv="refresh" content="1">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
  body { background-color: rgb(255, 255, 255)}
  h4{font-family: arail; text-align; color: white;}
  .card
  {
    max-width:  480px;
    min-height: 150px;
    background: rgba(137, 196, 244, 1);
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
</head>
<body>
  <div class="topnav">
    <h1>WEB SERVER (SSE)</h1>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card">
        <p><i style="color:#059e8a;"></i>HALL SENSOR READING:</p><p><span class="reading"><span id="hall">%HALL_SENSOR
        %</span></span></p>
      </div>
    </div>
  </div>
<script>
 setInterval(function()
 {
  getSensorReadings()
 },200);

 function GetHallSensorReading() {
        var HallValueRequest=new XMLHttpRequest();
        HallValueRequest.onreadystatechange = function()
        {
          if(this.readystate == 4 && this.status == 200)
          {
            document.getElementById("hall").innerHtml= e.data;
          }
        };

      }
</script>
</body>
</html>)rawliteral";

void setup() {
  Serial.begin(115200);
  initWiFi();

  // Handle Web Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 60000);
  });
  server.addHandler(&events);
  server.begin();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    getSensorReadings();
    Serial.printf("Hall Sensor = %d \n", value);
    Serial.println();

    // Send Events to the Web Server with the Sensor Readings
    events.send("ping",NULL,millis());
    events.send(String(value).c_str(),"Hall Sensor reading",millis());
      lastTime = millis();
  }
}
