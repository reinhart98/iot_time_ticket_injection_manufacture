// Set up the rgb led names
uint8_t ledR = 19;
uint8_t ledG = 5;
uint8_t ledB = 18;

uint8_t ledArray[3] = {1, 2, 3}; // three led channels

const boolean invert = true; // set true if common anode, false if common cathode


uint32_t R, G, B;           // the Red Green and Blue color components
uint8_t brightness = 255;  // 255 is maximum brightness, but can be changed.  Might need 256 for common anode to fully turn off.

// the setup routine runs once when you press reset:
void initialize()
{

  ledcAttachPin(ledR, 1); // assign RGB led pins to channels
  ledcAttachPin(ledG, 2);
  ledcAttachPin(ledB, 3);

  // Initialize channels
  // channels 0-15, resolution 1-16 bits, freq limits depend on resolution
  // ledcSetup(uint8_t channel, uint32_t freq, uint8_t resolution_bits);
  ledcSetup(1, 12000, 8); // 12 kHz PWM, 8-bit resolution
  ledcSetup(2, 12000, 8);
  ledcSetup(3, 12000, 8);
}

// void loop runs over and over again
void onRed()
{
 
  ledcWrite(1, 255);
  ledcWrite(2, 0);
  ledcWrite(3, 0);

}

void onBlue()
{

  ledcWrite(1, 0);
  ledcWrite(2, 0);
  ledcWrite(3, 255);

}

void onGreen()
{
  
  ledcWrite(1, 0);
  ledcWrite(2, 255);
  ledcWrite(3, 0);

}

void onYellow() {
  ledcWrite(1, 255);
  ledcWrite(2, 215);
  ledcWrite(3, 0);
}

void onOver(){
  onRed();
  delay(500);
  onGreen();
  delay(500);
  onBlue();
  delay(500);
  onYellow();
  delay(500);  
}
