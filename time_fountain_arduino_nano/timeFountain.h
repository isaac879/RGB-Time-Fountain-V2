#ifndef TIMEFOUNTAIN_H
#define TIMEFOUNTAIN_H

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

#define RED_PIN PD5
#define GREEN_PIN PD6
#define BLUE_PIN PD3

#define MAX_STRING_LENGTH 10

#define I2C_ADDRESS_ATTINY85 0x25 //0b00100101 (Decimal 37) 

#define POSITIVE 1
#define NEGATIVE -1

#define ON true
#define OFF false

#define FIRMWARE_VERSION "2.0.0"
//#define EEPROM_ADDRESS_MODE 36

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
int setDropFrequency(const char []);
void switchCase(char);
void initTimeFountain(void);

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

#endif
