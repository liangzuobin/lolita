LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int pin_A = 2; // pin 2, rotary pin
const int pin_B = 3; // pin 3, rotary pin, but not been used
const int pin_timer = 8; // pin 8, countdown timer pin
const int pin_counter = 9; // pin 9, countdown counter pin
const int buzzerPin = 10; // pin 10, buzzer
unsigned char encoder_A; // useless
unsigned char encoder_B; // useless
unsigned char encoder_A_prev = LOW; // useless

volatile bool status_ideal = true; // statuds, is ideal now 
volatile bool status_jumping = false; // status, is user jumping now
volatile bool status_counter = false; // status, countdown jumps counter has been set
volatile bool status_timer = false; // status, countdown timer has been set
volatile unsigned int jumps_count = 0;  // jumps count, as its name shows
volatile unsigned int countdown_counter = 0; // goals of jumps to countdown
volatile unsigned int countdown_timer = 0;
volatile unsigned long jumps_start_time = 0;
volatile unsigned long jumps_current_time = 0;

void setup() {
	pinMode(pin_A, INPUT_PULLUP);
	// pinMode(pin_B, INPUT);
	pinMode(pin_timer, INPUT);
	pinMode(pin_counter, INPUT);
	pinMode(buzzerPin, OUTPUT);
	attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(pin_A), interruptRevolving, CHANGE);
	attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(pin_timer), interruptAddTimer, RISING);
	attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(pin_counter), interruptAddCounter, RISING);
	Serial.begin(9600);
}

void loop() {
	lcd.begin(16, 2);
	while(true) {
		while(status_ideal) {
			displayWelcome();
		}
		while(status_jumping) {
			checkCountdown();
			if (status_counter) {
				displayCountdownCounter();
			} else if (status_timer) {
				displayCountdownTimer();
			} else {
				displayJumps();
			}
		}
	}
}

void displayWelcome() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("U jump I count");
	lcd.display();
	// Serial.println(count);
	delay(1000);
}

void displayJumps() {
	lcd.clear();
	lcd.print("Count: ");
	lcd.print(jumps_count);
	lcd.setCursor(0, 2);
	lcd.print("Cost: ");
	long cost = jumps_current_time - jumps_start_time;
	long s = cost/1000;
	long m = s / 60;
	s = s % 60;
	lcd.print(m);
	lcd.print(":");
	lcd.print(s);
	lcd.display();
	delay(1000);
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
	long cost = jumps_current_time - jumps_start_time;
	long s = cost/1000;
	long m = s / 60;
	s = s % 60;
	lcd.print(m);
	lcd.print(":");
	lcd.print(s);
	lcd.display();
	delay(1000);
}

void displayCountdownTimer() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Remains: ");
	int remains = countdown_timer - (millis() - jumps_start_time);
	if (remains < 0) {
		remains = 0;
	}
	long s = remains / 1000;
	long m = s / 60;
	s = s % 60;
	lcd.print(m);
	lcd.print(":");
	lcd.print(s);
	lcd.setCursor(0, 2);
	lcd.print("Count:")
	lcd.print(jumps_count);
	lcd.display();
	delay(1000);
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
	}
	encoder_A_prev = encoder_A;
}

void interruptAddCounter() {
	if (!status_counter) {
		status_counter = true;
	}
	if (status_timer) {
		status_timer = false;
		countdown_timer = 0;
	}
	if (status_jumping) {
		status_ideal = true;
		status_jumping = false;
		jumps_count = 0;
		jumps_current_time = 0;
		jumps_start_time = 0;
	}
	countdown_counter += 10;
	if (countdown_counter > 999) {
		countdown_counter = 999;
	}
}

void interruptAddTimer() {
	if (!status_timer) {
		status_timer = true;
	}
	if (status_counter) {
		status_counter = false;
		countdown_counter = 0;
	}
	if (status_jumping) {
		status_ideal = true;
		status_jumping = false;
		jumps_count = 0;
		jumps_current_time = 0;
		jumps_start_time = 0;
	}
	countdown_timer += 10;
	if (countdown_timer > 999) {
		countdown_timer = 999;
	}
}

void checkCountdown() {
	if (status_counter) {
		if (countdown_counter - jumps_count) {
			tone(buzzerPin, 1000, 1);
		}
	} 
	if (status_timer) {
		if (countdown_timer - (millis() - jumps_start_time)) {
			tone(buzzerPin, 1000, 1);
		}
	}
}