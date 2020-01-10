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
 * Last modified 10/01/2020
 *--------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "timeFountain.h"
//#include <Iibrary.h>//A library I created for Arduino that contains some simple functions I commonly use. Library available at: https://github.com/isaac879/Iibrary

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

//TODO:
//if serial func for modes
//mode sweep duty
//read sin increment back in degrees...
//improve double counts
//improve sine efficiency (see todos)
//fix brightness issue with sin functions
//save valalues in eeprom
//read eeprom values
//header on the files
//calibrate estimated frequency. it seems to be about 0.4hz too low//fine at low frequencies so just add the extra micros execution time
//Version number

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void setup(){
    initTimeFountain(); //initialize ports, serial, I2C, Interupts, mode etc.
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void loop(){ 
    modeLoops(); 
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
