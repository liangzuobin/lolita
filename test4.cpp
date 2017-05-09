#include <PinChangeInterrupt.h>
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>

LiquidCrystal lcd(12, 11, 5, 4, 6, 7);

const int delay_millis = 1000; // LCD display delay
const int pin_A = 2; // pin 2, rotary pin
const int pin_B = 3; // pin 3, rotary pin, but no use
const int pin_timer = 8; // pin 8, countdown timer pin
const int pin_counter = 9; // pin 9, countdown counter pin
const int buzzerPin = 10; // pin 10, buzzer
unsigned char encoder_A;
unsigned char encoder_A_prev = LOW;
unsigned char encoder_timer;
unsigned char encoder_timer_prev = LOW;
unsigned char encoder_counter;
unsigned char encoder_counter_prev = LOW;


volatile bool status_ideal = true; // statuds, is ideal now 
volatile bool status_jumping = false; // status, is user jumping now
volatile bool status_counter = false; // status, countdown jumps counter has been set
volatile bool status_timer = false; // status, countdown timer has been set
volatile bool status_buzzer = false; // status, buzzer
volatile unsigned int jumps_count = 0;  // jumps count, as its name shows
volatile unsigned int countdown_counter = 0; // goals of jumps to countdown
volatile unsigned int countdown_timer = 0;
volatile unsigned long jumps_start_time = 0;
volatile unsigned long jumps_current_time = 0;
int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};
int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};

void setup() {
	// pins in the app
	pinMode(pin_A, INPUT_PULLUP);
	// pinMode(pin_B, INPUT);
	pinMode(pin_timer, INPUT);
	pinMode(pin_counter, INPUT);
	pinMode(buzzerPin, OUTPUT);

	// attach interrupts
	attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(pin_A), interruptRevolving, CHANGE);
	attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(pin_timer), interruptAddTimer, CHANGE);
	attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(pin_counter), interruptAddCounter, CHANGE);

	// begin at port 9600
	Serial.begin(9600);
}

// the arduino main loop
void loop() {
	lcd.begin(16, 2);
	// there are 4(counter / timer have something to do with jumping) status in this app:
	// idea: waiting for jumping
	// counter: countdown counter has been set
	// timer: countdown timer has been set
	// jumping: jumping
	while(true) {  // prevent LCD show unknow characters
		if (status_buzzer) {
			ring();
		}
		if (status_counter) {
			displayCountdownCounter();
		} else if (status_timer) {
			displayCountdownTimer();
		} else if (status_jumping) {
			displayJumps();
		} else {
			displayWelcome();
		}
	}
}

void displayWelcome() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("U jump I count");
	lcd.display();
	delay(delay_millis);
}

void displayJumps() {
	lcd.clear();
	lcd.print("Count: ");
	lcd.print(jumps_count);
	lcd.setCursor(0, 2);
	lcd.print("Cost: ");
	long cost = (jumps_current_time - jumps_start_time) / 1000;
	long m = cost / 60;
	long s = cost % 60;
	lcd.print(m);
	lcd.print(":");
	lcd.print(s);
	lcd.display();
	delay(delay_millis);
}

void displayCountdownCounter() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Remains: ");
	int remains = countdown_counter - jumps_count;
	if (remains < 0) {
		remains = 0;
	}
	lcd.print(remains);
	lcd.setCursor(0, 2);
	lcd.print("Cost: ");
	long cost = (jumps_current_time - jumps_start_time) / 1000;
	long m = cost / 60;
	long s = cost % 60;
	lcd.print(m);
	lcd.print(":");
	lcd.print(s);
	lcd.display();
	delay(delay_millis);
}

void displayCountdownTimer() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Remains: ");
	long remains = countdown_timer - ((jumps_current_time - jumps_start_time) / 1000);
	if (remains < 0) {
		remains = 0;
	}
	long m = remains / 60;
	long s = remains % 60;
	lcd.print(m);
	lcd.print(":");
	lcd.print(s);
	lcd.setCursor(0, 2);
	lcd.print("Count:");
	lcd.print(jumps_count);
	lcd.display();
	delay(delay_millis);
}

void interruptRevolving() {
	if (status_ideal) {
		status_ideal = false;
		status_jumping = true;
		jumps_start_tim = millis();
	}
	encoder_A = digitalRead(pin_A);
	if ((encoder_A_prev == LOW) && (encoder_A == HIGH)) {
		jumps_count++;
		jumps_current_time = millis();
		checkCountdown();
	}
	encoder_A_prev = encoder_A;
}

void interruptAddCounter() {
	encoder_counter = digitalRead(pin_counter);
	if ((encoder_counter_prev == LOW) && (encoder_counter == HIGH)) {
		if (digitalRead(pin_timer) == HIGH) {
			resetAll();
			return;
		}
		if (!status_counter) {
			status_counter = true;
		}
		if (status_timer) {
			status_timer = false;
			countdown_timer = 0;
		}
		if (status_jumping) {
			resetJumps();
		}
		countdown_counter += 50;
		if (countdown_counter > 999) {
			countdown_counter = 999;
		}
	}
}

void interruptAddTimer() {
	encoder_timer = digitalRead(pin_timer);
	if ((encoder_timer_prev == LOW) && (encoder_timer == HIGH)) {
		if (digitalRead(pin_counter) == HIGH) {
			resetAll();
			return;
		}
		if (!status_timer) {
			status_timer = true;
		}
		if (status_counter) {
			status_counter = false;
			countdown_counter = 0;
		}
		if (status_jumping) {
			resetJumps();
		}
		countdown_timer += 30;
		if (countdown_timer > 999) {
			countdown_timer = 999;
		}
	}
}

void resetAll() {
	resetJumps();
	status_timer = false;
	countdown_timer = 0;
	status_counter = false;
	countdown_counter = 0;
}

void resetJumps() {
	status_ideal = true;
	status_jumping = false;
	jumps_count = 0;
	jumps_current_time = 0;
	jumps_start_time = 0;
}

void checkCountdown() {
	if (status_counter) {
		if (countdown_counter <= jumps_count) {
			status_buzzer = true;
		}
		return;
	} 
	if (status_timer) {
		if (countdown_timer <= (jumps_current_time - jumps_start_time) / 1000) {
			status_buzzer = true;
		}
		return;
	}
}

void ring() {
	if (status_buzzer) {
		status_buzzer = false;
	}
	for (int thisNote = 0; thisNote < 8; thisNote++) {
		int noteDuration = 1000 / noteDurations[thisNote];
		tone(buzzerPin, melody[thisNote], noteDuration);
		int pauseBetweenNotes = noteDuration * 1.30;
		delay(pauseBetweenNotes);
		noTone(8);
	}
}