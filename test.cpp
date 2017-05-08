#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int pin_A = 8; // pin 8
const int pin_B = 9; // pin 9, may useless 
const int buttonPin1 = A1;
const int buttonPin2 = A0;
const int buzzerPin = 10;

unsigned char encoder_A; // may useless due to attach interrupt

unsigned long current_time = 0;
unsigned long jumps_counter = 0;
unsigned long jumps_timer = 0;
unsigned long jumps_start_time = 0;
unsigned int countdown_counter = 0;
unsigned int countdown_timer = 0;
bool jumping = false;
bool countdown = false;
unsigned long cost;
unsigned int m = 0;
unsigned int s = 0;

void setup() {
	lcd.begin(16, 2);
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Hello world");
	delay(2000);
	lcd.clear();

	pinMode(pin_A, INPUT_PULLUP); // rotary pin
	// pinMode(pin_B, INPUT_PULLUP); // rotary pin
	pinMode(buttonPin1, INPUT);  // assume it is the countdown timer button 
	pinMode(buttonPin2, INPUT);  // assume it is the countdown counter button

	pinMode(9, OUTPUT);
	pinMode(buzzerPin, OUTPUT);

	attachInterrupt(pin_A, revolving, CHANGE);
	// attachInterrupt(1, revolving, CHANGE);
	attachInterrupt(buttonPin1, addCountdownTimer, CHANGE);
	attachInterrupt(buttonPin2, addCountdownCounter, CHANGE);

	Serial.begin(9600);
}

void loop() {
	show();
}

void show() {
	lcd.clear();
	lcd.print("Count: ");
	lcd.print(jumps_counter);
	lcd.setCursor(0, 2);
	lcd.print("Cost: ");
	s = cost/1000;
	m = s / 60;
	s = s % 60;
	lcd.print(m);
	lcd.print(":");
	lcd.print(s);
	lcd.display();
	delay(1000);
}

// button1 press event, add 30s per press to the timer.
void addCountdownTimer() {
	if (digitalRead(buttonPin1) == LOW) {
		countdown_timer += 10;
	}
}

// button2 press event, add 10 times to the counter
void addCountdownCounter() {
	if (digitalRead(buttonPin2) == LOW) {
		countdown_counter += 10;
	}
}

bool checkCountDown() {
	if (!countdown) {
		return false;
	}
	if (countdown_timer > 0) {
		return (countdown_timer + jumps_start_time) < current_time;
	}
	if (countdown_counter > 0) {
		return jumps_counter < countdown_counter;
	}
	return true;
}

// not involved right now, need a reset rule here.
void reset() {
	jumping = false;
	countdown = false;
	jumps_counter = 0;
	jumps_start_time = 0;
	jumps_timer = 0;
	countdown_counter = 0;
	countdown_timer = 0;
}

void revolving() {
	if (digitalRead(pin_A) == LOW) {
		return;
	}
	current_time = millis();
	if (!jumping) {
		jumping = true;
		jumps_start_time == millis();
		countdown = ((countdown_timer > 0) || (countdown_counter > 0));
	}
	jumps_counter++; // twice faster than before
	if (jumps_counter > 9999) {
		jumps_counter = 9999;
	}
	cost = current_time - jumps_start_time;
	if (cost > 999999) {
		cost = 999999;
	}
	analogWrite(9, jumps_counter);
	if (checkCountDown()) {
		buzz();
	}
	// show();
}

void buzz() {
	tone(buzzerPin, 1000, 1);
}