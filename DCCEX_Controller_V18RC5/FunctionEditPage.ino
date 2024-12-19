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
 * Function Edit Page
 *************************************************************************************************************************
*/
void functionEditPage(uint8_t button)
{
  if(message.startsWith("FF"))
  {
    functionsDirty = 1;
    hcLoco[editingID].FSlot[g_fSlot][0] = message.substring(2).toInt();              //Function Number
    return;
  }  
  if(message.startsWith("FI"))
  {
    functionsDirty = 1;
    hcLoco[editingID].FSlot[g_fSlot][1] = message.substring(2).toInt();              //Function Image
    return;
  }
  switch(button)
  {
    case Done_Press:
    {
      if(deleteMode == 1)
      {
        functionsDirty = 1;
        hcLoco[editingID].FSlot[g_fSlot][0] = 255;                                   //unUsed Function
      }
      deleteMode = 0;
      nextionSetValue("D", 0);
      initPage(LocoEditPage);
      break;
    }
    case Cancel_Press:
    {
      deleteMode = 0;
      nextionSetValue("D", 0);      //Turn off Delete Button
      functionsDirty == 0;
      initPage(LocoEditPage);
      break;
    }
    case Func_Latched:
    {
      functionsDirty = 1;
      hcLoco[editingID].FSlot[g_fSlot][0] & 0x7f;
      fType = LATCH;
      break;
    }
    case Func_Pulsed:
    {
      functionsDirty = 1;
      hcLoco[editingID].FSlot[g_fSlot][0] | 0x80;
      fType = PULSE;
      break;
    }
    case DELETE_OFF:
      deleteMode = 0;
      break;
    case DELETE_ON:
      deleteMode = 1;
      break;
    case TestButtonOn:
    {
      uint8_t newImage = toggleFunction(((hcLoco[editingID].FSlot[g_fSlot][0]) & 0x7f), hcLoco[editingID].FSlot[g_fSlot][1]);
      nextionCommand("Test.pic=" + String(newImage));
      break;
    }
    case TestButtonOff:
    {
      if(fType == PULSE)
      {
        uint8_t newImage = toggleFunction(((hcLoco[editingID].FSlot[g_fSlot][0]) & 0x7f), hcLoco[editingID].FSlot[g_fSlot][1]);
        nextionCommand("Test.pic=" + String(newImage));
      }
      break;
    }
    default:
      break;
  }
}
