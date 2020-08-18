/**
 * @file robotMind.ino
 * @author Umang Patel (1998patelumang@gmail.com)
 * @brief Use to receive the command on wirless channle and perform operations accordingly
 * @version 0.1
 * @date 2018-01-18
 * 
 */

#include<Wire.h> //For Wired Master/Slave Communication
#include<Servo.h> //For Controlling Servo Motor
#include <SPI.h> //Library For Serial Communication
// Libraries For nRF24L01 module Communication
#include <nRF24L01.h> 
#include <RF24.h> 

#define CE 7 //Chip Select Not Pin
#define CS 8 //Chip Enable Pin Used for Communication
//Servo Motor Pin Configuration
#define SP1 A0
#define SP2 A1
#define SP3 2
#define SP4 3
#define SP5 4
#define SP6 5
#define SP7 6
#define SP8 9
Servo s1, s2, s3, s4, s5, s6, s7, s8;
//Buzzer Color Sensor On off commands
#define BCOM 1 //buzzer ON Command
#define CCOM 2 //Color Sensor On Command
#define IBCOM 3 //Buzzer Off Command
#define ICCOM 4 //Color Sensor On Command
#define AR 90


/**
 * @brief Create object for Wireless Communication with writing Address
 * 
 */
RF24 command(CE, CS);
const byte address[6] = "00001";
const int addressw = 8;
//For Processing Command
void processCommand(int command);

//Function according to motion
void sendBuzzerOn();
void sendBuzzerOff();
void sendColorOn();
void sendColorOff();
void RR();
void RL();
void SPC();
void CSC();
void SWL();
void SWR();
void SPF();
void SPB();
void CPF();
void CPB();
void SPF();
void VSC();
void HSC();
char *visualization(int v);
/**
 * @brief Structure of Receiving Message
 * 
 */
struct message{
  bool buzzerState;
  bool colorState;
  int motion;
}m1;

void setup() {
  //Initializing the Wireless Communication
  command.begin();
  command.openReadingPipe(0, address);
  command.setPALevel(RF24_PA_LOW);
  command.startListening();
  //Wire Communication Initalization
  Wire.begin();
  //Initialze Servo Motors
  s1.attach(SP1);
  s2.attach(SP2);
  s3.attach(SP3);
  s4.attach(SP4);
  s5.attach(SP5);
  s6.attach(SP6);
  s7.attach(SP7);
  s8.attach(SP8);
  s1.write(AR);
  s2.write(AR);
  s3.write(AR);
  s4.write(AR);
  s5.write(AR);
  s6.write(AR);
  s7.write(AR);
  s8.write(AR);
  delay(2000);
  s1.detach();
  s2.detach();
  s3.detach();
  s4.detach();
  s5.detach();
  s6.detach();
  s7.detach();
  s8.detach();
  SPC();
  //serial Communication Initialization
  Serial.begin(9600);
}
void loop() {
  
  //Waiting for Command Available
  if (command.available())
  {
   command.read(&m1, sizeof(m1));
  
   Serial.print(visualization(m1.motion));
   Serial.print(" ");
   Serial.print(m1.buzzerState);
   Serial.print(" ");
   Serial.print(m1.colorState);
   Serial.println(" ");
   
   processCommand(m1.motion);
  }
}
/**
 * @brief Call Function according to command Arrives 
 * 
 * @param command 
 */
void processCommand(int command)
{
  switch (command)
  {
    
    case 1: CPF();
     break;
    case 2: CPB();
    break;
    case 3: RL();
    break;
    case 4: RR();
    break;
    case 5: SPC();
    break;
    case 7: SPF();
    break;
    case 8: SPB();
    break;
    case 10: SWL();
    break;
    case 11: SWR();
    break;
  }
  if(m1.buzzerState)
   sendBuzzerOn();
  else
   sendBuzzerOff();
  if(m1.colorState)
  sendColorOn();
  else
  sendColorOff();
}
/**
 * @brief Send BuzzerOn to Wired Slave
 * 
 */
void sendBuzzerOn()
{
  Wire.beginTransmission(addressw);
  Wire.write(BCOM);
  Wire.endTransmission();
}

/**
 * @brief Send BuzzerOff to Wired Slave
 * 
 */
void sendBuzzerOff()
{
  Wire.beginTransmission(addressw);
  Wire.write(IBCOM);
  Wire.endTransmission();
}

/**
 * @brief Send ColorOn to Wired Slave
 * 
 */
void sendColorOn()
{
  Wire.beginTransmission(addressw);
  Wire.write(CCOM);
  Wire.endTransmission();
}

/**
 * @brief Send ColorOff to Wired Slave
 * 
 */
void sendColorOff()
{
  Wire.beginTransmission(addressw);
  Wire.write(ICCOM);
  Wire.endTransmission();
}
//For Caterpillar Forward Motion
void CPF()
{
  VSC(AR+30, AR+15, AR+15, AR+70);
  delay(300);
  VSC(AR-30, AR-15, AR-15, AR-70);
  delay(300);
}
//For Caterpillar BackWord Motion
void CPB()
{

  HSC(AR+60, AR+60, AR+60, AR);
  delay(300);
  HSC(AR, AR, AR, AR);
  delay(300);
}
//For Rolling Right Motion
void RR()
{
  VSC(AR, AR+50, AR+50, AR+50);
  HSC(AR, AR+30, AR+30, AR+30);
  delay(300);
  VSC(AR, AR, AR, AR);
  HSC(AR , AR-30, AR-30, AR-30);
  delay(300);
  VSC(AR, AR-50, AR-50, AR-50);
  HSC(AR, AR-30, AR-30, AR-30);
  delay(300);
  VSC(AR, AR, AR, AR);
  HSC(AR , AR+30, AR+30, AR+30);
  delay(300);
}
//For Rolling Left Motion
void RL()
{
  VSC(AR, AR, AR, AR);
  HSC(AR, AR, AR, AR);
  delay(300);
  VSC(AR, AR, AR, AR);
  HSC(AR, AR, AR, AR);
  delay(300);
}
//For Stable Position
void SPC()
{
  VSC(AR, AR, AR, AR);
  HSC(AR, AR, AR, AR);
  delay(600);
}
//for Sidewinding Left Motion
void SWL()
{
  VSC(AR, AR, AR, AR);
  HSC(AR, AR, AR, AR);
  delay(300);
  VSC(AR, AR, AR, AR);
  HSC(AR, AR, AR, AR);
  delay(300);

}
//For Sidewinding Right Motion
void SWR()
{
  VSC(AR, AR, AR, AR);
  HSC(AR, AR, AR, AR);
  delay(300);
  VSC(AR, AR, AR, AR);
  HSC(AR, AR, AR, AR);
  delay(300);
}
//For Serpentine Forward Motion
void SPF()
{
  VSC(AR, AR, AR, AR);
  HSC(AR, AR, AR, AR);
  delay(300);
  VSC(AR, AR, AR, AR);
  HSC(AR, AR, AR, AR);
  delay(300);

}
//For Serpentine BackWord Motion
void SPB()
{
  VSC(AR, AR, AR, AR);
  HSC(AR, AR, AR, AR);
  delay(300);
  VSC(AR, AR, AR, AR);
  HSC(AR, AR, AR, AR);
  delay(300);
}
//Vertical Servo Command
void VSC(int x, int y, int z, int w)
{
  s1.attach(SP1);
  s3.attach(SP3);
  s5.attach(SP5);
  s7.attach(SP7);
  s1.write(x);
  s3.write(y);
  s5.write(z);
  s7.write(w);
  delay(200);
  s1.detach();
  s3.detach();
  s5.detach();
  s7.detach();
  }
//Horizontal Servo Command
void HSC(int x, int y, int z, int w)
{ s2.attach(SP2);
  s4.attach(SP4);
  s6.attach(SP6);
  s8.attach(SP8);
  s2.write(x);
  s4.write(y);
  s6.write(z);
  s8.write(w);
  delay(200);
  s2.detach();
  s4.detach();
  s6.detach();
  s8.detach();
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
