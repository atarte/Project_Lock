#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>

#define SS_PIN 53
#define RST_PIN 5
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
char closing_key[] = "C";
int index = 0;

void Open() {
  Serial.println("Opening door");
  digitalWrite(11, HIGH);
  myservo.write(0);
  delay(3000);


  digitalWrite(11, LOW);
}

void Close() {
  Serial.println("Closing door");
  digitalWrite(12, HIGH);
  myservo.write(90);
  delay(2000);

  digitalWrite(12, LOW);
}

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void code_porte() {
  char customKey = customKeypad.getKey();

  if (customKey) {
    digits[index] = customKey;
		Serial.print("Key press : ");
		Serial.println(digits[index]);

		// Closing with a button on the keypad
		if (strcmp(digits[index], closing_key) == 0) {
			Close();

			index = 0;
			return;
		}

    index = index + 1;

    if (index == DLENGTH) {
      //Serial.println(digits);
      index = 0;

      if (strcmp(digits,code) == 0) {
			  Serial.println("Authorized access");

        Open();
      } else {
				Serial.println("Access denied");
				digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)
				delay(2000);

				digitalWrite(12, LOW);
      }
    }
  }
}

void setup() {
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  myservo.attach(9);    // Pin du Servo
  myservo.write(0);    // Initiat servomotor position

  Serial.println("Approximate your card to the reader...");
  Serial.println();

  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
}

void loop() {
  // Check code
  code_porte();

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
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
	  Serial.println("Authorized access");

    Open();

		delay(7000);
		Close();
  }
  else {
		Serial.println("Access denied");
		digitalWrite(12, HIGH);
		delay(2000);

		digitalWrite(12, LOW);
  }
}
