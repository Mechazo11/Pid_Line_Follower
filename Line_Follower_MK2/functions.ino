void IR_read() // Funciton to read the IR sensor
{

for (int i=0; i<=4; i++)
   {
       sen[i]=analogRead(A0+i);
   }
}

void thres_check()
{
  if (sen[0]<=threshold)
    {
      x[0]=0;
    }
  else
    {
      x[0]=1;
    }
   //Serial.println(x[0]);
   
   if (sen[1]<=threshold)
    {
      x[1]=0;
    }
  else
    {
      x[1]=1;
    }
   //Serial.println(x[1]);
  
   if (sen[2]<=threshold)
    {
      x[2]=0;
    }
  else
    {
      x[2]=1;
    }
   //Serial.println(x[2]);
  
   if (sen[3]<=threshold)
    {
      x[3]=0;
    }
  else
    {
      x[3]=1;
    }
   //Serial.println(x[3]);
  
   if (sen[4]<=threshold)
    {
      x[4]=0;
    }
  else
    {
      x[4]=1;
    }
   //Serial.println(x[4]); 
   
}
void find_line()
{
  if (x[0]==1&&x[1]==1&&x[2]==1&&x[3]==1&&x[4]==1) // All white line
 { 
  new_line=old_line; // Reloading the last value
 }
 else
 {
  int den = 0;
  den=x[0]+x[1]+x[2]+x[3]+x[4];
  sum=0*x[0]+1000*x[1]+2000*x[2]+3000*x[3]+4000*x[4];
  if (x[0]==0 && x[1]==0 && x[2]==0 && x[3]==0 && x[4]==0) // All Black Condition
    {
      sum=2000;
      den=1;
    }
   
    
  line_position=sum/den;
  if (line_position>2500) line_position=2500;
  if (line_position<1500) line_position=1500;
  
  if (x[0]==0&&x[1]==0)
    {
      line_position=2250;
    }
    else if (x[1]==0&&x[2]==0)
      {
        line_position=2150;
      }
      else if(x[3]==0&&x[4]==0)
        {
          line_position=1750;
        }
        else if(x[2]==0&&x[3]==0)
          {
            line_position=1850;
          }
          else if (x[0]==0&&x[1]==0&&x[2]==0)
          {
            line_position=2500;
          }
          else if(x[2]==0&&x[3]==0&&x[4]==0)
            {
              line_position=1500;
            }
            else if(x[0]==0&&x[2]==0&&x[1]==1)
            {
              line_position=2333;
            }
             else if(x[4]==0&&x[2]==0&&x[3]==1)
              {
                line_position=1750;
              }
              else if(x[0]==0&&x[1]==0&&x[2]==0&&x[3]==0)
                {
                  line_position=2500;
                }
                else if(x[4]==0&&x[3]==0&&x[2]==0&&x[1]==0)
                {
                  line_position=1500;
                }
  
  
    new_line=line_position;
    old_line=new_line; // Updating the old line value for the next run
  
 } 
  
  
  
}


void PD_compute()
{

error = new_line - 2000; // 2000 is the value we obtain when our robot is perfectly aligned with the track
motorSpeed = Kp * error + Kd * (error - lastError);
lastError = error;
rightMotorSpeed = rightBaseSpeed + motorSpeed;
leftMotorSpeed = leftBaseSpeed - motorSpeed;

if (rightMotorSpeed >= rightMaxSpeed ) rightMotorSpeed = rightMaxSpeed; 
if (leftMotorSpeed >= leftMaxSpeed ) leftMotorSpeed = leftMaxSpeed; 

if (rightMotorSpeed <= 0) rightMotorSpeed = 0; 
if (leftMotorSpeed <= 0) leftMotorSpeed = 0; 

//Serial.print("Right Motor Speed: "); Serial.println(rightMotorSpeed);
//Serial.print("Left Motor Speed: "); Serial.println(leftMotorSpeed);





}
void update_motor()
{
  
digitalWrite(inA, HIGH);
digitalWrite(inB, LOW);
analogWrite(left_pwm, leftMotorSpeed);

digitalWrite(inC, HIGH);
digitalWrite(inD, LOW);
analogWrite(right_pwm, rightMotorSpeed);

}



