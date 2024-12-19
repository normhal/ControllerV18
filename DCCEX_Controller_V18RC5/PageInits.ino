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
 * Page Initializations
 *************************************************************************************************************************
*/
void initPage(uint8_t page)
{
  activatePage(page);
  if(!PowerState) nextionSetValue("Power",0);
  if(PowerState)  nextionSetValue("Power",1);
  wait(50);
  nextionCommand(("Sig.pic=" + String(wifiImage)).c_str());
#if defined WIFI
  if(client.connected())
  {
    nextionCommand("P2.pic=258");
    lastCSPic = 258; 
  }else
  {
    nextionCommand("P2.pic=259");
    lastCSPic = 259; 
  }
#endif
  switch(page)
  {
    //******************************************************************************************************************************
    case ThrottlePage:
    {  
      wait(50);
      populateSlots();
      activateSlot(activeSlot[thNum]);
      break;
    }
    //******************************************************************************************************************************
    case LocoEditPage:
    {
      if(eMode == 0) nextionSetValue("Edit", 0);
      nextionSetText("AD", String(hcLoco[editingID].LocoAddress));
      if(hcLoco[editingID].LocoAddress == 0)
      {
        nextionSetText("RN","");
        nextionSetText("LT","");
      }else
      {
        nextionSetText("RN", hcLoco[editingID].LocoShortName);      //Get Loco Name from Integrated EEPROM
        nextionSetText("LT", hcLoco[editingID].LocoType);      //Get Loco Type from Integrated EEPROM
        nextionSetText("FName", hcLoco[editingID].LocoLongName);
        nextionSetText("LN", String(hcLoco[editingID].LocoRNum));
        loadFunctions(LocoEditPage, editingID);
      }
      break;
    }
    //******************************************************************************************************************************
    case RosterPage:
    {
      backupID = selectedIDs[thNum][activeSlot[thNum]];       //Used by "Cancel" button
      rosterDrawPage(locoStartID);
      break;
    }
    //****************************************************************************************************************************** 
    case AccPage:                                                                                                                   
    {
      if (accStartID == 0) nextionCommand("PageDn.pic=PAGEDNGREYED");
      accDrawPage(accStartID);
      break;
    }
    //******************************************************************************************************************************
    case AccEditPage: 
    {
      nextionSetText("ID", String(editingID + accIDBase));
      nextionSetText("A", String(hcAcc[editingID].AccAddress));   //String(readAccAddress(editingID)));
      if(hcAcc[editingID].AccAddress == 0) nextionSetText("N",""); 
      else nextionSetText("N", hcAcc[editingID].AccName);             //readEEPROMName(accNameBase + (editingID * (accNameLen))));
      if(hcAcc[editingID].AccAddress != 0)
      {
        nextionCommand("ImageA.pic=" + String(hcAcc[editingID].AccImage));
        nextionCommand("ImageB.pic=" + String(hcAcc[editingID].AccImage+1));   //Add 1 for the partner image
        nextionCommand("Test.pic=" + String(hcAcc[editingID].AccImage));
        nextionCommand("Test.pic2=" + String(hcAcc[editingID].AccImage+1));    //Add 1 for the partner image
        nextionSetValue("DCC",1);
      }else{
        nextionCommand("ImageA.pic=23");   //+ String(GREYED_BUTTON)).c_str());  //23
        nextionCommand("ImageB.pic=23");  //+ String(GREYED_BUTTON)).c_str());
        nextionCommand("Test.pic=23");  //" + String(GREYED_BUTTON)).c_str());
        nextionCommand("Test.pic2=23");  //" + String(GREYED_BUTTON)).c_str());
        nextionSetValue("DCC",1);                                       //Default to DCC
      }
      eMode = 0;
      nextionSetValue("Edit",0);
      break;
    }
    //******************************************************************************************************************************
    case RoutesPage:
    {
      if (routeStartID == 0) nextionCommand("PageDn.pic=PAGEDNGREYED");
      accSelectMode = false;
      eMode = 0;
      routeDrawPage(routeStartID);
      break;
    }
    //******************************************************************************************************************************
    case ProgramPage:
    {
      nextionSetValue("Main", 0);
      nextionSetValue("Prog", 0);
      nextionSetValue("Read", 0);
      nextionSetValue("Write", 0);
      nextionCommand("ADText.pco=" + String(GREY));
      nextionCommand("LName.pco=" + String(GREY));
      nextionCommand("AD.pco=" + String(GREY));
      nextionCommand("AD.bco=" + String(GREY));
      nextionSetText("AD", String(hcLoco[selectedIDs[thNum][activeSlot[thNum]]].LocoAddress));
      nextionSetText("LName", hcLoco[selectedIDs[thNum][activeSlot[thNum]]].LocoLongName);
      nextionSetText("CVN", "");
      nextionSetText("CVV", "");
      nextionSetText("S0", "Select Main or Prog First");
      nextionSetText("S1", "");
      nextionSetText("S2", "");
      nextionSetText("S3", "");
      progMode = 0;
      break;
    }
    //******************************************************************************************************************************
    case ConfigPage:                                                                                //Done
    {
      nextionCommand("RN.val=" + String(readEEPROMByte(eeRNumEnabled)));
      nextionCommand("Stop.val=" + String(readEEPROMByte(eeLocoStopAll)));
      nextionCommand("PU.val=" + String(readEEPROMByte(eePUState)));
      nextionCommand("DCC.val=" + String(readEEPROMByte(eeDCCRefresh)));       //",String(readEEPROMByte(ee
      nextionCommand("Join.val=" + String(readEEPROMByte(eeJoinMode)));
      nextionSetText("n0", String(readEEPROMByte(eeThreshold)));
      nextionSetText("n1", String(readEEPROMByte(eeREIncrement)));
      nextionSetText("n2", String(readEEPROMByte(eeAccDelay)));
      nextionSetText("ExTex", "DCCEX Roster Not Supported");
      break;
    }
    //******************************************************************************************************************************
    case WiFiPage:                                                                            //In Progress
    { 
      #if defined WIFI
        nextionCommand("WiFiEN.val=" + String(WiFiEnabled));
        if(cred[credIndex].SSID == "your_ssid") cred[credIndex].SSID = "";
        nextionSetText("SSID", cred[credIndex].SSID);                     //String(ssid));
        if(cred[credIndex].Password == "your_password") cred[credIndex].Password = "";
        nextionSetText("Password", cred[credIndex].Password);             //password);
        nextionSetText("HostIP", cred[credIndex].IPAddress);                   //host);
        nextionSetText("Port", String(cred[credIndex].Port));             //String(port)); 
        if(WiFiEnabled == 0) 
        {
          nextionSetText("Progress","");
          nextionSetText("WiFiState", "WiFi Disconnected...");
          return;
        }
        nextionSetText("RSSI", String(WiFi.RSSI()));
        nextionSetText("Retries", String(readEEPROMByte(eeWiFiRetries)));
        nextionSetText("n3", String(readEEPROMByte(eeWiFiSeconds)));
        nextionSetText("Progress", "");
        if(WiFi.status() == WL_CONNECTED)
        {
          nextionSetText("WiFiState", wifiStatus);
        }else
        {
          nextionSetText("WiFiState", "WiFi Disconnected - Retry");
        }
      #endif
      break;
    }
    //******************************************************************************************************************************
    case SelectionPage:
    {
      Console.print("List Name Passed: ");
      Console.println(listName);
      fillSelection(listName);
      break;
    }
    //******************************************************************************************************************************
    case FunctionEditPage:
    {
      if(hcLoco[editingID].LocoAddress != 0)
      {
        nextionSetText("LName", hcLoco[editingID].LocoLongName);
      }else
      {
        nextionSetText("LName", "");
      }
      nextionSetText("AD", String(hcLoco[editingID].LocoAddress));
      nextionSetText("F", String(hcLoco[editingID].FSlot[g_fSlot][0] & 0x7f));     //String(funcNum & 0x7F));   //Load Function Number
      uint8_t funcImage = 118;
      if((hcLoco[editingID].FSlot[g_fSlot][0] & 0x7f) != 127)                     //Retrieve Image Number
      nextionSetValue("n1", hcLoco[editingID].FSlot[g_fSlot][1]);                                                               //Load hidden Image Number
      if((hcLoco[editingID].FSlot[g_fSlot][0] & 0x80) == 128) 
      {
        nextionSetValue("Type", 1);
        fType = 1;
      }else
      {
        nextionSetValue("Type", 0);
        fType = 0;
      }
      nextionCommand(("ImageA.pic=" + String(hcLoco[editingID].FSlot[g_fSlot][1])).c_str());                                         //Load ImageA
      nextionCommand(("Test.pic=" + String(hcLoco[editingID].FSlot[g_fSlot][1])).c_str());                                           //Load Test Image
      nextionCommand(("ImageB.pic=" + String(hcLoco[editingID].FSlot[g_fSlot][1] + 1)).c_str());                                     //Load ImageB
      break;
    }

    //******************************************************************************************************************************
    case UpdatePage:                                                                           //Still to Follow
    {
      nextionSetText("uSSID", ussid);
      nextionSetText("uPW", upassword);
      nextionSetText("TFTIP", tftHost);
      nextionSetText("TFTPort", String(tftPort));
      nextionSetText("TFTName", url);
      nextionSetValue("OTA", 0);
      nextionSetValue("Upload", 0);
      break;
    }
    //******************************************************************************************************************************
    case MenuPage:
    {
      switch(thNum)
      {
        case(0):
          nextionCommand("TH1.pic=" + String(TH1_BUTTON_ON));     //TH1_BUTTON_ON
          nextionCommand("TH2.pic=" + String(TH2_BUTTON_OFF));     //TH2_BUTTON_OFF
          nextionCommand("TH3.pic=" + String(TH3_BUTTON_OFF));     //TH3_BUTTON_OFF
          nextionCommand("TH4.pic=" + String(TH4_BUTTON_OFF));     //TH4_BUTTON_OFF
          break;
        case(1):
          nextionCommand("TH1.pic=" + String(TH1_BUTTON_OFF));
          nextionCommand("TH2.pic=" + String(TH2_BUTTON_ON));
          nextionCommand("TH3.pic=" + String(TH3_BUTTON_OFF));
          nextionCommand("TH4.pic=" + String(TH4_BUTTON_OFF));
          break;
        case(2):
          nextionCommand("TH1.pic=" + String(TH1_BUTTON_OFF));
          nextionCommand("TH2.pic=" + String(TH2_BUTTON_OFF));
          nextionCommand("TH3.pic=" + String(TH3_BUTTON_ON));
          nextionCommand("TH4.pic=" + String(TH4_BUTTON_OFF));
          break;
        case(3):
          nextionCommand("TH1.pic=" + String(TH1_BUTTON_OFF));
          nextionCommand("TH2.pic=" + String(TH2_BUTTON_OFF));
          nextionCommand("TH3.pic=" + String(TH3_BUTTON_OFF));
          nextionCommand("TH4.pic=" + String(TH4_BUTTON_ON));
          break;
        default:
          Console.println("Invalid Throttle Number");
      } 
    }
  }
}
/*
 *************************************************************************************************************************
 * Activate a Nextion Page
 *************************************************************************************************************************
*/
void activatePage(uint8_t page)
{
  nextionPage = page;                       //Save new Page reference
  nextionCommand("page " + String(page));   
}
/*
 *************************************************************************************************************************
 * Set PAGEUP and PAGEDN Buttons
 *************************************************************************************************************************
*/
void setPageButtons(uint8_t startID, uint8_t rowsPerPage, uint8_t numRows)
{
  #if !defined NO_PAGING
    if (startID == 0)
    {
      nextionCommand("PageUp.pic=24");    // + String(PAGEUP)).c_str());     //24
      nextionCommand("PageUp.pic2=25");   // + String(PAGEUPON)).c_str());  //25
      nextionCommand(F("PageDn.pic=29"));    // + String(PAGEDNGREYED)).c_str());   //29
      nextionCommand(F("PageDn.pic2=29"));    // + String(PAGEDNGREYED)).c_str());    //29
    }else{   
      nextionCommand("PageDn.pic=27");      // + String(PAGEDN)).c_str());     //27
      nextionCommand("PageDn.pic2=28");     // + String(PAGEDNON)).c_str());  //28
    }
    if (startID < (numRows - rowsPerPage))
    {
      nextionCommand("PageUp.pic=24");   // + String(PAGEUP)).c_str());    //24
      nextionCommand("PageUp.pic2=25");   // + String(PAGEUPON)).c_str());  //25
    }else{
      nextionCommand("PageUp.pic=26");     //+ String(PAGEUPGREYED)).c_str());   //26
      nextionCommand("PageUp.pic2=26");    //+ String(PAGEUPGREYED)).c_str());  //26
    }
  #endif  
}
