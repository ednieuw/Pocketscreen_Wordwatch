# Pocketscreen Word Watch

Install in Arduino IDE:<br> 
RTCZero and Time<br>
Rename in the Time library Time.h in XTime.h to avoid compiler errors<br>
Replace the PS_Fonts.h in the ...\Arduino\libraries\PocketScreen\ folder<br>
I am still studying how to add my own monospaced font to the program.<br>
The W and I were changed to become monospaced. The rest was monospaced<br>

<img alt="Word watch" src="Wordwatch.jpg" width="900" />

This word watch software is a adaption of the word clock with white LEDs or color<br>
RGBW LEDs that can be found in my repository.<br>
It was relative easy to adapt the software for thew pocket screen.<br>
The Pocketscreen uses the RTC from the SAMD21 microcontroller for time keeping.<br>
<br>
Things to be done:<br>
- Get the time from and NTP server and/or make a web server page for this clock<br>
- Get my own font in the compilation.<br>
