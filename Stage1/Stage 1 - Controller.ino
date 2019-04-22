
#include <Servo.h>
#include <HX711.h>
#include <Ultrasonic.h>
#include <KeyboardController.h>

USBHost usb;
String tag = "";
// Attach Keyboard controller to USB
KeyboardController keyboard(usb);

Servo myservoA;  // create servo object to control a servo
Servo myservoB;
Servo myservoC;
Servo myservoD;
HX711 scale(8, 9);
Ultrasonic ultrasonicHeight(50, 52);
Ultrasonic ultrasonicWidth(44, 46);
Ultrasonic ultrasonicLength(36, 38);

bool IsObjectPass = false;
//bool IsObjectExsit = false;

//const int enA = 10;
const int in1 = 7;
const int in2 = 6;

const int IRSensorPin = 10;
const int IRSensorPin2 = 4;
int count = 0;
float calibration_factor = -404.5;
float units;
float ounces;

void loadCellConfig()
{
  Serial.begin(9600);
  Serial.println("HX711 weighing");
  scale.set_scale(calibration_factor);
  scale.tare();
  Serial.println("Readings:");
}
void printDimensions()
{
  //height
  Serial.print("Height in CM: ");
  Serial.println(27 - ultrasonicHeight.distanceRead());
  //width
  Serial.print("Width in CM: ");
  Serial.println(21 - ultrasonicWidth.distanceRead());
  //Length
  Serial.print("Length in CM: ");
  Serial.println(24 - ultrasonicLength.distanceRead());  
}
void movePusheres()
{
  myservoA.write(0); 
  myservoB.write(38); 
  delay(1500);    
  myservoA.write(90);
  myservoB.write(90);
}

void openGate()
{
  // servo-A:degree=0, delay=1500; Comp degree=180, delay=1500;
  // servo-B:degree=35, delay=1500; Comp degree=145. delay=1500;
  // servo-C:degree=180, delay=1500; Comp degree=0, delay=1500
  // servo-D:degree=0, delay=1500; Comp degree=180, delay=1500
  myservoA.write(180); 
  myservoB.write(150); 
  myservoC.write(10);   
  delay(1500);    
  myservoA.write(90);
  myservoB.write(90);
  myservoC.write(90);
}
void closeGate()
{
  myservoC.write(140);              
  delay(1500);
  myservoC.write(90);
}
void getObjectWeight()
{
  Serial.print("Reading:");
  units = scale.get_units(),10;
  if (units < 0)
  {
    units = 0.00;
  }
  ounces = units * 0.035274;
  Serial.print(units);
  Serial.println(" grams");
  delay(1000);
}
bool IsObjectExsit()
{
  delay(50);
  if(digitalRead(IRSensorPin) == LOW)
  {
    return true;
  }
  else
    return false;
}
/*bool IsObjectPass()
{
  delay(50);
  if(digitalRead(IRSensorPin2) == LOW)
  {
    return true;
  }
  else
    return false;
}*/
void pushObjectToConvoyer()
{
  myservoD.write(0);              
  delay(1700);
  myservoD.write(90);
}
void returnBackInitialPusher()
{
  myservoD.write(180);              
  delay(1700);
  myservoD.write(90);
}
void turnOnStage1Motor()
{
  digitalWrite(in2, HIGH);
  digitalWrite(in1, LOW);
}
void turnOffStage1Motor()
{
  digitalWrite(in2, LOW);
  digitalWrite(in1, LOW);
}
void setup()
{
  // set all the motor control pins to outputs
  loadCellConfig();
  pinMode(IRSensorPin,INPUT);
  //pinMode(IRSensorPin2, INPUT);
  myservoA.attach(26);
  myservoB.attach(24);
  myservoC.attach(22);
  myservoD.attach(28);// attaches the servo on pin 9 to the servo object
  //pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  attachInterrupt( IRSensorPin2, checkIsObjectPass, FALLING);
}
void checkIsObjectPass()
{
  IsObjectPass = true;
}
void keyPressed() {
  char key = keyboard.getKey();
   Serial.print("Pressed:  ");
   if(key == 19)//control device stop transimission
   {
      Serial.print("Pressed:  ");
      Serial.print(tag);
      Serial.println();
      tag = "";
   }
   else
   {
      tag = tag + String(key);
   }
}

void loop() {
 usb.Task();
  // set speed to 200 out of possible range 0~255
  //analogWrite(enA, 100);
 //turnOnStage1Motor();
 if(IsObjectExsit())
  {
    closeGate();    
    delay(50);
    turnOnStage1Motor();
    delay(50);
    pushObjectToConvoyer();
    delay(50);
    returnBackInitialPusher();
  }
  if(IsObjectPass)
  {
    Serial.println("XXX");
    delay(100);
    movePusheres();
    delay(50);
    turnOffStage1Motor();
    delay(50);
    printDimensions();
    delay(50);
    openGate(); 
    delay(50);
    turnOnStage1Motor();
    delay(1000);
    turnOffStage1Motor();
    IsObjectPass = false;
    //read dimensions
  }
}








