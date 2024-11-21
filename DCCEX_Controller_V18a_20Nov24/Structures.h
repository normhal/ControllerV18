#ifndef STRUCTURES_H
  #define STRUCTURES_H

  struct locomotive 
  {
    uint8_t dir = 1;
    uint8_t speed;
  };
  typedef struct locomotive loco;

  #define NUM_FSLOTS 10
  #define PULSED 128

  // Structure used for the "Hard_Coded_Values" feature when loading pre-configured Locos
  
  #define numLocos 50
  struct HCLoco 
  {
    uint16_t LocoId;
    String LocoLongName;
    String LocoShortName;
    String LocoType;
    uint16_t LocoRNum;
    uint16_t LocoAddress;
//    String FavSlot;                         //Slot (0-9) for selectedIDs[] if present
    uint8_t FSlot[NUM_FSLOTS][4];         //10 Functions, then Function Slot number, Function Number, Function Image for each function
  }list_of_locos[numLocos];

HCLoco hcLoco[numLocos];

  #define numAccs 96
  struct HCAcc
  {
    uint16_t AccId;
    String AccName;
    uint16_t AccAddress;
    uint16_t AccImage;
    uint16_t AccType;
  }list_of_accs[numAccs];
 
HCAcc hcAcc[numAccs];

// Structure used for the "Hard_Coded_Values" feature when loading pre-configured Routes

  struct HCRoute
  {
    uint8_t AccIDs[6][2];
  };  

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

