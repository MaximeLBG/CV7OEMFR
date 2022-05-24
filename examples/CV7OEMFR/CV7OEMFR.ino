#define Lorawan    /*We decide on which system the program should run ! 
You are forced to choose between Lorawan or PC. 
If you work on Adruino IDE choose "#define Lorawan" or if you work on Visual Studio choose "#define PC".*/
#include "CV7OEMFR.h"
#include <EEPROM.h>
#define RXD 16
#define EEPROM_SIZE 10

int i = 0;        //Variable declaration
bool one = false, two = false;
String static strIdModule;

void setup()
{ 
 Serial.begin(115200);
 Serial2.begin(4800,SERIAL_8N1,RXD);
 EEPROM.begin(EEPROM_SIZE);

 String yesno;
 strIdModule = EEPROM.read(0);
 int result;
 
 Serial.println("The old module is : " + strIdModule);
 Serial.println("Do you want to enter a new module ? (y/n)");
 
 Serial.setTimeout(10000);
 yesno = Serial.readStringUntil('\n');
 
 if(yesno == "n" || yesno == 0)
 {
  Serial.println("You are on the module : " + strIdModule);
 }
 else
 {
  Serial.println("Enter the new module : ");
  strIdModule = Serial.readStringUntil('\n');
  
  EEPROM.write(0, strIdModule.toInt());  //second value must not be a string
  EEPROM.commit();
  Serial.print("The new module is : "); Serial.println(strIdModule.toInt());
 }
 delay(5000);
        
}
void loop()
{
 String value=Serial2.readStringUntil('\n');
 Serial.println(value);  //displays the entire frame
 float temp, dir, speeds;

 CV7OEMFR* fram;  //Declaring a pointer in the CV7OEMFR class
 fram = new CV7OEMFR(value);    //Half the time the frame will take either the value of parameter 1 or parameter 2
 if(fram->getFrameType() == 1)        //We call the getTypeFrame() function to find out if the frame is in "$IIMWV" or "$WIXDR" format
 {
  dir = fram->getDirection();      //We save the values ​​of getDirection() and getVitesse() in the expected variables
  speeds = fram->getSpeed();
  one = true;       //dirVit goes to true when the frame is in "$IIMWV" format
 }
  if(fram->getFrameType() == 2)       //We call the getTypeFrame() function to find out if the frame is in "$IIMWV" or "$WIXDR" format
  {
   temp = fram->getTemperature();       //We save the values ​​of getTemperature() in the expected variable
   two = true;      //temp goes to true when the frame is in "$IWXDR" format
  }
   if (one == true && two == true)      // When both are true, save the results
   {
    Serial.print("The direction is : ");
    Serial.println(dir);
    Serial.print("The speed is : ");
    Serial.println(speeds);
    Serial.print("The temperature is : ");
    Serial.println(temp);
    one = false; two = false;        //temp and dirVit go to false and start again in the loop
   }
}
