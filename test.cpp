#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int pin_A = 8;// pin 8
const int pin_B = 9;// pin 9
unsigned char encoder_A;
unsigned char encoder_B;
unsigned char encoder_A_prev = 0;
unsigned long count = 0;
unsigned long startTime;
unsigned long currentTime;
unsigned long loopTime;
unsigned int m = 0;
unsigned int s = 0;

void setup(){
	lcd.init();
	delay(20);
	lcd.init();
	lcd.begin(16,2);
	lcd.print("Count: 0");
	// declare pin 9 to be an output:
	pinMode(9, OUTPUT);
	pinMode(pin_A, INPUT_PULLUP);
	pinMode(pin_B, INPUT_PULLUP);
	currentTime = millis();
	loopTime = currentTime;
	Serial.begin(9600);
	startTime = millis();
}

void loop(){
	// get the current elapsed time
	// 5ms since last check of encoder = 200Hz
	loopTime = currentTime;
	encoder_A = digitalRead(pin_A); // Read encoder pins
	encoder_B = digitalRead(pin_B);
	if((!encoder_A) && (encoder_A_prev)) {
		if (count < 999) {
			count++;
		}
		Serial.println(count);
		analogWrite(9, count);
		lcd.clear();
		lcd.print("Count: ");
		lcd.print(count);
		lcd.setCursor(0, 2);
		lcd.print("Cost: ");
		currentTime = millis();
		s = (currentTime - startTime)/1000;
		m = s / 60;
		if m < 999 {
			m = 999
		}
		s = s % 60;
		lcd.print(m);
		lcd.print(":");
		lcd.print(s);
	}
	encoder_A_prev = encoder_A; // Store value of A for next time
	delay(5);
	// Other processing can be done here
}

