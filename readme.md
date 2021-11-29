# NonVisual Smart Terrarium (NVST)
## Monitor and Control Your Pet Reptile's Synthetic Environment With Ease
Currently, this project is part of a dream indefinitely on hold, partly due to my being in a sharehouse with housemates who are understandably uncomfortable with the notion of a pet snake in their house; and it requires more research on my own part anyway.

What I would like is for this repository to host the code for any sensors and apps use for this project.  The idea is inspired by (and will probably be partly, directly based upon) the following three [Arduino](https://www.arduino.cc/) projects:
* [Smart Terrarium](https://create.arduino.cc/projecthub/ryanjgill2/smart-terrarium-b6fa04)
* [Habitat Hub: An Alexa and Arduino Smart Home for terrariums.](https://create.arduino.cc/projecthub/jfforever70/habitat-hub-an-alexa-and-arduino-smart-home-for-terrariums-8c3f1a)
* [Vivarium Monitor and Controller](https://create.arduino.cc/projecthub/cfar/vivarium-monitor-and-controller-dba892)

One of the main economically and logistically-driven differences is that I would like to combine sensor and Wi-fi connectivity using the [ESP32](https://www.espressif.com/en/products/socs/esp32) microcontroller.  The ESP32 is less expensive than some other Arduino options; though not open-source, apparently there are known technical issues with the [Arduino Yún](https://www.arduino.cc/en/Main/ArduinoBoardYun/) bridge and the project should remain too primitive to require the use of any make and model of [Raspberry Pi](https://www.raspberrypi.org/).