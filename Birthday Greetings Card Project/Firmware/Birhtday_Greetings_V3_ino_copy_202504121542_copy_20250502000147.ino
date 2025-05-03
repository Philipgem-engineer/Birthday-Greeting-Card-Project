//Philip - ESP32 Version - Happy Birthday with Lyrics "Dear Angel", LEDs ON with Beat, and Constant OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

// Define the new SDA and SCL pins for ESP32 S2 Mini
#define SDA_PIN 35
#define SCL_PIN 37

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int speakerPin = 16;
int redLedPin = 18;
int yellowLedPin = 33;
int tempo = 120; // Adjust tempo as needed

// Melody for Happy Birthday (adjusted for "Dear Angel")
char notes[] = "GGAGcB GGAGdc GGecGBA FFEEDC";
// Beats for Happy Birthday (adjusted for "Dear Angel")
int beats[] = { 2, 2, 4, 4, 4, 4, 1,
                2, 2, 4, 4, 4, 4, 1,
                2, 2, 4, 4, 2, 2, 2, 2, 2, // Dear An-gel
                2, 2, 4, 4, 4, 4, 4, 4 };
int length = sizeof(notes) / sizeof(notes[0]);

// Corresponding lyrics with "Dear Angel" split
String lyrics[] = {"Hap-", "py ", "birth-", "day ", "to ", "you", "",
                    "Hap-", "py ", "birth-", "day ", "to ", "you", "",
                    "Hap-", "py ", "birth-", "day ", "dear ", "An-", "gel", "",
                    "Hap-", "py ", "birth-", "day ", "to ", "you", "", ""};

char names[] = {'C', 'D', 'E', 'F', 'G', 'A', 'B',
                'c', 'd', 'e', 'f', 'g', 'a', 'b',
                'x', 'y' };
int tones[] = { 262, 294, 330, 349, 392, 440, 494,  // Octave 4
                523, 587, 659, 698, 784, 880, 988,  // Octave 5
                659, 740 };                          // F#5, G#5 (approximations for x and y)

void playNote(char note, int duration, String lyric) {
  int newduration = duration;
  digitalWrite(redLedPin, HIGH);     // Turn on red LED
  digitalWrite(yellowLedPin, HIGH); // Turn on yellow LED

  display.clearDisplay();
  display.setTextSize(3); // Set text size to 3
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println(lyric);
  display.display();

  for (int i = 0; i < sizeof(names) / sizeof(names[0]); i++) {
    if (names[i] == note) {
      tone(speakerPin, tones[i], newduration);
      Serial.print(lyric); // Print the lyric
      delay(newduration);
      noTone(speakerPin);
      digitalWrite(redLedPin, LOW);
      digitalWrite(yellowLedPin, LOW);
      delay(tempo / 4); // Small pause after lyric/note
      break;
    }
  }
}

void setup() {
  pinMode(speakerPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);   // Set red LED pin as output
  pinMode(yellowLedPin, OUTPUT); // Set yellow LED pin as output
  Serial.begin(115200); // Initialize serial communication for lyrics
  Serial.println("Happy Birthday!");
  digitalWrite(redLedPin, LOW);
  digitalWrite(yellowLedPin, LOW); // Initialize LEDs off

  // Initialize OLED display with custom I2C pins
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  
}

void loop() {
  for (int i = 0; i < length; i++) {
    // Turn LEDs ON at the beginning of each note/lyric
    digitalWrite(redLedPin, HIGH);
    digitalWrite(yellowLedPin, HIGH);
    playNote(notes[i], beats[i] * tempo, lyrics[i]);
    // LEDs are turned OFF within playNote after the tone/delay finishes
    delay(tempo / 2); // Slight pause between notes/lyrics
  }
  Serial.println("\n"); // Newline after the song
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Happy");
  display.setCursor(0, 30);
  display.println("Birthday");
  display.setTextSize(2);
  display.setCursor(0, 50);
  display.println("Dear Angel");
  display.display();
  delay(2000); // Wait 2 seconds before repeating
}