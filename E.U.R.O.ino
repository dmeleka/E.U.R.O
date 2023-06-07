#include <Arduino.h>
#include <TM1637Display.h>
#include<Arduino_FreeRTOS.h>
#include<SD.h>
#include <TMRpcm.h>
void LKA(void *pvParameters);
void displayGear (void *pvParameters);
void intensity(void *pvParameters);
bool play=false;
bool start=true;
bool next=false;
int irPlay = A5;
int irNext= A2;
char *wavs[] = {"1.wav", "2.wav", "3.wav","4.wav"};
int counter=0;

void playMusic()
{ 
    #define sdChipSelect 4
    TMRpcm tmrpcm;
    if(start){
      tmrpcm.speakerPin = 9;
      if (!SD.begin(sdChipSelect)) 
      {
        Serial.println("SD fail"); return; 
      }
      Serial.println("SD cool starting to play");
      tmrpcm.setVolume(6);
      tmrpcm.play(wavs[counter]);
      Serial.print("song number :");
      Serial.println(counter+1); // REQUIRED: DO NOT REMOVE
      start=!start;
    }
    else{
      Serial.println("already started playing");
      if(!play)
      {
        Serial.println("change from play to pause or vice versa");
        tmrpcm.pause();
      }     
    }
}
void setup() {
  Serial.begin(9600);
  pinMode(irPlay,INPUT);
  pinMode(irNext,INPUT);
  //xTaskCreate(LKA,"LKA",100,NULL,3,NULL);
  //xTaskCreate(displayGear,"displayGear",100,NULL,2,NULL);
  //xTaskCreate(intensity,"intensity",100,NULL,2,NULL);
  //xTaskCreate(playMusic,"playMusic",100,NULL,1,NULL);
  vTaskStartScheduler();  
  }
void loop() {
    if(analogRead(irPlay)<100){
      Serial.println("play button detected");
      playMusic();
      play=true;
    }
    else{
      play=false;
    }
    if(analogRead(irNext)<100&&!next){
      Serial.println("next button detected");
      if(counter<3){
        counter++;
      }
      else{
        counter=0;
      }
      next=true;
      start = true;
      playMusic();
    }
    else if(analogRead(irNext)>100){
      next=false;
    }
}
void LKA(void *pvParameters)
{
  //LKA task
  #define irRight  A0
  #define irLeft  A1
  #define motorRight  3
  #define motorLeft  5
  #define redLight 10
  pinMode(motorRight,OUTPUT);
  pinMode(motorLeft,OUTPUT);
  pinMode(redLight,OUTPUT);
  pinMode(irRight,INPUT);
  pinMode(irLeft,INPUT); 
  while(1)
  {
    // Serial.print("Right Lane : ");
    // Serial.println(analogRead(irRight));
    // Serial.print("Left Lane : ");  
    // Serial.println(analogRead(irLeft));
    if(analogRead(irRight)<40&&analogRead(irLeft)<40)
    {
      analogWrite(motorRight,127);
      analogWrite(motorLeft,127);
      digitalWrite(redLight,LOW);
    }
    else
    {
      while(analogRead(irRight)>=40&&analogRead(irLeft)>=40){
        // Serial.println("what the fuck are you doing"); 
        analogWrite(motorRight,127);
        analogWrite(motorLeft,0);  
        digitalWrite(redLight,HIGH);

      }
      while(analogRead(irRight)>=40){
        // Serial.println("right IR detecting lane"); 
        analogWrite(motorRight,0);
        analogWrite(motorLeft,127);  
        digitalWrite(redLight,HIGH);
        
      }      
      while(analogRead(irLeft)>=40){
        // Serial.println("Left IR detecting lane"); 
        analogWrite(motorRight,127);
        analogWrite(motorLeft,0);
        digitalWrite(redLight,HIGH);   
      }      
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
void intensity(void *pvParameters)
{
    TickType_t lastwaketime = xTaskGetTickCount();
    #define lightIntensitySensor 8
    #define blueHeadLight 7
    while(1)
    { 
      // Serial.print("Intensity");
      // Serial.print(digitalRead(lightIntensitySensor));
      if(digitalRead(lightIntensitySensor)==1)
      {
        digitalWrite(blueHeadLight,HIGH);
      }
      else
      {
        digitalWrite(blueHeadLight,LOW);
      }
      vTaskDelayUntil(&lastwaketime, pdMS_TO_TICKS(1));
    }
}
void displayGear (void *pvParameters)
{
  const uint8_t SEG_EURO[] = 
  {
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,            //E
    SEG_C | SEG_D | SEG_E,                            //u
    SEG_E | SEG_G,                                    //r
    SEG_C | SEG_D | SEG_E | SEG_G,                    //o     
  };
  TickType_t lastwaketime = xTaskGetTickCount();
  #define joyX A3
  #define joyY A4
  #define DIO 2   // 7segment
  #define CLK 6  // 7segment
  TM1637Display display(CLK, DIO);
  char direction ="";
  while(1)
  {
    int xValue = analogRead(joyX);
    int yValue = analogRead(joyY);
    if(xValue > 1000)
    {
      direction='d'; //d
    }
    else if(xValue < 20)
    {
      direction='r'; //r
    }
    else if(yValue > 1000)
    {
      direction='n'; //n
    }
    else if(yValue < 20)
    {
      direction='p'; //p
    }
    display.setBrightness(0x0f);
    uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
    const uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };
    if(direction=='p')
    {
      const uint8_t SEGP[] = 
      {
      SEG_A | SEG_B | SEG_E | SEG_F | SEG_G,//P
      SEG_A | SEG_B | SEG_C|SEG_D | SEG_E | SEG_F| SEG_G,
      SEG_A | SEG_B | SEG_C|SEG_D | SEG_E | SEG_F| SEG_G,
      SEG_A | SEG_B | SEG_C|SEG_D | SEG_E | SEG_F| SEG_G,
      };
      // Serial.print(direction);
      display.setSegments(SEGP);
    }
    else if(direction=='r')
    {
        const uint8_t SEGR[] = 
        {
          SEG_E | SEG_G,//r
          SEG_A | SEG_B | SEG_C|SEG_D | SEG_E | SEG_F| SEG_G,
          SEG_A | SEG_B | SEG_C|SEG_D | SEG_E | SEG_F| SEG_G,
          SEG_A | SEG_B | SEG_C|SEG_D | SEG_E | SEG_F| SEG_G,
        };
        // Serial.print(direction);
        display.setSegments(SEGR);
    }
    else if(direction=='n')
    {
      const uint8_t SEGN[] = 
      {
        SEG_C | SEG_E | SEG_G,//n
        SEG_A | SEG_B | SEG_C|SEG_D | SEG_E | SEG_F| SEG_G,
        SEG_A | SEG_B | SEG_C|SEG_D | SEG_E | SEG_F| SEG_G,
        SEG_A | SEG_B | SEG_C|SEG_D | SEG_E | SEG_F| SEG_G,
      };
      display.setSegments(SEGN);
    }
    else if(direction=='d')
    {
      const uint8_t SEGD[] = 
      {
        SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,//d
        SEG_A | SEG_B | SEG_C|SEG_D | SEG_E | SEG_F| SEG_G,
        SEG_A | SEG_B | SEG_C|SEG_D | SEG_E | SEG_F| SEG_G,
        SEG_A | SEG_B | SEG_C|SEG_D | SEG_E | SEG_F| SEG_G,
      };
      display.setSegments(SEGD);
    }
    else
    {
      display.setSegments(SEG_EURO);
    }
    vTaskDelayUntil(&lastwaketime, pdMS_TO_TICKS(1));
  }
}