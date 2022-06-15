//too lazy for make it one function
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
TFT_eSPI tft = TFT_eSPI();

void greencircle(int x, int y) {
  tft.drawCircle(x, y - 5, 5, TFT_GREEN);
  tft.fillCircle(x, y - 5, 5, TFT_GREEN);
}

void redcircle(int x, int y) {
  tft.drawCircle(x, y - 5, 5, TFT_RED);
  tft.fillCircle(x, y - 5, 5, TFT_RED);
}

void titleSetupMode() {
  tft.setCursor((tft.width() / 2 - tft.width() / 4), tft.height() - 220);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.println("SETUP MODE:");
}

void updateStateList() {
  tft.setCursor(10, tft.height() - 180);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.println("UPDATE SETUP STATE:");
}

void tokendisplay(int token) {
  tft.setCursor(10, tft.height() - 150);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.print("Token: ");
  tft.println(token);
}

void defaultConnectionConnectStats() {
  tft.setCursor(30, tft.height() - 120);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.println("DEFAULT CONN STATS");
}

void ssidStats() {
  tft.setCursor(30, tft.height() - 90);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.println("SSID UPDATED");
}

void pwdStats() {
  tft.setCursor(30, tft.height() - 60);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.println("PASSWORD UPDATED");
}
void wcStats() {
  tft.setCursor(30, tft.height() - 30);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.println("WC UPDATED");
}

void restartESPAfterUpdateSetup() {
  tft.fillScreen(TFT_WHITE);
  tft.setCursor((tft.width() / 2 - tft.width() / 4), tft.height() - 220);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.println("RESTARTING IN:");
  for (int i = 5; i > 0; i--) {
    tft.fillCircle((tft.width() / 2 ), tft.height() - 150, 40, TFT_WHITE);
    tft.setCursor((tft.width() / 2 ), tft.height() - 150);
    tft.setTextSize(4);
    tft.setTextColor(TFT_BLACK);
    tft.println(i);
    delay(1000);
  }
  ESP.restart();
}

//start for detail machine
void WCLatestDetails(boolean connStats, char* wocenter, int tempCounter = 0, char woid[] = "-", char currcount[] = "T/C: - / -") {
  char title[25];
  sprintf(title, "WC_%s Latest Details", wocenter);
  tft.fillScreen(TFT_WHITE);
  tft.setCursor((tft.width() / 4 - tft.width() / 6), tft.height() - 220);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.println(title);

  tft.setCursor(10, tft.height() - 180);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.print("WOID: ");
  tft.println(woid);

  tft.setCursor(10, tft.height() - 150);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.println(currcount);

  tft.setCursor(10, tft.height() - 120);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.print("LATEST CONNECTION ");
  if (connStats) {
    tft.fillCircle(tft.width() - 80, tft.height() - 115, 5, TFT_GREEN);
  } else {
    tft.fillCircle(tft.width() - 80, tft.height() - 115, 5, TFT_RED);
  }

  tft.setCursor(10, tft.height() - 90);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.print("TEMP: ");
  tft.println(tempCounter);
  
}


void cycleText(boolean close_process, boolean inject_process, boolean open_process) {
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  if(close_process){
    tft.fillCircle(10, tft.height() - 30, 10, TFT_GREEN);
  }else{
    tft.fillCircle(10, tft.height() - 30, 10, TFT_RED);
  }
  
  //  tft.setCursor(13, tft.height() - 90);
  //  tft.print(close_process);

  if(inject_process){
    tft.fillCircle(40, tft.height() - 30, 10, TFT_GREEN);
  } else{
    tft.fillCircle(40, tft.height() - 30, 10, TFT_RED);
  }
  //  tft.setCursor(33, tft.height() - 90);
  //  tft.print(inject_process);

  if(open_process){
    tft.fillCircle(70, tft.height() - 30, 10, TFT_GREEN);
  } else{
    tft.fillCircle(70, tft.height() - 30, 10, TFT_RED);
  }
  
  //  tft.setCursor(53, tft.height() - 90);
  //  tft.print(open_process);
}
