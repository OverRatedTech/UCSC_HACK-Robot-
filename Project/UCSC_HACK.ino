/*
 PROJECT: ArduDroid for Robot interface
 Programmer: Roberto Chavez
 Reference: Hazim Bitar (techbitar at gmail dot com) and 
 DATE: 1/11/15 
 FILE: ardudroid.ino
 LICENSE: Public domain
*/

#define START_CMD_CHAR '*'
#define END_CMD_CHAR '#'
#define DIV_CMD_CHAR '|'
#define CMD_DIGITALWRITE 10
#define CMD_ANALOGWRITE 11
#define CMD_TEXT 12
#define CMD_READ_ARDUDROID 13
#define MAX_COMMAND 20  // max command number code. used for error checking.
#define MIN_COMMAND 10  // minimum command number code. used for error checking. 
#define IN_STRING_LENGHT 40
#define MAX_ANALOGWRITE 255
#define PIN_HIGH 3
#define PIN_LOW 2
volatile int rotaryCount = 0;

#define PINA 8
#define PINB 9
#define INTERRUPT 0  // that is, pin 2

#define DIRECTIONA 5
#define MOTORA 4

#define DIRECTIONB 6
#define MOTORB 7

#define TIME_FORWARDS 10000
#define TIME_BACKWARDS 10000
#define TIME_TURN 1200

// Interrupt Service Routine for a change to encoder pin A
void isr ()
{
 boolean up;

 if (digitalRead (PINA))
   up = digitalRead (PINB);
 else
   up = !digitalRead (PINB);

 if (up)
   rotaryCount++;
 else
   rotaryCount--;
}  // end of isr



String inText;

void setup() {
  Serial.begin(9600);
  Serial.println("ArduDroid 0.12 Alpha by TechBitar (2013)");
  Serial.flush();
  
  attachInterrupt (INTERRUPT, isr, CHANGE);   // interrupt 0 is pin 2, interrupt 1 is pin 3
 pinMode (MOTORA, OUTPUT);
 pinMode (DIRECTIONA, OUTPUT);
 pinMode (MOTORB, OUTPUT);
 pinMode (DIRECTIONB, OUTPUT);
}

byte phase;
unsigned long start;
int time_to_go;

void loop()
{
 analogWrite (MOTORA, 200);
 analogWrite (MOTORB, 200);
 start = millis ();
 
 // check current drain
 while (millis () - start < time_to_go)
   {
   if (analogRead (0) > 325)  // > 1.46 amps
     break;    
   }
  
  
  Serial.flush();
  int ard_command = 0;
  int pin_num = 0;
  int pin_value = 0;

  char get_char = ' ';  //read serial

  // wait for incoming data
  if (Serial.available() < 1) return; // if serial empty, return to loop().

  // parse incoming command start flag 
  get_char = Serial.read();
  if (get_char != START_CMD_CHAR) return; // if no command start flag, return to loop().

  // parse incoming command type
  ard_command = Serial.parseInt(); // read the command
  
  // parse incoming pin# and value  
  pin_num = Serial.parseInt(); // read the pin
  pin_value = Serial.parseInt();  // read the value

  // 1) GET TEXT COMMAND FROM ARDUDROID
  if (ard_command == CMD_TEXT){   
    inText =""; //clears variable for new input   
    while (Serial.available())  {
      char c = Serial.read();  //gets one byte from serial buffer
      delay(5);
      if (c == END_CMD_CHAR) { // if we the complete string has been read
        // add your code here
        break;
      }              
      else {
        if (c !=  DIV_CMD_CHAR) {
          inText += c; 
          delay(5);
        }
      }
    }
  }

  // 2) GET digitalWrite DATA FROM ARDUDROID
  if (ard_command == CMD_DIGITALWRITE){  
    if (pin_value == PIN_LOW) pin_value = LOW;
    else if (pin_value == PIN_HIGH) pin_value = HIGH;
    else return; // error in pin value. return. 
    set_digitalwrite( pin_num,  pin_value);  // Uncomment this function if you wish to use 
    return;  // return from start of loop()
  }

  // 3) GET analogWrite DATA FROM ARDUDROID
  if (ard_command == CMD_ANALOGWRITE) {  
    analogWrite(  pin_num, pin_value ); 
    // add your code here
    return;  // Done. return to loop();
  }

  // 4) SEND DATA TO ARDUDROID
  if (ard_command == CMD_READ_ARDUDROID) { 
    // char send_to_android[] = "Place your text here." ;
    // Serial.println(send_to_android);   // Example: Sending text
    Serial.print(" Analog 0 = "); 
    Serial.println(analogRead(A0));  // Example: Read and send Analog pin value to Arduino
    return;  // Done. return to loop();
  }
  
  
  analogWrite (MOTORA, 0);
 analogWrite (MOTORB, 0);
 delay (500);
  
  
}

// 2a) select the requested pin# for DigitalWrite action
void set_digitalwrite(int pin_num, int pin_value)
{
  analogWrite (MOTORA, 200);
 analogWrite (MOTORB, 200);
 start = millis ();
 
 // check current drain
 while (millis () - start < time_to_go)
   {
   if (analogRead (0) > 325)  // > 1.46 amps
     break;    
   }
  
  
  
  switch (pin_num) {
  case 13:
    pinMode(13, OUTPUT);
    digitalWrite(13, pin_value);  
    // add your code here      
    break;
  case 12:
    pinMode(12, OUTPUT);
    digitalWrite(12, pin_value);   
    // add your code here       
    break;
  case 11:
    pinMode(11, OUTPUT);
    digitalWrite(11, pin_value);         
    // add your code here 
    break;
  case 10:
    pinMode(10, OUTPUT);
    digitalWrite(10, pin_value);         
    // add your code here 
    break;
  case 9:
    pinMode(9, OUTPUT);
    digitalWrite(9, pin_value);         
    // add your code here 
    break;
  case 8:
    pinMode(8, OUTPUT);
    digitalWrite(8, pin_value);         
    // add your code here 
    break;
  case 7:
    pinMode(7, OUTPUT);
    digitalWrite(7, pin_value);         
    // add your code here 
    
    digitalWrite (DIRECTIONA, 1); 
     digitalWrite (DIRECTIONB, 1); 
     time_to_go = TIME_FORWARDS;
    
    break;
  case 6:
    pinMode(6, OUTPUT);
    digitalWrite(6, pin_value);         
    // add your code here 
    
    digitalWrite (DIRECTIONA, 1); 
     digitalWrite (DIRECTIONB, 0); 
     time_to_go = TIME_TURN;
    
    break;
  case 5:
    pinMode(5, OUTPUT);
    digitalWrite(5, pin_value); 
    // add your code here       
    
    digitalWrite (DIRECTIONA, 0); 
     digitalWrite (DIRECTIONB, 0); 
     time_to_go = TIME_BACKWARDS;
    
    
    break;
  case 4:
    pinMode(4, OUTPUT);
    digitalWrite(4, pin_value);    

    digitalWrite (DIRECTIONA, 0); 
     digitalWrite (DIRECTIONB, 1); 
     time_to_go = TIME_TURN;
    
    // add your code here 
    break;
  case 3:
    pinMode(3, OUTPUT);
    digitalWrite(3, pin_value);         
    // add your code here 
    break;
  case 2:
    pinMode(2, OUTPUT);
    digitalWrite(2, pin_value); 
    // add your code here       
    break;      
    // default: 
    // if nothing else matches, do the default
    // default is optional
    
    analogWrite (MOTORA, 0);
    analogWrite (MOTORB, 0);
    delay (500);
  } 
}
