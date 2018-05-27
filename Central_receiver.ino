//#include <SoftwareSerial.h>

#include<SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//Variable declarations

#define bin1 5
#define bin2 6

int servo_pos_1 = 0;
int servo_pos_1_1 = 0;
int servo_pos_2_2 = 0;
int servo_pos_2 = 0;
//String textmessage ;
int distance1=0;
int distance2 = 0;
 
RF24 radio(7,8); // (CE,CSN)

//SoftwareSerial SIM800(9,10); //RX,TX

 
//Read and write addresses for bin1 and bin2

const uint64_t raddress[] = {0xB00B1E50D2LL, 0xB00B1E50C3LL};  
const uint64_t waddress[] = {0xB00B1E50B1LL, 0xB00B1E50A4LL};


void setup() {

  //Intialise serial communication between arduino uno and the PC
    //Serial.begin(9600);
    //delay(1000);

  //Declare the data reception LEDS as output
    pinMode(bin1,OUTPUT);
    pinMode(bin2,OUTPUT);

//Turn them off at intial conditions
    digitalWrite(bin1,LOW);
    digitalWrite(bin2,LOW);
    delay(1000);
    
//Give the GSM module time to establish network comm
    //SIM800.begin(9600);
    //delay(3000);
  
  //Intialise the radio module
    radio.begin();

 
  //pipe to transmit data to node 1
  //radio.WritingPipe(waddress[0]);

  //pipe to transmit data to node 2
  //radio.WritingPipe(waddresss[1]);

  //Set the power amplifier level for the radio module 
   radio.setPALevel(RF24_PA_MIN);
   
  
  //Set appropriate channel for radio communication to avoid interfering with other commm channels
    //radio.setChannel(110);
  
  //The data rate for transmission
    radio.setDataRate(RF24_250KBPS);
    
  }

void loop() {

  delay(2000);
  
   readBin1();
  
  radio.closeReadingPipe(raddress[0]);
   delay(1000);
   
   readBin2();
    radio.closeReadingPipe(raddress[1]);
    delay(1000);
     
 
} 
  
   void readBin1(){
    
    //Opens a pipe to receive data from node 1 :
  
    radio.openReadingPipe(1,raddress[0]);

    //Configures to receiver mode
    radio.startListening();
    
    if(radio.available()){
    //Check for incoming data from the radio module 
    while(radio.available()){ 


  
  //Open the reading pipe before reading data from node 1
  //Read data from node 1;
   radio.read(&distance1,sizeof(distance1));
   delay(1000);

    //Turn off indicator for bin2 when data is received from bin1
    digitalWrite(bin2,LOW);
    delay(1000);
    digitalWrite(bin1,HIGH);
    delay(1000);
    digitalWrite(bin1,LOW);
    delay(1000);

    //Print to serial monitor and delay for one sec
//    Serial.print("Received from Node 1 :  " );
//    Serial.print(distance1);
//    Serial.print("cm");
//    Serial.println("");
//    delay(1000);

    }
    
  if(distance1<=14 && distance1>=16){

    // sendmessage1();
      delay(500);

  }
  
  if (distance1<=5 || distance1>=7){
  
      delay(1000);

      radio.stopListening();
      
      delay(100);
      
      radio.openWritingPipe(waddress[0]);
      delay(200);

      //Wirelessly lock bin1

      for(servo_pos_1 = 180; servo_pos_1 <=0; servo_pos_1--){

      radio.write(&servo_pos_1,sizeof(servo_pos_1));
      delay(100);

      }

      //Wirelessly open bin 2
       radio.openWritingPipe(waddress[1]);

       for(servo_pos_2_2 = 0; servo_pos_2_2 <=180; servo_pos_2_2++){

       radio.write(&servo_pos_2_2,sizeof(servo_pos_2_2));
        delay(100);

       }

       //receivemessage1();
        delay(1000);
        
      
   
      }
  

  
  }
  
  }

    


  /*Function to read data from Bin2 and send or receive a command depending 
  on the amount of waste in the bin*/
   
  void readBin2(){
    
   //Opens a pipe to receive data from node 2 :
   radio.openReadingPipe(2,raddress[1]);

   //Configures to receiver mode
   radio.startListening();
    
   if(radio.available()){
  //Check for incoming data from the radio module 
    while(radio.available()){ 

  
  
  //Open the reading pipe before reading data from node 2
  //Read data from node 2;
   radio.read(&distance2,sizeof(distance2));


     //Turn off indicator for bin1 when data is received
     digitalWrite(bin1,LOW);
     delay(1000);

    //Turn on the indicator for bin2 when data is received for a second and go off
    digitalWrite(bin2,HIGH);
    delay(1000);
    digitalWrite(bin2,LOW);
    delay(1000);

       
  //Print to serial monitor and delay for one sec
//  Serial.print("Received from Node 2 :  " );
//  Serial.print(distance2);
//  Serial.print("cm");
//  Serial.println("");
//  delay(1000);
    
    }
   
  
   }
   
  //Check for the level of waste in bin2 and execute an action thereof
   
    if(distance2>=14 && distance2<=16){
      delay(1000);
      
      //Send SMS to the garbage collector that bin2 is almost full
    //  sendmessage2();
      //
    }
    
  else if(distance2>=5 && distance2>=7){
        
      delay(1000);

      radio.stopListening();
     
      radio.openWritingPipe(waddress[1]);
      delay(500);


      //Lock bin2 
      for(servo_pos_2 = 180; servo_pos_2 <= 0; servo_pos_2--){

      radio.write(&servo_pos_2,sizeof(servo_pos_2));
      delay(500);

      }

         //Open Bin1
      
      radio.openWritingPipe(waddress[0]);
      delay(1000);

      for(servo_pos_1_1 = 180; servo_pos_1_1 <= 0; servo_pos_1_1--){

      radio.write(&servo_pos_1_1,sizeof(servo_pos_1_1));
      delay(500);
      
        
      }


   }
   
  //receivemessage2();
} 


     

 
//Function to send message if bin1 is 80 percent full

// void sendmessage1()
//{
////  SIM800.println("AT+CMGF=1"); //AT command for text mode of gsm module 
////  delay(1000);  
////  
////  SIM800.println("AT+CMGS=\"+233203930215\"\r"); //  x is your mobile number
////  delay(1000);
////  
////  SIM800.println("Bin one is almost full oo");// the text you want to send
////  delay(3000);
////  
////  SIM800.println((char)26);// ASCII code of CTRL+Z
////  delay(1000);
//
//Serial.println("Bin 1 is almost full ooo");
//delay(1000);
// 
//}


//Function to send message if bin2 is 80 percent full
// void sendmessage2()
//{
////  SIM800.println("AT+CMGF=1"); //AT command for text mode of gsm module 
////  delay(1000);  
////  
////  SIM800.println("AT+CMGS=\"+233203930215\"\r"); //  x is your mobile number
////  delay(1000);
////  
////  SIM800.println("Bin two  is almost full oo");// the text you want to send
////  delay(3000);
////  
////  SIM800.println((char)26);// ASCII code of CTRL+Z
////  delay(1000);
//
//Serial.println("Bin two is almost full oo");
//delay(1000);
//}
//

//Function to receive command when bin1 is locked (open or close)

//void receivemessage1()
//{
////  SIM800.println("AT+CNMI=2,2,0,0,0"); // AT command to receive SMS
////  delay(3000);
////
////  if(SIM800.available()> 0){
////    textmessage = SIM800.readString();
////    delay(3000);
////    Serial.print(textmessage);
////    delay(1000);
////    
////  }
////
////
////if(textmessage.indexOf("OPEN BIN ONE")>=0){
////
//////pipe to transmit data to node 1
////  
////  radio.openWritingPipe(waddress[0]);
////  delay(100);
////
////  radio.stopListening();
////
////  for(servo_pos_1_1_1 = 0; servo_pos_1_1_1 <=180; servo_pos_1_1_1++){
////
////  radio.write(&servo_pos_1_1_1,sizeof(servo_pos_1_1_1));
////  delay(500);
////
////    textmessage = "";  
////}
////
////}
////
////if(textmessage.indexOf("LOCK BIN ONE")>=0){
////
////  
////  radio.openWritingPipe(waddress[0]);
////  delay(100);
////
////  radio.stopListening();
////
////  for(servo_pos_1 = 180; servo_pos_1 <=0; servo_pos_1--){
////
////  radio.write(&servo_pos_1,sizeof(servo_pos_1));
////  delay(500);
////
////  textmessage = "";
////
////}
////
////}
//
//if(Serial.available()>0){
//  
//  while(Serial.available()){
//  char state = Serial.read();
//  delay(150);
//   
//   switch(state){
//
//    case  'O':
//    
//    radio.openWritingPipe(waddress[0]);
//    delay(150);
//
//    radio.stopListening();
//
//  for(servo_pos_1 = 0; servo_pos_1 <=180; servo_pos_1++){
//
//    radio.write(&servo_pos_1,sizeof(servo_pos_1));
//    delay(1000);
//  }
//      break;
//    
//      case  'L' :
//      
//      radio.openWritingPipe(waddress[0]);
//      delay(150);
//
//      radio.stopListening();
//
//      for(servo_pos_1 = 180; servo_pos_1 <=0; servo_pos_1--){
//
//      radio.write(&servo_pos_1,sizeof(servo_pos_1));
//      delay(1000);
//      
//      }
//      
//      break;
//      
//
//        
//    }
//    
//  }
//
//
//}
//
//}
//
//
////Function to receive command when bin2 is locked (open or close)
////void receivemessage2()
////{
//////    SIM800.println("AT+CNMI=2,2,0,0,0"); // AT command to receive SMS
//////    delay(7000);
//////
//////  if(SIM800.available()> 0){
//////    textmessage = SIM800.readString();
//////    delay(3000);
//////    Serial.print(textmessage);
//////    delay(1000);
//////    
//////  }
//////
//////if(textmessage.indexOf("OPEN BIN TWO")>=0){
////////pipe to transmit data to node 2
//////  
//////  radio.openWritingPipe(waddress[1]);
//////
//////  radio.stopListening();
//////
//////  for(servo_pos_2 = 0; servo_pos_2 <=180; servo_pos_2++){
//////
//////  radio.write(&servo_pos_2,sizeof(servo_pos_2));
//////
//////    textmessage = "";  
//////}
//////
//////
//////}
////// if(textmessage.indexOf("LOCK BIN TWO")>=0){
//////
//////  //pipe to transmit data to node 2
//////  radio.openWritingPipe(waddress[1]);
//////
//////  radio.stopListening();
//////
//////  for(servo_pos_2 = 180; servo_pos_2 <=0; servo_pos_2--){
//////
//////  radio.write(&servo_pos_2,sizeof(servo_pos_2));
//////
//////  textmessage = "";
//////
//////      }
//////
//////}
////
//////if(Serial.available()>0){
//////  
//////  while(Serial.available()){
//////  char state_1 = Serial.read();
//////  delay(150);
//////   
//////   switch(state_1){
//////
//////    case  'O':
//////    
//////   radio.openWritingPipe(waddress[1]);
//////
//////  radio.stopListening();
//////
//////  for(servo_pos_2 = 0; servo_pos_2 <=180; servo_pos_2++){
//////
//////  radio.write(&servo_pos_2,sizeof(servo_pos_2));
//////
//////  }
//////
//////  break ;
//////  
//////      case  'L' :
//////      
//////       radio.openWritingPipe(waddress[1]);
//////
//////      radio.stopListening();
//////
//////  for(servo_pos_2 = 180; servo_pos_2 <=0; servo_pos_2--){
//////
//////  radio.write(&servo_pos_2,sizeof(servo_pos_2));
//////
//////  }
//////      break;
//////      
//////
//////        
//////    }
//    
//  }
//
//
//}
//
//}
