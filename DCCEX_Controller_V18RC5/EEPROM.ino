/***********************************************************************
*                  
* COPYRIGHT (c) 2022 Norman Halland (NormHal@gmail.com)
*
*  This program and all its associated modules is free software: 
*  you can redistribute it and/or modify it under the terms of the 
*  GNU General Public License as published by the Free Software 
*  Foundation, either version 3 of the License, or (at your option) 
*  any later version.
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see http://www.gnu.org/licenses
*
*************************************************************************
*/
void initEEPROM()
{
  Serial.printf("Current EEPROM Code: %d New EEPROM Code: %d\n", EEPROM.read(eepromEnd), EEPROMCODE);
  writeEEPROMByte(eeJoinMode, JOIN_OPTION);
  writeEEPROMByte(eeThreshold, ReverseThreshold);
  writeEEPROMByte(eeREIncrement, REAccAmount);
  writeEEPROMByte(eePUState, PowerUpState);
  writeEEPROMByte(eeDCCRefresh, DCCRefresh);
  writeEEPROMByte(eeWiFiEnabled, WiFiEnabled);                //WiFi default
  writeEEPROMByte(eeAccDelay, AccDelay);
  writeEEPROMByte(eeWiFiSeconds, WIFISECS);
  writeEEPROMByte(eeLocoStopAll, LocoStopAll);
  writeEEPROMByte(eeWiFiRetries, WiFiRetries);

  writeEEPROMByte(eepromEnd, EEPROMCODE);   //Indicate that EEPROM has now been initialized

  if(!EEPROM.commit()) Console.println("EEPROM.commit Failed");

  // Copy Initial Credentials to Last Used
//  cred[9].SSID = cred[0].SSID;
//  cred[9].Password = cred[0].Password;
//  cred[9].IPAddress = cred[0].IPAddress;
//  cred[9].Port = cred[0].Port;
}
/*
 ***********************************************************************************************************************************
 * Read a String from Arduino or ESP EEPROM
 * Last Byte is null character
 * Some Names are stored at random locations
 ***********************************************************************************************************************************
*/

String readEEPROMName(uint16_t eeAddress)
{
  char data [21];
  uint8_t i;
  for (i = 0; i < 21; i++)
  {
    data[i] = EEPROM.read(eeAddress + i);
    if(data[i] == '\0') break;
  }
  return String(data);
}
/*
 ***********************************************************************************************************************************
 * Read 2 bytes from a specific EEPROM location
 ***********************************************************************************************************************************
*/

uint16_t readEEPROMAddr(uint16_t eeAddress)
{
  uint8_t uint8_t1 = EEPROM.read(eeAddress);
  uint8_t uint8_t2 = EEPROM.read(eeAddress + 1);
  return (uint8_t1 << 8) + uint8_t2;
}
/*
 ***********************************************************************************************************************************
 * Read 1 byte from a specific EEPROM location
 ***********************************************************************************************************************************
*/
uint8_t readEEPROMByte(uint16_t eeAddress)
{
  uint8_t eeData = EEPROM.read(eeAddress);
  return eeData;
}
/*
 ***********************************************************************************************************************************
 * Write 1 byte to EEPROM
 ***********************************************************************************************************************************
*/
void writeEEPROMByte(uint16_t eeAddress, uint8_t eeData)
{
  EEPROM.write(eeAddress, eeData);
}
/*
 ***********************************************************************************************************************************
 * Write a String to Arduino or ESP EEPROM
 ***********************************************************************************************************************************
*/
void writeEEPROMName(uint16_t eeAddress, const String &Name)
{
  uint8_t i;
  uint8_t len = Name.length();
  for (i = 0; i < len; i++) EEPROM.write(eeAddress + i, Name[i]);
  EEPROM.write(eeAddress + i, '\0');
}
/*
 ***********************************************************************************************************************************
 * Write a 2 uint8_t "Address" to EEPROM
 ***********************************************************************************************************************************
*/
void writeEEPROMAddr(uint16_t eeAddress, uint16_t wordToWrite)
{ 
  uint8_t uint8_t1 = wordToWrite >> 8;
  uint8_t uint8_t2 = wordToWrite & 0xFF;
  EEPROM.write(eeAddress, uint8_t1);
  EEPROM.write(eeAddress + 1, uint8_t2);
}
