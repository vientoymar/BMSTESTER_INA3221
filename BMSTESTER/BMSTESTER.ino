#include <Beastdevices_INA3221.h> 
#include <Wire.h> //I2C
#include <Adafruit_GFX.h> //Oled Screen
#include <Adafruit_SSD1306.h> //Oled Screen

Adafruit_SSD1306 display(128, 64, &Wire, -1); // Set the Oled Screen. Reset pin # (or -1 if sharing Arduino reset pin)

// Next we will name each INA3221 according with the address given by soldering the jumpers
Beastdevices_INA3221 ina3221_1(INA3221_ADDR40_GND); //The INA3221 with the jumper GND soldered will be the ina3221_1
Beastdevices_INA3221 ina3221_2(INA3221_ADDR42_SDA); //The INA3221 with the jumper SDA soldered will be the ina3221_2
    //Explanation of I2C address for INA3221:
    //INA3221_ADDR40_GND = 0b1000000, // A0 pin -> GND
    //INA3221_ADDR41_VCC = 0b1000001, // A0 pin -> VCC
    //INA3221_ADDR42_SDA = 0b1000010, // A0 pin -> SDA
    //INA3221_ADDR43_SCL = 0b1000011  // A0 pin -> SCL


void setup()
{
  Serial.begin(115200);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);

  ina3221_1.begin(); //begin and reset each INA3221
  ina3221_1.reset();
  ina3221_1.setShuntRes(100, 100, 100);//Values for shunt resistors in mOhm
  ina3221_2.begin();
  ina3221_2.reset();
  ina3221_2.setShuntRes(100, 100, 100);//Values for shunt resistors in mOhm
  
}
void loop()
{
  
  float current[4]; //create 4 values for current
  float voltage[4]; //create 4 values for voltage
  float Vref[3]; //create 3 values for adjust the voltage of each cell (mind each channel measure the total amount of cells)

  current[0] = ina3221_1.getCurrent(INA3221_CH1); //here we get the balancing current from the BMS of first cell connected to the first channel of the first INA3221
  voltage[0] = ina3221_1.getVoltage(INA3221_CH1); //here we get the voltage of first cell connected to the first channel of the first INA3221

  current[1] = ina3221_1.getCurrent(INA3221_CH2);
  voltage[1] = ina3221_1.getVoltage(INA3221_CH2);

  current[2] = ina3221_1.getCurrent(INA3221_CH3);
  voltage[2] = ina3221_1.getVoltage(INA3221_CH3);

  current[3] = ina3221_2.getCurrent(INA3221_CH1); 
  voltage[3] = ina3221_2.getVoltage(INA3221_CH1); //here we get the voltage of last cell connected to the first channel of the second INA3221

  Vref[0] = voltage[1] - voltage[0]; //adjust the voltage measure in the second channel of the first INA3221 minus the first channel to get the value of the second cell
  Vref[1] = voltage[2] - voltage[1];
  Vref[2] = voltage[3] - voltage[2];
  
  Serial.print("BAT1: "); // for checking values in the serial monitor
  Serial.print(current[0], 3);
  Serial.print("A, ");
  Serial.print(voltage[0], 3);
  Serial.println("V");
  Serial.print("BAT2: ");
  Serial.print(current[1], 3);
  Serial.print("A, ");
  Serial.print(Vref[0], 3);
  Serial.println("V");
  Serial.print("BAT3: ");
  Serial.print(current[2], 3);
  Serial.print("A, ");
  Serial.print(Vref[1], 3);
  Serial.println("V");
  Serial.print("BAT4: ");
  Serial.print(current[3], 3);
  Serial.print("A, ");
  Serial.print(Vref[2], 3);
  Serial.println("V");
  Serial.print("BAT: ");
  Serial.print(voltage[3], 3);
  Serial.println("V");     

  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.print("BAT1: ");
  display.print(current[0], 3);
  display.print("A, ");
  display.print(voltage[0], 3);
  display.println("V");
  display.print("BAT2: ");
  display.print(current[1], 3);
  display.print("A, ");
  display.print(Vref[0], 3);
  display.println("V");
  display.print("BAT3: ");
  display.print(current[2], 3);
  display.print("A, ");
  display.print(Vref[1], 3);
  display.println("V");
  display.print("BAT4: ");
  display.print(current[3], 3);
  display.print("A, ");
  display.print(Vref[2], 3);
  display.println("V");
  display.print("BAT: ");
  display.print(voltage[3], 3);
  display.println("V");
  display.display();
  delay(2000);
}
