# Introduction 
A simple oven controller that can switch an oven with a potential free trigger input on and off using an ESP32 and a solid state relay. I have used a Panasonic AQY212EH SSR for this task.  
It is based on my [PIO-ESP32-Template](https://github.com/juepi/PIO-ESP32-Template), please consult the readme of the template on the prerequisites (mainly a local MQTT broker) and how to compile the project.

## Triggering the oven
The oven can be triggered by a simple HTTP frontend or by setting the configured MQTT topic to either `on` or `off`.  
Note that triggering the oven through the webUI will also publish the change to the configured MQTT topic.

### MQTT Topic
In the `user-config.h` you can configure your topic tree (prepended to all used topics for OTA updating and also user-topics) and also the desired name of the oven state topic (see `OvenState_topic`).

### GPIO
The GPIO used to trigger your SSR can be configured in the `user-config.h` file by modifying the `SSR_GPIO` define.

## Change History

### v1.0.0
- initial commit

### v1.0.1
- Bugfix: forgot to subscribe to `OvenState_topic`
- optimized some template settings and removed unneccessary stuff