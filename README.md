# Pocketscreen Word Watch

The software in still in developing state but V009 and V012 are stable.
To compile the source:
- Install in Arduino IDE: RTCZero and Time libraries<br>
- Rename in the Time library ...\Arduino\libraries\Time\ Time.h in XTime.h to avoid compiler errors<br>
- Replace the PS_Fonts.h in the ...\Arduino\libraries\PocketScreen\ folder<br>
I am still studying how to add my own monospaced font to the program.<br>
The W and I were changed to become monospaced. The rest was already monospaced<br>

<img alt="Word watch" src="Word-watch.jpg" width="900" />

This word watch software is a adaption of the word clock with white LEDs or <br>
color RGBW-LEDs that can be found in my repository.<br>
It was relative easy to adapt the software for thew pocket screen.<br>
The Pocketscreen Word watch uses the RTC from the SAMD21 microcontroller for time keeping.<br>
With the buttons time can be changed<br>
Things to be done:<br>
Done V012 - Get the other languages UK, DE and FR running<br>
- Get several colour palette options running<br>
- Get the time from and NTP server<br> 
- Make a web server page for this clock<br>
- Get my own font in the compilation<br>
- Get the power from a Lipo battery and reduce the power need

<img alt="Word watch" src="Word-watch1.jpg" width="900" />
