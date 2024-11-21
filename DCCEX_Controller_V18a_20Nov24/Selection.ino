/*
 **********************************************************************
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
 ***********************************************************************
*/
void selectionPage(uint8_t button)
{
  switch(button)
  {
    case Done_Press:
      initPage(returnPage);
      break;
    case Cancel_Press:
      initPage(returnPage);
      break;
    default:
    {
      if(button >= 240 && button <= 249)
      {
        button = button - 240;
        switch(listName)
        {
          case SSIDs:
            ssid = foundSSID[button];
            break;
          case uSSIDs:
            ussid = foundSSID[button];
            break;
          case CREDs:
            ssid = cred[button].SSID;
            password = cred[button].Password;
            host = cred[button].IPAddress;
            port = cred[button].Port;
            break;
          case TFTs:
            url = String(tftFileName[button] + EEPROMCODE + String(TFT_SUFFIX));
            break;
          default:
            break;
        }
        initPage(returnPage);
      }
      break;
    }
  }
}
/*
 ********************************************************************************************
 * Function to Populate the Selection Page with a passed List Name
 ********************************************************************************************
*/
void fillSelection(enum List listName)
{
#if defined WIFI
  for(int i = 0; i<10; i++)
  {
    nextionSetText("b"+String(i), "");
    nextionSetText("b1"+String(i), "");
  }
//  if(WiFiEnabled == 1)
//  {
    nextionSetText("List","Populating...");
    Console.print("List Passed to Fill: ");
    Console.println(listName);
    switch(listName)
    {
      case SSIDs:
      case uSSIDs:
      {
      if(!client.connected())
      {
        WiFi.mode(WIFI_STA);
        wait(200);
        WiFi.disconnect();
        wait(1000);
      }

      int n = WiFi.scanNetworks();
      Console.print("SSIDs Found: ");
      Console.println(n);
      if (n != 0) 
      {
        for (int i = 0; i < n; ++i) 
        {
          foundSSID[i] = WiFi.SSID(i);
          nextionSetText("b"+String(i), WiFi.SSID(i));
          wait(50);
          nextionSetText("b1"+String(i), String(WiFi.RSSI(i)));
        }
        nextionSetText("List","Press the SSID you Need");
      }
//      nextionSetText("List","List Complete!");
      break;
    }
      case CREDs:
        for(int i=0; i<10; i++)
        {
          if(cred[i].SSID != "")
          {
            foundSSID[i] = cred[i].SSID;
            nextionSetText("b"+String(i), String(cred[i].SSID));
          }
        }
        nextionSetText("List","Press the SSID you require");
        break;
      case TFTs:
      {
        for(int i=0; i<10; i++)
        {
          if(tftFileName[i] != "")
          nextionSetText("b"+String(i), String(tftFileName[i] + EEPROMCODE + String(TFT_SUFFIX)));
        }
        nextionSetText("List","Press the TFT to Upload");
        break;
      }
      default:
        break;
    }
  //} else      //WiFi IS NOT Enabled
  //{
    
//  }
#endif 
}
