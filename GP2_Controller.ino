#include <HX711.h>
#include <Ultrasonic.h>
#include <Servo.h>
#include <KeyboardController.h>

// Initialize USB Controller
USBHost usb;
String tag = "";
bool barCodeStatusIncomingData = false;
// Attach Keyboard controller to USB
KeyboardController keyboard(usb);
////-----------------------Servo Configuration----------------------

// servo-A:degree=0, delay=1500; Comp degree=180, delay=1500;
// servo-B:degree=35, delay=1500; Comp degree=145. delay=1500;
// servo-C:degree=180, delay=1500; Comp degree=0, delay=1500
// servo-D:degree=0, delay=1500; Comp degree=180, delay=1500
//servo F: degree=180, delay=1700: comp :degree=0, delay=1500
//servo E: degree=0, delay=1  1500: comp :degree=180, delay=1500
//servo G: degree=180, delay=1550: comp :degree=180, delay=1550
Servo myservoA;  
Servo myservoB;
Servo myservoC;
Servo myservoD;
Servo myservoE; 
Servo myservoF;
Servo myservoG;
void servosConfig()
{
  myservoA.attach(26);
  myservoB.attach(24);
  myservoC.attach(22);
  myservoD.attach(28);
  myservoE.attach(39);
  myservoF.attach(35);
  myservoG.attach(37);
}
////////--------------UltraSonic Sensors(Height, Width, Length)-------------
// Ultrasonic::Ultrasonic(TriggerPin, EchoPin)
Ultrasonic ultrasonicHeight(50, 52);
Ultrasonic ultrasonicWidth(44, 46);
Ultrasonic ultrasonicLength(36, 38);
const int MaxHeight = 27;
const int MaxWidth = 21;
const int MaxLength = 24;

////////-----------------------LoadCellConfig(Weight)-------------------
// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 12;
const int LOADCELL_SCK_PIN = 13;
//HX711(Dout, scl)
HX711 loadcell;


HX711 scale;
const float calibration_factor = -404.5;
float weight;

void configLoadCell()
{  
  Serial.println("HX711 weighing");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(loadcell.read());      // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(loadcell.read_average(20));   // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(loadcell.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(loadcell.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
            // by the SCALE parameter (not set yet)

  loadcell.set_scale(calibration_factor);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  loadcell.tare();               // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(loadcell.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(loadcell.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(loadcell.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(loadcell.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
            // by the SCALE parameter set with set_scale

  Serial.println("Readings:");
  
}
////////-----------------------ConvoyerMotorConfig----------------
///Convoyer 1
const int in1 = 7;
const int in2 = 6;
const int en1 = 12;
///Convoyer 2
const int in21 = 11;
const int in22 = 12;
const int en2 = 12;
void configDirectionOfConvoyerMotors()
{
  //Convoyer 1 direction
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  //Convoyer 2 direction
  pinMode(in21, OUTPUT);
  pinMode(in22, OUTPUT);
}
///////////---------------IRSensors Config (ObjectPass, ObjectExsitence)-------------
const int IRSensorPin1 = 10;//ObjectExsitence
const int IRSensorPin2 = 33;//ObjectPass
bool IsObjectPass = false;

void configIRSensors()
{
  pinMode(IRSensorPin1,INPUT);
  pinMode(IRSensorPin2, INPUT);
  attachInterrupt( IRSensorPin2, checkIsObjectPass, FALLING);
}
/////check is Object Pass ?
void checkIsObjectPass()
{
  IsObjectPass = true;
}
/////check is there an box on load cell ?
bool IsObjectExsit()
{
  if(digitalRead(IRSensorPin1) == LOW)
  {
    return true;
  }
  else
    return false;
}
////////-----------------------pushers----------------------------
void movePusheres()
{
  myservoA.write(0); 
  myservoB.write(38); 
  delay(1500);    
  myservoA.write(90);
  myservoB.write(90);
}
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
void openGate()
{  
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
///////-------------------------GetDimensions--------------------------
void printDimensions()
{
  //height
  Serial.print("Height in CM: ");
  Serial.println(MaxHeight - ultrasonicHeight.distanceRead());
  //width
  Serial.print("Width in CM: ");
  Serial.println(MaxWidth - ultrasonicWidth.distanceRead());
  //Length
  Serial.print("Length in CM: ");
  Serial.println(MaxLength - ultrasonicLength.distanceRead());  
}
////////-------------------------GetWeight---------------------------
//void getObjectWeight()
//{
//  Serial.print("Reading:");
//  weight = scale.get_units(),10;
//  if (weight < 0)
//  {
//    weight = 0.00;
//  }
//  //ounces = units * 0.035274;
//  Serial.print(weight);
//  Serial.println(" grams");  
//}
///////////-----------------------DefaultStatus-------------------
void setDefaultStatus()
{
  barCodeStatusIncomingData = false;
  IsObjectPass = false;  
}
//////////-----------------------BarCode Reader-------------------
void getBarCdoeOfBox()
{
  Serial.println("Start...."); 
  while (1) {
    usb.Task();  
    if( barCodeStatusIncomingData ) break;
  }
  Serial.println("End!");
}
void keyPressed() {
  char key = keyboard.getKey();
   if(key == 19)//control device stop transimission
   {
      Serial.print("Pressed:  ");
      Serial.print(tag);
      Serial.println();
      tag = "";
      barCodeStatusIncomingData = true;
   }
   else
   {
      tag = tag + String(key);
   }
}
//////////-------------------------Convoyer motors functionalities------------------
///Convoyer 1
void turnOnStage1Motor()
{
  //analogWrite(en1, 50);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}
void turnOffStage1Motor()
{
  digitalWrite(in2, LOW);
  digitalWrite(in1, LOW);
}
///Convoyer 2
void turnOnStage2Motor()
{
  digitalWrite(in22, HIGH);
  digitalWrite(in21, LOW);
}
void turnOffStage2Motor()
{
  digitalWrite(in21, LOW);
  digitalWrite(in22, LOW);
}
///////////--------------------------SortObjects-----------------------------
void sortObject()
{
  turnOnStage2Motor();
  myservoG.write(0); 
  delay(1550);
  myservoG.write(90);
  delay(2000);
  delay(2000);  
  myservoG.write(180);
  turnOffStage2Motor();
  delay(1550);
  myservoG.write(90);
}
/////////------------------------Stage 1 Processes--------------------------
void execute_Stage_1_Part_1_Process()
{
  closeGate();
  delay(50);
  //getObjectWeight();    
  delay(50);
  pushObjectToConvoyer();    
  delay(50);
  turnOnStage1Motor();
  delay(50);
  returnBackInitialPusher();
}
void execute_Stage_1_Part_2_Process()
{
  delay(50);
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
  //sortObject();
}

////////////------------------------Setup--------------------
void setup(){
  Serial.begin(9600);
  configLoadCell();
  configDirectionOfConvoyerMotors();
  servosConfig();
  configIRSensors();
}
//////////------------------------Main-----------------------
void loop(){
  setDefaultStatus();
  getBarCdoeOfBox();
  if(IsObjectExsit())
  {
    execute_Stage_1_Part_1_Process();
  }
  if(IsObjectPass)
  {
    execute_Stage_1_Part_2_Process();
  }
}



