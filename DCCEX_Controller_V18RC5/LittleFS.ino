//LittleFS Routines
//ListDir is only used at bootup time for debug purposes
//
//****************************************************************************************************************
// Data Files Used by LittleFS
//****************************************************************************************************************
//
/*
String fileList[20] =
{
  "accessories.txt",
  "credentials.txt",
  "functions.txt",
  "locos.txt",
  "routes.txt",
  "throttleids.txt"
};
*/
//
//****************************************************************************************************************
//
//****************************************************************************************************************
//
void listDir(fs::FS &fs, const char *dirname, uint8_t levels) 
{
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname, "r");
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());

      // Serial.print(file);
    }
    file = root.openNextFile();
  }
}
//****************************************************************************************************************
//  Parser Routine which loads the Accessory file from LittleFS
//****************************************************************************************************************
void populateAccArray(const char *path)
{
  File file = LittleFS.open(path, "r");
  if(!file)
  {
      Serial.println("Failed to open file for reading");
      return;
  }
  Serial.println("Calling Acc Parser");
  CSV_Parser cp(/*format*/ "udsududud", /*has_header*/ true, /*delimiter*/ ',');

  while (file.available()) 
  {
    cp << (char)file.read();
  }
  cp.parseLeftover();

//  if(cp.readFSfile(path))      // Serial.println("readFSfile failed");
//  {
  uint16_t *accId = (uint16_t *)cp["ID"];
  char **accName = (char **)cp["Name"];
  uint16_t *accAddress = (uint16_t *)cp["Address"];
  uint16_t *accImage = (uint16_t *)cp["Image"];
  uint16_t *accType = (uint16_t *)cp["Type"];

  Serial.print("Accessories read from LittleFS: ");
  Serial.println(cp.getRowsCount());

  for(int row = 0; row < cp.getRowsCount(); row++)      
  {
    hcAcc[row].AccId = accId[row];
    hcAcc[row].AccName = accName[row];
    hcAcc[row].AccAddress = accAddress[row];
    hcAcc[row].AccImage = accImage[row];
    hcAcc[row].AccType = accType[row];
  }
  file.close();
  Serial.print("Acc Rows Read: ");
  Serial.println(cp.getRowsCount());
}
//****************************************************************************************************************
//  Parser Routine which loads the Locos file from LittleFS
//****************************************************************************************************************
void populateLocoArray(const char *path)
{
  File file = LittleFS.open(path, "r");
  if(!file)
  {
      Serial.println("Failed to open file for reading");
      return;
  }
  Serial.println("Calling Locos Parser");
  CSV_Parser cp(/*format*/ "udsssudud", /*has_header*/ true, /*delimiter*/ ',');

  while (file.available()) 
  {
    cp << (char)file.read();
  }
  cp.parseLeftover();

  uint16_t *locoId = (uint16_t *)cp["ID"];
  char **locoLongName = (char **)cp["LongName"];
  char **locoShortName = (char **)cp["ShortName"];
  char **locoType = (char **)cp["Type"];
  uint16_t *locoRNum = (uint16_t *)cp["RoadNumber"];
  uint16_t *locoAddress = (uint16_t *)cp["Address"];

  Serial.print("Locos read from LittleFS: ");
  Serial.println(cp.getRowsCount());

  for(int row = 0; row < cp.getRowsCount(); row++)      
  {
/*
      hcLoco[row].LocoId = locoId[row];
      hcLoco[row].LocoLongName = locoLongName[row];
      hcLoco[row].LocoShortName = locoShortName[row];
      hcLoco[row].LocoType = locoType[row];
      hcLoco[row].LocoRNum = locoRNum[row];
      hcLoco[row].LocoAddress = locoAddress[row];
*/    
    uint16_t index = locoId[row];
    hcLoco[index].LocoId = locoId[row];
    hcLoco[index].LocoLongName = locoLongName[row];
    hcLoco[index].LocoShortName = locoShortName[row];
    hcLoco[index].LocoType = locoType[row];
    hcLoco[index].LocoRNum = locoRNum[row];
    hcLoco[index].LocoAddress = locoAddress[row];
  }
  file.close();
}
//****************************************************************************************************************
//  Parser Routine which loads the Function file from LittleFS and populates the related Locos
//****************************************************************************************************************
void populateLocoFunctions(const char *path)
{
  File file = LittleFS.open(path, "r");
  if(!file)
  {
      Serial.println("Failed to open file for reading");
      return;
  }

  Serial.println("Calling Functions Parser");
  CSV_Parser cp(/*format*/ "ududududud");       //5x uint16_t values

  Serial.print("Reading from file: ");
  Serial.println(path);

  while (file.available()) 
  {
    cp << (char)file.read();
  }
  cp.parseLeftover();

  uint16_t *ids = (uint16_t*)cp["LocoID"];
  uint16_t *slot = (uint16_t*)cp["Slot"];
  uint16_t *function = (uint16_t*)cp["Function"];
  uint16_t *image = (uint16_t*)cp["Image"];
  uint16_t *moment = (uint16_t*)cp["Momentary"];

  Serial.print("Functions read from LittleFS: ");
  Serial.println(cp.getRowsCount());

  for(int row = 0; row < cp.getRowsCount(); row++)      
  {
    hcLoco[ids[row]].FSlot[slot[row]][0] = function[row];                                                //0= Function | Momentary, 1=image
    if(moment[row] == 1) hcLoco[ids[row]].FSlot[slot[row]][0] = (function[row] | 128);
    hcLoco[ids[row]].FSlot[slot[row]][1] = image[row];
  }
  file.close();
}
//****************************************************************************************************************
//  Parser Routine which loads the Routes Array from LittleFS
//  ID, Acc1,State1, Acc2,State2, Acc3,State3, Acc4,State4, Acc5,State5, Acc6,State6
// States :- Closed = 0, Thrown = 1
//****************************************************************************************************************
void populateRoutesArray(const char *path)
{
  File file = LittleFS.open(path,"r");
  if(!file)
  {
      Serial.println("Failed to open file for reading");
      return;
  }
  Serial.println("Calling Routes Parser");
  CSV_Parser cp(/*format*/ "ududududududududududududud", /*has_header*/ true, /*delimiter*/ ',');

  while (file.available()) 
  {
    cp << (char)file.read();
  }
  cp.parseLeftover();

  uint16_t *routeID = (uint16_t *)cp["ID"];
  uint16_t *acc1 = (uint16_t *)cp["Acc1"];
  uint16_t *state1 = (uint16_t *)cp["State1"];
  uint16_t *acc2 = (uint16_t *)cp["Acc2"];
  uint16_t *state2 = (uint16_t *)cp["State2"];
  uint16_t *acc3 = (uint16_t *)cp["Acc3"];
  uint16_t *state3 = (uint16_t *)cp["State3"];
  uint16_t *acc4 = (uint16_t *)cp["Acc4"];
  uint16_t *state4 = (uint16_t *)cp["State4"];
  uint16_t *acc5 = (uint16_t *)cp["Acc5"];
  uint16_t *state5 = (uint16_t *)cp["State5"];
  uint16_t *acc6 = (uint16_t *)cp["Acc6"];
  uint16_t *state6 = (uint16_t *)cp["State6"];

  Serial.print("Routes read from LittleFS: ");
  Serial.println(cp.getRowsCount());

  for(int row = 0; row < cp.getRowsCount(); row++)
  {
    hcRoute[row].AccIDs[0][0] = acc1[row];
    hcRoute[row].AccIDs[0][1] = state1[row];
    hcRoute[row].AccIDs[1][0] = acc2[row];
    hcRoute[row].AccIDs[1][1] = state2[row];
    hcRoute[row].AccIDs[2][0] = acc3[row];
    hcRoute[row].AccIDs[2][1] = state3[row];
    hcRoute[row].AccIDs[3][0] = acc4[row];
    hcRoute[row].AccIDs[3][1] = state4[row];
    hcRoute[row].AccIDs[4][0] = acc5[row];
    hcRoute[row].AccIDs[4][1] = state5[row];
    hcRoute[row].AccIDs[5][0] = acc6[row];
    hcRoute[row].AccIDs[5][1] = state6[row];
  }
  file.close();
  Serial.print("Route Rows Read: ");
  Serial.println(cp.getRowsCount());
}
//****************************************************************************************************************
//  Parser Routine which loads the Credentials Array from LittleFS
//****************************************************************************************************************
void populateCredArray(const char *path)
{
  File file = LittleFS.open(path, "r");
  if(!file)
  {
      Serial.println("Failed to open file for reading");
      return;
  }
  Serial.println("Calling Credentials Parser");
  CSV_Parser cp(/*format*/ "udsssud", /*has_header*/ true, /*delimiter*/ ',');

  while (file.available()) 
  {
    cp << (char)file.read();
  }
  cp.parseLeftover();

  uint16_t *credID = (uint16_t *)cp["ID"];
  char **ssid = (char **)cp["SSID"];
  char **password = (char **)cp["Password"];
  char **ipAddress = (char **)cp["IPAddress"];
  uint16_t *port = (uint16_t *)cp["Port"];

  Serial.print("Credentials read from LittleFS: ");
  Serial.println(cp.getRowsCount());

  for(int row = 0; row < cp.getRowsCount(); row++)      
  {
    cred[row].ID = credID[row];
    cred[row].SSID = ssid[row];
    cred[row].Password = password[row];
    cred[row].IPAddress = ipAddress[row];
    cred[row].Port = port[row];
  }
  file.close();
  Serial.print("Credential Rows Read: ");
  Serial.println(cp.getRowsCount());
}
//****************************************************************************************************************
//  Parser Routine which loads the Selected Throttle IDs Array from LittleFS
//****************************************************************************************************************
void populateSelected(const char *path)
{
  File file = LittleFS.open(path, "r");
  if(!file)
  {
      Serial.println("Failed to open file for reading");
      return;
  }
  Serial.println("Calling Selected IDs Parser");
  CSV_Parser cp(/*format*/ "ucucuc", /*has_header*/ true, /*delimiter*/ ',');   //Throttle,LocoID,Slot

  while (file.available()) 
  {
    cp << (char)file.read();
  }
  cp.parseLeftover();

  uint8_t *throttleID = (uint8_t *)cp["Throttle"];
  uint8_t *slot = (uint8_t *)cp["Slot"];
  uint8_t *locoID = (uint8_t *)cp["LocoID"];
  
  Serial.print("Selected Loco IDs read from LittleFS: ");
  Serial.println(cp.getRowsCount());

  for(int row = 0; row < cp.getRowsCount(); row++)      
  {
    selectedIDs[throttleID[row]][slot[row]] = locoID[row];
//      Serial.printf("Selected Throttle: %d Slot: %d has Loco ID: %d\n", throttleID[row], slot[row], locoID[row]);
  }
  file.close();
  Serial.print("Selected ID Rows Read: ");
  Serial.println(cp.getRowsCount());
}
//
//****************************************************************************************************************
// File Saving Routines
//  1. Create empty work file                     eg. work.txt
//  2. Construct delimited record from array
//  3. Write all valid records
//  4. Delete work file and Exit if ANY error
//  5. Delete Oldest File
//  6. Rename Backup file to become Oldest        eg. locos.old -> locos.saf
//  7. Rename existing file to become new Backup  eg. locos.txt -> locos.old
//  8. Rename newly created work file             eg. work.txt -> locos.txt
//  9. Reverse above 3 steps if any Error
//  10. clear "Dirty" Flag
//  11. close file
//****************************************************************************************************************
//
void saveLittleFS()
{
  if(locosDirty == 1) saveLocos(LittleFS, "/locos.new", "ID,LongName,ShortName,Type,RoadNumber,Address\n");
  if(functionsDirty == 1) saveFunctions(LittleFS, "/functions.new", "LocoID,Function,Image,Slot,Momentary\n");
  if(throttlesDirty == 1) saveThrottles(LittleFS, "/throttleids.new", "Throttle,Slot,LocoID\n");
  if(accessoriesDirty == 1) saveAccessories(LittleFS, "/accessories.new", "ID,Name,Address,Image,Type\n");
  if(routesDirty == 1) saveRoutes(LittleFS, "/routes.new", "ID,Acc1,State1,Acc2,State2,Acc3,State3,Acc4,State4,Acc5,State5,Acc6,State6\n");
  if(credentialsDirty == 1) saveCredentials(LittleFS, "/credentials.new", "ID,SSID,Password,IPAddress,Port\n");
  Serial.println("All Updated Files Saved");
}

void saveLocos(fs::FS &fs, const char * path, const char * message)
{
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, "w");
  if(!file)
  {
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message))
  {
    Serial.println("Writing File");
  } else 
  {
    Serial.println("- write failed");
    file.close();
    return;
  }
  int row = 0;
  while(hcLoco[row].LocoAddress !=0)
  {
    String message = String(hcLoco[row].LocoId) + "," + hcLoco[row].LocoLongName + "," + hcLoco[row].LocoShortName + "," + hcLoco[row].LocoType + "," + hcLoco[row].LocoRNum + "," + hcLoco[row].LocoAddress + "\n";
    file.print(message);
    Serial.print(".");
    row++;
  }
  Serial.println("Locos file written");
  file.close();

  locosDirty = 0;

  deleteFile(LittleFS, "/locos.bak");
  renameFile(LittleFS, "/locos.old", "/locos.bak");
  renameFile(LittleFS, "/locos.txt", "/locos.old");
  renameFile(LittleFS, "/locos.new", "/locos.txt");

  Serial.println("File Saved and Backup Created");
}

//
//*********************************************************************************************
//
//*********************************************************************************************
//
void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("- file renamed");
    } else {
        Serial.println("- rename failed");
    }
}
//
//*********************************************************************************************
//
//*********************************************************************************************
//
void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}
//
//****************************************************************************************************************
// Save Loco Functions
//****************************************************************************************************************
//
void saveFunctions(fs::FS &fs, const char * path, const char * message)
{
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, "w");
  if(!file)
  {
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message))
  {
    Serial.println("Writing File");
  } else 
  {
    Serial.println("- write failed");
    file.close();
    return;
  }
  int row = 0;
  while(hcLoco[row].LocoAddress !=0)
  {
    for(uint8_t j = 0; j < numFSlots; j++)
    {
      if(hcLoco[row].FSlot[j][0] != 255)
      {
        uint8_t m = 0;
        uint8_t n = hcLoco[row].FSlot[j][0];
        if(hcLoco[row].FSlot[j][1] != 0)
        {
          if((n & 0x80) != 0)
          {
            m = 1;                              //M = 1 means momentary
            n = n & 0x7F;                       //Only leave the Function Number
          }
          String record = String(hcLoco[row].LocoId) + "," + n + "," + 
                          String(hcLoco[row].FSlot[j][1]) + "," + String(hcLoco[row].FSlot[j][2]) + "," + 
                          m + "\n";
          file.print(record);
        }
      }
    }
    row++;
  }
  Serial.println("Functions file written");
  file.close();

  functionsDirty = 0;

  deleteFile(LittleFS, "/functions.bak");
  renameFile(LittleFS, "/functions.old", "/functions.bak");
  renameFile(LittleFS, "/functions.txt", "/functions.old");
  renameFile(LittleFS, "/functions.new", "/functions.txt");
}
//
//****************************************************************************************************************
// Throttles are stored in a multi-dimensional Array "SelectedIDs"
// SelectedIDs has four first level elements (0,1,2, &3) 
// each having 10 LocoIDs
// An unoccupied element contains "255" - ie a LocoID of 0 is invalid
//****************************************************************************************************************
//
void saveThrottles(fs::FS &fs, const char * path, const char * message)
{
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, "w");
  if(!file)
  {
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message))
  {
    Serial.println("Writing File");
  } else 
  {
    Serial.println("- write failed");
    file.close();
    return;
  }
  for(uint8_t i = 0; i < tCount; i++)            //tCount = throttle Count (4)
  {
    for(uint8_t j = 0; j < numLocoSlots; j++)  //numLocoSlots (10)
    {
      if(selectedIDs[i][j] != 255)
      {
        String record = String(i) + "," + String(j) + "," + String(selectedIDs[i][j]) + "\n";
        file.print(record);
      }
    }
  }
  Serial.println("Throttles file written");
  file.close();

  throttlesDirty = 0;

  deleteFile(LittleFS, "/throttleids.bak");
  renameFile(LittleFS, "/throttleids.old", "/throttleids.bak");
  renameFile(LittleFS, "/throttleids.txt", "/throttleids.old");
  renameFile(LittleFS, "/throttleids.new", "/throttleids.txt");
}
//
//****************************************************************************************************************
// Save all Accessories
//****************************************************************************************************************
//
void saveAccessories(fs::FS &fs, const char * path, const char * message)
{
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, "w");
  if(!file)
  {
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message))
  {
    Serial.println("Writing File");
  } else 
  {
    Serial.println("- write failed");
    file.close();
    return;
  }
  int row = 0;

  while(hcAcc[row].AccAddress != 0)
  {
    String record = hcAcc[row].AccId + "," + hcAcc[row].AccName + "," + hcAcc[row].AccAddress + "," + hcAcc[row].AccImage + "," + hcAcc[row].AccType + "\n" ;
    file.print(record);
    row++;
  }

  Serial.println("Accessories file written");
  file.close();

  accessoriesDirty = 0;

  deleteFile(LittleFS, "/accessories.bak");
  renameFile(LittleFS, "/accessories.old", "/accessories.bak");
  renameFile(LittleFS, "/accessories.txt", "/accessories.old");
  renameFile(LittleFS, "/accessories.new", "/accessories.txt");
}
//
//****************************************************************************************************************
// Save All Routes
//****************************************************************************************************************
//
void saveRoutes(fs::FS &fs, const char * path, const char * message)
{
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, "w");
  if(!file)
  {
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message))
  {
    Serial.println("Writing File");
  } else 
  {
    Serial.println("- write failed");
    file.close();
    return;
  }
  for(uint8_t row = 0; row < numRoutes; row++)            //numRoutes = throttle Count (4)
  {
    if(hcRoute[row].AccIDs[0][0] != 255)
    {
      String record = String(row) + "," + hcRoute[row].AccIDs[0][0] + "," + hcRoute[row].AccIDs[0][1] + "," +
                                          hcRoute[row].AccIDs[1][0] + "," + hcRoute[row].AccIDs[1][1] + "," +
                                          hcRoute[row].AccIDs[2][0] + "," + hcRoute[row].AccIDs[2][1] + "," +
                                          hcRoute[row].AccIDs[3][0] + "," + hcRoute[row].AccIDs[3][1] + "," +
                                          hcRoute[row].AccIDs[4][0] + "," + hcRoute[row].AccIDs[4][1] + "," +
                                          hcRoute[row].AccIDs[5][0] + "," + hcRoute[row].AccIDs[5][1] + "\n";
      Serial.print(record);
    }
  }
  Serial.println("Routes file written");
  file.close();

  routesDirty = 0;

  deleteFile(LittleFS, "/routes.bak");
  renameFile(LittleFS, "/routes.old", "/routes.bak");
  renameFile(LittleFS, "/routes.txt", "/routes.old");
  renameFile(LittleFS, "/routes.new", "/routes.txt");
}
//
//****************************************************************************************************************
// ToDo
//****************************************************************************************************************
//
void saveCredentials(fs::FS &fs, const char * path, const char * message)
{
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, "w");
  if(!file)
  {
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message))
  {
    Serial.println("Writing File");
  } else 
  {
    Serial.println("- write failed");
    file.close();
    return;
  }
  for(uint8_t row = 0; row < 10; row++)
  {
    String message = String(cred[row].ID) + "," + cred[row].SSID + "," + cred[row].Password + "," + cred[row].IPAddress + "," + cred[row].Port + "\n";
    file.print(message);
  }
  Serial.println("Credentials file written");
  file.close();

  credentialsDirty = 0;

  deleteFile(LittleFS, "/credentials.bak");
  renameFile(LittleFS, "/credentials.old", "/credentials.bak");
  renameFile(LittleFS, "/credentials.txt", "/credentials.old");
  renameFile(LittleFS, "/credentials.new", "/credentials.txt");
}
//
//****************************************************************************************************************
//
//****************************************************************************************************************
//
void restoreLittleFS()
{

}
