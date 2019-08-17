/*
  F4HJH - August 2019
  Sends I2C commands to the Charly 25AB device on the I2C bus interface of the Arduino board
  Display state of band filter and PTT state with LEDs
  thanks to F6EHJ for his contributions

B00000001   6m        Port 1 : 0x03
B00000010   10/12m    Port 1 : 0x03
B00000100   15/17m    Port 1 : 0x03
B00001000   20m       Port 1 : 0x03
B00010000   30/40m    Port 1 : 0x03
B00100000   60/80m    Port 1 : 0x03
B01000000   160m      Port 1 : 0x03

Bxx11xxxx   PTT ON    Port 0 : 0x02
Bxx00xxxx   PTT OFF   Port 0 : 0x02

Per Amp and Att functions not implemented yet

*/

#include <Wire.h>

  byte PORT0_ = 0x02;        // port 0 on PCA9555
  byte PORT1_ = 0x03;        // port 1 on PCA9555
  byte COM;                  // port com for I2C device
  byte DATA;                 // Register value
  byte TRX_REG = B00000000;  // Register value for others controls (ptt, ant, pre amp, att)
  int BAND=2;                // Counter 80m default
  int i2c_address = 0x20;    // Default I2C address 0x20

// Optical coder
 #define CLK 10
 #define DT 9
 #define PTT 11              // Switch button for testing => Replace SW button with TTL outpour from RedPitaya for example
 
// Array for optical coder
  const unsigned char ttable[7][4] = {
  {0x0, 0x2, 0x4, 0x0}, {0x3, 0x0, 0x1, 0x10},
  {0x3, 0x2, 0x0, 0x0}, {0x3, 0x2, 0x1, 0x0},
  {0x6, 0x0, 0x4, 0x0}, {0x6, 0x5, 0x0, 0x20},
  {0x6, 0x5, 0x4, 0x0},
                                      }; 

volatile unsigned char state = 0;  // Status of the dial
 
  #define DIR_CCW 0x10        // optical coder : Anti Clockwise
  #define DIR_CW 0x20         // optical coder : Clockwise
 
// LED for each band group
   #define LED6M 13
   #define LED10M 3
   #define LED15M 4
   #define LED20M 5
   #define LED40M 6
   #define LED80M 7
   #define LED160M 8

// PTT control (0 = PTT ON)
   #define LEDPTT 12
   int PTTSTATE = HIGH;
   int PTT_LAST_STATE = HIGH;

//////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);         // open the serial port at 9600 bps:
  
// Optical encoder
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);
  pinMode(PTT, INPUT_PULLUP);
 
// LED for each band group
  pinMode (LED6M, OUTPUT);
  pinMode (LED10M, OUTPUT);
  pinMode (LED15M, OUTPUT);
  pinMode (LED20M, OUTPUT);
  pinMode (LED40M, OUTPUT);
  pinMode (LED80M, OUTPUT);
  pinMode (LED160M, OUTPUT);
  pinMode (LEDPTT, OUTPUT);

// I2C device setup     
  Wire.begin();  // Join i2c bus
  Wire.beginTransmission(i2c_address);
  Wire.write (0x006);           // configuration register
  Wire.write (0x00);            // port 0 all output
  Wire.write (0x00);            // port 1 all output 
  Wire.endTransmission();

// Set the default values
// 80m band
  COM=PORT1_;
  DATA=B00100000;
  load_I2C();
  digitalWrite(LED6M, LOW);
  digitalWrite(LED10M, LOW);
  digitalWrite(LED15M, LOW);
  digitalWrite(LED20M, LOW);
  digitalWrite(LED40M, LOW);
  digitalWrite(LED80M, HIGH);
  digitalWrite(LED160M, LOW);
// Set Pre Amp, Attenuator and PTT to OFF
  COM=PORT0_;
  DATA=B00000000;
  load_I2C();
}

//////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
  int btn = digitalRead(PTT);   //Read the state of the sw button in the loop
 
unsigned char result;           // Reset the counter
  
   while(1)
  {// Read the status of the dial
    unsigned char pinstate = (digitalRead(CLK) << 1) | digitalRead(DT);
    state = ttable[state & 0xf][pinstate];
   result=state&0x30;
    if(result==DIR_CCW) BAND++;
        if(result==DIR_CW) BAND--;
  
switch (BAND) {   // Band selection with rotary encoder

        case 7:  // 6 meter band value
          COM=PORT1_;
          DATA=B00000001;
          load_I2C();
          digitalWrite(LED6M, HIGH);
          digitalWrite(LED10M, LOW);
          digitalWrite(LED15M, LOW);
          digitalWrite(LED20M, LOW);
          digitalWrite(LED40M, LOW);
          digitalWrite(LED80M, LOW);
          digitalWrite(LED160M, LOW);
        break;
        
        case 6:  // 10/12 meter band value
          COM=PORT1_;
          DATA=B00000010;
          load_I2C();
          digitalWrite(LED6M, LOW);
          digitalWrite(LED10M , HIGH);
          digitalWrite(LED15M, LOW);
          digitalWrite(LED20M, LOW);
          digitalWrite(LED40M, LOW);
          digitalWrite(LED80M, LOW);
          digitalWrite(LED160M, LOW);
        break;
        
        case 5:  // 15/17 meter band value
          COM=PORT1_;
          DATA=B00000100;
          load_I2C();
          digitalWrite(LED6M, LOW);
          digitalWrite(LED10M, LOW);
          digitalWrite(LED15M, HIGH);
          digitalWrite(LED20M, LOW);
          digitalWrite(LED40M, LOW);
          digitalWrite(LED80M, LOW);
          digitalWrite(LED160M, LOW);
        break;

        case 4: // 20 meter band value
          COM=PORT1_;
          DATA=B00001000;
          load_I2C();
          digitalWrite(LED6M, LOW);
          digitalWrite(LED10M, LOW);
          digitalWrite(LED15M, LOW);
          digitalWrite(LED20M, HIGH);
          digitalWrite(LED40M, LOW);
          digitalWrite(LED80M, LOW);
          digitalWrite(LED160M, LOW);
        break;

        case 3: // 30/40 meter band value
          COM=PORT1_;
          DATA=B00010000;
          load_I2C();
          digitalWrite(LED6M, LOW);
          digitalWrite(LED10M, LOW);
          digitalWrite(LED15M, LOW);
          digitalWrite(LED20M, LOW);
          digitalWrite(LED40M, HIGH);
          digitalWrite(LED80M, LOW);
          digitalWrite(LED160M, LOW);
        break;

        case 2: // 60/80 meter band value
          COM=PORT1_;
          DATA=B00100000;
          load_I2C();
          digitalWrite(LED6M, LOW);
          digitalWrite(LED10M, LOW);
          digitalWrite(LED15M, LOW);
          digitalWrite(LED20M, LOW);
          digitalWrite(LED40M, LOW);
          digitalWrite(LED80M, HIGH);
          digitalWrite(LED160M, LOW);
        break;

        case 1: // 160 meter band value
          COM=PORT1_;
          DATA=B01000000;
          load_I2C();
          digitalWrite(LED6M, LOW);
          digitalWrite(LED10M, LOW);
          digitalWrite(LED15M, LOW);
          digitalWrite(LED20M, LOW);
          digitalWrite(LED40M, LOW);
          digitalWrite(LED80M, LOW);
          digitalWrite(LED160M, HIGH);
        break;         
  
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:

PTTSTATE = digitalRead(PTT);            // check the state of PTT (command or SW button)

if (PTTSTATE != PTT_LAST_STATE) {       // check the lats value of the ptt button if different => execute the routine

switch (PTTSTATE) {  

        case LOW:  // PTT ON
        TRX_REG = TRX_REG ^ B00110000;        // operand ^ with mask Bxx11xxxx to set two bits high
        COM=PORT0_;
        DATA=TRX_REG;
        load_I2C();
        digitalWrite(LEDPTT, HIGH);
        PTT_LAST_STATE=LOW;
        break;

        case HIGH:  // PTT OFF
        TRX_REG = TRX_REG & B11001111;        // operand & with mask Bxx00xxxx to set two bits low
        COM=PORT0_;
        DATA=TRX_REG;
        load_I2C();
        digitalWrite(LEDPTT, LOW);
        PTT_LAST_STATE=HIGH;
        break;
                  }
                              }
  }
}
 
void load_I2C()                               // Load value into register on port 0 or 1 on the device
  {
Wire.beginTransmission(i2c_address);//adresse I2C réputée correcte 
Wire.write (COM);// Chargement mot de configuration (sélection du port : 0x02 = port 0 / 0x03 =port 1
Wire.write (DATA);// Chargement port 
Wire.endTransmission();
 
  }

 
 


 
