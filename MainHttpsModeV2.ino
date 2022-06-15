#include "lcdTextController.h"
#include "connectionController.h"
//#include "memoryController.h"
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
#include <esp_task_wdt.h>


TaskHandle_t Task1;     //Initialization Task
TaskHandle_t Task2;     //for each Core

boolean wifiConnected;

boolean clientConStats;
boolean setupState;

int token;
boolean tokenGen = false;

char* savedssid;
char* savedpass;
char* mqtt_server;
char* savedwocenter;

char* tempShootFormatted;

char publishEnd[25];
char subsEnd[25];

#define TFT_GREY 0x5AEB

//--shoot4count needed variable
//machine state
boolean a = false;
boolean b = false;
boolean c = false;
boolean errstate = false;
//send state
boolean sendstate = false;
boolean errsendstate = false;
//machine pin GPIO
const int proses1 = 32;
const int proses2 = 35; //< 15
const int proses3 = 34;
//others
char type[20];

int latestSignalStats = 0;

//for reset button
const int BUTTON_PIN = 33; // the number of the pushbutton pin
const int SHORT_PRESS_TIME = 5000; // 500 milliseconds

// Variables will change:
int lastState = HIGH;  // the previous state from the input pin
int currentState;     // the current reading from the input pin
unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;


void init_setupALL() {
  //  saveDataToEEPROM('d', 0, 0, false, 0, "BANGGACOSMOS");
  //  saveDataToEEPROM('p', 0, 0, false, 0, "Cosmos*123");
  //  delay(1000);
  delay(100);
  savedssid = readSSID();
  savedpass = readPASS();
  savedwocenter = readWOCenter();

  Serial.println("=====");
  delay(100);
  //  setTextLCD(0, 3, "restart...");
  Serial.print("SSID : ");
  Serial.println(savedssid);
  Serial.print("PASS : ");
  Serial.println(savedpass);
  Serial.print("WOCENTER : ");
  Serial.println(savedwocenter);

  if (strcmp(savedssid, "") == 0 && strcmp(savedpass, "") == 0) {
    saveDataToEEPROM('d', 0, 0, false, 0, "BANGGACOSMOS");
    saveDataToEEPROM('p', 0, 0, false, 0, "Cosmos*123");
    delay(1000);

    savedssid = readSSID();
    savedpass = readPASS();
    Serial.print("SSID RESET TO DEFAULT: ");
    Serial.println(savedssid);
    Serial.print("PASS RESET TO DEFAULT: ");
    Serial.println(savedpass);
  }

  //  if (strcmp(savedwocenter, "000") == 0) {
  //    Serial.println("NEW DEVICE");
  //    clearlcd();
  //    setTextLCD(0, 0, "NEW DEVICE");
  //    setTextLCD(0, 1, "NO WOCENTER");
  //    setTextLCD(0, 2, savedwocenter);
  //
  //  }

}

void connection() {
  boolean wifi = setup_wifi(savedssid, savedpass);
  Serial.println(wifi);
}

//END CODE FOR PROSES 1 IN CORE 0


void saveTempShootData(int tempShoot) {
  if (tempShoot <= 255) {
    saveDataToEEPROM('t', 0, tempShoot);
  }
  else if (tempShoot > 255 && tempShoot <= 510) {
    int numSave = tempShoot - 255;
    saveDataToEEPROM('t', 0, 255);
    saveDataToEEPROM('t', 1, numSave);
  }
  else if (tempShoot > 510 && tempShoot <= 765) {
    int numSave = tempShoot - 510;
    saveDataToEEPROM('t', 0, 255);
    saveDataToEEPROM('t', 1, 255);
    saveDataToEEPROM('t', 2, numSave);
  }
  else if (tempShoot > 765 && tempShoot <= 1020) {
    int numSave = tempShoot - 765;
    saveDataToEEPROM('t', 0, 255);
    saveDataToEEPROM('t', 1, 255);
    saveDataToEEPROM('t', 2, 255);
    saveDataToEEPROM('t', 3, numSave);
  }
  else if (tempShoot > 1020 && tempShoot <= 1275) {
    int numSave = tempShoot - 1020;
    saveDataToEEPROM('t', 0, 255);
    saveDataToEEPROM('t', 1, 255);
    saveDataToEEPROM('t', 2, 255);
    saveDataToEEPROM('t', 3, 255);
    saveDataToEEPROM('t', 4, numSave);
  }
  else {
    //do something if more temporary
  }
}

void processShootData(int shoot) {
  boolean wifiConn = checkConn();
  Serial.print("wifistas: ");
  Serial.println(wifiConn);
  int readeepromShoot = readTempShoot();
  int totData = shoot + readeepromShoot;
  Serial.print("wifi Conn status : ");
  Serial.println(wifiConn);
  //  char* res = convertParseIntToChar(totData);
  //convert res into json format first
  //  char* res = formatDataToJson(totData, savedwocenter);
  char result[110] = {0};
  sprintf(result, "http://192.168.3.23:20003/api/shootHandler?param={'event':'shoot','shoot':%d,'wocenter':'%s'}", totData, savedwocenter);
  char* requestget = httpGetRequests(result);

  if (!wifiConn || strcmp(requestget, "!200") == 0 || strcmp(requestget, "ERROR") == 0) {
    //save to eeprom
    int eepromValCheck = readTempShoot();
    Serial.print("inserted : ");
    Serial.println(shoot);
    Serial.print("tempshoot : ");
    Serial.println(eepromValCheck);
    int newtempshoot = shoot + eepromValCheck;
    Serial.print("newtempshoot : ");
    Serial.println(newtempshoot);
    saveTempShootData(newtempshoot);

    int currentTemp = readTempShoot();
    char charCurrent[20] = {0};
    sprintf(charCurrent, "CURR TEMP DATA : %d", currentTemp);
    char* currentTempChar = charCurrent;
    //    clearlcd();
    ///show lcd details
    WCLatestDetails(false, savedwocenter, currentTemp);

  } else {
    //    we want display lcd too later here
    //    clearlcd();
    Serial.println(requestget);
    char c2[100];
    strcpy(c2, requestget);
    String dd = String(c2);

    String woid = getValueSplit(dd, '-', 1);
    String counter = getValueSplit(dd, '-', 2);
    char cwoid[25];
    char ccounter[25];
    woid.toCharArray(cwoid, 25);
    counter.toCharArray(ccounter, 25);
    //    setTextLCD(0, 0, woid);
    //    setTextLCD(0, 1, counter);

    WCLatestDetails(true, savedwocenter, 0, cwoid, ccounter);
    //clear eeprom temp
    for (int i = 0; i <= 4; i++) {
      EEPROM.write(i, 0);
    }
    EEPROM.commit();
  }

}

void signal_readerLogic() {

  if (!digitalRead(proses1)) {
    if (!digitalRead(proses1) && !digitalRead(proses2) && !digitalRead(proses3)) {
      strcpy(type, "mesin error");
      Serial.println(type);
      errstate = true;
    }
    else {
      if (b) {
        a = true;
        c = false;
      }
      else {
        a = true;
        b = false;
        c = false;
      }
      //      Serial.println("true");
      EEPROM.write(5, a);
      EEPROM.write(6, b);
      EEPROM.write(7, c);
      EEPROM.commit();
    }

  }
  if (!digitalRead(proses2)) {
    a = EEPROM.read(5);
    if (a == true) {
      if (!digitalRead(proses1) && !digitalRead(proses2) && !digitalRead(proses3)) {
        strcpy(type, "mesin error");
        Serial.println(type);
        errstate = true;
      }
      else {
        b = true;
        c = false;
        EEPROM.write(6, b);
        EEPROM.write(7, c);
        EEPROM.commit();
      }
    }
    else {
      a = false;
      b = false;
      c = false;
      EEPROM.write(5, a);
      EEPROM.write(6, b);
      EEPROM.write(7, c);
      EEPROM.commit();
    }
  }
  if (!digitalRead(proses3)) {
    b = EEPROM.read(6);
    if (b == true) {
      if (!digitalRead(proses1) && !digitalRead(proses2) && !digitalRead(proses3)) {
        strcpy(type, "mesin error");
        Serial.println(type);
        errstate = true;
      }
      else {
        c = true;
        EEPROM.write(7, c);
        EEPROM.commit();
      }
    }
    else {
      a = false;
      b = false;
      c = false;
      EEPROM.write(5, a);
      EEPROM.write(6, b);
      EEPROM.write(7, c);
      EEPROM.commit();
    }
  }
}

int shootCountLogic() {
  boolean close_process = EEPROM.read(5);
  boolean inject_process = EEPROM.read(6);
  boolean open_process = EEPROM.read(7);
  //  setTextLCD(0, 3, String(close_process));
  //  setTextLCD(2, 3, String(inject_process));
  //  setTextLCD(4, 3, String(open_process));
  //  cycleText(close_process, inject_process, open_process);
  if (close_process && inject_process && open_process) {
    for (int j = 5; j <= 7; j++) {
      EEPROM.write(j, 0);
    }
    EEPROM.commit();
    return 1;
  }
  return 0;
}

void buttonReset() {
  // START read the state of the switch/button:
  currentState = digitalRead(BUTTON_PIN);
  //    Serial.println(currentState);

  if (lastState == HIGH && currentState == LOW)       // button is pressed
    pressedTime = millis();
  else if (lastState == LOW && currentState == HIGH) { // button is released
    releasedTime = millis();

    long pressDuration = releasedTime - pressedTime;

    if ( pressDuration < SHORT_PRESS_TIME ) {
      Serial.println("A short press is detected,SKIP");

    }
    else {
      Serial.println("long press");
      //      setTextLCD(0, 1, "RESET             ");
      tft.fillScreen(TFT_WHITE);
      tft.setCursor((tft.width() / 2 - tft.width() / 4), tft.height() - 220);
      tft.setTextSize(2);
      tft.setTextColor(TFT_BLACK);
      tft.println("RESET BUTTON PRESSED:");
      clearEEPROM('a');
      setupState = true;
      EEPROM.write(67, setupState);
      EEPROM.commit();
      delay(1000);
      ESP.restart();
    }

  }

  // save the the last state
  lastState = currentState;
  // END read the state of the switch/button:
}

void setupMode() {
  tft.fillScreen(TFT_WHITE);
  //  list the setup mode, with red circle for status
  titleSetupMode();
  updateStateList();

  redcircle(10, tft.height() - 110);
  defaultConnectionConnectStats();
  redcircle(10, tft.height() - 80);
  ssidStats();
  redcircle(10, tft.height() - 50);
  pwdStats();
  redcircle(10, tft.height() - 20);
  wcStats();

  if (!tokenGen) {
    token = random(10000000, 99999999);
    tokenGen = true;
  }
  tokendisplay(token);
  char urlSett[100] = {0};
  sprintf(urlSett, "http://192.168.3.23:20003/api/getsetup?token=%d", token);
  Serial.println(urlSett);

  char* getSetup = httpGetRequests(urlSett);
  boolean chkConnection = checkConn();
  Serial.println(getSetup);
  if (strcmp(getSetup, "!200") == 0 || strcmp(getSetup, "ERROR") == 0 ) {
    //    DO SOMETHING IF FAIL REQUESTS
    if (chkConnection == false) {
      Serial.println("inconnect");
    } else {
      Serial.println("connect");
      greencircle(10, tft.height() - 110);
    }
    Serial.println("failed");
    delay(2000);
  } else {
    char c4[100];
    strcpy(c4, getSetup);
    greencircle(10, tft.height() - 110);
    String settWOCenter = getValueSplit(String(c4), '-', 1);
    String settSSID = getValueSplit(String(c4), '-', 2);
    String settPWD = getValueSplit(String(c4), '-', 3);
    Serial.println(settWOCenter);
    if (settWOCenter == "ERROR") {
      delay(2000);
      tft.fillScreen(TFT_WHITE);
      tft.setCursor(10, tft.height() - 150);
      tft.setTextSize(2);
      tft.setTextColor(TFT_BLACK);
      tft.println("SETUP WC ERROR/FAILED");
      delay(3000);
      return;
    } else {
      int upwocenter = settWOCenter.toInt();
      Serial.print("upwocenter before save: ");
      Serial.println(upwocenter);
      saveDataToEEPROM('r', 66, 0, false, upwocenter);
      greencircle(10, tft.height() - 20);
      delay(500);
      char ssiddata[30] = {0};
      settSSID.toCharArray(ssiddata, 30);
      saveDataToEEPROM('d', 0, 0, false, 0, ssiddata);
      greencircle(10, tft.height() - 80);
      delay(500);
      char uppwd[30] = {0};
      settPWD.toCharArray(uppwd, 30);
      saveDataToEEPROM('p', 0, 0, false, 0, uppwd);
      greencircle(10, tft.height() - 50);
      delay(500);

      Serial.println(settWOCenter);
      Serial.println(upwocenter);
      Serial.println(ssiddata);
      Serial.println(uppwd);

      setupState = false;
      Serial.println("update setup state");
      delay(2000);
      EEPROM.write(67, setupState);
      EEPROM.commit();
      Serial.println("restart in 5 sec");
      setupState = EEPROM.read(67);
      Serial.print("setupState now : ");
      Serial.println(setupState);
      delay(2000);
      restartESPAfterUpdateSetup();
      //      delay(5000);
      //      ESP.restart();
    }
  }

}

void showSignalStatsLatest() {
  boolean close_process = EEPROM.read(5);
  boolean inject_process = EEPROM.read(6);
  boolean open_process = EEPROM.read(7);
  //  setTextLCD(0, 3, String(close_process));
  //  setTextLCD(2, 3, String(inject_process));
  //  setTextLCD(4, 3, String(open_process));
  cycleText(close_process, inject_process, open_process);
}

//  ###### MAIN CODE ######
void setup() {
  Serial.begin(115200);
  init_EEPROM();
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_WHITE);
  //  clearEEPROM('a');////////////
  pinMode(proses1, INPUT);
  pinMode(proses2, INPUT);
  pinMode(proses3, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  setupState = EEPROM.read(67);
  //  TESTING FUNCTION
  init_setupALL();             // use a WiFi function
  delay(100);
  //  Serial.println("FORCE SETUPSTATE");
  //  clearEEPROM('a');
  //  setupState = true;
  //  EEPROM.write(67, setupState);
  //  EEPROM.commit();
  //  Serial.println("DONE FORCE TRUE SETUP STATE");
  //  delay(60000);
  //START create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
    Task1code,   /* Task function. */
    "process&send",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task1,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */
  delay(500);
  xTaskCreatePinnedToCore(
    Task2code,   /* Task function. */
    "callback&display",     /* name of task. */
    30000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task2,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */
  delay(500);
  //END create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1

}

//START CODE FOR PROSES 1 IN CORE 0
void Task1code(void *pvParameters) {
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());


  for (;;) {
    TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
    TIMERG0.wdt_feed = 1;
    TIMERG0.wdt_wprotect = 0;
    boolean chkCon = checkConn();
    if (!chkCon) {
      connection();
    }
  }
}

//START CODE FOR PROSES 2 IN CORE 1
void Task2code( void * pvParameters ) {
  Serial.print("Task2 running on core ");
  WCLatestDetails(false, savedwocenter);
  Serial.println(xPortGetCoreID());


  for (;;) {
    if (!setupState) {
      //read signal from cb and save to eeprom
      signal_readerLogic();
      //read signal from eeprom
      int shot = shootCountLogic();
      //    Serial.println(setupState);
      if (shot != 0) {
        Serial.print("satu dari tombol : ");
        Serial.println(shot);
        processShootData(shot);
      } else {
        latestSignalStats++;
        if (latestSignalStats == 150) {
          showSignalStatsLatest();
          latestSignalStats = 0;
        }
      }
    } else {
      setupMode();

    }

    buttonReset();
  }
  delay(10);
}

//MAIN LOOP CODE
void loop() {

}
