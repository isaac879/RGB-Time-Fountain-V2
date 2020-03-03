/*--------------------------------------------------------------------------------------------------------------------------------------------------------
 * CHECK THE CODE FOR "TODO:" AND EDIT APPROPRIATELY 
 * 
 * The code is developed for a RGB Time Fountain. The RGB Time Fountain is controlled by an Arduino Nano, ATtiny85, JDY-30 Bluetooth module (HC-05 and other serial pass-through modules should also be compatable but please check before using them.) and custom circuits. 
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

#include <TinyWireS.h>

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef TWI_RX_BUFFER_SIZE
#define TWI_RX_BUFFER_SIZE (16)
#endif

#define I2C_ADDRESS_ATTINY85 0x25 //0b00100101 (Decimal 37)  
#define PUMP_PIN PB1 //Physical pin 6
#define PHOTOTRANSISTOR_PIN PB4 //Physical pin 3
#define SDA_PIN PB0 //Physical pin 5
#define SCL_PIN PB2 //Physical pin 7
#define CIRCULAR_BUFFER_SIZE 15
#define HZ_MIN 50
#define HZ_MAX 60
#define DEADZONE 0.1

unsigned int last_state;
unsigned int ir_state;
unsigned long us_now;
unsigned long us_last;
unsigned long us_elapsed;
unsigned long ms_now;
unsigned long ms_last;
unsigned long period_buffer[CIRCULAR_BUFFER_SIZE];
int index = 0;
float error = 0;
volatile int PWM = 0;
volatile float average_frequency = 0; 
volatile float set_point_frequency = 0;
volatile bool FLAG_PWM_CONTROL = false;

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

double mapNumber(double x, double in_min, double in_max, double out_min, double out_max) {//Remaps a number to a given range
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void resetBuffer(void){
    for(int i = 0; i < CIRCULAR_BUFFER_SIZE; i++){
        period_buffer[i] = 0;
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void receiveEvent(uint8_t howMany){
    if (howMany < 1) return;// Sanity-check
    if (howMany > TWI_RX_BUFFER_SIZE) return;// Also insane number
    if (!howMany) return;// This write was only to set the buffer for next read

    String pwmStr = "";
    while(howMany--){
        char c = TinyWireS.receive();
        pwmStr += c;
    }
    float pwm = pwmStr.toFloat();

    if(pwm == 0){
        set_point_frequency = 0;
        PWM = 0;
        FLAG_PWM_CONTROL = false;
    }
    else if(pwm >= HZ_MIN && pwm <= HZ_MAX){
        set_point_frequency = pwm;
        average_frequency = 0;
        resetBuffer();
        PWM = round(mapNumber(set_point_frequency, 0, HZ_MAX, 0, 255));
        FLAG_PWM_CONTROL = false;
    }
    else if(pwm < 0 && pwm >= -255){
        set_point_frequency = 0;
        average_frequency = 0;
        resetBuffer();
        PWM = -pwm;
        analogWrite(PUMP_PIN, PWM);
        FLAG_PWM_CONTROL = true;       
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void setup(){
    resetBuffer(); //set all buffer values to 0
    pinMode(PUMP_PIN, OUTPUT);
    
    TCCR0A = (2 << COM0A0) | (2 << COM0B0) | (3 << WGM00);
    TCCR0B = (0 << WGM02) | (1 << CS01);//prescaler to 8
    TCCR1 = (0 << PWM1A) | (0 << COM1A0) | (1 << CS10);
    GTCCR = (1 << PWM1B) | (2 << COM1B0);
  
    TinyWireS.begin(I2C_ADDRESS_ATTINY85);
    TinyWireS.onReceive(receiveEvent);
    
    analogWrite(PUMP_PIN, 0);
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void loop(){
    if(FLAG_PWM_CONTROL != true){
        ir_state = (PINB & 0x10); //read the pin state
        if((ir_state == 0) && (last_state != 0)){ //On a falling edge
            us_now = micros();//get current microsecond count
            us_elapsed = us_now - us_last;
            us_last = us_now;//update the time this was last executed       
            if(us_elapsed >= 133333 && us_elapsed <= 200000 ){//40-60hz period //I think that the number of microseconds passed needed to be 8x higher because I set the clock to 8MHz instead of 1MHz
                PORTB |= (1 << PB3); //set PB3 high
                period_buffer[index] = us_elapsed;//store values
                index = (++index) % CIRCULAR_BUFFER_SIZE;//increments and resets the circular buffer index
                unsigned long avgSum = 0;
                for(int i = 0; i < CIRCULAR_BUFFER_SIZE; i++){//sum the values in the buffer
                    avgSum += period_buffer[i];
                }
                avgSum = avgSum / CIRCULAR_BUFFER_SIZE;
                if(avgSum != 0){ 
                    average_frequency = 8000000.0 / avgSum;//calulate the average frequency
                }
            }
        }
        last_state = ir_state;
    
        ms_now = millis();
        if(ms_now - 2400 > ms_last){//only execue every 50 ms (8 * 50ms for 8MHz clock)
            ms_last = ms_now;
            error = set_point_frequency - average_frequency;
            if(abs(error) <= DEADZONE){
                error = 0;
            }
            if(error > 0){
                PWM = PWM + 1;
                if(PWM > 255){
                    PWM = 255;
                }
            }
            else if(error < 0){
                PWM = PWM - 1;
                if(PWM < 210 && (set_point_frequency >= HZ_MIN && set_point_frequency <= HZ_MAX)){
                    PWM = 210;
                }
                if(PWM < 0){
                    PWM = 0;
                }
            }
            analogWrite(PUMP_PIN, PWM);
        }
    
        PORTB &= (~(1 << PB3)); //set PB3 low
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
