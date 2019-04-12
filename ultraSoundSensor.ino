const int pingPin = 8; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 9; // Echo Pin of Ultrasonic Sensor
long duration, inches, objectHeightInCm; 
long ultraSoundErrorFactor = 2;
//ultraSound3
long ultraSound3OffsetInCm;

void setup() {
   Serial.begin(9600); // Starting Serial Terminal
   objectHeightInCm = 0;
   pinMode(pingPin, OUTPUT);
   pinMode(echoPin, INPUT);
   duration = getAvgOfDimensionValue(20);
   ultraSound3OffsetInCm = microsecondsToCentimeters(duration);;
   Serial.println(ultraSound3OffsetInCm);
   delay(100);
}

long microsecondsToCentimeters(long microseconds) {
   return (int)(microseconds / 29.4 / 2);
}

long getAvgOfDimensionValue(int avgCount)
{
  long sum = 0;
  long duration;
  for (int i=0;i<avgCount;i++)
  {
      digitalWrite(pingPin, LOW);
      delayMicroseconds(2);
      digitalWrite(pingPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(pingPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      sum=sum+duration;
  }
  return(int(sum/avgCount));
  
}

long getCorrectObjectHeightInCm3()
{
     long newDuration = getAvgOfDimensionValue(20);
     if(newDuration < 0)
     {
       return objectHeightInCm;
     }
     else if(abs(ultraSound3OffsetInCm - microsecondsToCentimeters(newDuration)) > 50)
     {
        return objectHeightInCm;
     }else
     {         
         long cm =  ultraSound3OffsetInCm - microsecondsToCentimeters(newDuration);
         if(cm>0)//error range
         {
           cm = cm - ultraSoundErrorFactor;
         }
         if(cm <0)//error range
         {
           cm = objectHeightInCm;
         }
         return cm;
     }     
}
long getCorrectObjectHeightInCm()
{
     long newDuration = getAvgOfDimensionValue(20);
     if(newDuration < 0)
     {
       return microsecondsToCentimeters(duration);
     }
     else if(abs(microsecondsToCentimeters(duration) - microsecondsToCentimeters(newDuration)) > 50)
     {
        return microsecondsToCentimeters(duration);
     }else
     {
         
         long cm =  microsecondsToCentimeters(duration - newDuration);
         if(cm>0)//error range
         {
           cm = cm - ultraSoundErrorFactor;
         }
         if(cm <0)//error range
         {
           cm = objectHeightInCm;
         }
         return cm;
     }     
}
void loop() {
     
   
   long newDuration = getAvgOfDimensionValue(20);
   //objectHeightInCm = getCorrectObjectHeightInCm3();
   //Serial.print(inches);
   //Serial.print("in, ");
   Serial.print(ultraSound3OffsetInCm);
   Serial.print(",cm");
   Serial.print(ultraSound3OffsetInCm - microsecondsToCentimeters(newDuration));
   Serial.println();
   delay(100);
}

