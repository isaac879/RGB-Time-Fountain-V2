/*--------------------------------------------------------------------------------------------------------------------------------------------------------
 * //TODO:this header
 * CHECK THE CODE FOR "TODO:" AND EDIT APPROPRIATELY 
 * 
 * The code is developed for a RGB Time Fountain. The RGB Time Fountain is controlled by an Arduino Uno, HC-05 Bluetooth module and custom circuits. 
 * 
 * CAD files available at: Not currently publicly available.
 * 
 * All measurements are in SI units unless otherwise specified
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE
 * 
 * Code written by isaac879
 * 
 * Last modified 11/12/2019
 *--------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "timeFountain.h"
#include <Iibrary.h>//A library I created for Arduino that contains some simple functions I commonly use. Library available at: https://github.com/isaac879/Iibrary

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

//TODO:
//fix brightness issue with sin functions
//micros overflow issue?
//look into the bluetooth connecting setting the mode???
//save valalues in eeprom
//read eeprom values
//restructure code into header and cpp files
//are the port manipulation macros the fastest?
//Version number
//header on the files
//calibrate estimated frequency. it seems to be about 0.4hz too low//fine at low frequencies so just add the extra micros execution time
//todo: just sending p

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

extern unsigned int base_loop_count;
extern unsigned int red_count;
extern unsigned int green_count;
extern unsigned int blue_count;
extern bool flag_ms_start;
extern bool flag_ms_stop;
extern unsigned long us;
extern unsigned int red_duty_value;
extern unsigned int green_duty_value;
extern unsigned int blue_duty_value;
extern float red_duty;
extern float green_duty;
extern float blue_duty;
extern unsigned int base_count;
extern bool flag_red_sin;
extern bool flag_green_sin;
extern bool flag_blue_sin;
extern int red_sin_direction;
extern int green_sin_direction;
extern int blue_sin_direction;
extern float base_loop_count_temp;
extern short sin_amplitude;
extern float sin_count;
extern float sin_count_increment;
extern unsigned int red_max_count;
extern unsigned int green_max_count;
extern unsigned int blue_max_count;

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void setup(){
    initTimeFountain(); //initialize ports, serial, I2C, Interupts, mode etc.
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void loop(){   
    if(Serial.available()){//Checks if serial data is available
        char c = Serial.read();//read and store the first character sent
        switchCase(c);//TODO: test with: switchCase(Serial.read()); and/or switchCase((char)Serial.read());
    }

    if(red_count < red_duty_value){//Switch LEDs on/off for the set duty/frequency
        PORTD |= _BV(RED_PIN);  //write port HIGH
    }
    else{
        PORTD &= ~_BV(RED_PIN);  //write port LOW
    }
    
    if(green_count < green_duty_value){
        PORTD |= _BV(GREEN_PIN);  //write port HIGH
    }
    else{
        PORTD &= ~_BV(GREEN_PIN);  //write port LOW
    }
    
    if(blue_count < blue_duty_value){
        PORTD |= _BV(BLUE_PIN);  //write port HIGH
    }
    else{
        PORTD &= ~_BV(BLUE_PIN);  //write port LOW
    }

    base_count++;//Increment counts  
    red_count++;
    green_count++;
    blue_count++;

    if(base_count >= base_loop_count){
        base_count = 0;
        if(flag_ms_stop){
            us = micros() - us;
            flag_ms_stop = false;
            flag_ms_start = false;
            float hz = 1/((float)us/1000000.0);
            printi(F("Estimated frequency: "), hz, 3, F("Hz\n"));
        }
        if(flag_ms_start){
            us = micros();
            flag_ms_stop = true;
        }
        if(flag_red_sin){
            if(red_sin_direction == POSITIVE){
                red_count = base_loop_count_temp * (sin_amplitude * cos(sin_count) + sin_amplitude);
            }
            else{
                red_count = base_loop_count_temp * (360 - (sin_amplitude * cos(sin_count) + sin_amplitude));
            }
        }
        if(flag_green_sin){
            if(green_sin_direction == POSITIVE){
                green_count = base_loop_count_temp * (sin_amplitude * cos(sin_count) + sin_amplitude);
            }
            else{
                green_count = base_loop_count_temp * (360 - (sin_amplitude * cos(sin_count) + sin_amplitude));
            }
            
        }
        if(flag_blue_sin){
            if(blue_sin_direction == POSITIVE){
                blue_count = base_loop_count_temp * (sin_amplitude * cos(sin_count) + sin_amplitude);
            }
            else{
                blue_count = base_loop_count_temp * (360 - (sin_amplitude * cos(sin_count) + sin_amplitude));
            }
        }
        
        sin_count += sin_count_increment;//0.0174533 rads = 1 deg
        if(sin_count >= 6.283188){//6.283188 radians in 360 degrees
        }
    }

    if(red_count >= red_max_count){
        red_count = 0;
    }
    if(green_count >= green_max_count){
        green_count = 0;
    }
    if(blue_count >= blue_max_count){
        blue_count = 0;
    }  
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
