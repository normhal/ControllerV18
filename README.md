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
-  With the ESP32 boards Version 2.0.14 or later you sould be ready to compile
