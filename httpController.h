#include <HTTPClient.h>

HTTPClient http;

char* httpGetRequests(char url[]) {
  static char datas[300] = {0};
  http.begin(url); //Specify the URL
  int httpCode = http.GET();
  if (httpCode > 0) { //Check for the returning code

    String payload = http.getString();
    
    if (httpCode == 200) {
      payload.toCharArray(datas, 300);
      char *returnDatas = datas;
      return returnDatas;
      
    }
    else{
      return "!200";
      
    }
  }

  else {
    Serial.println("Error on HTTP request");
    return "ERROR";
  }


}

char* subStr (char* input_string, char *separator, int segment_number)
{
  char *act, *sub, *ptr;
  static char copy[100] = {0};
  int i;
 
  strcpy(copy, input_string);
  for (i = 1, act = copy; i <= segment_number; i++, act = NULL)
  {
    sub = strtok_r(act, separator, &ptr);
    if (sub == NULL) break;
  }
  return sub;
}

String getValueSplit(String data, char separator, int index)
{
  
  
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length();

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
