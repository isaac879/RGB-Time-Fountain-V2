/*--------------------------------------------------------------------------------------------------------------------------------------------------------
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE
 * 
 * Code written by isaac879
 *--------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef TIMEFOUNTAIN_H
#define TIMEFOUNTAIN_H

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

#define RED_PIN PD5
#define GREEN_PIN PD3
#define BLUE_PIN PD6

#define MAX_STRING_LENGTH 10

#define I2C_ADDRESS_ATTINY85 0x25 //0b00100101 (Decimal 37) 

#define POSITIVE 1
#define NEGATIVE -1

#define ON true
#define OFF false

#define FIRMWARE_VERSION "2.3.3"

#define EEPROM_ADDRESS_MODE 0
#define EEPROM_ADDRESS_PUMP_DUTY 2
#define EEPROM_ADDRESS_LED_DUTY 6
#define EEPROM_ADDRESS_BASE_LOOP_COUNT 10
#define EEPROM_ADDRESS_SINE_INCREMENT 12

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void serialFlush(void);
void setBaseStrobeFrequency(unsigned int);
void setPhase(int, char []);
void setSingleDuty(int, char);
void setLedDuty(int);
void coloursOn(bool, bool, bool);
void sinDirections(int, int, int);
//ISR(TIMER1_COMPA_vect){//timer1 interrupt
void setPosition(float, char);
void setSinCountIncrement(float);
void setMode(int);
int setDropFrequency(char []);
void switchCase(char);
void initTimeFountain(void);
void modeLoops(void);
void updateEeprom(void);
void setEepromValues(void);
void getSerialData(void);

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

#endif

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
