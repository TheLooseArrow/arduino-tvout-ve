# Arduino-TVout for Video Experimenter

This is a library for generating composite video on an ATmega microcontroller and is a modified version of TVout specifically for using features of the Video Experimenter shield. It allows video output to be overlaid on top of a video signal, allows frames to be captured and modified, etc. For more information, see the [Video Experimenter website](https://nootropicdesign.com/ve).

This repository is based upon the last published version on Google Code, known as Beta1. It has been patched to get around compilation issues associated with using assembly macros on newer versions of the Arduino IDE. The compilation problems with macros emerged after Arduino 1.6.8.

Install the 3 libraries in your Arduino sketchbook with the following structure:

```
Arduino sketchbook
    |
    +--libraries
          |
          +--TVout
          |     |
          |     +--...
          |
          +--TVoutfonts
          |     |
          |     +--...
          |
          +--pollserial
                |
                +--...
```

# Closed Caption Encoder

You can now encode your own captions using the TV.printCC function, so check out the ClosedCaptionsEncoder and ClosedCaptionsSerialTerm examples for more info on how to use the new functions.

This new functionality does not require the TVExperimenter shield. You simply need to hook up the arduino the same way as using the normal TVout library, and make some minor changes to the code to make it work. Again look at the examples for more info.

Closed Caption Caveats:

You cannot currently overlay captions over an existing closed caption signal, this will simply result in garbage captions.  Some devices send a closed caption signal even when they are not actively producing captions, this is definitely true for a PS3 and may be true for other devices as well. It may be possible to erase and replace existing captions, but it will require extra circuitry of some kind.

It also seems that some TVs will not accept a closed caption signal from a monochrome signal, and they will require a colorburst for the closed captions to work. This means that you will need to overlay the closed captions onto a color signal for the captions to show up since this library cannot not produce its own colorburst from the arduino hardware alone.
