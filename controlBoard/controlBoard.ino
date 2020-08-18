/**
 * @file controlBoard.ino
 * @author Umang Patel (1998patelumang@gmail.com)
 * @brief Use to Control Robot using two Joysticks with wirless Communication
 * @version 0.1
 * @date 2018-01-16
 * 
 * 
 */
#include <SPI.h> //Library For Serial Communication
// Libraries For nRF24L01 module Communication
#include <nRF24L01.h> 
#include <RF24.h> 


#define CS 8 //Chip Select Not Pin
#define CE 7 //Chip Enable Pin Used for Communication
#define J1x A0 //Analog Input pin for Joy Stick 1 in X direction
#define J1y A1 //Analog Input pin for Joy Stick 1 in Y direction
#define J1s 2  //Digital Input pin for Joy Stick 1 button
#define J2x A2 //Analog Input pin for Joy Stick 2 in X direction
#define J2y A3 //Analog Input pin for Joy Stick 2 in Y direction
#define J2s 3  //Digital Input pin for Joy Stick 2 button


/**
 * @brief Create object for Wireless Communication with writing Address
 * 
 */
RF24 command(CE, CS);
const byte address[6] = "00001";

/**
 * @brief Joystick Variables 
 * 
 */
int J1xPos, J1yPos, J1Cvalue = 5;
int J2xPos, J2yPos, J2Cvalue = 5;

//Utility Function For Operation
void readJ1();
void readJ2();
void setCommand();
void sendCommand();
void takeCommandJ1();
void takeCommandJ2();
char *visualization(int v);
/**
 * @brief Structure of Sending Message
 * 
 */
struct message
{
  bool buzzerState; //Buzzer Status
  bool colorState;  // Color Status
  int motion;   //Type of Motion
} m1;


void setup()
{

  //Initializing the Wireless Communication
  command.begin();
  command.openWritingPipe(address);
  command.setPALevel(RF24_PA_LOW);
  command.stopListening();
  //Initializing INPUT and OUTPUT pins
  pinMode(J1x, INPUT);
  pinMode(J1y, INPUT);
  pinMode(J2x, INPUT);
  pinMode(J2y, INPUT);
  pinMode(J1s, INPUT_PULLUP);
  pinMode(J2s, INPUT_PULLUP);
  //serial Communication Initialization
  Serial.begin(9600);
}
void loop()
{
  readJ1();
  readJ2();
  setCommand();
  sendCommand();
}
/**
 * @brief Read the Status of Joystick 1
 * 
 */
void readJ1()
{
  J1xPos = analogRead(J1x);
  J1yPos = analogRead(J1y);
  m1.buzzerState = !digitalRead(J1s);
}
/**
 * @brief Read the Status of Joystick 2
 * 
 */
void readJ2()
{
  J2xPos = analogRead(J2x);
  J2yPos = analogRead(J2y);
  m1.colorState = !digitalRead(J2s);
}
/**
 * @brief From State of JoyStick computing Command
 * 
 */
void setCommand()
{
  takeCommandJ1();
  takeCommandJ2();
}
/**
 * @brief Setting Command Value for Joystick 1 according to it's Position
 * 
 */
void takeCommandJ1()
{
  if ((J1xPos < 600 && J1xPos > 450) && (J1yPos < 600 && J1yPos > 450))
    J1Cvalue = 5;
  if (J1xPos < 1024 && J1xPos > 599)
    J1Cvalue = 4;
  if (J1xPos < 451 && J1xPos >= 0)
    J1Cvalue = 3;
  if (J1yPos < 1024 && J1yPos > 599)
    J1Cvalue = 1;
  if (J1yPos < 451 && J1yPos >= 0)
    J1Cvalue = 2;
}

/**
 * @brief Setting Command Value for Joystick 2 according to it's Position
 * 
 */
void takeCommandJ2()
{
  if ((J2xPos < 600 && J2xPos > 450) && (J2yPos < 600 && J2yPos > 450))
    J2Cvalue = 5;
  if (J2xPos < 1024 && J2xPos > 599)
    J2Cvalue = 11;
  if (J2xPos < 451 && J2xPos >= 0)
    J2Cvalue = 10;
  if (J2yPos < 1024 && J2yPos > 599)
    J2Cvalue = 7;
  if (J2yPos < 451 && J2yPos >= 0)
    J2Cvalue = 8;
}
/**
 * @brief Send Command Through Wireless Communication
 * 
 */
void sendCommand()
{
  if (J1Cvalue != 5 && J2Cvalue == 5)
    m1.motion = J1Cvalue;
  else if (J1Cvalue == 5 && J2Cvalue != 5)
    m1.motion = J2Cvalue;
  else
    m1.motion = 5;
  

  //Sending Data to Serial Communication port
  Serial.print(visualization(m1.motion));
  Serial.print(" ");
  Serial.print(m1.buzzerState);
  Serial.print(" ");
  Serial.println(m1.colorState);
  command.write(&m1, sizeof(m1));
  delay(600);
}
/**
 * @brief Convert Code to Motion for visualization
 * 
 * @param value Code for Motion
 * @return char* 
 */
char *visualization(int value)
{
  switch (value)
  {
  case 1:
    return (char *)("Caterpillar Forward Motion");
  case 2:
    return (char *)("Caterpillar Backword Motion");
  case 3:
    return (char *)("Rolling Left Motion");
  case 4:
    return (char *)("Rolling Right Motion");
  case 5:
    return (char *)("Stable Position");
  case 6:
    return (char *)("Buzzer Beeped");
  case 7:
    return (char *)("Serpentine Forward Motion");
  case 8:
    return (char *)("Serpentine Backword Motion");
  case 9:
    return (char *)("Color Sensor Control");
  case 10:
    return (char *)("Sidewinding Left Motion");
  case 11:
    return (char *)("Sidewinding Right Motion");
  }
}
