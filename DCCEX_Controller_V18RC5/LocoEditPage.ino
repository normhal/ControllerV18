//LocoEditPage
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
 ***************************************************
 * Process Loco Edit Page
 *************************************************
*/
void locoEditPage(uint8_t button)
{
  String receivedData = message.substring(2);
  if(message.startsWith("FN"))                //Function Name
  {
    locosDirty = 1;
    hcLoco[editingID].LocoLongName = receivedData;
    return;
  }
  if(message.startsWith("AD"))                //Address
  {
    locosDirty = 1;
    hcLoco[editingID].LocoAddress = receivedData.toInt();
    return;
  }
  if(message.startsWith("RN"))                //Road Name
  {
    locosDirty = 1;
    hcLoco[editingID].LocoShortName = receivedData;
    return;
  }
  if(message.startsWith("LT"))
  {
    locosDirty = 1;
    hcLoco[editingID].LocoType = receivedData;
    return;
  }
  if(message.startsWith("LN"))              //Road Number
  {
    locosDirty = 1;
    hcLoco[editingID].LocoRNum = receivedData.toInt(); 
    return;
  }
  switch(button)
  {
    case Cancel_Press:
    {
      locosDirty = 0;
      nextionSetValue("Edit", 0);                     //Force EDIT Button OFF
      eMode = 0;
      initPage(LocoEditReturnPage);
      break;
    }
    case Done_Press:
    {
      if(eMode == 2) selectedIDs[thNum][activeSlot[thNum]] = 255;
      eMode = 0;                                      //Turn off Edit Mode
      nextionSetValue("Edit", 0);                     //Force EDIT Button OFF
      nextionSetValue("Remove", 0);                     //Force EDIT Button OFF
      Serial.printf("Loco Dirty State %d\n", locosDirty);
      initPage(LocoEditReturnPage);
      break;
    }
    case DeleteButton:
      if(eMode == 1)
      {
        locosDirty = 1;
        hcLoco[editingID].LocoAddress = 0;
        hcLoco[editingID].LocoRNum = 0;
        hcLoco[editingID].LocoLongName = "";
        hcLoco[editingID].LocoShortName = "";
        hcLoco[editingID].LocoType = "";      
        for(uint8_t i=0; i<10; i++)
        {
          hcLoco[editingID].FSlot[i][0] = 127;              //Function Number
          hcLoco[editingID].FSlot[i][1] = BLANK;            //Image
        }
        eMode = 0;              //Turn off Edit Mode
        nextionSetValue("Edit", 0);
        initPage(LocoEditReturnPage);
      }
      break;
    case ProgramButton:
    {
      ProgReturnPage = LocoEditPage;
      initPage(ProgramPage);
    }
    case EditButtonOn:                      //
    {
      eMode = 1;
      nextionSetValue("Remove",0);
      break;
    }
    case EditButtonOff:                      //
    {
      eMode = 0;
      break;
    }
    case RemoveButton:
    {
      eMode = 2;      //eMode 2 = Remove Active
      nextionSetValue("Edit",0);
      break;
    }
    case UndoButton:
    {
      eMode = 0;
      break;
    }
    default:
    {
      if(button >= SlotAStart && button < (SlotAStart + functionsPerPage))        //This comes from the IMAGE Slot which was pressed (200 to 209)
      {
        g_fSlot = button - SlotAStart;
        if(eMode == 1)
        {
          initPage(FunctionEditPage);
          return;
        }
      }
    }
  }
}
