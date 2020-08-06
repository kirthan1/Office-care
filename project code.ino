/*Description of project:
Due to the covid-19 pandemic,people have to be more conscious than ever. 
Sanitization is now mandatory wherever we go. 
So we have come up with a small project which will help office-going people to have touch-free environment, in order to enter the office.


So, as per this project, when a person walks into the office, the glassdoor opens automatically and closes after few seconds.
A Ultrasonic sensor is used to calculate the distance of the approaching person and opens when the person is near to it.
The glassdoor remains closed ,if no one is nearby. 
Once inside the office, the person is required to go through a passgate,where he is instructed to sanitize hands.
There is an automatic sanitizer-dispenser present at the passgate.
PIR sensor present in the dispenser, senses the presence of hand and then dispenses the liquid.
If the person doesn't hold out his hand, an instruction saying "Sanitization is mandatory" is displayed and asks the person to sanitize.
Access is granted through the passgate, only if  the person has sanitized the hands.
Once the liquid is dispensed, 6 seconds are given to rub the liquid into their hands.
The passgate will be open for three seconds with the green light  blinking thrice as an indication.
After that, the passgate will close automatically with a long beep sound and the red light turns ON denoting the closure of gate.
*/



//import LiquidCrystal library & Servo library
#include<LiquidCrystal.h>
#include<Servo.h>

//create instance , pass pin numbers
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
Servo myservo;

//defining pin numbers for LEDs
#define g_led 5
#define r_led 4

//declaration of variables
int i,j;
int pos=0;
int pir = 0;
float distance=0;

//function to receive signals from Ultrasonic sensor 
 long readUltrasonicDistance(int triggerPin, int echoPin)
{
    pinMode(triggerPin, OUTPUT);  // Clear the trigger
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    // Sets the trigger pin to HIGH state for 10 microseconds
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    pinMode(echoPin, INPUT);
    // Reads the echo pin, and returns the sound wave travel time in microseconds
    return pulseIn(echoPin, HIGH);
}

//function to sanitize and have access to pass gate
void sanitize()
{
  
    //dipenses liquid
    tone(2,440,1000);
    lcd.setCursor(3,0); //col:3,row:0
    lcd.print("Dispensed");
    delay(4000);
    lcd.clear();
  
  
    //6 seconds given to rub the liquid
    
    for(j=6;j>0;j--)
    {
        lcd.setCursor(4,0); //col:4,row:0
        lcd.print(j);
        lcd.setCursor(6,0); //col:6,row:0
        lcd.print("seconds");
        delay(1000);
    }
    lcd.clear();
    digitalWrite(r_led,LOW);//red led is turned OFF
    
    delay(1000);
    lcd.setCursor(2,0); //col:2,row:0
    lcd.print("Access granted");
    
 /*pass gate is opened for 3 seconds with green led blinking thrice*/
    //gate opens
    for(pos=0;pos<=180;pos+=5)
    {
      myservo.write(pos);
      delay(200);
      }
    //green led blinks 3 times
    for(i=0;i<3;i++)
    {
      digitalWrite(g_led,HIGH);
      delay(1000);
      digitalWrite(g_led,LOW);
      delay(1000);
      
    }
    delay(3000);//delay of 3 seconds
    lcd.clear();
   

    /*closing of pass gate*/
    //a long beep with red led turned ON
    tone(2,500,6000);//buzzer connected to pin 2, beeps at 500Hz for 6 seconds
    for(pos=180;pos>=0;pos-=5)
    {
      myservo.write(pos);
      delay(200);
      }
    digitalWrite(r_led,HIGH);
    
  
}


void setup()
{
  pinMode(A5, INPUT);//pir sensor
  pinMode(2, OUTPUT);//buzzer
  pinMode(3, OUTPUT);//servo motor
  pinMode(4, OUTPUT);//red led
  pinMode(5, OUTPUT);//green led

  lcd.begin(16, 2);
  myservo.attach(3);
}

void loop()
{
    
   lcd.clear();
  //formula to calculate distance in cm
  distance = 0.01723 * readUltrasonicDistance(7, 6);
  digitalWrite(r_led,HIGH);
  
 /*checks if person is within 50 cm range*/
 //if TRUE , glass door opens
  while(distance<=50)
  {
      lcd.display();
      lcd.setCursor(4,0); //col:4,row:0
      lcd.print("Welcome!");
      delay(2000);
      //glassdoor opens
        for(pos=0;pos<=180;pos+=5)
      {
        myservo.write(pos);
        delay(200);
      }
      
      lcd.clear();
     
    //glassdoor closes
    for(pos=180;pos>=0;pos-=5)
    {
      myservo.write(pos);
      delay(200);
      }
    
 /*in order to have access to pass gate*/
 //officer must sanitize his hands
    lcd.setCursor(1,0); //col:1,row:0
    lcd.print("Please sanitize");
    lcd.setCursor(2,1); //col:2,row:1
    lcd.print("your hands");
    delay(4000);
    lcd.clear();
    
    //The officer is required to hold his hand in front of sanitizer
    //a pir sensor is attached to the sanitizer, which senses the presence of hand and drops a blob of the liquid
    
    do
    {
      delay(1000);
      pir =digitalRead(A5);
      delay(2000);
    
      if (pir == 1)
      {
        sanitize();
      }
     
      else
    {
      lcd.setCursor(2,0); //col:1,row:0
      lcd.print("Sanitization");
      lcd.setCursor(1,1); //col:2,row:1
      lcd.print("is mandatory");
      delay(4000);
      lcd.clear();
      }
      
    }while(pir!=1);
    
     lcd.noDisplay();
     break;
     
     
  }//end of while loop
  
  //if the while condition is FALSE,then glassdoor remains closed
    
    lcd.clear();
    delay(2000);
    lcd.display();
    lcd.setCursor(1,0); //col:1,row:0
    lcd.print("Doors closed");
    delay(5000);
    
    
  
}
