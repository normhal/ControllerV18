# 21 November 2024 Update
- Each update released will now have the date the changes were made included in the sketch name
  -   Today's version was completed on 20th November:-)
- Naming of LittleFS data files changed to have .txt extension
- locos.txt now has no "slot" number - the last variable in each row has been removed
- Support for defining specific Loco IDs for each of the 4 "Throttles" each in a specified Slot now functional
- Saving of updated Data NOT YET IMPLEMENTED - coming soon:-)

# 19 December 2024 Update
- Further to the initial release which was only tested on the ESP32, ESP8266 and RaspBerry Pi Pico are now supported
- Major update is the inclusion of LittleFS file updates. With the exception of WiFi Credentials, any updates to Locos, functions Accessories, and contents of the Four Loco "Throttles" (A, B, C, and D) need to be "Committed" by pressing the "Save" Button on the Menu BEFORE powering off the Controller.

# ControllerV18
Repository for Everything related to Version 1.8x of the Nextion Based DCCEX Controller

# How to Install

- Download ALL the files in this repository:-
  -  Arduino Sketch
  -  If you're using Arduino 2.2.1 or later: 
      - On your computer, go to the following path: C:\Users\<username>\.arduinoIDE\. Create a new folder called plugins if you haven’t already.
      - copy the .vsix plugin file included in this repository into the newly created plugins folder
  - If you're using Ardiono IDE 1.8.19:-
    -  ESP8266 variants need a specific plugin (Included in the repository)
    -  navigate to your Arduino sketch folder (typically /users/username/Arduino) and create a "tools" folder if it doesn't already exist
    -  in the "tools" folder create an folder "esp32fs"
    -  in the "esp32fs" folder create a folder called "tool"
    -  copy the files "esp32fs.jar" into the "tool" folder
    -  For ESP8266 repeat the above steps replacing every instance of "esp32fs" with "esp8266Littlefs"
  -  You're now ready to load files into an ESP32, Pico or ESP8266 LittleFS flash space.
-  Also install the included "NextionUpload" Library as with previous Controller versions
-  With the ESP32 boards Version 2.0.14 or later you should be ready to compile
-  For ESP32 boards, select the ESP32 Dev Module as the processor in the tools dropdown, and then make sure you select the partition scheme "Minimal SPIFFS....". Others might work, but I have been using this scheme.
-  For ESP8266 boards, select the appropriate board you're using and make sure to select a Flash Size which includes OTA - 3MB FS and 512kB OTA works fine.
-  For Pico 1 boards, select a sketch size of 1536kB and FS of 512kB. Others might work, but it's vital to have FS space allocated
-  For Pico 2 boards, select any sketch size and FS size of at least 512kB. Others might work, but it's vital to have FS space allocated
-  When the sketch has compiled, you'll need to upload the LittleFS data. In the Arduino Sketch folder you'll find a new "data" folder which contains 5 .csv files. Example data has been prepared, but feel free to modify if you feel the need.
-  With Arduino IDE 2.2.x, with the ESP32 connected, press "CTRL, SHIFT and P" and the upload procedure starts. It'll take a few seconds.
-  With Adruino IDE 1.8.19, you should find the "Sketch Data Upload" in the "Tools" section. Select it and then choose LittleFS. The data files should be uploaded.

# New Feature

- The WiFi Enable button now has a new use:-)
  - When disabled/Off, when pressing the "Scan" button the list of LittleFS WiFi profiles will be displayed
  - When enabled/On, the "Scan" button does a scan for local APs
