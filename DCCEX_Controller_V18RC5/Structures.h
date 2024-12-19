/***********************************************************************
*                  
* COPYRIGHT (c) 2024 Norman Halland (NormHal@gmail.com)
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
#ifndef STRUCTURES_H
  #define STRUCTURES_H

  #define NUM_FSLOTS 10
  #define PULSED 128

  #define numLocos 50
  struct HCLoco 
  {
    uint16_t LocoId;
    String LocoLongName;
    String LocoShortName;
    String LocoType;
    uint16_t LocoRNum;
    uint16_t LocoAddress;
    uint8_t FSlot[NUM_FSLOTS][4];         //10 Slots, each having Function Number (with Momentary bit), Function Image, and State for each function
    uint8_t speed;
    uint8_t direction;
  }list_of_hcLoco[numLocos];

HCLoco hcLoco[numLocos];

  #define numAccs 252
  struct HCAcc
  {
    uint16_t AccId;
    String AccName;
    uint16_t AccAddress;
    uint16_t AccImage;
    uint16_t AccType;
  }list_of_accs[numAccs];
 
HCAcc hcAcc[numAccs];

  #define numRoutes 48
  struct HCRoute
  {
    uint16_t AccIDs[6][2];
  }list_of_routes[numRoutes]; 

HCRoute hcRoute[numRoutes];

// Structure used for WiFi Credentials

#define numCreds 10
struct Cred
{
  uint16_t ID;
  String SSID;
  String Password;
  String IPAddress;
  uint16_t Port;
}list_of_creds[numCreds];

Cred cred[numCreds];

#endif
