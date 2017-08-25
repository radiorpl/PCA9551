#include "Automaton.h"
#include "Atm_sev_seg.h"

Atm_sev_seg displayMain;
Atm_button btn;

void setup() {
	Serial.begin( 9600 );
	Serial.println("serial");
	displayMain.begin();
	btn.begin(6);
	btn.onPress(displayMain, displayMain.EVT_OFF);
	displayMain.trace( Serial );
}

void loop() {
	automaton.run();
}