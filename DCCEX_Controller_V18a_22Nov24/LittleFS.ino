//LittleFS Routines
//ListDir is only used at bootup time for debug purposes
//
//****************************************************************************************************************
// Data Files Used by LittleFS
//****************************************************************************************************************
//
String fileList[20] =
{
  "accessories.txt",
  "credentials.txt",
  "functions.txt",
  "locos.txt",
  "routes.txt",
  "throttleids.txt"
};

void listDir(fs::FS &fs, const char *dirname, uint8_t levels) 
{
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
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
  Serial.println("Calling Acc Parser");
  CSV_Parser cp(/*format*/ "udsududud", /*has_header*/ true, /*delimiter*/ ',');

  if(cp.readFSfile(path))      // Serial.println("readFSfile failed");
  {
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
  }
  Serial.print("Acc Rows Read: ");
  Serial.println(cp.getRowsCount());
}
//****************************************************************************************************************
//  Parser Routine which loads the Locos file from LittleFS
//****************************************************************************************************************
void populateLocoArray(const char *path)
{
  Serial.println("Calling Locos Parser");
  CSV_Parser cp(/*format*/ "udsssudud", /*has_header*/ true, /*delimiter*/ ',');

  if(cp.readFSfile(path))      // Serial.println("readFSfile failed");
  {
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
      hcLoco[row].LocoId = locoId[row];
      hcLoco[row].LocoLongName = locoLongName[row];
      hcLoco[row].LocoShortName = locoShortName[row];
      hcLoco[row].LocoType = locoType[row];
      hcLoco[row].LocoRNum = locoRNum[row];
      hcLoco[row].LocoAddress = locoAddress[row];
//      Serial.printf("Row: %d Loco ID: %d\n", row, locoId[row]);
    }
  }
}
//****************************************************************************************************************
//  Parser Routine which loads the Function file from LittleFS and populates the related Locos
//****************************************************************************************************************
void populateLocoFunctions(const char *path)
{
  CSV_Parser cp(/*format*/ "ududududud");       //5x uint16_t values
  int row = 0;
  Serial.print("Reading from file: ");
  Serial.println(path);

  if(cp.readFSfile(path))      // Serial.println("readFSfile failed");
  {
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
  }
}
//****************************************************************************************************************
//  Parser Routine which loads the Credentials Array from LittleFS
//****************************************************************************************************************
void populateCredArray(const char *path)
{
  Serial.println("Calling Credentials Parser");
  CSV_Parser cp(/*format*/ "udsssud", /*has_header*/ true, /*delimiter*/ ',');

  if(cp.readFSfile(path))      // Serial.println("readFSfile failed");
  {
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
  }
  Serial.print("Credential Rows Read: ");
  Serial.println(cp.getRowsCount());
}
//****************************************************************************************************************
//  Parser Routine which loads the Selected Throttle IDs Array from LittleFS
//****************************************************************************************************************
void populateSelected(const char *path)
{
  Serial.println("Calling Selected IDs Parser");
  CSV_Parser cp(/*format*/ "ucucuc", /*has_header*/ true, /*delimiter*/ ',');   //Throttle,LocoID,Slot

  if(cp.readFSfile(path))      // Serial.println("readFSfile failed");
  {
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
  }
  Serial.print("Selected ID Rows Read: ");
  Serial.println(cp.getRowsCount());
}
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
///
void saveAccs()
{
  int row = 0;
  String record = "ID,Name,Address,Image,Type\n";
  Serial.print(record);
  while(hcAcc[row].AccAddress !=0)
  {
    String record = String(hcAcc[row].AccId) + "," + hcAcc[row].AccName + "," + hcAcc[row].AccAddress + "," + hcAcc[row].AccImage + "," + hcAcc[row].AccType + "\n" ;
    Serial.print(record);
    row++;
  }
  Serial.println();
  row = 0;
  record = "ID,LongName,ShortName,Type,RoadNumber,Address\n";
  Serial.print(record);
  while(hcLoco[row].LocoAddress !=0)
  {
    String record = String(hcLoco[row].LocoId) + "," + hcLoco[row].LocoLongName + "," + hcLoco[row].LocoShortName + "," + hcLoco[row].LocoType + "," + hcLoco[row].LocoRNum + "," + hcLoco[row].LocoAddress + "\n";
    Serial.print(record);
    row++;
  }
}