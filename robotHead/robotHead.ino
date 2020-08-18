/**
 * @file robotHead.ino
 * @author Umang Patel (1998patelumang@gmail.com)
 * @brief Use to receive the command from Master sand perform operations accordingly
 * @version 0.1
 * @date 2018-01-21
 * 
 */
#include<Wire.h> //For Wired Master/Slave Communication
#include<Servo.h> //For Controlling Servo Motor

#define Buzzer 2 //Buzzer Pin Number

//Color Sensor Pin Connection
#define EN 4
#define S0 6
#define S1 5
#define S2 8
#define S3 7
#define OUT A1 

//Color LED Configuration
#define R 9
#define G 10
#define B 11

//Address For Wire Communication
const int addressw = 8;


//Variabales for rgb color
int r, g, b;
//Utility for Color Sensor
void readColor();
int readRGB(int dataForS2, int dataForS3);
int smallestRGB(int r, int g, int b);
void writeColor(int value);

//Utility for 
void buzzerOn();
void buzzerOff();
void colorSensorOn();
void colorSensorOff();


void setup() {

  //Initalizing Buzzer
  pinMode(Buzzer, OUTPUT);
  //Initializing Color Sensor
  pinMode(EN, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(OUT, INPUT);
  digitalWrite(EN, HIGH);
  digitalWrite(S0, HIGH);
  digitalWrite(S1, HIGH);

  //Wire Communication Initalization
  Wire.begin(addressw);
  //Initalize Callback Event
  Wire.onReceive(OnEvent);
  Serial.begin(9600);


  buzzerOff();
  RGB(255, 255, 255);
}

void loop() {
}
/**
 * @brief Callback For Wired Communication
 * 
 * @param Counting 
 */
void OnEvent(int Counting)
{
  if (Wire.available())
  {
    int command = Wire.read();
    Serial.println(command);
    if (command == 1)
      buzzerOn();
    if (command == 2)
      colorSensorOn();
    if (command == 3)
      buzzerOff();
    if (command == 4)
      colorSensorOff();
  }
}
/**
 * @brief Trun On Buzzer
 * 
 */
void buzzerOn()
{
  tone(Buzzer, 1000);
}
/**
 * @brief Turn Off Buzzer
 * 
 */
void buzzerOff()
{
  noTone(Buzzer);
}
/**
 * @brief Check the Value of Color Sensor, Accordingly glow RGB LED
 * 
 */
void colorSensorOn()
{
  digitalWrite(EN, LOW);
  readColor();
  Serial.print(r);
  Serial.print(" ");
  Serial.print(g);
  Serial.print(" ");
  Serial.print(b);
  Serial.print(" ");
  writeColor(smallestRGB(r, g, b));
}
/**
 * @brief Trun off Color Sensor
 * 
 */
void colorSensorOff()
{
  digitalWrite(EN, HIGH);
   RGB(255, 255, 255);
}
/**
 * @brief Read RGB color Data one by one
 * 
 */
void readColor()
{
  r = readRGB(LOW, LOW);
  g = readRGB(HIGH, HIGH);
  b = readRGB(LOW, HIGH);
}
/**
 * @brief Read data from Color Sensor and return color according to value of parameters
 * 
 * @param dataForS2 High or low for reading perticular color
 * @param dataForS3 High or low for reading perticular color
 * @return int 
 */
int readRGB(int dataForS2, int dataForS3)
{
  digitalWrite(S2, dataForS2);
  digitalWrite(S3, dataForS3);
  int out = pulseIn(OUT, LOW);
  digitalWrite(S2, HIGH);
  digitalWrite(S3, LOW);
  return out;
}
/**
 * @brief Calculate Smallest Value in for Color Commination
 * 
 * @param r red color value
 * @param g green color value
 * @param b vlue color value
 * @return int smallest of all
 */
int smallestRGB(int r, int g, int b)
{
  if (r < g && r < b)
    return  0;
  else if (g > b)
    return 2;
  else
    return 1;
}
/**
 * @brief Right Color Value according to Value
 * 
 * @param value 
 */
void writeColor(int value)
{
  if (value == 0)
    RGB(0, 255, 255);
  if (value == 1)
    RGB(255, 0, 255);
  if (value == 2)
    RGB(255, 255, 0);
}
/**
 * @brief Right Color value to RGB LED
 * 
 * @param r red color
 * @param g green color
 * @param b blue color
 */
void RGB(int r, int g, int b)
{
  analogWrite(R, r);
  analogWrite(G, g);
  analogWrite(B, b);
}



