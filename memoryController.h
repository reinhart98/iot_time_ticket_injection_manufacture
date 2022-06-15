#include <EEPROM.h>
#include <ArduinoJson.h>
#include <string.h>

#define EEPROM_SIZE 73  //previous 68

void init_EEPROM() {
  EEPROM.begin(EEPROM_SIZE);
}

void clearEEPROM(char type) {
  if (type == 'a') {
    for (int i = 0 ; i < EEPROM_SIZE ; i++) {
      EEPROM.write(i, 0);

      //    Serial.println(i);
    }
    EEPROM.commit();
    Serial.println("EEPROM CLEARED");
  }
}

void saveDataToEEPROM(char type, int Addr = 0, int tempShoot = 0, boolean signalData = false, int wc = 0, char stringData[] = "") {   //signalData can also updateMode
  if (type == 'l') {
    EEPROM.write(Addr, signalData);
    EEPROM.commit();
  }
  else if (type == 't') {
    EEPROM.write(Addr, tempShoot);
    EEPROM.commit();
  }
  else if (type == 'm') {
    EEPROM.write(Addr, signalData);
    EEPROM.commit();
  }
  else if (type == 'r') {
    Serial.print("save wc to eeprom: ");
    Serial.println(wc);
    EEPROM.write(Addr, wc);
    EEPROM.commit();
    Serial.println("saved wocenter to EEPROM");
  }
  else if (type == 'd') {
    int minAddr = 9;
    int maxAddr = 35;
    int indexString = 0;
    while (minAddr <= maxAddr) {
      EEPROM.write(minAddr, stringData[indexString]);

      minAddr++;
      indexString++;
      if (indexString > maxAddr) {
        //do something for notif
        break;
      }
    }
    EEPROM.commit();
    Serial.println("saved SSID to EEPROM");
  }
  else if (type == 'p') {
    int minAddr = 36;
    int maxAddr = 50;
    int indexString = 0;
    while (minAddr <= maxAddr) {
      EEPROM.write(minAddr, stringData[indexString]);

      minAddr++;
      indexString++;
      if (indexString > maxAddr) {
        //do something for notif
        break;
      }
    }
    EEPROM.commit();
    Serial.println("saved PASSWORD to EEPROM");
  }
  else if (type == 'b') {
    int minAddr = 51;
    int maxAddr = 65;
    int indexString = 0;
    while (minAddr <= maxAddr) {
      EEPROM.write(minAddr, stringData[indexString]);

      minAddr++;
      indexString++;
      if (indexString > maxAddr) {
        //do something for notif
        break;
      }
    }
    EEPROM.commit();
    Serial.println("saved BROKER_IP to EEPROM");
  }
  else if (type == 'v') {
    int minAddr = 68;
    int maxAddr = 72;
    int indexString = 0;
    while (minAddr <= maxAddr) {
      EEPROM.write(minAddr, stringData[indexString]);

      minAddr++;
      indexString++;
      if (indexString > maxAddr) {
        //do something for notif
        break;
      }
    }
    EEPROM.commit();
    Serial.println("saved firmware version to EEPROM");
  }
}

char* firmwareVersion() {
  static char firmwareVersion[8] = {0};
  int minAddr = 68;
  int maxAddr = 72;
  for (int i = minAddr; i <= maxAddr; i++) {
    byte readValue = EEPROM.read(i);
    if (readValue == 0) {
      break;
    }

    char readValueData = char(readValue);
    strncat(firmwareVersion, &readValueData, 1);
  }


  char *valuefversion = firmwareVersion;
  return valuefversion;
}

char* readSSID() {
  static char ssid[24] = {0};
  int minAddr = 9;
  int maxAddr = 35;
  for (int i = minAddr; i <= maxAddr; i++) {
    byte readValue = EEPROM.read(i);
    if (readValue == 0) {
      break;
    }

    char readValueData = char(readValue);
    strncat(ssid, &readValueData, 1);
  }


  char *valuessid = ssid;
  return valuessid;
}

char* readPASS() {
  static char pass[24] = {0};

  int minAddr = 36;
  int maxAddr = 50;
  for (int i = minAddr; i <= maxAddr; i++) {
    byte readValue = EEPROM.read(i);
    if (readValue == 0) {
      break;
    }

    char readValueData = char(readValue);
    strncat(pass, &readValueData, 1);
  }


  char *valuepass = pass;
  return valuepass;
}

char* readBrokerIP() {
  static char brokerIp[24] = {0};

  int minAddr = 51;
  int maxAddr = 65;
  for (int i = minAddr; i <= maxAddr; i++) {
    byte readValue = EEPROM.read(i);
    if (readValue == 0) {
      break;
    }

    char readValueData = char(readValue);
    strncat(brokerIp, &readValueData, 1);
  }
  //    Serial.print("BROKER IP in FUNCTION : ");
  //    Serial.println(brokerIp);


  char *valuebrokerIp = brokerIp;
  return valuebrokerIp;
}

char* readWOCenter() {
  static char wocenter[4] = {0};
  int savedVal = EEPROM.read(66);
  Serial.print("savedVal:");
  Serial.println(savedVal);
  if (savedVal < 100 && savedVal > 9) {
    sprintf(wocenter, "0%d", savedVal);
  }
  else if (savedVal < 10) {
    sprintf(wocenter, "00%d", savedVal);
  }
  else if (savedVal > 99) {
    sprintf(wocenter, "%d", savedVal);
  }

  char* valueWOCenter = wocenter;
  return valueWOCenter;

}

int readTempShoot() {
  int tmpData = 0;
  for (int i = 0; i <= 4; i++) {
    int getVal = EEPROM.read(i);
    if (getVal > 0) {
      tmpData = tmpData + getVal;
    }
  }
  return tmpData;
}

char* convertParseIntToChar(int integer) {
  static char charInteger[4] = {0};
  sprintf(charInteger, "%d", integer);
  char* valueCharInteger = charInteger;
  return valueCharInteger;

}

char* formatDataToJson(int shoot, char wocenter[]) {
  static char jsonkey[512];
  StaticJsonDocument<512>doc;
  doc["event"] = "shoot";
  doc["shoot"] = shoot;
  doc["wocenter"] = wocenter;
  serializeJson(doc, jsonkey);
  char* jsonData = jsonkey;

  return jsonData;
}

char* formatDataToJsonMqttStats(char Stats[], char desc[]) {
  static char jsonkey[512];
  StaticJsonDocument<512>doc;
  doc["event"] = "mqttStats";
  doc["mqttStatus"] = Stats;
  doc["desc"] = desc;
  serializeJson(doc, jsonkey);
  char* jsonData = jsonkey;

  return jsonData;
}

char* var(String datas){
  static char charBuf[50];
  datas.toCharArray(charBuf, 50);
  char* returnDatas = charBuf;
  return returnDatas;
}
