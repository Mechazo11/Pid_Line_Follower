void callibrate()
{
int a=0; // Sensor 2
int b=0; // Sensor 0
for (int i=0; i<=4; i++)
  {
   IR_read();
   a=a+sen[2];
   b=b+sen[0];
  }
a=a/5;
b=b/5;
threshold=a+((b-a)/2);
//Serial.print("Threshold Value is: "); Serial.println(threshold);
}


