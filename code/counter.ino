/*
 * Title       Encoder ticks counter for Sky Safari
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
 *   This program will count the encoder ticks that Magic Digital Setting Circles is sending
 *     to SkySafari
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
  
  Serial.begin(115200);

}

void loop() {
         Serial.print("az ");
         printEncoderValue(encoderValueAz);
         Serial.print("\t alt ");
         printEncoderValue(encoderValueAl);

         Serial.println("\r");           
        
         delay(50); 
 
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
    Serial.print("-");
  else
    Serial.print("+");

  aval = abs(val);

  Serial.print(aval);  
}
