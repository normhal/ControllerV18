//LittleFS Routines
//ListDir is only used at bootup time for debug purposes
//

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
  CSV_Parser cp(/*format*/ "udsssudud-", /*has_header*/ true, /*delimiter*/ ',');

  if(cp.readFSfile(path))      // Serial.println("readFSfile failed");
  {
    uint16_t *locoId = (uint16_t *)cp["ID"];
    char **locoLongName = (char **)cp["LongName"];
    char **locoShortName = (char **)cp["ShortName"];
    char **locoType = (char **)cp["Type"];
    uint16_t *locoRNum = (uint16_t *)cp["RoadNumber"];
    uint16_t *locoAddress = (uint16_t *)cp["Address"];
    uint16_t *locoSlot = (uint16_t *)cp["Slot"];            //This should not be here - must be in the specific throttle array

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
