#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>

#define LED_AUTHORIZED 11
#define LED_DENIED 12
#define SS_PIN 53
#define RST_PIN 5
#define PIN_SERVO 9
#define OPEN_ANGLE 0
#define CLOSE_ANGLE 90
#define DLENGTH 7

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo myservo;	// Create Servo instance.

// Code
char card[] = "39 38 36 94";
char code[] = "071017A";

const byte ROWS = 4;    //four rows
const byte COLS = 4;    //four columns

//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
};

byte rowPins[ROWS] = {29, 28, 27, 26};  //connect to the row pinouts of the keypad
byte colPins[COLS] = {25, 24, 23, 22};  //connect to the column pinouts of the keypad

char digits[DLENGTH];
char closing_key = 'C';
int index = 0;

// Open rotate the servo moteur to open the door
void Open() {
	Serial.println("Opening door");

	index = 0; // Reset the key pad 

	digitalWrite(LED_AUTHORIZED, HIGH);
	myservo.write(OPEN_ANGLE);
	delay(3000);
	digitalWrite(LED_AUTHORIZED, LOW);
}

void Blink() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_AUTHORIZED, HIGH);
    delay(500);
    digitalWrite(LED_AUTHORIZED, LOW);
    delay(500);
  }
}

// Close rotate the servo moteur to close the door
void Close() {
	Serial.println("Closing door");

	index = 0; // Reset the key pad 

  myservo.write(CLOSE_ANGLE);
	Blink();
}

// Denied turn on a light to show the user that his access is denied
void Denied() {
	Serial.println("Access denied");

	index = 0; // Reset the key pad 

	digitalWrite(LED_DENIED, HIGH);
	delay(2000);
	digitalWrite(LED_DENIED, LOW);
}

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void code_porte() {
	char customKey = customKeypad.getKey();

	if (customKey) {
		digits[index] = customKey;
		Serial.print("Key press : ");
		Serial.println(digits);

		// Closing with a button on the keypad
		if (digits[index] == closing_key) {
			Close();

			return;
		}

		index++;

		if (index == DLENGTH) {
			
			index = 0; // Reset the key pad 

			if (strcmp(digits,code) == 0) {
				Serial.println("Authorized access key pad");

				Open();
			} else {
				Denied();
			}
		}
	}
}

// Reset take a new code for the keypad
void Reset() {
	Serial.println("Take a new password");

	int i = 0;

	while (i != DLENGTH) {
		char customKey = customKeypad.getKey();

		if (customKey && customKey != closing_key) {
			code[i] = customKey;

			Serial.print("new key press : ");
			Serial.println(code);

			i++;
		}
	}

	Serial.print("new password : ");
	Serial.println(code);
}

// INIT -------------------------------
void setup() {
	Serial.begin(9600);   // Initiate a serial communication
	Serial.println("Initialisation of the system");

	SPI.begin();      // Initiate  SPI bus
	mfrc522.PCD_Init();   // Initiate MFRC522

	myservo.attach(PIN_SERVO);    // Pin du Servo
	myservo.write(OPEN_ANGLE);    // Initiat servomotor position

	pinMode(LED_AUTHORIZED, OUTPUT);
	pinMode(LED_DENIED, OUTPUT);

	Serial.println("Done !");
}

// MAIN -------------------------------
void loop() {
	// Check code
	code_porte();

	// Look for new cards
	if (!mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if (!mfrc522.PICC_ReadCardSerial()) {
		return;
	}

	// Card ID
	String content= "";
	byte letter;

	for (byte i = 0; i < mfrc522.uid.size; i++) {
		content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
		content.concat(String(mfrc522.uid.uidByte[i], HEX));
	}

	Serial.println();
	content.toUpperCase();

	if (content.substring(1) == card) {
		Serial.println("Authorized access card");

		Reset();
	}
	else {
		Denied();
	}
}
