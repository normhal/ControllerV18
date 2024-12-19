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
************************************************************************/
/*
 *************************************************************************************************************************
 * Process Status Page
 *************************************************************************************************************************
*/
void wifiPage(uint8_t button)
{
#if defined WIFI
  if(message.startsWith("SS"))
  { 
    WiFiEnabled = 0;
    cred[credIndex].SSID = message.substring(2);
    credentialsDirty = 1;
    return;
  }
  if(message.startsWith("PA"))
  {
    WiFiEnabled = 0;
    cred[credIndex].Password = message.substring(2);
    credentialsDirty = 1;
    return;
  }
  if(message.startsWith("IP"))
  {
    WiFiEnabled = 0;
    cred[credIndex].IPAddress = message.substring(2);
    credentialsDirty = 1;
    return;
  }
  if(message.startsWith("PO"))
  {
    WiFiEnabled = 0;
    cred[credIndex].Port = message.substring(2).toInt();
    credentialsDirty = 1;
    return;
  }
  if(message.startsWith("RC"))
  {
    WiFiRetries = message.substring(2).toInt(); 
    writeEEPROMByte(eeWiFiRetries, WiFiRetries);
    return;
  }
  if(message.startsWith("N3"))
  {
    wifiSeconds = message.substring(2).toInt();
    writeEEPROMByte(eeWiFiSeconds, wifiSeconds);
    return;
  }
  switch (button)
  {
    case ConfigButton:
        if(!EEPROM.commit()) Console.println("EEPROM.commit Failed"); 
        initPage(ConfigPage);
        break;
    case Done_Press:
      Serial.printf("Credentials Dirty = %d\n", credentialsDirty);
      if(!EEPROM.commit()) Console.println("EEPROM.commit Failed");
      if(credentialsDirty == 1) saveCredentials(LittleFS, "/credentials.new", "ID,SSID,Password,IPAddress,Port\n");
      initPage(MenuPage);
      break;
    case Scan_Press:
      if(WiFiEnabled == 0) listName = CREDs;
      else listName = SSIDs;
      writeEEPROMByte(eeWiFiEnabled, WiFiEnabled);
      nextionSetValue("WiFiEN",0);
      wifiImage = WIFI_X;
      Serial.println(listName);
      nextionSetText("Progress", "Reading List...");
      returnPage = WiFiPage;
      initPage(SelectionPage);
      break;
    case  RetryButton:
      WiFiEnabled = 1;
      writeEEPROMByte(eeWiFiEnabled, WiFiEnabled);
      nextionSetValue("WiFiEN",1);
      nextionSetText("Progress", "Retrying...");
      if(!EEPROM.commit()) Console.println("EEPROM.commit Failed");
      saveLastCred();
      updateWiFi();
      break;
    case WiFiEnabledON:
      WiFiEnabled = 1;
      writeEEPROMByte(eeWiFiEnabled, WiFiEnabled);
      nextionSetText("Progress", "Press Retry to Connect...");
      break;
    case WiFiEnabledOFF:
      WiFiEnabled = 0;
      writeEEPROMByte(eeWiFiEnabled, WiFiEnabled);                //WiFi default
      nextionSetText("Progress", "Disabling WiFi...");
      wait(500);
      nextionCommand("Sig.pic=" + String(WIFI_X));
      updateWiFi();
      break;
    default:
      break;
  }
  #endif
}
void saveLastCred()
{
  cred[9].SSID = cred[credIndex].SSID;
  cred[9].Password = cred[credIndex].Password;
  cred[9].IPAddress = cred[credIndex].IPAddress;
  cred[9].Port = cred[credIndex].Port;
  saveCredentials(LittleFS, "/credentials.new", "ID,SSID,Password,IPAddress,Port\n");

}
