/*--------------------------------------------------------------------------------------------------------------------------------------------------------
 * CHECK THE CODE FOR "TODO:" AND EDIT APPROPRIATELY 
 * 
 * The code is developed for a RGB Time Fountain. The RGB Time Fountain is controlled by an Arduino Nano, ATtiny85, JDY-31 Bluetooth module (HC-05 and other serial pass-through modules should also be compatable but please check before using them.) and custom circuits. 
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
 * Last modified 03/03/2020
 *--------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "timeFountain.h"

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

//TODO:
    //sweep duty: why is it changing frequency?
    //i2c timeout?

//improve double counts
//improve sine efficiency (see todos)
//fix brightness issue with sin functions
//calibrate estimated frequency. it seems to be about 0.4hz too low//fine at low frequencies so just add the extra micros execution time
//sine/cos three phase
//music reactive? probably change position or duty

///header on the files
///Version number

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void setup(){
    initTimeFountain(); //initialize ports, serial, I2C, Interupts, mode etc.
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void loop(){ 
    modeLoops(); 
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
