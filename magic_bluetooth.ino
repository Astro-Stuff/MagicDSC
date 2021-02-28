/*
 * Title       Magic Digital Setting Circles
 * by          Florin M
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Description:
 *   Full featured digital setting circles microcontroller for Equatorial and
 *   Alt-Azimuth mounts, with bluetooth connectivity to Sky Safari.
 *
 * Author: Florin M
 *   https://astrostuff.wixsite.com/magic/magic
 *   
 * Revision history, and newer versions:
 *   See GitHub: https://github.com/MagicDigitalSettingCircles/DSC
 *
 * Documentation:
 *   https://astrostuff.wixsite.com/magic/magic
 *
 */


#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
int request;
 
String firmwareVersion = "1.4";

#define enc_al_A 27                        
#define enc_al_B 26                        
#define enc_az_A 25                        
#define enc_az_B 33                   

volatile int lastEncodedAl = 0, lastEncodedAz = 0;
volatile long encoderValueAl = 0, encoderValueAz = 0;

void setup()
{
  SerialBT.begin("ESP32test");
  pinMode(enc_al_A, INPUT_PULLUP);
  pinMode(enc_al_B, INPUT_PULLUP);
  pinMode(enc_az_A, INPUT_PULLUP);
  pinMode(enc_az_B, INPUT_PULLUP);
  

  attachInterrupt(digitalPinToInterrupt(enc_al_A), EncoderAl, CHANGE);
  attachInterrupt(digitalPinToInterrupt(enc_al_B), EncoderAl, CHANGE);
  attachInterrupt(digitalPinToInterrupt(enc_az_A), EncoderAz, CHANGE);
  attachInterrupt(digitalPinToInterrupt(enc_az_B), EncoderAz, CHANGE);
   
}

void loop() {
if (SerialBT.available() > 0) {
      request = SerialBT.read();
          
      if (request == 81)
      {
         printEncoderValue(encoderValueAz);
         SerialBT.print("\t");
         printEncoderValue(encoderValueAl);
         SerialBT.println("\r");           
      }else if (request == 86)
      {    
        SerialBT.print(firmwareVersion);  
        SerialBT.println("\r");           
      }
    }    
 } 

void EncoderAl() {
  int encodedAl = (digitalRead(enc_al_A) << 1) | digitalRead(enc_al_B);
  int sum  = (lastEncodedAl << 2) | encodedAl;

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValueAl ++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValueAl --;
  lastEncodedAl = encodedAl;
}

void EncoderAz() {
  int encodedAz = (digitalRead(enc_az_A) << 1) | digitalRead(enc_az_B);
  int sum  = (lastEncodedAz << 2) | encodedAz;

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValueAz ++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValueAz --;
  lastEncodedAz = encodedAz;
}

void printEncoderValue(long val)
{  
  unsigned long aval; 

  if (val < 0)
    SerialBT.print("-");
  else
    SerialBT.print("+");

  aval = abs(val);

  if (aval < 10)
    SerialBT.print("0000");
  else if (aval < 100)
    SerialBT.print("000");
  else if (aval < 1000)
    SerialBT.print("00");
  else if (aval < 10000) 
    SerialBT.print("0");

  SerialBT.print(aval);  
}
