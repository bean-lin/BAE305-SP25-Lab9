/*
  SparkFun Inventor’s Kit
  Circuit 5B - Remote Control Robot

  Control a two wheeled robot by sending direction commands through the serial monitor.
  This sketch was adapted from one of the activities in the SparkFun Guide to Arduino.
  Check out the rest of the book at
  https://www.sparkfun.com/products/14326

  This sketch was written by SparkFun Electronics, with lots of help from the Arduino community.
  This code is completely free for any use.

  View circuit diagram and instructions at: https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v40
  Download drawings and code at: https://github.com/sparkfun/SIK-Guide-Code
*/
const int trigPin = 6;
const int echoPin = 7;
float distanceTo;
int duration;
int isObstacle = 1;

//the right motor will be controlled by the motor A pins on the motor driver
const int AIN1 = 8;           //control pin 1 on the motor driver for the right motor
const int AIN2 = 9;            //control pin 2 on the motor driver for the right motor
const int PWMA = 10;            //speed control pin on the motor driver for the right motor

//the left motor will be controlled by the motor B pins on the motor driver
const int PWMB = 11;           //speed control pin on the motor driver for the left motor
const int BIN2 = 12;           //control pin 2 on the motor driver for the left motor
const int BIN1 = 13;           //control pin 1 on the motor driver for the left motor

int switchPin = 7;             //switch to turn the robot on and off

const int driveTime = 20;      //this is the number of milliseconds that it takes the robot to drive 1 inch
                               //it is set so that if you tell the robot to drive forward 25 units, the robot drives about 25 inches

const int turnTime = 8;        //this is the number of milliseconds that it takes to turn the robot 1 degree
                               //it is set so that if you tell the robot to turn right 90 units, the robot turns about 90 degrees

                               //Note: these numbers will vary a little bit based on how you mount your motors, the friction of the
                               //surface that your driving on, and fluctuations in the power to the motors.
                               //You can change the driveTime and turnTime to make them more accurate

String botDirection;           //the direction that the robot will drive in (this change which direction the two motors spin in)
String distance;               //the distance to travel in each direction
String speed;

/********************************************************************************/
void setup()
{
  pinMode(switchPin, INPUT_PULLUP);   //set this as a pullup to sense whether the switch is flipped

  //set the motor control pins as outputs
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  // Assigns input and output pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);           //begin serial communication with the computer

  //prompt the user to enter a command
  Serial.println("Enter a direction followed by a speed and a distance.");
  Serial.println("f = forward, b = backward, r = turn right, l = turn left, 1 = fast, 2 = medium, 3 = slow");
  Serial.println("Example command: f 2 50");
}

/********************************************************************************/
void loop()
{
  if (Serial.available() > 0)                         //if the user has sent a command to the RedBoard
  {
    botDirection = Serial.readStringUntil(' ');       //read the characters in the command until you reach the first space
    speed = Serial.readStringUntil(' '); // Record a speed value from the user input 
    distance = Serial.readStringUntil(' ');           //read the characters in the command until you reach the second space

    int iSpeed = speed.toInt();

    //print the command that was just received in the serial monitor
    Serial.print(botDirection);
    Serial.print(" ");
    Serial.print(speed);
    Serial.print(" ");
    Serial.println(distance.toInt());

    if (botDirection == "f")                         //if the entered direction is forward
    {
      distanceCheck();
      rightMotor(-240/iSpeed * isObstacle);                                //drive the right wheel forward
      leftMotor(240/iSpeed * isObstacle);                                 //drive the left wheel forward
      
      // Drives the motor forward in discrete steps, periodically checking for obstacles
      for (int i = 0; i < (driveTime * distance.toInt() * iSpeed * isObstacle); i += 10) {
        delay(10);            //drive the motors for 10 ms 
        distanceCheck(); // Checks if there is an obstacle within 10 cm. If so, break the loop
      }

      rightMotor(0);                                  //turn the right motor off
      leftMotor(0);                                   //turn the left motor off
    }
    else if (botDirection == "b")                    //if the entered direction is backward
    {
      distanceCheck();
      rightMotor(240/iSpeed * isObstacle);                               //drive the right wheel forward
      leftMotor(-240/iSpeed * isObstacle);                                //drive the left wheel forward
      
      // Drives the motor forward in discrete steps, periodically checking for obstacles
      for (int i = 0; i < (driveTime * distance.toInt() * iSpeed * isObstacle); i += 100) {
        delay(10);            //drive the motors for 10 ms 
        distanceCheck(); // Checks if there is an obstacle within 10 cm. If so, break the loop
      }

      rightMotor(0);                                  //turn the right motor off
      leftMotor(0);                                   //turn the left motor off
    }
    else if (botDirection == "r")                     //if the entered direction is right
    {
      distanceCheck();
      rightMotor(200/iSpeed * isObstacle);                               //drive the right wheel forward
      leftMotor(255/iSpeed * isObstacle);                                 //drive the left wheel forward
      
      // Drives the motor forward in discrete steps, periodically checking for obstacles
      for (int i = 0; i < (driveTime * distance.toInt() * iSpeed * isObstacle); i += 100) {
        delay(10);            //drive the motors for 10 ms 
        distanceCheck(); // Checks if there is an obstacle within 10 cm. If so, break the loop
      }

      rightMotor(0);                                  //turn the right motor off
      leftMotor(0);                                   //turn the left motor off
    }
    else if (botDirection == "l")                   //if the entered direction is left
    {
      distanceCheck();
      rightMotor(-255/iSpeed * isObstacle);                                //drive the right wheel forward
      leftMotor(-200/iSpeed * isObstacle);                                //drive the left wheel forward
            
      // Drives the motor forward in discrete steps, periodically checking for obstacles
      for (int i = 0; i < (driveTime * distance.toInt() * iSpeed * isObstacle); i += 10) {
        delay(10);            //drive the motors for 10 ms 
        distanceCheck(); // Checks if there is an obstacle within 10 cm. If so, break the loop
      }

      rightMotor(0);                                  //turn the right motor off
      leftMotor(0);                                   //turn the left motor off
    }
  }
}

/********************************************************************************/
void rightMotor(int (motorSpeed))                       //function for driving the right motor
{
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(AIN1, HIGH);                         //set pin 1 to high
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, HIGH);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
  }
  analogWrite(PWMA, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}

/********************************************************************************/
void leftMotor(int motorSpeed)                        //function for driving the left motor
{
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(BIN1, HIGH);                         //set pin 1 to high
    digitalWrite(BIN2, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(BIN1, LOW);                          //set pin 1 to low
    digitalWrite(BIN2, HIGH);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(BIN1, LOW);                          //set pin 1 to low
    digitalWrite(BIN2, LOW);                          //set pin 2 to low
  }
  analogWrite(PWMB, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}

void distanceCheck() {
  // Resets the transmitter to low
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Transmits a sonic burst from the transmitter
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Records the travel time of the sonic burst between the transmitter and the reciever
  duration = pulseIn(echoPin, HIGH);
  // Computes distance by comparing to the speed of sound, round trip
  distanceTo = (duration*.0343)/2;

  // Prints distance to the user's Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distanceTo);
  if (distanceTo < 10)
    isObstacle = 0;
  else
    isObstacle = 1;
}
