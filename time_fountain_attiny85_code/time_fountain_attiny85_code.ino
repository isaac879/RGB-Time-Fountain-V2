/*--------------------------------------------------------------------------------------------------------------------------------------------------------
 * CHECK THE CODE FOR "TODO:" AND EDIT APPROPRIATELY 
 * 
 * The code is developed for a my RGB Time Fountain. The RGB Time Fountain is controlled by an Arduino Nano, ATtiny85, JDY-30 Bluetooth module (HC-05 and other serial pass-through modules should also be compatable but please check before using them.) and custom circuits. 
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

volatile uint16_t pwm = 0;
volatile uint16_t set_point_frequency = 0;

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
    pwm = pwmStr.toFloat();
    analogWrite(PUMP_PIN, pwm); 
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

void setup(){
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

void loop() {
    if(PINB & 0x10){
        PORTB |= (1 << PB3);
    }
    else{
        PORTB &= (~(1 << PB3));
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
