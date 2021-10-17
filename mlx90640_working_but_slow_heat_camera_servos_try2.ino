#include <Adafruit_MLX90640.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

float frame[32*24]; // buffer for full frame of temperatures

#ifdef ESP8266
   #define STMPE_CS 16
   #define TFT_CS   0
   #define TFT_DC   15
   #define SD_CS    2
#elif defined(ESP32)
   #define STMPE_CS 32
   #define TFT_CS   15
   #define TFT_DC   33
   #define SD_CS    14
#elif defined(TEENSYDUINO)
   #define TFT_DC   10
   #define TFT_CS   4
   #define STMPE_CS 3
   #define SD_CS    8
#elif defined(ARDUINO_STM32_FEATHER)
   #define TFT_DC   PB4
   #define TFT_CS   PA15
   #define STMPE_CS PC7
   #define SD_CS    PC5
#elif defined(ARDUINO_NRF52832_FEATHER) /* BSP 0.6.5 and higher! */
   #define TFT_DC   11
   #define TFT_CS   31
   #define STMPE_CS 30
   #define SD_CS    27
#elif defined(ARDUINO_MAX32620FTHR) || defined(ARDUINO_MAX32630FTHR)
   #define TFT_DC   P5_4
   #define TFT_CS   P5_3
   #define STMPE_CS P3_3
   #define SD_CS    P3_2
#else
  #define STMPE_CS 6
   #define TFT_CS   9
   #define TFT_DC   10
   #define SD_CS    5
#endif

//Comment this out to remove the text overlay
#define SHOW_TEMP_TEXT

//low range of the sensor (this will be blue on the screen)
#define MINTEMP 20

//high range of the sensor (this will be red on the screen)
#define MAXTEMP 28

//the colors we will be using
const uint16_t camColors[] = {0x480F,
0x400F,0x400F,0x400F,0x4010,0x3810,0x3810,0x3810,0x3810,0x3010,0x3010,
0x3010,0x2810,0x2810,0x2810,0x2810,0x2010,0x2010,0x2010,0x1810,0x1810,
0x1811,0x1811,0x1011,0x1011,0x1011,0x0811,0x0811,0x0811,0x0011,0x0011,
0x0011,0x0011,0x0011,0x0031,0x0031,0x0051,0x0072,0x0072,0x0092,0x00B2,
0x00B2,0x00D2,0x00F2,0x00F2,0x0112,0x0132,0x0152,0x0152,0x0172,0x0192,
0x0192,0x01B2,0x01D2,0x01F3,0x01F3,0x0213,0x0233,0x0253,0x0253,0x0273,
0x0293,0x02B3,0x02D3,0x02D3,0x02F3,0x0313,0x0333,0x0333,0x0353,0x0373,
0x0394,0x03B4,0x03D4,0x03D4,0x03F4,0x0414,0x0434,0x0454,0x0474,0x0474,
0x0494,0x04B4,0x04D4,0x04F4,0x0514,0x0534,0x0534,0x0554,0x0554,0x0574,
0x0574,0x0573,0x0573,0x0573,0x0572,0x0572,0x0572,0x0571,0x0591,0x0591,
0x0590,0x0590,0x058F,0x058F,0x058F,0x058E,0x05AE,0x05AE,0x05AD,0x05AD,
0x05AD,0x05AC,0x05AC,0x05AB,0x05CB,0x05CB,0x05CA,0x05CA,0x05CA,0x05C9,
0x05C9,0x05C8,0x05E8,0x05E8,0x05E7,0x05E7,0x05E6,0x05E6,0x05E6,0x05E5,
0x05E5,0x0604,0x0604,0x0604,0x0603,0x0603,0x0602,0x0602,0x0601,0x0621,
0x0621,0x0620,0x0620,0x0620,0x0620,0x0E20,0x0E20,0x0E40,0x1640,0x1640,
0x1E40,0x1E40,0x2640,0x2640,0x2E40,0x2E60,0x3660,0x3660,0x3E60,0x3E60,
0x3E60,0x4660,0x4660,0x4E60,0x4E80,0x5680,0x5680,0x5E80,0x5E80,0x6680,
0x6680,0x6E80,0x6EA0,0x76A0,0x76A0,0x7EA0,0x7EA0,0x86A0,0x86A0,0x8EA0,
0x8EC0,0x96C0,0x96C0,0x9EC0,0x9EC0,0xA6C0,0xAEC0,0xAEC0,0xB6E0,0xB6E0,
0xBEE0,0xBEE0,0xC6E0,0xC6E0,0xCEE0,0xCEE0,0xD6E0,0xD700,0xDF00,0xDEE0,
0xDEC0,0xDEA0,0xDE80,0xDE80,0xE660,0xE640,0xE620,0xE600,0xE5E0,0xE5C0,
0xE5A0,0xE580,0xE560,0xE540,0xE520,0xE500,0xE4E0,0xE4C0,0xE4A0,0xE480,
0xE460,0xEC40,0xEC20,0xEC00,0xEBE0,0xEBC0,0xEBA0,0xEB80,0xEB60,0xEB40,
0xEB20,0xEB00,0xEAE0,0xEAC0,0xEAA0,0xEA80,0xEA60,0xEA40,0xF220,0xF200,
0xF1E0,0xF1C0,0xF1A0,0xF180,0xF160,0xF140,0xF100,0xF0E0,0xF0C0,0xF0A0,
0xF080,0xF060,0xF040,0xF020,0xF800,};
unsigned long delayTime;
uint16_t displayPixelWidth, displayPixelHeight;

Adafruit_MLX90640 mlx;
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
//#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
//#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
//#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
//#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
#define SERVO_LEFT  460
#define SERVO_RIGHT  100
#define SERVO_TURN_LEFT  +10
#define SERVO_TURN_RIGHT  -10




// our servo # counter
uint8_t servonum = 0;
//#define ADJ_STEP_SIZE 10
//uint8_t motor_adjustment = (SERVOMAX-SERVOMIN)/2;
//uint8_t motor_adjustment = (SERVO_LEFT-SERVO_RIGHT)/2;
int motor_adjustment = 150;

//Servos
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= SERVO_FREQ;   // Analog servos run at ~60 Hz updates
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000000;  // convert input seconds to us
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}


//Servos
int calc_new_position(int adjustment) {

  int normalized_adj=adjustment;
  
  if(adjustment > SERVO_LEFT ) {
      Serial.print( "adjustment left: " );
      Serial.println( adjustment );
      normalized_adj=SERVO_LEFT;
  }
  if(adjustment < SERVO_RIGHT ) {
       Serial.print( "adjustment right: " );
       Serial.println( adjustment );  
    normalized_adj=SERVO_RIGHT;
  }

  return normalized_adj;
}


void setup() {

  //mlx.setResolution(MLX90640_ADC_18BIT);
  //mlx90640_resolution_t res = mlx.getResolution();

  //mlx.setRefreshRate(MLX90640_2_HZ);
  //rate = mlx.getRefreshRate();

  // start serial port at 9600 bps and wait for port to open:
  Serial.begin(9600);
  
  if (! mlx.begin(MLX90640_I2CADDR_DEFAULT, &Wire)) {
    Serial.println("MLX90640 not found!");
    while (1) delay(10);
  }
    Serial.println("MLX90640 found!");
    
  //mlx.setMode(MLX90640_INTERLEAVED);
  mlx.setMode(MLX90640_CHESS);
  Serial.print("Current mode: ");
  if (mlx.getMode() == MLX90640_CHESS) {
    Serial.println("Chess");
  } else {
    Serial.println("Interleave");    
  }


    tft.begin();
    tft.fillScreen(ILI9341_BLACK);

    displayPixelHeight = 240 / 24;
    displayPixelWidth = 320 / 32; //Keep pixels square 

    tft.setRotation(0);

  //Servos
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ); // Analog servos run at ~50 Hz updates
    

}



void loop() {
  Serial.println("Start");
  if (mlx.getFrame(frame) != 0) {
    Serial.println("Failed");
    return;
  }
  //delay(500);
  int colorTemp;

  float left_heat=0;
  float center_heat=0;
  float right_heat=0;
  int marker0=0;
  int marker1=10;
  int marker2=21;
  int marker3=32;
  for (uint8_t h=0; h<24; h++) {
    for (uint8_t w=0; w<32; w++) {
      if( w>=marker2) {
        left_heat += frame[h*32 + w];
      }
      else if (w>=marker1) {
        center_heat += frame[h*32 + w];

      }
      else if (w>=marker0) {
        right_heat += frame[h*32 + w];
      }
    }
  }

  if(left_heat >= center_heat and left_heat >= right_heat) {
    Serial.println( motor_adjustment+SERVO_TURN_LEFT );
    motor_adjustment=calc_new_position(motor_adjustment+SERVO_TURN_LEFT);  
    Serial.println( "left_heat" );
  }
  else if(right_heat >= center_heat and right_heat >= left_heat) {
    motor_adjustment=calc_new_position(motor_adjustment+SERVO_TURN_RIGHT);  
    Serial.println( "right_heat" );
  }
  //else if( left_heat < center_heat and right_heat < center_heat ){
  else {
    motor_adjustment=calc_new_position(motor_adjustment);
    Serial.println( "center_heat" );
  }
  
  //Servos
  //Serial.println(servonum);
  //Serial.println(motor_adjustment);
  pwm.setPWM(servonum, 0, motor_adjustment);
  //delay(500);

  for (uint8_t h=0; h<24; h++) {
    for (uint8_t w=0; w<32; w++) {
      float t = frame[h*32 + w];
          //Serial.print(h);
          //Serial.print(",");
          //Serial.print(w);
          //Serial.print(",");
          //Serial.print(t);
          //Serial.print("\n");
          //Serial.print("\n");


      if(t >= MAXTEMP) colorTemp = MAXTEMP;
      else if(t <= MINTEMP) colorTemp = MINTEMP;
      else colorTemp = t;
      uint8_t colorIndex = map(colorTemp, MINTEMP, MAXTEMP, 0, 255);
      colorIndex = constrain(colorIndex, 0, 255);
      //draw the pixels!
      tft.fillRect(displayPixelHeight * h, 40 + displayPixelWidth * w,
        displayPixelHeight, displayPixelWidth, camColors[colorIndex]);

    }
  }
}
