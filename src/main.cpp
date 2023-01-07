#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <main.h>

void handleRoot();
void handleUpdate();
WebServer server(80);
Servo servos[5];
void setup() {
  Serial.begin(115200);
  servos[0].attach(s1);
  servos[1].attach(s2);
  servos[2].attach(s3);
  servos[3].attach(s4);
  servos[4].attach(s5);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.print("Server IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.on("/update", handleUpdate);
  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String content = "<h1>Control Your Robot</h1><form action='/update' method='POST'>Servo 1: <input type='number' name='servo1' id='servo1' min='0' max='180' /><br />Servo 2: <input type='number' name='servo2' id='servo2' min='0' max='180' /><br />Servo 3: <input type='number' name='servo3' id='servo3' min='0' max='180' /><br />Servo 4: <input type='number' name='servo4' id='servo4' min='0' max='180' /><br />Servo 5: <input type='number' name='servo5' id='servo5' min='0' max='180' /><br /><input type='submit' value='Update' /></form><script>setInterval(function() {var xhttp = new XMLHttpRequest();xhttp.onreadystatechange = function() {if (this.readyState == 4 && this.status == 200) {var servoAngles = this.responseText.split(',');document.getElementById('servo1').value = servoAngles[0].split('=')[1];document.getElementById('servo2').value = servoAngles[1].split('=')[1];document.getElementById('servo3').value = servoAngles[2].split('=')[1];document.getElementById('servo4').value = servoAngles[3].split('=')[1];document.getElementById('servo5').value = servoAngles[4].split('=')[1];}};var formData = new FormData();formData.append('servo1', document.getElementById('servo1').value);formData.append('servo2', document.getElementById('servo2').value);formData.append('servo3', document.getElementById('servo3').value);formData.append('servo4', document.getElementById('servo4').value);xhttp.open('POST', '/update', true);xhttp.send(formData);}, 500);</script>";
  server.send(200, "text/html", content);
}

void handleUpdate() {
  if (server.method() == HTTP_POST) {
    servos[0].write(server.arg("servo1").toInt());
    servos[1].write(server.arg("servo2").toInt());
    servos[2].write(server.arg("servo3").toInt());
    servos[3].write(server.arg("servo4").toInt());
    servos[4].write(server.arg("servo5").toInt());
  }
}
