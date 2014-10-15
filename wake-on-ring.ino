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
 
int lame = 0;
char packetBuffer[32];	// Où sont stockés les données reçues.x
char ReplyBuffer[] = " OK "; // Réponse de l'arduino
int demande = 0;

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
}

void loop() {
	int packetSize = Udp.parsePacket();
	if(packetSize) { // Si il y a un paquet
		Udp.read(packetBuffer,32); // le lire
		lame = packetBuffer[4]-'0'; // et convertir le 5e caractére en chiffre (char -> int)
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
	}
	delay(10);		
}