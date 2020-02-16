/*--------------------------------------------------------------------------------------------------------------------------------------------------------
 * CHECK THE CODE FOR "TODO:" AND EDIT APPROPRIATELY 
 * 
 * The code is developed for a RGB Time Fountain. The RGB Time Fountain is controlled by an Arduino Nano, ATtiny85, JDY-31 Bluetooth module (HC-05 and other serial pass-through modules should also be compatable but please check before using them.) and custom circuits. 
 * 
 * CAD files available at: https://www.thingiverse.com/isaac879/designs
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
 * Last modified 16/02/2020
 *--------------------------------------------------------------------------------------------------------------------------------------------------------*/

//TODO: After uploading the code the EEPROM values will need to be set before the Time Fountain will work properly. Check the global variables in timeFountain.cpp for defualt values you can set the EEPROM to.
// Also check the switchCase() function in timeFountain.cpp for the various commands you can send.

// I do intend to improve this code and write proper documentations for it but for now you will have to struggle :p

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "timeFountain.h"

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void setup(){
    initTimeFountain(); //initialize ports, serial, I2C, Interupts, mode etc.
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void loop(){ 
    modeLoops(); 
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
