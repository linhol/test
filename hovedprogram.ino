/*
Dette er et hovedprogram som skal la en bruker scanne en brukerbrikke og deretter et oppgavekort for å få poeng. Poengene vises på en adafruit 24-led neopixel-ring og krever også en rfid-leser.

Programmets hovedfunksjon.
To brukere konkurrerer om å få flest poeng. Man får poeng ved å utføre oppgaver. Kortene som hrer til løsningen inneholder oppgaver.
En bruker gjør en oppgave. For å få poeng for oppgaven scanner brukeren sin brukerbrikke og lysringen lyser brukerens farge. Deretter scanner man oppgavekortet man har utført for å få poeng. Forskjellige oppgaver har forskjellig antall poeng. Første spiller til å lyse opp halve sirkelen vinner.

*/


/*
Oppsett på arduinoen:
LED
Port 6 - Data-In

RFID
Port 9 - RST
Port 10 - NSS
Port 11 - MOSI
Port 12 - MISO
Port 13 - SCK

Husk GND og strøm på begge.
*/


//RFID-setup
#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Dette er RFID-leseren vår.


// LED-setup
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN        6
#define NUMPIXELS 24
#define BRIGHTNESS 50

// Dette er LED-ring-objektet vårt og brukes til å styre den.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Dette er tidverdien som brukes for å få LED-ringen til å lyse i ring.
#define DELAYVAL 30

// Brukere
String bruker1 = "CD 5A 51 7B"; // GRØNN BRUKER
String bruker2 = "56 0C 93 4C"; // RØD BRUKER
String poengKort1 = "A9 C9 A6 99";
String poengKort2 = "92 C9 4E 07";
int poengBruker1 = 0;
int poengBruker2 = 0;
String brukerNaa = "";


 

// Her starter vi å sette opp alle delene våre.
void setup() {
  Serial.begin(9600);   // For å teste RFID starter vi serial monitor.
  // Disse hører til RFID-leseren.
  SPI.begin();
  mfrc522.PCD_Init();

  // LED - Nullstilles. Eventuelt lys skrus av og lysstyrken settes
  pixels.begin();
  pixels.show();
  pixels.setBrightness(BRIGHTNESS);
}

// Her er hovedprogrammet vårt.
void loop() {
  status();
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();



// Dette er et 1-poengers kort.
  if (content.substring(1) == poengKort1) {
    giPoeng(1);
    // Ved å resette brukerNaa slipper man å feilregistrere oppgaver flere ganger.
    brukerNaa = "";
  }

// Dette er et 2-poengers kort.
  else if (content.substring(1) == poengKort2) {
    giPoeng(2);
    brukerNaa = "";
  }
 
 else if (content.substring(1) == bruker1) {
  gronnFyll(); // Denne kan brukes for å fylle hele ringen
//   brukerA();    // Denne fyller halve ringen
  delay(500);
  tomLED();
  }

  else if (content.substring(1) == bruker2) {
  rodFyll();
//   brukerB();
  delay(500);
  tomLED();
  }
}





// HER KOMMER METODENE


// Denne skrur LED-ene av.
void tomLED() {
  pixels.clear();
  pixels.show();
}

// Denne markerer hvilken bruker som er valgt.
// GRØNN
void brukerA() {
  pixels.clear();
  brukerNaa = bruker1;
  for(int i=0; i<(NUMPIXELS/2); i++) {
 
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    pixels.show();
    delay(DELAYVAL);
  }
}

// Denne markerer hvilken bruker som er valgt.
// RØD
void brukerB() {
  pixels.clear();
  brukerNaa = bruker2;
  for(int i=24; i>=(NUMPIXELS/2); i--) {
 
    pixels.setPixelColor(i, pixels.Color(150, 0, 0));
    pixels.show();
    delay(DELAYVAL);
  }
}

// Denne farger alle LED-ene 1 og 1 til fargen grønn.
void gronnFyll() {
  pixels.clear();
  brukerNaa = bruker1;
  for(int i=0; i<NUMPIXELS; i++) {
 
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    pixels.show();
    delay(DELAYVAL);
  }
}

// Denne farger alle LED-ene 1 og 1 til fargen rød.
void rodFyll() {
  pixels.clear();
  brukerNaa = bruker2;
  for (int i = NUMPIXELS; i >= 0; i--) {
    pixels.setPixelColor(i, pixels.Color(150, 0, 0));
    pixels.show();
    delay(DELAYVAL);
  }
}

// Denne viser antall poeng en bruker har
void poengTeller1() {
  pixels.clear();
  for(int i=0; i<poengBruker1; i++) {
 
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    pixels.show();
    delay(DELAYVAL);
  }
}

// Denne viser antall poeng en bruker har
void poengTeller2() {
    pixels.clear();
    for(int i = 24; i >= 24-poengBruker2 && i >= 12; i--) {
      pixels.setPixelColor(i, pixels.Color(150, 0, 0));
      pixels.show();
      delay(DELAYVAL);
  }
}

// Denne viser brukerne opp mot hverandre
void status() {
  for(int i=0; i<poengBruker1; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    pixels.show();
    delay(DELAYVAL/2);
  }
  for(int i = 24; i > 24-poengBruker2-1; i--) {
    pixels.setPixelColor(i, pixels.Color(150, 0, 0));
    pixels.show();
    delay(DELAYVAL/2);
  }
}


void giPoeng (int antPoeng) {
    if (brukerNaa == bruker1) {
      if (poengBruker1 <= 12) {
          poengBruker1 = poengBruker1 + antPoeng;
      }
      poengTeller1();
      delay(1000);
      tomLED();
    }

    else if (brukerNaa == bruker2) {
      if (poengBruker2 <= 12) {
          poengBruker2 = poengBruker2 + antPoeng;
      }
      poengTeller2();
      delay(1000);
      tomLED();
    }
}