<img alt="Word watch" src="4PocketClockswide.jpg" width="900" />
# PocketScreen Word Watch

The software in still in developing state but V009 and V012 are stable.
To compile the source:
- Install in Arduino IDE: "RTCZero" and "Time" libraries<br>
- Rename in the Time library folder ...\Arduino\libraries\Time\ Time.h in XTime.h to avoid compiler errors<br>

I changed the PocketScreen font slightly.<br>
The W and I were altered to become monospaced. The rest was already monospaced<br>
This font is stored in the PS_MSFonts.h<br>

<img alt="Word watch" src="Word-watch.jpg" width="900" />

This word watch software is a adaption of the word clock with white LEDs or <br>
color RGBW-LEDs that can be found in my repository.<br>
It was relative easy to adapt the software for the pocket screen.<br>
I used the same structure with the same subroutines as used for LED-strips<br>
to address the OLED-display.<br>
The PocketScreen Word watch uses the RTC from the SAMD21 microcontroller for time keeping.<br>
With the buttons time can be changed<br>
One can choose between four languages as shown above; NL, UK, DE and FR<br>
and several colour palette options<br>

Things to be done:<br>
- Get the time from a NTP server<br> 
- or make a web server page for this clock<br>
- Get the power from a Lipo battery and reduce the power need


