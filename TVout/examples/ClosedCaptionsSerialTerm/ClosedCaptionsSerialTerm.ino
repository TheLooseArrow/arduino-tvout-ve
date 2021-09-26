#include <TVout.h>
#include <pollserial.h>
#include <fontALL.h>

TVout TV;
pollserial pserial;
uint8_t W = 184;
uint8_t H = 72;

void setup() {
  TV.begin(NTSC,W,H);
  TV.select_font(font6x8);
  TV.set_hbi_hook(pserial.begin(19200));
  initOverlay();

  //uncomment this line and comment out initOverlay()
  //if you are not using the video experimenter shield:
  //TV.ccTVOutMode();
  
  TV.delay_frame(300);

  //Roll up mode 4 lines
  TV.printCC(0x14, 0x27);
  TV.printCC(0x14, 0x27);

  //Newline
  TV.printCC(0x14, 0x2D);
  TV.printCC(0x14, 0x2D);
}

void loop() {
  char firstByte, secondByte = 0;
  for(;;)
  {
    firstByte = 0;
    secondByte = 0;
    TV.delay_frame(1);
    
    if(pserial.available() > 0)
    { 
      firstByte = pserial.read();
      
      if(pserial.available() > 0)
      { 
        secondByte = pserial.read();
      }

      if(firstByte == '\n')
      {
        TV.printCC(0x14, 0x2D);
        TV.printCC(0x14, 0x2D);
        TV.printCC(secondByte, 0);
      }
      else if(secondByte == '\n')
      {
        TV.printCC(firstByte, 0);
        TV.printCC(0x14, 0x2D);
        TV.printCC(0x14, 0x2D);
      }
      else
      {
        
        TV.printCC(firstByte, secondByte);
      }
      
      //Uncomment for debugging
      //TV.print(firstByte);
      //TV.print(secondByte);

      //pserial.print(firstByte);
      //pserial.print(secondByte);
    }
  }
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
