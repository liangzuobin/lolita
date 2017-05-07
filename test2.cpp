#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int buttonPin1 = A1;
const int buttonPin2 = A0;
const int ledPin = 6;
const int buzzerPin = 10;
int count = 0;
float reaction = 0;
int timeDelay, timeNumber;

void setup() {
	lcd.begin(16, 2);
	pinMode(buttonPin1, INPUT); //start button
	pinMode(buzzerPin, OUTPUT);
	pinMode(buttonPin2, INPUT);
	pinMode(ledPin, OUTPUT);//end button




	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Boxing");
	lcd.setCursor(0, 1);
	lcd.print("Punch Reaction");
	delay(2000);
	lcd.clear();
	delay(1000);
}

void loop() {

	SetCountdown(); //function to set the countdown

}

void SetCountdown() {
	lcd.clear();
	lcd.print("Please set the");
	lcd.setCursor(0, 1);
	lcd.print("Countdown");
	delay(1000);
	lcd.clear();


	while (digitalRead(buttonPin1) == HIGH) //while you not press START
	{
		if (digitalRead(buttonPin1) == LOW) //if start is pressed
		{
			analogWrite(10, 50);
			delay(200);
			analogWrite(10, 0);
			analogWrite(10, 50);
			delay(200);
			analogWrite(10, 0);
			lcd.print("Get ready");
			delay(2000);
			lcd.clear();
			timeNumber = random(1, 10);//generates a number between 1 and 12
			timeDelay = timeNumber * 1000;//times this number by 1000 to get the time in milliseconds
			delay(timeDelay);//delay the program by this time period

			lcd.print("Punch Me");
			digitalWrite(ledPin, HIGH);//turn on the led
			//tone(buzzerPin, 1000, 1);//trigger the buzzer
			analogWrite(10, 50);
			delay(500);
			analogWrite(10, 0);
			lcd.clear();

			do{
				delay(1);
				reaction++;
			} while (digitalRead(buttonPin2) == LOW);

			reaction = reaction / 1000;
			digitalWrite(ledPin, LOW);
			lcd.setCursor(0, 0);
			lcd.print(reaction);
			delay(10000);

		}
	}
}
