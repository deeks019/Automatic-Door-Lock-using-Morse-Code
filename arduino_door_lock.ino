#include <Servo.h>

const int textButton = 2;
const int spaceButton = 3;
const int buzzerPin = 4;
const int ledPin = 5;
const int servoPin = 6;

String morse = "";
String decodedText = "";
const String password = "HI";  // Morse password

unsigned long pressTime;
bool isPressed = false;

Servo doorServo;

void setup() {
  pinMode(textButton, INPUT_PULLUP);
  pinMode(spaceButton, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  doorServo.attach(servoPin);
  doorServo.write(0);  // Locked position

  Serial.begin(9600);
}

void loop() {
  // Button for dot/dash
  if (digitalRead(textButton) == LOW && !isPressed) {
    pressTime = millis();
    isPressed = true;
  }

  if (digitalRead(textButton) == HIGH && isPressed) {
    unsigned long duration = millis() - pressTime;
    if (duration < 300) {
      morse += ".";
      Serial.print("."); 
      beep(100);
    } else {
      morse += "-";
      Serial.print("-");
      beep(200);
    }
    isPressed = false;
    delay(200);  // Debounce
  }

  // Space button ends letter
  if (digitalRead(spaceButton) == LOW) {
    if (morse.length() > 0) {
      char letter = convertToLetter(morse);
      decodedText += letter;
      Serial.print(" --> ");
      Serial.println(letter);
      morse = "";
    }
    delay(300);  // Debounce
  }

  // Check password
  if (decodedText.length() >= password.length()) {
    if (decodedText == password) {
      Serial.println("✅ Password correct. Door unlocked!");
      digitalWrite(ledPin, HIGH);
      doorServo.write(90); // Unlock (rotate servo)
      delay(2000);         // Wait 5 seconds
      doorServo.write(0);  // Lock again
      digitalWrite(ledPin, LOW);
    } else {
      Serial.println("❌ Wrong password.");
    }
    decodedText = "";  // Reset for next attempt
  }
}

void beep(int duration) {
  digitalWrite(buzzerPin, HIGH);
  delay(duration);
  digitalWrite(buzzerPin, LOW);
}

// Morse code translator
char convertToLetter(String code) {
  if (code == ".-") return 'A';
  if (code == "-...") return 'B';
  if (code == "-.-.") return 'C';
  if (code == "-..") return 'D';
  if (code == ".") return 'E';
  if (code == "..-.") return 'F';
  if (code == "--.") return 'G';
  if (code == "....") return 'H';
  if (code == "..") return 'I';
  if (code == ".---") return 'J';
  if (code == "-.-") return 'K';
  if (code == ".-..") return 'L';
  if (code == "--") return 'M';
  if (code == "-.") return 'N';
  if (code == "---") return 'O';
  if (code == ".--.") return 'P';
  if (code == "--.-") return 'Q';
  if (code == ".-.") return 'R';
  if (code == "...") return 'S';
  if (code == "-") return 'T';
  if (code == "..-") return 'U';
  if (code == "...-") return 'V';
  if (code == ".--") return 'W';
  if (code == "-..-") return 'X';
  if (code == "-.--") return 'Y';
  if (code == "--..") return 'Z';
  return '?';
}
