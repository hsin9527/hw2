#include "mbed.h"
#include<cmath>
#include<iostream>
Serial pc( USBTX, USBRX );
AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
DigitalIn Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);

int sample = 500;
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
float ADCdata[500];
float a = 0;
float b = 0;
int n = 0;
int main(){
  for (int i = 0; i < sample; i++){
    Aout = Ain;
    ADCdata[i] = Ain;
    if(i == 0){
      a = Ain;
      b = Ain;
    }
    b = a;
    a = Ain;
    
    if(b > 0.5 && a <= 0.5){
      n++;
    }
    wait(1./sample);
  }
  for (int i = 0; i < sample; i++){
    pc.printf("%1.3f\r\n", ADCdata[i]);
   
  }
  int freq = round(n);
  int digit;
  if(freq>99){
    digit = 3;
  }
  else digit = 2;
  char num[digit];
  int cal = freq;
  for(int i = 0; i < digit; i++){
    num[i] = table[cal%10];
    cal = cal/10;
  }
  num[0] = num[0]^(1<<7);


  int j= digit - 1;
  while(1){
    if(Switch == 0){
      redLED = 1;
      greenLED = 0;
      
      display = num[j];
      j--;
      if(j<0) j= digit -1;
      wait(1);
    } 
    else{
      display = 0x00;
      redLED = 0;
      greenLED = 1;
    }
    for( float k=0; k<2; k+=0.05 ){
       Aout = 0.5 + 0.5*sin(k*3.14159);
       wait(1./freq/40);
     } 
  }
  
}