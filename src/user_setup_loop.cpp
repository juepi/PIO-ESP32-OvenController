/*
 * ESP32 Oven Controller
 * ======================
 * User specific function "user_loop" called in main loop
 * User specific funtion "user_setup" called in setup loop
 * Add stuff you want to run here
 */
#include "setup.h"

bool OvenState = false;
bool SSRState = false;

const char *index_html PROGMEM = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Oven Controller v%TEMPL_VERSION%</title>
</head>
<body>
    <h3>System Status</h3>
<p>
<b>WiFi RSSI:</b> %TEMPL_WIFI_RSSI% dBm<br>
<b>MQTT:</b> %TEMPL_MQTT_STAT%<br>
<b>Uptime:</b> %TEMPL_UPTIME%<br>
<form action="/get" method="get">
    <input type="submit" name="action" value="Reboot" />
</form>
</p>
    <h3>Oven Status</h3>
<p>
<b>Status:</b> %TEMPL_OVEN_STAT%
</p>
    <h3>Control Oven</h3>
<p>
<form action="/get" method="get">
    <input type="submit" name="action" value="on" />&nbsp;&nbsp;&nbsp;
    <input type="submit" name="action" value="off" />
</form>
</p> 
</body>
</html>
)";
const char *PARAM_MESSAGE PROGMEM = "action";

AsyncWebServer server(80);

/*
 * User Setup Loop
 * ========================================================================
 */
void user_setup()
{
  // AsyncWebserver initialization
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", index_html, processor); });
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String message;
    if (request->hasParam(PARAM_MESSAGE)) {
      message = request->getParam(PARAM_MESSAGE)->value();
      if (message == "on") {
        //Webclient requested to turn oven on
        mqttClt.publish(OvenState_topic, String("on").c_str(), true);
        OvenState = true;
      } else if (message == "off")
      {
        //Webclient requested to turn oven off
        mqttClt.publish(OvenState_topic, String("off").c_str(), true);
        OvenState = false;
      } else if (message == "Reboot")
      {
        if (UptimeSeconds > 10)
        {
          ESP.restart();
        }
      }
    }
    request->redirect("/"); });
  server.onNotFound(notFound);
  server.begin();

  // Setup GPIO for SSR
  pinMode(SSR_GPIO, OUTPUT);
  digitalWrite(SSR_GPIO, LOW);
}

/*
 * User Main Loop
 * ========================================================================
 */
void user_loop()
{
  // publish uptime to broker every minute
  static uint32_t LastUptimePub = 0;

  if ((JustBooted) || (UptimeSeconds - LastUptimePub) >= 60)
  {
    mqttClt.publish(Uptime_topic, String(UptimeSeconds).c_str(), false);
    LastUptimePub = UptimeSeconds;
  }
  // Check if oven state needs to be changed
  if (SSRState != OvenState)
  {
    digitalWrite(SSR_GPIO, OvenState);
    SSRState = OvenState;
  }
}

//
// 404
//
void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

//
// Website processor to replace templates of our HTML page with useful data
//
String processor(const String &var)
{
  if (var == "TEMPL_VERSION")
    return (String(FIRMWARE_VERSION));
  if (var == "TEMPL_WIFI_RSSI")
    return (String(WiFi.RSSI()));
  if (var == "TEMPL_UPTIME")
    return (String(UptimeSeconds));
  if (var == "TEMPL_MQTT_STAT")
  {
    if (mqttClt.connected())
    {
      return F("<font color=\"green\">connected</font>");
    }
    else
    {
      return F("<font color=\"red\">not connected!</font>");
    }
  }
  if (var == "TEMPL_OVEN_STAT")
  {
    if (OvenState)
    {
      return F("<font color=\"green\"><b>On</b></font>");
    }
    else
    {
      return F("<font color=\"red\"><b>Off</b></font>");
    }
  }
  return String();
}
