#include <LiquidCrystal.h>
const int buzzer = 10;
int hours = 0; // start hours
int minutes = 0; //start min
int seconds = 10; //start seconds

//LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //<--removed- Different LCD manufacture
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);//Pin Code for Arduino SainSmart LCD 1602 KeyPad

void setup() {
	pinMode(buzzer, OUTPUT);
}

void loop() {
	lcd.begin(16, 2);
	lcd.print("Count Down Timer ");

	// lcd.scrollDisplayLeft();
	// wait a bit:
	delay(150);

	while (hours > 0 || minutes > 0 || seconds >= 0) {

		lcd.setCursor(4, 2);

		(hours < 10) ? lcd.print("0") : NULL;
		lcd.print(hours);
		lcd.print(":");
		(minutes < 10) ? lcd.print("0") : NULL;
		lcd.print(minutes);
		lcd.print(":");
		(seconds < 10) ? lcd.print("0") : NULL;
		lcd.print(seconds);
		lcd.display();
		stepDown();
		delay(1000);
	}
}

void stepDown() {
	if (seconds > 0) {
		seconds -= 1;
	} else {
		if (minutes > 0) {
			seconds = 59;
			minutes -= 1;
		} else {
			if (hours > 0) {
				seconds = 59;
				minutes = 59;
				hours -= 1;
			} else {
				trigger();
			}
		}
	}
}

void trigger() {
	lcd.clear(); // clears the screen and buffer
	lcd.setCursor(5, 1); // set timer position on lcd for end.

	lcd.println("completed!");
	delay(1000);
	digitalWrite(10,50);
	delay(500);
	digitalWrite(10,0);

	lcd.display();
}
