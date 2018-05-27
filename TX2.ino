
#include<SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include<avr/sleep.h>
#include<avr/power.h>
#include<avr/wdt.h>
#include <Servo.h>


//Variable and address declaration :
#define echo_pin 2
#define trig_pin 3 
#define ultrasonic_power_pin 4 //Turns sensor on and off 
#define red_indicator 6 //Full indicator
//#define servo_control 10 //Servo power pin 
int distance2; // amount of waste in the bin




int servo_pos_2 =  0; // locking mechanism
int servo_pos_2_2= 0;
const uint64_t waddress_2 = 0xB00B1E50C3LL;  // Pipe to write or transmit data to receiver.
const uint64_t raddress_2 = 0xB00B1E50A4LL ;  // Pipe to receive data from receiver.

//create an object called  radio to access the RF24 class and its members.
RF24 radio(7,8); // (CE,CSN)

//create an object called myservo to access the Servo class and its members
Servo myservo ;


void setup() {
//Intialises serial communication between arduino and the PC
//Serial.begin(9600);

//Notifies the controller that servo is connected to PWM pin 9
myservo.attach(9);

//Declarations of the I/O'S
pinMode(echo_pin,INPUT);
pinMode(trig_pin,OUTPUT);
pinMode(ultrasonic_power_pin,OUTPUT);
pinMode(red_indicator,OUTPUT);
//pinMode(servo_control,OUTPUT);

//Turn off the RED LED at intial conditions
digitalWrite(red_indicator,LOW);
delay(1000);

//Lock bin2 at intial conditions

for(int j = 0; j<1;j++){
for(servo_pos_2 = 180; servo_pos_2 = 0; servo_pos_2--){
myservo.write(&servo_pos_2);
delay(500);
}
}


//Turn ultrasonic sensor off at intial conditions
digitalWrite(ultrasonic_power_pin,HIGH);
delay(1000);

//Intialises the radio module
radio.begin();

//Sets the power amplifier to min
radio.setPALevel(RF24_PA_MIN);

//Set the comm channel suitable for radio comm
//radio.setChannel(110);

//Set the transmission and reception data rate
radio.setDataRate(RF24_250KBPS);


}

void loop() {

  delay(1000);
  
  sleepNow();
  

 
 

}
//Interrupt Service Routine for WatchDog Timer:

ISR(WDT_vect)
{
  volatile int f_wdt =1;
  
    if(f_wdt == 0)
    {
    f_wdt == 1;
       
    }
  
    else 
    {
    
   Serial.println("WDT Overrun!!!");
   delay(1000);
  
     }

  }


void getReading(){ 

/*A 10microsecond pulse is sent to the trigger input
to start the ranging and then the module sends out an 8 cycle
burst of ultrasound at 40kHz and raises it echo*/

long duration;


digitalWrite(trig_pin,LOW);
delayMicroseconds(2);

digitalWrite(trig_pin,HIGH);
delayMicroseconds(10);

digitalWrite(trig_pin,LOW);

//Computes the duration
duration = pulseIn(echo_pin,HIGH);

//Computes the level of waste in the bin
distance2 = (duration / 29.4) / 2 ;

}


void transmitReading(){
//Opens pipe to transmit data on  
radio.openWritingPipe(waddress_2);

//Configures the radio module as a transmitter
radio.stopListening(); 
        
 radio.write(&distance2,sizeof(distance2));
 
// Serial.print("Sent from node 2 : ");
// Serial.print(distance2);
// Serial.print("cm");
// Serial.println("");
// delay(1000);
                
   }


void getInstruction(){

  radio.powerUp();
      delay(150);
    
//  Receiver mode
    radio.startListening();
    
    radio.openReadingPipe(1,raddress_2);

    if(radio.available()){
      while(radio.available()){

        //Wireless open bin2

      radio.read(&servo_pos_2_2,sizeof(&servo_pos_2_2));
      for(int i=0;i<1;i++){
      myservo.write(servo_pos_2);
      delay(1000);
      }
     
     //Turn ultrasonic sensor on
     digitalWrite( ultrasonic_power_pin,LOW);
     delay(150);

      //Turn off the radio module

       getReading();
      transmitReading();
      delay(1000);
      
    radio.closeReadingPipe(raddress_2);
    delay(100);





}

}


}

void lockbin2_1(){
      
      radio.powerUp();
      delay(150);
  
      radio.openReadingPipe(1,raddress_2);
      radio.startListening();
      if(radio.available()){

        while(radio.available()){

//Lock bin2 again
    radio.read(&servo_pos_2,sizeof(&servo_pos_2));
    for(int t=0;t<1;t++){
    myservo.write(servo_pos_2);
    delay(1000);
    }
     //Turn indicator on
     digitalWrite(red_indicator,HIGH);
     delay(1000);

     //Turn ultrasonic sensor off
     digitalWrite( ultrasonic_power_pin,HIGH);
     delay(150);

      //Turn off the radio module

   
      
        }
     
      
      
      }
 
}


    

    
 void sleepNow(){
  
 set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  //Turn off the ultrasonic sensor
  digitalWrite(ultrasonic_power_pin,HIGH);
  delay(1000);

  //Turn off the LED 
  digitalWrite(red_indicator,LOW);
  delay(1000);
    
  //Turn the radio module off 
  radio.powerDown();
  delay(1000);

//  //Cut off power supply from servo motor
//  digitalWrite(servo_control,LOW);
//  delay(1000);

  
  //Turn off the ADC module
   
   ADCSRA &= ~(1<<ADEN);

  //Enable sleep capability
  
    sleep_enable();
  
 
  //Sleep sharp kraa
    sleep_mode();

   //Serial.println(radio.write(&text,sizeof(text)));
   //delay(1000);
 
  //Wake up from sleep
    sleep_disable();

 
  //Turn on the ADC module
  
   ADCSRA |= (1<<ADEN);

  //Turn on ultrasonic sensor(1s)

  digitalWrite(ultrasonic_power_pin,LOW);
  delay(1000);


  //Turn on the servo motor

//  digitalWrite(servo_control,HIGH);
//  delay(1000);
//  
      
// Serial.println("I just woke up,how far");
//  delay(1000); 


  
    getInstruction();
    delay(2000);
   
    lockbin2_1();
    
    
 }

  
