#include "timeFountain.h"
#include <Iibrary.h>//A library I created for Arduino that contains some simple functions I commonly use. Library available at: https://github.com/isaac879/Iibrary
#include <Wire.h> //For I2C with the ATtiny85
#include <EEPROM.h>

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

//Global variables
unsigned int base_loop_count = 1750;//1010

unsigned int phase_offset = 10;

float red_duty = 3.0;
float green_duty = 3.0;
float blue_duty = 3.0;
float base_duty = 3.0;

unsigned int red_duty_value = (float)base_loop_count * red_duty / 100.0;//switching value to achieve the desired duty cycle
unsigned int green_duty_value = (float)base_loop_count * green_duty / 100.0;//switching value to achieve the desired duty cycle
unsigned int blue_duty_value = (float)base_loop_count * blue_duty / 100.0;//switching value to achieve the desired duty cycle

unsigned int red_max_count = base_loop_count;
unsigned int green_max_count = base_loop_count;
unsigned int blue_max_count = base_loop_count;

unsigned int red_max_count_2 = base_loop_count;
unsigned int green_max_count_2 = base_loop_count;
unsigned int blue_max_count_2 = base_loop_count;

unsigned int base_count = 0;
unsigned int red_count = 0;
unsigned int green_count = 0;
unsigned int blue_count = 0;
unsigned int red_count_2 = 0;
unsigned int green_count_2 = 0;
unsigned int blue_count_2 = 0;
float pump_drop_frequency = 0;

int red_sin_direction = POSITIVE;
int green_sin_direction = POSITIVE;
int blue_sin_direction = POSITIVE;

//char stringText[MAX_STRING_LENGTH + 1];
char stringText[MAX_STRING_LENGTH + 1];

short sin_amplitude = 180;

bool flag_red_sin = false;
bool flag_green_sin = false;
bool flag_blue_sin = false;

short mode_status = 0;//just used for a status reprot
float base_loop_count_temp = base_loop_count * 0.00277777777777777777777; //1/360

float sin_count = 0;
float sin_count_increment = 0.0174533 * 3; //1 degree =  0.0174533 rads

bool flag_ms_start = true;
bool flag_ms_stop = false;
unsigned long ms;
unsigned long us;

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void serialFlush(){
    while(Serial.available() > 0){
        char c = Serial.read();
    }
} 

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void setBaseStrobeFrequency(unsigned int targetFreq){
    if(targetFreq < 0 || targetFreq > 65535){
        printi(F("ERROR: loop count out of range (0-65,535)... value entered: "), (long)targetFreq);
    }
    else{
        flag_ms_start = true;
        int diff = targetFreq - base_loop_count;
        base_loop_count = targetFreq;
        base_loop_count_temp = base_loop_count * 0.00277777777777777777777;
        
        red_max_count += diff;
        red_max_count_2 += diff;
        red_duty_value = (float)red_max_count * red_duty / 100.0;
        green_max_count += diff;
        green_max_count_2 += diff;
        green_duty_value = (float)green_max_count * green_duty / 100.0;
        blue_max_count += diff;
        blue_max_count_2 += diff;
        blue_duty_value = (float)blue_max_count * blue_duty / 100.0;
        printi(F("Loop count set to: "), base_loop_count);
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void setPhase(int offset, const char colour[]){
    red_count = 0;
    green_count = 0;
    blue_count = 0;
    
    if(strcmp(colour, "R") == 0){
        red_max_count += offset;
        printi(F("red_max_count set to: "), (int)red_max_count);
    }
    else if(strcmp(colour, "G") == 0){
        green_max_count += offset;
        printi(F("green_max_count set to: "), (int)green_max_count);
    }
    else if(strcmp(colour, "B") == 0){
        blue_max_count += offset;
        printi(F("blue_max_count set to: "), (int)blue_max_count);
    }
    else if(strcmp(colour, "RG") == 0){
        red_max_count += offset;
        green_max_count += offset;
        printi(F("red_max_count set to: "), (int)red_max_count);
        printi(F("green_max_count set to: "), (int)green_max_count);
    }
    else if(strcmp(colour, "RB") == 0){
        red_max_count +=  offset;
        blue_max_count += offset;
        printi(F("red_max_count set to: "), (int)red_max_count);
        printi(F("blue_max_count set to: "), (int)blue_max_count);
    }
    else if(strcmp(colour, "GB") == 0){
        green_max_count += offset;
        blue_max_count += offset;
        printi(F("green_max_count set to: "), (int)green_max_count);
        printi(F("blue_max_count set to: "), (int)blue_max_count);
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void setSingleDuty(int duty, char led){
    if(duty < 0 || duty > 100){
        printi(F("ERROR: Duty must be between 0-100%... Duty entered: "), duty);
    }    
    else if(led == 'r' || led == 'R'){
        red_duty = duty;
        red_duty_value = (float)base_loop_count * red_duty / 100.0;//switching value to achieve the desired duty cycle
        printi(F("Red duty set to: "), duty);
    }
    else if(led == 'g' || led == 'G'){
        green_duty = duty;
        green_duty_value = (float)base_loop_count * green_duty / 100.0;//switching value to achieve the desired duty cycle
        printi(F("Green duty set to: "), duty);
    }
    else if(led == 'b' || led == 'B'){
        blue_duty = duty;
        blue_duty_value = (float)base_loop_count * blue_duty / 100.0;//switching value to achieve the desired duty cycle
        printi(F("Blue duty set to: "), duty);
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void setLedDuty(int duty){
    if(duty < 0 || duty > 100){
        printi(F("ERROR: Duty must be between 0-100%... Duty entered: "), duty);
    }
    else{
        base_duty = duty;
        setSingleDuty(duty, 'R');
        setSingleDuty(duty, 'G');
        setSingleDuty(duty, 'B');
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void coloursOn(bool redOn, bool greenOn, bool blueOn){
    if(redOn){
        setSingleDuty(base_duty, 'R');
    }
    else{
        setSingleDuty(0, 'R');
    }
    
    if(greenOn){
        setSingleDuty(base_duty, 'G');
    }
    else{
        setSingleDuty(0, 'G');
    }
    
    if(blueOn){
        setSingleDuty(base_duty, 'B');
    }
    else{
        setSingleDuty(0, 'B');
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void sinDirections(int redDirection, int greenDirection, int blueDirection){
    red_sin_direction = redDirection;
    green_sin_direction = greenDirection;
    blue_sin_direction = blueDirection;
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

ISR(TIMER1_COMPA_vect){//timer1 interrupt
    setPosition(random(360), 'R');
    setPosition(random(360), 'G');
    setPosition(random(360), 'B');
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void setPosition(float phase, char colour){
    if(colour == 'r' || colour == 'R'){
        red_count = base_loop_count_temp * phase;
    }
    else if(colour == 'g' || colour == 'G'){
        green_count = base_loop_count_temp * phase;
    }
    else if(colour == 'b' || colour == 'B'){
        blue_count = base_loop_count_temp * phase;
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void setSinCountIncrement(float deg){
    if(deg < 0 || deg > 360){
        printi(F("ERROR: Increment must be between 0-360... Increment entered: "), deg);
    }
    else{
        sin_count_increment = degToRads(deg);
        printi(F("Increment set to: "), deg);
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void switchCase(char command){
    delay(10); //wait to make sure all data in the serial message has arived 
    memset(&stringText[0], 0, sizeof(stringText)); //clear the array
    while(Serial.available()){//set elemetns of stringText to the serial values sent
        char digit = Serial.read(); //read in a char
        strncat(stringText, &digit, 1); //add digit to the end of the array
        //if(stringText.length() >= MAX_STRING_LENGTH) break;//exit the loop when the stringText array is full
    }
    serialFlush();//Clear any excess data in the serial buffer
    int serialCommandValueInt = atoi(stringText); //converts stringText to an int
    float serialCommandValueFloat = atof(stringText); ////converts stringText to an float
    
//    printi("command:\t", command, "\n");
//    printi("String text:\t");
//    printi(stringText);
//    printi("\n");
    if(command == 'G' && stringText[0] == 0) return;
    switch(command){
        case 'f'://set base strobe frequency
            setBaseStrobeFrequency(serialCommandValueInt);
        break;
        case 'p'://set pump PWM
            setDropFrequency(stringText);
        break;
        case 'm'://set mode
            if(serialCommandValueInt > 0 && serialCommandValueInt <= 48){//set maximum mode number
                setMode(serialCommandValueInt);
                printi(F("Mode set to: "), serialCommandValueInt);
            }
            else{
                printi(F("Invalid mode number... Mode number entered: "), serialCommandValueInt);
            }
        break;
        case 'o'://everything off
            printi(F("Time Fountain switched off...\n"));
            setDropFrequency((char *)"0");
            coloursOn(OFF, OFF, OFF);
        break;
        case 'a':
            sin_amplitude = abs(serialCommandValueInt);
            printi(F("sin_amplitude set to: "), abs(sin_amplitude));
        break;
        case 'd':
            setLedDuty(serialCommandValueInt);
        break;
        case 'r':
            setPhase(serialCommandValueInt, "R");
        break;
        case 'R':
            setSingleDuty(serialCommandValueInt, command);
        break;
        case 'g':
            setPhase(serialCommandValueInt, "G");
        break;
        case 'G':
            setSingleDuty(serialCommandValueInt, command);
        break;
        case 'b':
            setPhase(serialCommandValueInt, "B");
        break;
        case 'B':
            setSingleDuty(serialCommandValueInt, command);
        break;
        case 'q'://set base strobe frequency
            setBaseStrobeFrequency(base_loop_count + 10);
        break;
        case 'w'://set base strobe frequency
            setBaseStrobeFrequency(base_loop_count - 10);
        break;
        case 's':
            setSinCountIncrement(serialCommandValueFloat);
        break;
        case '+'://Bluetooth module sents some test when connecting begging with a + so just ignore it and flush the serial buffer
            delay(50);
            serialFlush();
        break;
        case 'U': //Update the saved valuses in EEPROM
            updateEeprom();
        break;
        case 'H':
            printi(F("______________Status______________\n"));
            printi(F("Firmware version: "));
            printi(F(FIRMWARE_VERSION));
            printi(F("\n"));
            printi(F("Base loop count: "), base_loop_count, F("\n\n")); 
            printi(F("Red loop count: "), red_max_count);
            printi(F("Red duty: "), red_duty, 1, F("\n\n"));
            printi(F("Green loop count: "), green_max_count);
            printi(F("Green duty: "), green_duty, 1, F("\n\n"));
            printi(F("Blue loop count: "), blue_max_count);
            printi(F("Blue duty: "), blue_duty, 1, F("\n\n"));
            printi(F("Mode: "), mode_status, F("\n\n"));
            printi(F("Pump drop frequency: "), pump_drop_frequency, 1, F("Hz\n"));
            printi(F("Sin increment: "), sin_count_increment);
            printi(F("Sin peak "), sin_amplitude);
            printi(F("_____________________________\n\n"));              
        break;
        case 'h'://TODO: correct all values and ensure all commands are included they are correct
            printi(F("_____________________________\n"));
            printi(F("loop count: f\n"));
            printi(F("Pump frequency: p\n"));
            printi(F("Mode: m\n"));
            printi(F("Off: o\n"));
            printi(F("Sin peak: a\n"));
            printi(F("All LED duty cycles: d\n"));
            printi(F("Red duty: r\n"));
            printi(F("Green duty: g\n"));
            printi(F("Blue duty: b\n"));
            printi(F("Red count change: R\n"));
            printi(F("Green count change: G\n"));
            printi(F("Blue count change: B\n"));
            printi(F("Increase loop count: q\n"));
            printi(F("Decrease loop count: w\n"));
            printi(F("Sin increment: s\n"));
            printi(F("Sin peak a\n"));
            printi(F("Display current settings: H\n"));
            printi(F("_____________________________\n\n"));
        break;
        case 'M':
            printi(F("_____________________________\n"));
            printi(F("Modes:\n"));
            printi(F("1-7: Single colours\n"));
            printi(F("8-13: Double colours\n"));
            printi(F("14-19: Bidirectional\n"));
            printi(F("20-23: Tripple variations\n"));
            printi(F("24-30: Single sins\n"));
            printi(F("31-36: Double merging\n"));
            printi(F("37-39: Triple merging\n"));
            printi(F("40: Random Positions\n"));
            printi(F("41-46: Flowing colours\n"));
            printi(F("_____________________________\n\n"));
        break;
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void setMode(int mode){
    mode_status = mode;
    red_count = 0;
    green_count = 0;
    blue_count = 0;
    base_count = 0;
    sin_count = 0;
    flag_red_sin = false;
    flag_green_sin = false;
    flag_blue_sin = false;
    red_max_count = base_loop_count;
    green_max_count = base_loop_count;
    blue_max_count = base_loop_count;
    sinDirections(POSITIVE, POSITIVE, POSITIVE);
    sin_amplitude = 180;
    setLedDuty(base_duty);
    TIMSK1 &= ~(1 << OCIE1A);// disable timer compare interrupt
    
    switch(mode){
        case 1: //Red
            coloursOn(ON, OFF, OFF);
        break;
        case 2://green
            coloursOn(OFF, ON, OFF);
        break;
        case 3://blue
            coloursOn(OFF, OFF, ON);
        break;
        case 4://yellow
            coloursOn(ON, ON, OFF);
        break;
        case 5://purple
            coloursOn(ON, OFF, ON);
        break;
        case 6://cyan
            coloursOn(OFF, ON, ON);
        break;
        case 7://white
            coloursOn(ON, ON, ON);
        break;
        case 8://red green
            red_count = base_loop_count / 2;
            coloursOn(ON, ON, OFF);
        break;
        case 9://red blue
            red_count = base_loop_count / 2;
            coloursOn(ON, OFF, ON);
        break;
        case 10://red cyan
            red_count = base_loop_count / 2;
            coloursOn(ON, ON, ON);
        break;
        case 11://green blue
            green_count = base_loop_count / 2;
            coloursOn(OFF, ON, ON);
        break;
        case 12://green purple
            green_count = base_loop_count / 2;
            coloursOn(ON, ON, ON);
        break;
        case 13://blue yellow
            blue_count = base_loop_count / 2;
            coloursOn(ON, ON, ON);
        break;
        case 14://red green up-down
            setPhase(phase_offset, "R");
            setPhase(-phase_offset, "G");
            coloursOn(ON, ON, OFF);
        break;
        case 15://red blue up-down
            setPhase(phase_offset, "R");
            setPhase(-phase_offset, "B");
            coloursOn(ON, OFF, ON);
        break;
        case 16://red cyan up-down
            setPhase(phase_offset, "R");
            setPhase(-phase_offset, "GB");
            coloursOn(ON, ON, ON);
        break;
        case 17://green blue up-down
            setPhase(phase_offset, "B");
            setPhase(-phase_offset, "G");
            coloursOn(OFF, ON, ON);
        break;
        case 18://green purble up-down
            setPhase(phase_offset, "G");
            setPhase(-phase_offset, "RB");
            coloursOn(ON, ON, ON);
        break;
        case 19://blue yellow up-down
            setPhase(phase_offset, "B");
            setPhase(-phase_offset, "RG");
            coloursOn(ON, ON, ON);
        break;  
        case 20://red green blue
            red_count = base_loop_count;
            green_count = base_loop_count / 3;
            blue_count = base_loop_count * 2 / 3;
            coloursOn(ON, ON, ON);
        break;
        case 21://red green blue up still down
            setPhase(phase_offset, "R");
            setPhase(-phase_offset, "B");
            coloursOn(ON, ON, ON);
        break;
        case 22://red green blue up up down
            setPhase(phase_offset, "R");
            setPhase(phase_offset, "G");
            setPhase(-phase_offset, "B");
            red_count = base_loop_count;
            green_count = base_loop_count / 2;
            coloursOn(ON, ON, ON);
        break;  
        case 23://red green blue up down down
            setPhase(phase_offset, "R");
            setPhase(-phase_offset, "G");
            setPhase(-phase_offset, "B");
            green_count = base_loop_count;
            blue_count = base_loop_count / 2;
            coloursOn(ON, ON, ON);
        break;
        case 24://red sin
            flag_red_sin = true;
            coloursOn(ON, OFF, OFF);
        break;
        case 25://green sin
            flag_green_sin = true;
            coloursOn(OFF, ON, OFF);
        break;
        case 26://blue sin
            flag_blue_sin = true;
            coloursOn(OFF, OFF, ON);
        break;
        case 27://yellow
            flag_red_sin = true;
            flag_green_sin = true;
            coloursOn(ON, ON, OFF);
        break;
        case 28://purple sin
            flag_red_sin = true;
            flag_blue_sin = true;
            coloursOn(ON, OFF, ON);
        break;
        case 29://cyan sin
            flag_green_sin = true;
            flag_blue_sin = true;
            coloursOn(OFF, ON, ON);
        break;
        case 30://white sin
            flag_red_sin = true;
            flag_green_sin = true;
            flag_blue_sin = true;
            coloursOn(ON, ON, ON);
        break;
        case 31://red green merging
            flag_red_sin = true;
            flag_green_sin = true;
            coloursOn(ON, ON, OFF);
            sinDirections(POSITIVE, NEGATIVE, POSITIVE);
        break;
        case 32://red blue merging
            flag_red_sin = true;
            flag_blue_sin = true;
            coloursOn(ON, OFF, ON);
            sinDirections(POSITIVE, POSITIVE, NEGATIVE);
        break;
        case 33://green blue merging
            flag_green_sin = true;
            flag_blue_sin = true;
            coloursOn(OFF, ON, ON);
            sinDirections(POSITIVE, POSITIVE, NEGATIVE);
        break;
        case 34://red cyan merging
            flag_red_sin = true;
            flag_green_sin = true;
            flag_blue_sin = true;
            coloursOn(ON, ON, ON);
            sinDirections(NEGATIVE, POSITIVE, POSITIVE);
        break;
        case 35://green purple merging
            flag_red_sin = true;
            flag_green_sin = true;
            flag_blue_sin = true;
            coloursOn(ON, ON, ON);
            sinDirections(POSITIVE, NEGATIVE, POSITIVE);
        break;
        case 36://blue yellow merging
            flag_red_sin = true;
            flag_green_sin = true;
            flag_blue_sin = true;
            coloursOn(ON, ON, ON);
            sinDirections(POSITIVE, POSITIVE, NEGATIVE);
        break;
        case 37://red green blue purple merging 
            flag_red_sin = true;
            //flag_green_sin = true;
            flag_blue_sin = true;
            coloursOn(ON, ON, ON);
            sinDirections(POSITIVE, POSITIVE, NEGATIVE);
            sin_amplitude = 90;
        break;
        case 38://red green blue cyan merging 
            flag_green_sin = true;
            flag_blue_sin = true;
            coloursOn(ON, ON, ON);
            sinDirections(POSITIVE, POSITIVE, NEGATIVE);
            sin_amplitude = 90;
        break;
        case 39://red green blue yellow merging 
            flag_red_sin = true;
            flag_green_sin = true;
            coloursOn(ON, ON, ON);
            sinDirections(POSITIVE, NEGATIVE, POSITIVE);
            sin_amplitude = 90;
        break;
        case 40://red green blue random positions
            coloursOn(ON, ON, ON);
            TIMSK1 |= (1 << OCIE1A);// enable timer compare interrupt
        break;
        case 41://red green blue rainbow
            coloursOn(ON, ON, ON);
            red_count = base_loop_count;
            green_count = base_loop_count / 3;
            blue_count = base_loop_count * 2 / 3;
            setSingleDuty(45, 'R');
            setSingleDuty(45, 'G');
            setSingleDuty(45, 'B');
        break;
        case 42://red green blue rainbow
            coloursOn(ON, ON, ON);
            red_count = base_loop_count;
            green_count = base_loop_count / 3;
            blue_count = base_loop_count * 2 / 3;
            setSingleDuty(70, 'R');
            setSingleDuty(70, 'G');
            setSingleDuty(70, 'B');
        break;  
        case 43://red cyan flow
            red_count = base_loop_count / 2;
            coloursOn(ON, ON, ON);
            setSingleDuty(70, 'R');
            setSingleDuty(70, 'G');
            setSingleDuty(70, 'B');
        break;
        case 44://green blue flow
            green_count = base_loop_count / 2;
            coloursOn(OFF, ON, ON);
            setSingleDuty(70, 'G');
            setSingleDuty(70, 'B');
        break;
        case 45://green purple flow
            green_count = base_loop_count / 2;
            coloursOn(ON, ON, ON);
            setSingleDuty(70, 'R');
            setSingleDuty(70, 'G');
            setSingleDuty(70, 'B');
        break;
        case 46://blue yellow flow
            blue_count = base_loop_count / 2;
            coloursOn(ON, ON, ON);
            setSingleDuty(70, 'R');
            setSingleDuty(70, 'G');
            setSingleDuty(70, 'B');
        break;
        case 47:
            modeLoops();
        break;
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

int setDropFrequency(char freq[]){
    if(!(atof(freq) >= 0 &&  atof(freq) <= 255)){//60 is the actual maximum pump frequency
        printi(F("Invalid duty... It must be between 0 and 255. Frequency entered: "), atof(freq), 3, F("\n"));
        return -1;
    }
    if(freq[0] == '\0'){//if it's an emty string
        freq[0] = '0';//set to char 0
    }
    Wire.beginTransmission(I2C_ADDRESS_ATTINY85);
    Wire.write(freq, 3);
    int returnVal = Wire.endTransmission();
    if(returnVal == 0){//Sucsessful
        pump_drop_frequency = atof(freq); 
        printi(F("Pump drop rate set to: "));
        printi(freq);
        printi(F("Hz\n"));
        return 0;
    }
    else if(returnVal == 1){//Data too long to fit in transmit buffer
        printi(F("Data too long to fit in transmit buffer\n"));
        return -1;
    }
    else if(returnVal == 2){//Received NACK on transmit of address
        printi(F("Received NACK on transmit of address\n"));
        return -1;
    }
    else if(returnVal == 3){//Received NACK on transmit of data
        printi(F("Received NACK on transmit of data"));
        return -1;
    }
    else if(returnVal == 4){//Other error
        printi(F("Unknown error comunicating on the I2C bus\n"));
        return -1;
    }   
    printi(F("after ifs\n"));
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void updateEeprom(void){//Save current settings
    EEPROM.put(EEPROM_ADDRESS_MODE, mode_status);
    printi(F("\nSaved current settings.\n"));
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void setEepromValues(void){
    EEPROM.get(EEPROM_ADDRESS_MODE, mode_status);
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void initTimeFountain(){
    Serial.begin(9600);//Bluetooth module serial baud rate
    Wire.begin(); //Join the I2C bus as a master
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    PORTD &= ~_BV(RED_PIN);//Sets digital pin low
    PORTD &= ~_BV(GREEN_PIN);//Sets digital pin low
    PORTD &= ~_BV(BLUE_PIN);//Sets digital pin low  

    digitalWrite(A4, LOW);//Disables the internal pull-up resistors for the I2C bus
    digitalWrite(A5, LOW);//Disables the internal pull-up resistors for the I2C bus

    //set timer1 interrupt at 1Hz
    TCCR1A = 0;// set entire TCCR1A register to 0
    TCCR1B = 0;// same for TCCR1B
    TCNT1  = 0;//initialize counter value to 0
    OCR1A = 15624;//m16000000 / (1*1024) - 1;// = (16*10^6) / (36*1024) - 1 (must be <65536)  // set compare match register for 1hz increments
    TCCR1B |= (1 << WGM12);// turn on CTC mode
    TCCR1B |= (1 << CS12) | (1 << CS10);  // Set CS12 and CS10 bits for 1024 prescaler
    setEepromValues();
    setMode(mode_status);
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void modeLoops(void){
    while(mode_status <= 46){
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
                                //TODO: 1/360 calculated when sin amp is set?
                                //add count offset if colour is hight when calculating cos
            if(flag_red_sin){ //TODO: test removing sine amplitue
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
    
    while(mode_status == 47){
        if(Serial.available()){//Checks if serial data is available 
            char c = Serial.read();//read and store the first character sent
            switchCase(c);//TODO: test with: switchCase(Serial.read()); and/or switchCase((char)Serial.read());  
        }
    
        if(red_count < red_duty_value || red_count_2 < red_duty_value){//Switch LEDs on/off for the set duty/frequency
            PORTD |= _BV(RED_PIN);  //write port HIGH
        }
        else{
            PORTD &= ~_BV(RED_PIN);  //write port LOW
        }
        
        if(green_count < green_duty_value || green_count_2 < green_duty_value){
            PORTD |= _BV(GREEN_PIN);  //write port HIGH
        }
        else{
            PORTD &= ~_BV(GREEN_PIN);  //write port LOW
        }
        
        if(blue_count < blue_duty_value || blue_count_2 < blue_duty_value){
            PORTD |= _BV(BLUE_PIN);  //write port HIGH
        }
        else{
            PORTD &= ~_BV(BLUE_PIN);  //write port LOW
        }
    
        base_count++;//Increment counts  
        red_count++;
        green_count++;
        blue_count++;
        red_count_2++;
        green_count_2++;
        blue_count_2++;
    
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
        if(red_count_2 >= red_max_count_2){
            red_count_2 = 0;
        }
        if(green_count_2 >= green_max_count_2){
            green_count_2 = 0;
        }
        if(blue_count_2 >= blue_max_count_2){
            blue_count_2 = 0;
        }
    }

    while(mode_status == 48){
        if(Serial.available()){//Checks if serial data is available 
            char c = Serial.read();//read and store the first character sent
            switchCase(c);//TODO: test with: switchCase(Serial.read()); and/or switchCase((char)Serial.read());  
        }
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
