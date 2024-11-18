# ControllerV18
Repository for Everything related to Version 1.8x of the Nextion Based DCCEX Controller

# How to Install

- Download ALL the files in this repository:-
  -  Arduino Sketch
  -  If you're using Arduino 2.2.1 or later: 
      - On your computer, go to the following path: C:\Users\<username>\.arduinoIDE\. Create a new folder called plugins if you haven’t already.
      - copy the .vsix plugin file included in this repository into the newly created plugins folder
  - If you're using Ardiono IDE 1.8.19:-
    -  navigate to your Arduino sketch folder (typically /users/username/Arduino) and create a "tools" folder if it doesn't already exist
    -  in the "tools" folder create an folder "esp32fs"
    -  in the "esp32fs" folder create a folder called "tool"
    -  copy the files "esp32fs.jar" into the "tool" folder
  -  You're now ready to load files into an ESP32 LittleFS flash space.
-  open the Arduino IDE and install the supplied "CSV_Parser" library. NOTE: This is a MODIFIED version of the standard "CSV_Parser" library. To get it to work with LittleFS I made a few modifications.
-  Also install the included "NextionUpload" Library as with previous Controller versions
-  With the ESP32 boards Version 2.0.14 or later you should be ready to compile
-  Select the ESP32 Dev Module as the processor in the tools dropdown, and then make sure you select the partition scheme "Minimal SPIFFS....". Others might work, but I have been using this scheme.
-  When the sketch has compiled, you'll need to upload the LittleFS data. In the Arduino Sketch folder you'll find a new "data" folder which contains 5 .csv files. Example data has been prepared, but feel free to modify if you feel the need.
-  With Arduino IDE 2.2.x, with the ESP32 connected, press "CTRL, SHIFT and P" and the upload procedure starts. It'll take a few seconds.
-  With Adruino IDE 1.8.19, you should find the "ESP32 Sketch Data Upload" in the "Tools" section. Select it and then choose LittleFS. The data files should be uploaded.
-  If you're in a hurry, use the currently available latest version of the HMI Version 1.7.8 to suit the model you're using. I will be uploading some identical (but renamed) HMIs in zip form to this repo shortly.

# New Feature

- The WiFi Enable button now has a new use:-)
  - When disabled/Off, when pressing the "Scan" button the list of LittleFS WiFi profiles will be displayed
  - When enabled/On, the "Scan" button does a scan for local APs
