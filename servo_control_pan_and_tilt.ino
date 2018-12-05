#include<ros.h>
#include<std_msgs/Int64.h>
#include<std_msgs/Int64.h>
#include<Servo.h>

Servo ser1,ser2,ser3;
int clk_anti_clk = 0;

std_msgs::Int64 pan;
std_msgs::Int64 tilt;
int pos1 = 0,pos2 = 0,pos3 = 180;
ros::NodeHandle nh;
int v = 10;
int ser12 = 0;
ros::Publisher degree1("pan",&pan);
ros::Publisher degree2("tilt",&tilt);

//bottom servo is servo1, middle is servo2, top is servo3
//ser12 == 1 means move bottom servo if ser12 == 2 move middle servo

void message( const std_msgs::Int64 &ms ){

  int moveit = ms.data;
  if (moveit == 0)
  { 
  //add code if needed
  }
  
  else if(moveit == 1 && pos1 + pos2 < 180)
  {
  if (clk_anti_clk == 0)
    {
    pos1 = pos1 + v;
    }
  else if(clk_anti_clk == 1)
    {
    pos1 = pos1 - v;
    }
  ser1.write(pos1);
  }

  else if(moveit == 1 && pos1 + pos2 >= 180)
  {
  if (clk_anti_clk == 0)
    {
    pos2 = pos2 + v;
    }
  else if(clk_anti_clk == 1)
    {
    pos2 = pos2 - v;
    }
  ser2.write(pos2);
  }


  if (pos1 + pos2 == 360)
  {
  clk_anti_clk = 1;
  pos3 = pos3 - 15;
  ser3.write(pos3);
  }

  else if (pos1 + pos2 == 0)
  {
  clk_anti_clk = 0;
  pos3 = pos3 - 15;
  ser3.write(pos3);
  }

  pan.data = pos1 + pos2;
  tilt.data = pos3;

  degree1.publish(&pan);
  degree2.publish(&tilt);

  delay(5); //add delay if needed with higher frequency data
}
  
ros::Subscriber<std_msgs::Int64> sub("servo_work", &message );

void setup() {
  // put your setup code here, to run once:
nh.getHardware()->setBaud(9600);
nh.initNode();
nh.advertise(degree1);
nh.advertise(degree2);
nh.subscribe(sub);
ser1.attach(10);
ser2.attach(9);
ser3.attach(8);
ser1.write(pos1);
ser2.write(pos2);
ser3.write(pos3);
}

void loop() {
  
  nh.spinOnce(); 
    
  // put your main code here, to run repeatedly:
}
