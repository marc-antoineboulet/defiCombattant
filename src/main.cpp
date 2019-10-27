#include <Wire.h>
#include <LibRobus.h>
#include "Adafruit_TCS34725.h"

// Pick analog outputs, for the UNO these three work well
// use ~560  ohm resistor
// between Red & Blue, ~1K for green (its brighter)
#define redpin 3
#define greenpin 5
#define bluepin 6
// for a common anode LED, connect the common pin to +5V
// for common cathode, connect the common to ground

// set to false if using a common cathode LED
#define commonAnode true

// our RGB -> eye-recognized gamma color
byte gammatable[256];

int lireCouleur();
float lireDistance();

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  Serial.begin(9600);
  Serial.println("Color View Test!");

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }
  
  // use these three pins to drive an LED
  pinMode(redpin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
  
  // thanks PhilB for this gamma table!
  // it helps convert RGB colors to what humans see
  for (int i=0; i<256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;
      
    if (commonAnode) {
      gammatable[i] = 255 - x;
    } else {
      gammatable[i] = x;      
    }
    //Serial.println(gammatable[i]);
  }
}


void loop() {
  uint16_t clear, red, green, blue;
  tcs.setInterrupt(false);      // turn on LED

  delay(60);  // takes 50ms to read 
  
  tcs.getRawData(&red, &green, &blue, &clear);

  tcs.setInterrupt(true);  // turn off LED
  
  // //Serial.print("C:\t"); Serial.print(clear);
  // Serial.print("\tR:\t"); Serial.print(red);
  // Serial.print("\tG:\t"); Serial.priint(green);
  // Serial.print("\tB:\t"); Serial.print(blue);
  // Serial.print("\n");
  // Serial.print("\n");

  delay(50);

  // Figure out some basic hex code for visualization
  /*uint32_t sum = clear;
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;
  Serial.print("\t");

  Serial.print("\tR:\t");   Serial.print((int)r); Serial.print("|"); Serial.print(red); 
  Serial.print("\tG:\t");   Serial.print((int)g); Serial.print("|"); Serial.print(green);
  Serial.print("\tB:\t");   Serial.print((int)b);Serial.print("|"); Serial.print(blue); 

  Serial.println();*/
  int couleur = lireCouleur();
  if(couleur==0)
  {
    Serial.println("ROUGE");
  }
  if(couleur==1)
  {
    Serial.println("VERT");
  }
   if(couleur==2)
   {
      Serial.println("BLEU");
   }
   if(couleur==3)
   {
     Serial.println("JAUNE");
   }
   if(couleur==4)
   {
     Serial.println("noir");
   }
    Serial.println(lireDistance());


  // //Serial.print((int)r ); Serial.print(" "); Serial.print((int)g);Serial.print(" ");  Serial.println((int)b );

  // analogWrite(redpin, gammatable[(int)r]);
  // analogWrite(greenpin, gammatable[(int)g]);
  // analogWrite(bluepin, gammatable[(int)b]);
}
int lireCouleur()
{
 uint16_t clear, red, green, blue;
 tcs.setInterrupt(false);
 tcs.getRawData(&red,&green,&blue,&clear);
 tcs.setInterrupt(true);
 float r,g,b, sum;
 sum = clear;
 r = red / sum;
 g = green / sum;
 b = blue / sum;
 r = r*256.0;
 b = b*256.0;
 g = g*256.0;
 /*Serial.print("\tR:\t");   Serial.print((int)red);
 Serial.print("\tg:\t");   Serial.print((int)green);
 Serial.print("\tb:\t");   Serial.print((int)blue);
 Serial.print("\tclear:\t");   Serial.print((int)clear);
 Serial.println();*/

if(clear<650)
  {
    return 4;
  }
 if(r>95 && r<125 && g>55 && g<75 && b>55 && b<85)
  {
    return 0;
  }
  if(r>40 && r<60 && g>85 && g<105 && b>80 && b<100)
  {
    return 1;
  }
  if(r>30 && r<50 && g>70 && g<90 && b>100 && b<130)
  {
    return 2;
  }
  if(r>90 && r<130 && g>80 && g<100 && b>40 && b<60)
  {
    return 3;
  }
  
  return -1;
}
float lireDistance()
{
  float brut=ROBUS_ReadIR(0)/200.0;
  Serial.println(brut);
  Serial.println(analogRead(0));
  double distance, step1, step2, step3;
  step1 = (brut-49.8115)/(brut-0.230724);
  step2 = 0.679454*-1*step1;
  step3 = pow(step2,0.899171);
  
  /*Serial.print("\n"); Serial.print(step1);
  Serial.print("\n"); Serial.print(step2);
  Serial.print("\n"); Serial.print(step3);*/

  distance = pow(0.679454*-1*((brut-49.8115)/(brut-0.230724)),(125000.0/139017.0));
  return distance;
  
  
  }