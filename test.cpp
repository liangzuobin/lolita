/*
 ** Rotary Encoder Example
 ** Use the Sparkfun Rotary Encoder to vary brightness of LED
 **
 ** Sample the encoder at 200Hz using the millis() function
 */

unsigned long currentTime;
unsigned long loopTime;
const int pin_A = 8;// pin 8
const int pin_B = 9;// pin 9
unsigned char encoder_A;
unsigned char encoder_B;
unsigned char encoder_A_prev=0;
unsigned int count = 0;

void setup(){
	// declare pin 9 to be an output:
	pinMode(9, OUTPUT);
	pinMode(pin_A, INPUT_PULLUP);
	pinMode(pin_B, INPUT_PULLUP);
	currentTime = millis();
	loopTime = currentTime;
	Serial.begin(9600)
}

void loop(){
	// get the current elapsed time
	currentTime = millis();
	if(currentTime >= (loopTime + 10)) {
		// 5ms since last check of encoder = 200Hz
		encoder_A = digitalRead(pin_A); // Read encoder pins
		encoder_B = digitalRead(pin_B);
		if((!encoder_A) && (encoder_A_prev)) {
			count++
		}
		Serial.println(count);
		encoder_A_prev = encoder_A; // Store value of A for next time
		// set the brightness of pin 9:
		analogWrite(9, count);
		loopTime = currentTime;// Updates loopTime
	}
	// Other processing can be done here
}
