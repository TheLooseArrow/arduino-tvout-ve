#include <TVout.h>

TVout TV;
uint8_t W = 184;
uint8_t H = 72;

void setup() {
  TV.begin(NTSC,W,H);
  initOverlay();
  
  //uncomment this line and comment out initOverlay()
  //if you are not using the video experimenter shield:
  //TV.ccTVOutMode();

  TV.delay_frame(300);
  
  //Roll up mode 4 lines
  TV.printCC(0x14, 0x25);
  TV.printCC(0x14, 0x25);

  //Newline
  TV.printCC(0x14, 0x2D);
  TV.printCC(0x14, 0x2D);

  //These are optional control codes that will
  //change color of the text and backgtound
  //they may not be compatible with all TVs

  //TV.printCC(0x11, 0x29);
  //TV.printCC(0x11, 0x29);

  //TV.printCC(0x10, 0x22);
  //TV.printCC(0x10, 0x22);
  
  TV.printCC('H', 'e');
  TV.printCC('l', 'l');
  TV.printCC('o', ' ');

  //More control codes that can change text
  //style and/or background colors. Again 
  //May not be compatible with all TVs
  //TV.printCC(0x14, 0x28);
  //TV.printCC(0x17, 0x2E);
  //TV.printCC(0x10, 0x2D);
  
  TV.printCC('W', 'o');
  TV.printCC('r', 'l');
  TV.printCC('d', '!');

  //Newline
  TV.printCC(0x14, 0x2D);
  TV.printCC(0x14, 0x2D);
}

void loop() {

}

// Initialize ATMega registers for video overlay capability.
// Must be called after tv.begin().
void initOverlay() {
  TCCR1A = 0;
  // Enable timer1.  ICES0 is set to 0 for falling edge detection on input capture pin.
  TCCR1B = _BV(CS10);

  // Enable input capture interrupt
  TIMSK1 |= _BV(ICIE1);

  // Enable external interrupt INT0 on pin 2 with falling edge.
  EIMSK = _BV(INT0);
  EICRA = _BV(ISC01);
}

// Required to reset the scan line when the vertical sync occurs
ISR(INT0_vect) {
  display.scanLine = 0;
}
