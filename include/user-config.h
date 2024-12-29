/*
 *   ESP32 Oven Controller
 *   Include all config files and Setup Function declarations
 */
#ifndef USER_SETUP_H
#define USER_SETUP_H

#include "mqtt-ota-config.h"

// Define required user libraries here
// Don't forget to add them into platformio.ini
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Declare user setup, main and custom functions
extern void user_setup();
extern void user_loop();
// Webserver functions
extern String processor(const String &var);
extern void notFound(AsyncWebServerRequest *request);

// MQTT Topic Tree prepended to all topics
// ATTN: Must end with "/"!
// will be set in mqtt-ota-config.h if not defined here
#define TOPTREE "HB7/Indoor/WZ/SUMO/"

// MQTT Topic for oven state
#define OvenState_topic TOPTREE "State"
#define Uptime_topic TOPTREE "Uptime"

// GPIO used for SSR
#define SSR_GPIO 12

// Oven State flag
extern bool OvenState; // desired oven state (set through webui or MQTT topic)
extern bool SSRState;  // Actual state of the SSR (true equals SSR short)

// webserver stuff
extern const char *PARAM_MESSAGE;
extern const char *index_html;

#endif // USER_SETUP_H