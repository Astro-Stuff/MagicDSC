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
 *   Alt-Azimuth mounts, with Wi-Fi and bluetooth connectitity to Sky Safari.
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

String firmwareVersion = "2.0";
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

const char *ssid = "stargazing";
const char *password = "orionnebula";

WiFiServer server(80);
WiFiClient client;
 
#define enc_az_A 27                        
#define enc_az_B 26                        
#define enc_al_A 25                        
#define enc_al_B 33                        

volatile int lastEncodedAl = 0, lastEncodedAz = 0;
volatile long encoderValueAl = 0, encoderValueAz = 0;

void setup()
{
  delay(1000);
  pinMode(enc_al_A, INPUT_PULLUP);
  pinMode(enc_al_B, INPUT_PULLUP);
  pinMode(enc_az_A, INPUT_PULLUP);
  pinMode(enc_az_B, INPUT_PULLUP);  

  attachInterrupt(digitalPinToInterrupt(enc_al_A), EncoderAl, CHANGE);
  attachInterrupt(digitalPinToInterrupt(enc_al_B), EncoderAl, CHANGE);
  attachInterrupt(digitalPinToInterrupt(enc_az_A), EncoderAz, CHANGE);
  attachInterrupt(digitalPinToInterrupt(enc_az_B), EncoderAz, CHANGE);
  
  //Serial.begin(115200);
  //Serial.println();

  WiFi.softAP(ssid, password);
  server.begin();
}

void loop() {
  client = server.available();   // listen for incoming clients

  if (client) {

    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {  
        int c = client.read();
        if (c == 81) {
         printEncoderValue(encoderValueAz);
         client.print("\t");
         printEncoderValue(encoderValueAl);
         client.print("\r");           
        }else if(c == 86) {
         client.print(firmwareVersion);
         client.print("\r"); 
        }
      }
    }
    // close the connection:
    //client.stop();
   //Serial.println("Client Disconnected.");
   //delay(50); 
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
    client.print("-");
  else
    client.print("+");

  aval = abs(val);

  if (aval < 10)
    client.print("0000");
  else if (aval < 100)
    client.print("000");
  else if (aval < 1000)
    client.print("00");
  else if (aval < 10000) 
    client.print("0");

  client.print(aval);  
}
