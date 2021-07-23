#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>

#define SS_PIN 53
#define RST_PIN 5
#define DLENGTH 7
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// Function -------------

/*
	Il faudra suremnent utiliser un "StateDoor"
	Pour savoir dans quelle état et la porte et ne 
	pas trop tourner le servo moteur
*/

void Open() {
	Serial.println("Authorized access");
	digitalWrite(11, HIGH);
	delay(3000);

	// Open servo moteur
	
	digitalWrite(11, LOW);
}

void Close() {
	Serial.println("Access denied");
	digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)
	delay(3000);

	// Close servo moteur

	digitalWrite(12, LOW);
}

// Code -----------------
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
};

byte rowPins[ROWS] = {29, 28, 27, 26}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {25, 24, 23, 22}; //connect to the column pinouts of the keypad

char digits[DLENGTH];
char code[]="071017A";

int index=0;

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void code_porte() {
	char customKey = customKeypad.getKey();

	if (customKey) {
		//Serial.println(customKey);
		digits[index]=customKey;
		index=index+1;

		if(index==DLENGTH){
			//Serial.println(digits);
			index=0;
				
			if(strcmp(digits,code)==0){
				Open();
			} else {
				Close();
			}
		}
	}
}

void setup() {
	Serial.begin(9600);   // Initiate a serial communication
	SPI.begin();      // Initiate  SPI bus
	mfrc522.PCD_Init();   // Initiate MFRC522

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
	
	if (content.substring(1) == "39 38 36 94") {
		Open();
	}
	else if (content.substring(1) != "39 38 36 94") {
		Close();
	}
}
