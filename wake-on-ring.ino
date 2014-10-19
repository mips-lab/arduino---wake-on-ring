// Ce sketch arduino permet d'allumer un ordinateur via le Wake on Ring
// en appliquant une tension sur le pin 9 du port série
// Au MIPS-lab, il faut envoyer en UDP à l'arduino "lamex" où x est le numéro de la
// lame du Cray à activer.

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x4F, 0x39 }; // Défini l'adresse MAC
IPAddress ip(192, 168, 42, 250); // Défini l'adresse IP

int localPort = 9694; // Défini le port

int pin_LAME1 = 5;
int pin_LAME2 = 6;
int pin_LAME3 = 2;
int pin_LAME4 = 3;

int pin_STATUS1 = 10;
int pin_STATUS2 = 11;
int pin_STATUS3 = 12;
int pin_STATUS4 = 13;


int lame;
int lameNumber;
char packetBuffer[32];	// Où sont stockés les données reçues.x
char ReplyBuffer[] = " OK "; // Réponse de l'arduino
char ReplyOFF[] = " OFF ";
char ReplyON[] = " ON ";
int demande = 0;
String op;

EthernetUDP Udp;

void setup() {
	Ethernet.begin(mac,ip); // Initialise le shield ethernet
	Udp.begin(localPort); // Initialise la communication UDP
	Serial.begin(9600);
	pinMode(pin_LAME1, OUTPUT);
	pinMode(pin_LAME2, OUTPUT);
	pinMode(pin_LAME3, OUTPUT);
	pinMode(pin_LAME4, OUTPUT);
	pin_LAME1 = LOW;
	pin_LAME2 = LOW;
	pin_LAME3 = LOW;
	pin_LAME4 = LOW;

	pinMode(pin_STATUS1, INPUT);
	pinMode(pin_STATUS2, INPUT);
	pinMode(pin_STATUS3, INPUT);
	pinMode(pin_STATUS4, INPUT);
}

void sendStatus(int lame, int lameNumber) {
  int Status = digitalRead(lame);
  Serial.print("status de la lame ");
  Serial.print(lameNumber);
	Serial.print(" : ");
	Serial.println(lame);
  Udp.beginPacket(Udp.remoteIP(), 9694);
  if (Status)
    {
      Udp.write(ReplyON);
    }
  else
    {
      Udp.write(ReplyOFF);
    }
  Udp.endPacket();
}

void loop() {
	int packetSize = Udp.parsePacket();
	if(packetSize) { // Si il y a un paquet
		Udp.read(packetBuffer,32); // le lire
		op = String(packetBuffer).substring(0,4);
		lame = packetBuffer[4]-'0'; // et convertir le 5e caractére en chiffre (char -> int)
		if(op == "lame") {
		  switch (lame) {
		  case 1:
		    pin_LAME1 = HIGH;
		    Serial.println("Active la lame 1");
		    delay(1000);
		    pin_LAME1 = LOW;
		    break;
		  case 2:
		    pin_LAME2 = HIGH;
		    Serial.println("Active la lame 2");
		    delay(1000);
		    pin_LAME2 = LOW;
		    break;
		  case 3:
		    pin_LAME3 = HIGH;
		    Serial.println("Active la lame 3");
		    delay(1000);
		    pin_LAME3 = LOW;
		    break;
		  case 4:
		    pin_LAME4 = HIGH;
		    Serial.println("Active la lame 4");
		    delay(1000);
		    pin_LAME4 = LOW;
		    break;
		  }

		  Udp.beginPacket(Udp.remoteIP(), 9694);
		  Udp.write(ReplyBuffer);
		  Udp.endPacket();
	  } else if(op == "stat") {
		  switch(lame) {
 		  case 1:
		    sendStatus(pin_STATUS1, 1);
		    break;
		  case 2:
		    sendStatus(pin_STATUS2, 2);
		    break;
		  case 3:
		    sendStatus(pin_STATUS3, 3);
		    break;
		  case 4:
		    sendStatus(pin_STATUS4, 4);
		    break;
		  }
	}
}
	delay(10);
}
