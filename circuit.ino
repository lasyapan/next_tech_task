#include <IRremote.h>

/* Arduino Robot Using IR Sensors
   This robot can follow lines using sensors
   or it can take user input through a remote control
   to move around. */

// Creating objects from IRremote.h for the remote control //
const int receiver_pin = A5; // IR Sensor 3 is the input for the remote control. //
IRrecv irrecv(receiver_pin);
decode_results result;

//Speed//
int speed = 0;
int motorspeed1 = 0;
int motorspeed2 = 0;
int rightspeed = 250; // Rotation speed for right //
int leftspeed = 250;  // Rotation speed for left //

// Declaring pins as constants b/c they don't change //
const int motor1 =  9;
const int motor2 = 8;
const int motor3 = 7;
const int motor4 = 6;
const int enA = 10;
const int enB = 5;

void turnleft(); 
void turnright(); 
void goforward(); 
void gobackward(); 
void inputspeed(int speed); //Function to input speed //
void stop();
void speeddown(); // Function to decrease speed by 5 //
void speedup(); // Function to increase speed by 5 //

void setup()
{
 Serial.begin(9600);
 irrecv.enableIRIn(); // Start receiver //
 pinMode(motor1,OUTPUT);
 pinMode(motor2,OUTPUT);
 pinMode(motor3,OUTPUT);
 pinMode(motor4,OUTPUT);
 pinMode(enA,OUTPUT);
 pinMode(enB,OUTPUT);
 pinMode(A0, INPUT); // Left sensor input //
 pinMode(A1, INPUT); // Right sensor input //
 pinMode(A5, INPUT); // Remote sensor input //
}

void loop()
{ 
    if (irrecv.decode(&result)) // Checks if remote control is giving input //
    {	
      switch(result.value)
      {
            case 0xFD807F: //Volume+ Button //
              speedup();
            break;
            case 0xFD40BF: //Func Stop Button //
              stop();
            break;
            case 0xFD20DF:// |<< Button //
              turnleft();
            break;  
            case 0xFD609F:// >>| Button //
             turnright();
            break ;               
            case 0xFD10EF://Down Button //
              gobackward();
            break ;  
            case 0xFD906F://Volume- Button //
              speeddown();
            break ;  
            case 0xFD50AF://Up Button //
              goforward();
            break ; 
            case 0xFD00FF: //Power button //
                irrecv.decode(&result);
                inputspeed(result.value);
            break;
            default: 
              break;
      }
      irrecv.resume(); // Used to resume listening after using decode //
    }
    else if(irrecv.isIdle())
      /* If remote control input is not picked up on, rc.decode returns
         false. If so, program checks for 
         other two sensors in case it picks up on a line*/
    {
      int left = digitalRead(A0); // Using digitalread to differentiate b/w light and dark //
      int right = digitalRead(A1);
      
      if(left == 0 && right == 0) //0 = Low = Dark, so move forward //
      {
        goforward();
      } 
      if (right == 1 && left == 0) // 0 = Low = Dark, 1 = High = Light, so move left //
      {
        turnleft();
      } 
      if (right == 0 && left == 1)// 0 = Low = Dark, 1 = High = Light, so move right //
      {
        turnright();
      } 
      if (left == 1 && right == 1)// 1 = High = Light, so stop //
      {
        stop();
      } 
    }
}

void turnleft()
{
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, HIGH); // Turns off motors on one side to turn left //
  digitalWrite(motor3, HIGH);
  digitalWrite(motor4, LOW);
  analogWrite(enA, leftspeed);
  analogWrite(enB, rightspeed);
}

void turnright()
{
  digitalWrite(motor1, HIGH);
  digitalWrite(motor2, LOW); 
  digitalWrite(motor3, LOW);
  digitalWrite(motor4, HIGH);
  analogWrite(enA, leftspeed);
  analogWrite(enB, rightspeed);
}

void goforward()
{
  digitalWrite(motor1, HIGH);
  digitalWrite(motor2, LOW);
  digitalWrite(motor3, HIGH);
  digitalWrite(motor4, LOW);
  analogWrite(enA, motorspeed1);
  analogWrite(enB, motorspeed2);
}

void gobackward()
{
  digitalWrite(motor1, LOW); 
  digitalWrite(motor2, HIGH);
  digitalWrite(motor3, LOW);
  digitalWrite(motor4, HIGH);
  analogWrite(enA, motorspeed1);
  analogWrite(enB, motorspeed2);
}

void inputspeed(int input)
{
  speed = input;
  analogWrite(motorspeed1, speed);
  analogWrite(motorspeed2, speed);
}

void speedup()
{
  if (speed >= 250)
  {
    speed = 250;
  }
  else
  {
  speed += 5;
  }
  analogWrite(motorspeed1, speed);
  analogWrite(motorspeed2, speed);
}

void speeddown()
{
  if (speed <= 0)
  {
    speed = 0;
  }
  else
  {
  speed -= 5;
  }
  analogWrite(motorspeed1, speed);
  analogWrite(motorspeed2, speed);
}

void stop()
{
  	  digitalWrite(motor1, LOW);
      digitalWrite(motor2, LOW);
      digitalWrite(motor3, LOW);
      digitalWrite(motor4, LOW);
}


