// =============================================================================================================================
/* 
This Arduino code controls the Pocketscreen

Load and install in IDE:
http://files.zepsch.com/arduino/package_zepsch_index.json
Board: Pocketscreen
Built: type default

******>>> RENAME Time.h --> xTime.h in folder ..\Arduino\libraries\Time\
            |--------|
Button    2 |=      =| 1
            |        |
            |       =| 4
            |--------|  

 1   Set time: Plus one a minute
 4   Set time: Plus one a hour
2+1  Switch to Digital display and back
2+4  Switch to between languages
  
 Author .    : Ed Nieuwenhuys
 Changes-V007: Adapted from Character_Colour_Clock_V066
 Changes V008: Added button control
 Changes V009: Try to install own font
 Changes V010: NTP time server attempt
 Changes V011: Web page attempt
 Changes V012: V010-V011 are dead ends. Added the other three languages
 Changes V013: Clean up code. Make several display choices available. Updated printing
 Changes V014: Added Monospaced fonts in include PS_MSFonts.h. Updated printing. More compatible with a SK6812 LEDs
               Letter, color and its position is written into a struct LEDPrintbuffer that is printed with ShowLeds()
 Changes V015: EEPROM storage. Code cleanup
 Changes todo V016:  PrintMenuScreen(). Time by NTP or webserver page
*/
// =============================================================================================================================
char VERSION[] = "PocketWordClockV015";

//--------------------------------------------
// ARDUINO Includes defines and initialisations
//--------------------------------------------
#include <PocketScreen.h>
#include "PS_MSFonts.h"
#include <FlashStorage.h>  // Arduino library or https://github.com/cmaglie/FlashStorage
#include <RTCZero.h>       // The library enables control of the internal RTC. https://github.com/arduino-libraries/RTCZero
#include <TimeLib.h>       // For time management  REMOVE Time.h from Arduino\libraries\Time\


#define HET     ColorLeds("Het",     0,   2, MINColor);   
#define IS      ColorLeds("is",      4,   5, SECColor);    Is = true; 
#define WAS     ColorLeds("was",     7,   9, SECColor);    Is = false;
#define MVIJF   ColorLeds("vijf",    11, 14, LetterColor); 
#define PRECIES ColorLeds("precies", 16, 22, LetterColor);
#define MTIEN   ColorLeds("tien",    25, 28, LetterColor); 
#define KWART   ColorLeds("kwart",   32, 36, LetterColor);
#define VOOR    ColorLeds("voor",    38, 41, LetterColor);
#define OVER    ColorLeds("over",    43, 46, LetterColor);
#define HALF    ColorLeds("half",    48, 51, LetterColor);
#define MIDDER  ColorLeds("midder",  53, 58, LetterColor);
#define VIJF    ColorLeds("vijf",    60, 63, LetterColor);
#define TWEE    ColorLeds("twee",    65, 68, LetterColor);
#define EEN     ColorLeds("een",     71, 73, LetterColor);
#define VIER    ColorLeds("vier",    76, 79, LetterColor);
#define ELF     ColorLeds("elf",     80, 82, LetterColor);
#define TIEN    ColorLeds("tien",    84, 87, LetterColor);
#define TWAALF  ColorLeds("twaalf",  89, 94, LetterColor);
#define DRIE    ColorLeds("drie",    97,100, LetterColor);
#define NEGEN   ColorLeds("negen",  102,106, LetterColor);
#define ACHT    ColorLeds("acht",   107,110, LetterColor);
#define NACHT   ColorLeds("nacht",  114,118, LetterColor);
#define ZES     ColorLeds("zes",    112,114, LetterColor);
#define ZEVEN   ColorLeds("zeven",  116,120, LetterColor);
#define NOEN    ColorLeds("noen",   120,123, LetterColor);
#define UUR     ColorLeds("uur",    125,127, LetterColor);
//#define EDSOFT  ColorLeds("EdSoft", 132,132, LetterColor);
#define X_OFF   ColorLeds("",         0,  2, 0);
#define X_ON    ColorLeds("",         0,  2, LetterColor);

#define IT      ColorLeds("It",     1,   2, MINColor);   
#define ISUK    ColorLeds("is",       4,   5, SECColor);    Is = true;
#define WASUK   ColorLeds("was",      7,   9, SECColor);    Is = false;
#define EXACTUK ColorLeds("exact",   11,  15, LetterColor);
#define HALFUK  ColorLeds("half",    16,  19, LetterColor); 
#define TWENTY  ColorLeds("twenty",  21,  26, LetterColor); 
#define MFIVE   ColorLeds("five",    28,  31, LetterColor);
#define QUARTER ColorLeds("quarter", 32,  38, LetterColor);
#define MTEN    ColorLeds("ten",     39,  41, LetterColor);
#define PAST    ColorLeds("past",    43,  46, LetterColor);
#define TO      ColorLeds("to",      48,  49, LetterColor);
#define MID     ColorLeds("mid",     53,  55, LetterColor);
#define SIXUK   ColorLeds("six",     57,  59, LetterColor);
#define TWO     ColorLeds("two",     61,  63, LetterColor);
#define FIVE    ColorLeds("five",    64,  67, LetterColor);
#define TWELVE  ColorLeds("twelve",  69,  74, LetterColor);
#define TEN     ColorLeds("ten",     76,  78, LetterColor);
#define ELEVEN  ColorLeds("eleven",  80,  85, LetterColor);
#define FOUR    ColorLeds("four",    88,  91, LetterColor);
#define ONE     ColorLeds("one",     93,  95, LetterColor);
#define EIGHT   ColorLeds("eight",   97, 101, LetterColor);
#define THREE   ColorLeds("three",  101, 105, LetterColor);
#define NIGHT   ColorLeds("night",  106, 110, LetterColor);
#define NINE    ColorLeds("nine",   112, 115, LetterColor);
#define SEVEN   ColorLeds("seven",  116, 120, LetterColor);
#define OCLOCK  ColorLeds("O'clock",121, 126, LetterColor);

#define ES      ColorLeds("Es",     0,   1, MINColor);   
#define IST     ColorLeds("ist",      3,   5, SECColor);    Is = true;
#define WAR     ColorLeds("war",      6,   8, SECColor);    Is = false;
#define GENAU   ColorLeds("genau",   10,  14, LetterColor);
#define MZEHN   ColorLeds("zehn",    16,  19, LetterColor);
#define MFUNF   ColorLeds("funf",    20,  23, LetterColor);
#define VIERTEL ColorLeds("viertel", 24,  30, LetterColor);
#define ZWANZIG ColorLeds("zwanzig", 32,  38, LetterColor);
#define KURZ    ColorLeds("kurz",    40,  43, LetterColor);
#define VOR     ColorLeds("vor",     45,  47, LetterColor);
#define NACH    ColorLeds("nach",    48,  51, LetterColor);
#define HALB    ColorLeds("halb",    53,  56, LetterColor);
#define FUNF    ColorLeds("funf",    58,  61, LetterColor);
#define EINS    ColorLeds("eins",    64,  67, LetterColor);
#define VIERDE  ColorLeds("vier",    69,  72, LetterColor);
#define ZWOLF   ColorLeds("zwolf",   73,  77, LetterColor);
#define MITTER  ColorLeds("mitter",  80,  85, LetterColor);
#define ACHTDE  ColorLeds("acht",    87,  90, LetterColor);
#define NACHTDE ColorLeds("nacht",   86,  90, LetterColor);
#define DREI    ColorLeds("drei",    92,  95, LetterColor);
#define SECHS   ColorLeds("sechs",   96, 100, LetterColor);
#define SIEBEN  ColorLeds("sieben", 102, 107, LetterColor);
#define NEUN    ColorLeds("neun",    107, 110, LetterColor);
#define ZWEI    ColorLeds("zwei",   112, 115, LetterColor);
#define ZEHN    ColorLeds("zehn",   116, 119, LetterColor);
#define ELFDE   ColorLeds("elf",    120, 122, LetterColor);
#define UHR     ColorLeds("uhr",    125, 127, LetterColor);

#define IL      ColorLeds("Il",     0,   1, MINColor);   
#define EST     ColorLeds("est",      3,   5, SECColor);    Is = true;
#define ETAIT   ColorLeds("etait",    6,  10, SECColor);    Is = false;
#define EXACT   ColorLeds("exact",   11,  15, LetterColor);
#define SIX     ColorLeds("six",     16,  18, LetterColor); 
#define DEUX    ColorLeds("deux",    19,  22, LetterColor); 
#define TROIS   ColorLeds("trois",   23,  27, LetterColor);
#define ONZE    ColorLeds("onze",    28,  31, LetterColor);
#define QUATRE  ColorLeds("quatre",  32,  37, LetterColor);
#define MINUIT  ColorLeds("minuit",  39,  44, LetterColor);
#define DIX     ColorLeds("dix",     46,  47, LetterColor);
#define CINQ    ColorLeds("cinq",    48,  51, LetterColor);
#define NEUF    ColorLeds("neuf",    52,  55, LetterColor);
#define MIDI    ColorLeds("midi",    57,  60, LetterColor);
#define HUIT    ColorLeds("huit",    64,  67, LetterColor);
#define SEPT    ColorLeds("sept",    69,  72, LetterColor);
#define UNE     ColorLeds("une",     73,  75, LetterColor);
#define HEURE   ColorLeds("heure",   80,  84, LetterColor);
#define HEURES  ColorLeds("heures",  80,  85, LetterColor);
#define ET      ColorLeds("et",      87,  88, LetterColor);
#define MOINS   ColorLeds("moins",   90,  94, LetterColor);
#define LE      ColorLeds("le",      96,  97, LetterColor);
#define DEMI    ColorLeds("demie",   99, 103, LetterColor);
#define QUART   ColorLeds("quart",  106, 110, LetterColor);
#define MDIX    ColorLeds("dix",    112, 114, LetterColor);
#define VINGT   ColorLeds("vingt",  116, 120, LetterColor);
#define MCINQ   ColorLeds("cinq",   122, 125, LetterColor);
#define DITLEHEURE DitLeHeure();


 //--------------------------------------------
// COLOURS
//--------------------------------------------   

const byte DEFAULTCOLOUR = 0;
const byte HOURLYCOLOUR  = 1;          
const byte WHITECOLOR    = 2;
const byte OWNCOLOUR     = 3;
const byte OWNHETISCLR   = 4;
const byte WHEELCOLOR    = 5;
const byte DIGITAL       = 6;

// Defined colours in header file
//const uint16_t BLACK_16b        = 0x0000;
//const uint16_t DARKGRAY_16b     = 0x4208;
//const uint16_t GRAY_16b         = 0x8410;
//const uint16_t LIGHTGRAY_16b    = 0xBDF7;
//const uint16_t WHITE_16b        = 0xFFFF;
//const uint16_t RED_16b          = 0xF800;
//const uint16_t ORANGE_16b       = 0xFC00;
//const uint16_t YELLOW_16b       = 0xFFE0;
//const uint16_t GREENYELLOW_16b  = 0x87E0;
//const uint16_t GREEN_16b        = 0x07E0;
//const uint16_t CYAN_16b         = 0x07FF;
//const uint16_t BLUE_16b         = 0x001F;
//const uint16_t PURPLE_16b       = 0x801F;
//const uint16_t MAGENTA_16b      = 0xF81F;
//const uint16_t DARKRED_16b      = 0x8000;
//const uint16_t DARKGREEN_16b    = 0x0400;
//const uint16_t DARKBLUE_16b     = 0x0010;                    


//--------------------------------------------
// Pocketscreen initialysations
//--------------------------------------------
PocketScreen pocketscreen = PocketScreen();
bool     PrintDigital   = false;                 
byte     ButtonsPressed = 0;                    // contains button pressed 1= PIN_BUTTON_0; 2 = PIN_BUTTON_1, 4 PIN_BUTTON_2
byte     Language       = 0;                    //0=NL,1=UK,2=DE,3=FR
char*    PocketScreenTekst;
char     Template[128];   
char*    pinNames[]     = {"D0","D1","D2","D3","D4","A0","A1","A2"};
int      pins[]         = {D0,D1,D2,D3,D4,A0,A1,A2};  
uint32_t LastButtonTime = 0;
//--------------------------------------------
// RTC initialysations
//--------------------------------------------
RTCZero RTCklok;

//--------------------------------------------
// CLOCK initialysations
//--------------------------------------------                                 
#define MAXTEXT 82
static  uint32_t msTick;                        // Number of millisecond ticks since we last incremented the second counter
byte    Isecond, Iminute, Ihour, Iday, Imonth, Iyear; 
byte    lastminute = 0, lasthour = 0, sayhour = 0;
bool    Zelftest             = false;
bool    Is                   = true;            // toggle of displaying Is or Was
bool    ZegUur               = true;            // Say or not say Uur in NL clock
//--------------------------------------------
// LED Pixel initialysations
//--------------------------------------------
const byte NUM_LEDS         = 128;              // How many character in display? I know.. = 8x16
const byte MATRIX_WIDTH     = 16;
const byte MATRIX_HEIGHT    = 8;
struct   LEDPrintbuffer {
         char     Character;  
         uint16_t RGBColor;
} Strippos[NUM_LEDS+1] = {0};
bool     LEDsAreOff            = false;         // If true LEDs are off except time display
bool     NoTextInColorLeds     = false;         // Flag to control printing of the text in function ColorLeds()
int      Previous_LDR_read     = 512;           // The actual reading from the LDR + 4x this value /5  
uint16_t MINColor              = RED_16b;
uint16_t SECColor              = GREEN_16b;
uint16_t LetterColor           = YELLOW_16b;      
uint16_t HourColor[24] ={WHITE_16b,RED_16b,ORANGE_16b,YELLOW_16b,GREENYELLOW_16b,GREEN_16b,
                         CYAN_16b,BLUE_16b,PURPLE_16b,MAGENTA_16b,DARKRED_16b,DARKGREEN_16b,
                         WHITE_16b,RED_16b,ORANGE_16b,YELLOW_16b,GREENYELLOW_16b,GREEN_16b,
                         CYAN_16b,BLUE_16b,PURPLE_16b,MAGENTA_16b,DARKRED_16b,DARKGREEN_16b};  
//----------------------------------------
// Common
//----------------------------------------
char sptext[MAXTEXT];                    // For common print use    
typedef struct {                   // Data storage in EEPROM to maintain them after power loss
  byte DisplayChoice;
  byte TurnOffLEDsAtHH;
  byte TurnOnLEDsAtHH;
  byte LanguageChoice;
  uint16_t OwnColour;               // Self defined colour for clock display
} EEPROMplek;
FlashStorage(EEPROMstore, EEPROMplek);
EEPROMplek Mem ; 
//--------------------------------------------
// Menu
//--------------------------------------------  
 char menu[][MAXTEXT] = {
 "Time as: hhmmss (132145)",
 "D D15012021 is 15Jan2021",
 "L L0=NL,L1=UK,L2=DE,L3=FR",
 "N (N2208) LEDs off Nhhhh",
 "P (P4F8A) own colour(0-F)",
 "Q Display choice (Q0-6)",
 "  Q0= Yellow colour",
 "  Q1= Hourly colour",
 "  Q2= All white",
 "  Q3= All Own colour",
 "  Q4= Own colour",
 "  Q5= Wheel colour",
 "  Q6= Digital display",
 "I For this info",
 "S Self test",
 "Ed Nieuwenhuys Jan 2021" };
 
//  -------------------------------------   End Definitions  ---------------------------------------

//--------------------------------------------
// ARDUINO Setup
//--------------------------------------------
void setup() 
{
 for (int i=0; i<5; i++)  pinMode(pins[i], OUTPUT);                // The digital pins must be declared as output, while the analog pins must not
 SerialUSB.begin(115200);                                          // Setup the serial port to 9600 baud //
 pocketscreen.begin();
 pocketscreen.setBitDepth(BitDepth16);                             // Set Bit depth
 pocketscreen.setFont(pocketScreen6pt);
 pocketscreen.drawRect(0, 0, 96, 64, true, YELLOW_16b);            // Clear screen
 pocketscreen.setFontColor(BLACK_16b,YELLOW_16b);                  // Set color
 pocketscreen.setCursor(0, 48);
 pocketscreen.print(VERSION);
 pocketscreen.setFont(pocketScreenMS7pt);                          // Select the monospaced font
 PrintTimeInScreen();                                              // Print the time in pocket screen
 int32_t Tick = millis();                                        // Start the timer 
 while (!SerialUSB)                                                // Wait until serial port is started 
      {if (millis() - Tick >5000) break;}                        // Prevents hanging if serial monitor/port is not connected 
 Tekstprintln("\n*********\nSerial started");
 Tekstprintln("Pocket screen started");
 RTCklok.begin();                                                  // Start the RTC-module 
 Tekstprintln("RTC enabled");
 RTCklok.setEpoch(ConvertDateTime(__DATE__, __TIME__));            // Set clock to compile DATE TIME
 GetTijd(1);                                                       // Start the RTC-module 
 SWversion();                                                      // Start the RTC-module 
 msTick = LastButtonTime = millis();
 Mem = EEPROMstore.read();                                         // Get the data from EEPROM
 if(Mem.DisplayChoice>DIGITAL) Mem.DisplayChoice = DEFAULTCOLOUR;  // Any strange value in EEPROM?
 if(Mem.OwnColour==0) Mem.OwnColour = YELLOW_16b;                  // If memory is empty then store default value, blue  
//Play_Lights(); 
}

//--------------------------------------------
// ARDUINO Loop
//--------------------------------------------
void loop() 
{
 if(ButtonsPressed = CheckButtons()) ChangeTheTime(ButtonsPressed);
 EverySecondCheck();  
}

//--------------------------------------------
// CLOCK Update routine done every second
//--------------------------------------------
void EverySecondCheck(void)
{
 int32_t msLeap       = millis()- msTick;
 int32_t msLeapButton = millis()- LastButtonTime;
// if (msLeap >100)  pocketscreen.setLEDColor(LEDRed);            // Turn OFF the second on LED. 
 if (msLeap > 999)                                                // Every second enter the loop
  {
   GetTijd(0);                                                    // Update Isecond, Iminute, Ihour, Iday, Imonth, Iyear
   msTick = millis();
   if (Iminute != lastminute) EveryMinuteUpdate();                // Enter the every minute routine after one minute
   else if (PrintDigital)     PrintTimeInScreen();                //  Show the digital time in the screen
   // pocketscreen.setLEDColor(LEDBlue);                          // Turn ON the second on LED 
   SerialCheck();                                                 // Check serial port every second
   SetSecondColour();
  }  
 }
//--------------------------------------------
// CLOCK Update routine done every minute
//--------------------------------------------
void EveryMinuteUpdate(void)
{
 static byte lastday = 0;
 lastminute = Iminute;  
 if(LEDsAreOff) { LedsOff(); ShowLeds();}                         // Turn the LEDs off
 else PrintDigital?PrintTimeInScreen():Displaytime();             // Turn the display on                 
 Print_RTC_tijd();
 if(Ihour != lasthour) 
   {
    lasthour = Ihour;
    if(Ihour == Mem.TurnOffLEDsAtHH) 
        {LEDsAreOff = true;  LedsOff(); ShowLeds();}              // is it time to turn off the LEDs?
    if(Ihour == Mem.TurnOnLEDsAtHH)   
         LEDsAreOff = false;                                      // 
    }
 if(Iday != lastday) lastday = Iday; 
}
//--------------------------------------------
// CLOCK check for serial input
//--------------------------------------------
void SerialCheck(void)
{
 String SerialString; 
 while (SerialUSB.available())
    { 
     char c = SerialUSB.read();                                   //     SerialUSB.write(c);
     if (c>31 && c<128) SerialString += c;                        // allow input from Space - Del
     else c = 0;                                                  // delete a CR
    }
 if (SerialString.length()>0) 
    {
      ReworkInputString(SerialString);                            // Rework ReworkInputString();
      SerialString = "";
    }
}
//--------------------------------------------
// CLOCK common print routines
//--------------------------------------------
void Tekstprint(char tekst[])  { SerialUSB.print(tekst);}
void Tekstprintln(char tekst[]){ SerialUSB.println(tekst);}

//--------------------------------------------
//  CLOCK Input from Bluetooth or Serial
//--------------------------------------------
void ReworkInputString(String InputString)
{
 InputString.toCharArray(sptext, MAXTEXT-1);                      // Tekstprintln(sptext);
 SerialUSB.println(InputString);
 if(InputString.length()> 10) return;
 if(InputString[0] > 64 && InputString[0] <123 )                  // Does the string start with a letter?
  { 
// Tekstprintln(sptext);
   switch ((byte)InputString[0])
   {
    case 'D':
    case 'd':  
            if (InputString.length() == 9 )
              {
               Iday   = (byte) InputString.substring(1,3).toInt(); 
               Imonth = (byte) InputString.substring(3,5).toInt(); 
               Iday   = constrain(Iday  , 0, 31);
               Imonth = constrain(Imonth, 0, 12); 
               Iyear   = constrain(InputString.substring(5,9).toInt() , 1000, 9999);
               SetRTCTime();
               Print_tijd();  //Tekstprintln(sptext);             //Print_Tijd() fills sptext with time string
              }
            else Tekstprintln("**** Length fault. Enter ddmmyyyy ****");
            break;
    case 'I':
    case 'i': 
            if (InputString.length() == 1) SWversion();
            break;
    case 'L':                                                     // Language to choose
    case 'l':
             if (InputString.length() == 2 )
               {
                byte res = (byte) InputString.substring(1,2).toInt();   
                Mem.LanguageChoice = res%4;                       // Result between 0 and 3
                byte ch = Mem.LanguageChoice;                
                sprintf(sptext,"Language choice:%s",ch==0?"NL":ch==1?"UK":ch==2?"DE":ch==3?"FR":"NOP"); 
                Tekstprintln(sptext);
                EEPROMstore.write(Mem);                           // Update EEPROM 
                lastminute = 99;                                  // Force a minute update
               }
             else Tekstprintln("**** Display choice length fault. Enter L0 - L3"); 
            break;     

    case 'N':
    case 'n':
             if (InputString.length() == 1 )         Mem.TurnOffLEDsAtHH = Mem.TurnOnLEDsAtHH = 0;
             if (InputString.length() == 5 )
              {
               Mem.TurnOffLEDsAtHH =(byte) InputString.substring(1,3).toInt(); 
               Mem.TurnOnLEDsAtHH = (byte) InputString.substring(3,5).toInt(); 
              }
             Mem.TurnOffLEDsAtHH = constrain(Mem.TurnOffLEDsAtHH, 0, 23);
             Mem.TurnOnLEDsAtHH  = constrain(Mem.TurnOnLEDsAtHH,  0, 23); 
             sprintf(sptext,"LEDs are OFF between %2d:00 and %2d:00", Mem.TurnOffLEDsAtHH,Mem.TurnOnLEDsAtHH );
             EEPROMstore.write(Mem);                              // Update EEPROM 
             Tekstprintln(sptext); 
             break;
    case 'O':
    case 'o':
             if(InputString.length() == 1)
               {
                LEDsAreOff = !LEDsAreOff;
                sprintf(sptext,"LEDs are %s", LEDsAreOff?"OFF":"ON" );
                Tekstprintln(sptext);
                if(LEDsAreOff) { LedsOff(); ShowLeds();}               // Turn the LEDs off
                else PrintDigital?PrintTimeInScreen():Displaytime();   // Turn the LEDs on                
               }
             break;                                                                   
    case 'P':
    case 'p':  
             if (InputString.length() == 9 )
               {
                LetterColor = Mem.OwnColour = HexToDec(InputString.substring(1,9));               // Display letter color 
                sprintf(sptext,"Own colour stored0X%X", Mem.OwnColour);
                Tekstprintln(sptext); 
                Tekstprintln("**** Own colour changed ****"); 
                EEPROMstore.write(Mem);                                 // Update EEPROM       
                Displaytime();
               }
             else Tekstprintln("**** Length fault. Enter Pwwrrggbb ****");            
             break;
    case 'q':
    case 'Q':  
             if (InputString.length() == 2 )
               {
                Mem.DisplayChoice = (byte) InputString.substring(1,2).toInt(); 
                sprintf(sptext,"Display choice: Q%d", Mem.DisplayChoice);
                Tekstprintln(sptext);
                EEPROMstore.write(Mem);                                 // Update EEPROM  
                lastminute = 99;                                        // Force a minute update
               }
             else Tekstprintln("**** Display choice length fault. Enter Q0 - Q6"); 
             Displaytime();                                             // Turn on the LEDs with proper time             
            break;     
    case 'S':
    case 's':
             if (InputString.length() == 1)
               {   
                Zelftest = 1 - Zelftest; 
                sprintf(sptext,"Zelftest: %d",Zelftest);
                Tekstprintln(sptext); 
                Displaytime();                                          // Turn on the LEDs with proper time
               }                                
             else Tekstprintln("**** Length fault. Enter S ****");
             break; 
    case 'T':
    case 't':

             if(InputString.length() == 7)  // T125500
               {
                Ihour   = InputString.substring(1,3).toInt()%24;  
                Iminute = InputString.substring(3,5).toInt()%60; 
                Isecond = InputString.substring(5,7).toInt()%60; 
                SetRTCTime();
                Print_tijd(); 
               }
             else Tekstprintln("**** Length fault. Enter Thhmmss ****");
             break;    
    default: break;
    }
  }
 else if (InputString.length() == 6 )
  {   
   Ihour   = InputString.substring(0,2).toInt()%24;  
   Iminute = InputString.substring(2,4).toInt()%60; 
   Isecond = InputString.substring(4,6).toInt()%60; 
   SetRTCTime();
   Print_tijd();  
}                                        
 InputString = "";
}
//--------------------------------------------
//  LED Set color for LED
//--------------------------------------------
void ColorLeds(char* Texkst, int FirstLed, int LastLed, uint16_t RGBColor)
{ 
 int n, i=0;
 char Tekst[128];
  if (!NoTextInColorLeds && (strlen(Texkst) > 0 && strlen(Texkst) <10) )
     {sprintf(sptext,"%s ",Texkst); Tekstprint(sptext); }                      // Print the text  
// sprintf(sptext," %s, F:%d, L:%d F-L:%d ",Texkst, FirstLed,LastLed,1+LastLed-FirstLed );  Tekstprint(sptext);
 strcpy(Tekst,Texkst);
 to_upper(Tekst);   
 for (n=FirstLed; n<=FirstLed+ (LastLed-FirstLed); n++)
 {
  Strippos[n].Character = Tekst[i++];
  Strippos[n].RGBColor = RGBColor;
//  sprintf(sptext,"-Strippos[%d].Character=:%c",n, Strippos[n].Character); Tekstprint(sptext);   
 }
}
//--------------------------------------------
//  COMMON String upper
//--------------------------------------------
void to_upper(char* string)
{
 const char OFFSET = 'a' - 'A';
 while (*string)
  {
  *string = (*string >= 'a' && *string <= 'z') ? *string -= OFFSET : *string;
  string++;
  }
}
//--------------------------------------------
//  LED Clear display settings of the LED's
//--------------------------------------------
void LedsOff(void) 
{ 
// pocketscreen.drawRect(0, 0, 96, 64, true, BLACK_16b);
 for (int n=0; n<NUM_LEDS; n++) 
     Strippos[n].Character = Strippos[n].RGBColor = 0;                 // Erase the struct Strippos
}
//--------------------------------------------
//  LED Push data in LED strip to commit the changes
//--------------------------------------------
void ShowLeds(uint16_t BackgroundColor)
{
 int LEDnr=0;
 for(int y=0;y<MATRIX_HEIGHT;y++)
   {
  for(int x=0;x<MATRIX_WIDTH;x++)
    {
     pocketscreen.setFontColor(Strippos[LEDnr].RGBColor, BackgroundColor); 
     pocketscreen.setCursor(6*x, 8*y);                                // Character is 6 pixels wide and 8 pixel height
     pocketscreen.print(Strippos[LEDnr++].Character);
    }
   }  
}

//--------------------------------------------
//  LED Set second color
//  Set the colour per second of 'IS' and 'WAS'
//--------------------------------------------
void SetSecondColour(void)
{
 switch (Mem.DisplayChoice)
  {
   case DEFAULTCOLOUR: LetterColor = YELLOW_16b;MINColor = RED_16b;SECColor=GREEN_16b; break;
   case HOURLYCOLOUR : LetterColor = MINColor = SECColor = HourColor[Ihour];           break; // A colour every hour
   case WHITECOLOR   : LetterColor = MINColor = SECColor = WHITE_16b;                  break; // all white
   case OWNCOLOUR    : LetterColor = MINColor = SECColor = Mem.OwnColour;              break; // own colour
   case OWNHETISCLR  : LetterColor = Mem.OwnColour; MINColor = SECColor = LetterColor; break; // own colour except HET IS WAS  
   case WHEELCOLOR   : LetterColor = MINColor = SECColor = (17*(Iminute*60));          break; // Colour of all letters changes per second
   case DIGITAL      : LetterColor = WHITE_16b; MINColor = SECColor = BLACK_16b;       break; // digital display of time. No IS WAS turn color off in display
  }
 ShowLeds();                                                     // Updating IS and WAS with ShowLeds is done here to avoid updating all letters every second with Displaytime function 
}
//--------------------------------------------
//  LED Push data to display to commit the changes
//--------------------------------------------
void ShowLeds(void)
{
 int LEDnr = 0;
 for(int y = 0; y < MATRIX_HEIGHT; y++)
  for(int x = 0; x < MATRIX_WIDTH; x++)
    {
     pocketscreen.setFontColor(Strippos[LEDnr].RGBColor, BLACK_16b); 
     pocketscreen.setCursor(6*x, 8*y);                             //character is 6 pixels wide and 8 pixel height
     pocketscreen.print(Strippos[LEDnr++].Character);
    }
}
//--------------------------------------------
// CLOCK Version info
//--------------------------------------------
void SWversion(void) 
{ 
 PrintLine(30);
 for (int i = 0; i < sizeof(menu) / sizeof(menu[0]); Tekstprintln(menu[i++]));
 PrintLine(40);
 byte ch = Mem.LanguageChoice;
 sprintf(sptext,"LEDs off between: %0.2d - %0.2d",Mem.TurnOffLEDsAtHH, Mem.TurnOnLEDsAtHH); Tekstprintln(sptext);
 sprintf(sptext,"  Display choice: %d", Mem.DisplayChoice);                                 Tekstprintln(sptext);
 sprintf(sptext," Language choice: %s",ch==0?"NL":ch==1?"UK":ch==2?"DE":ch==3?"FR":"NOP");  Tekstprintln(sptext);
 sprintf(sptext,"Software: %s",VERSION);                                                    Tekstprintln(sptext); 
 GetTijd(1);  
 PrintLine(30);
}
void PrintLine(byte Lengte)
{
 for (int n=0; n<Lengte; n++) {SerialUSB.print(F("_"));} SerialUSB.println(); 
}

//--------------------------------------------
// CLOCK Say the time and load the LEDs 
// with the proper colour and intensity
//--------------------------------------------
void Displaytime(void)
{ 
 LedsOff();                                    // Start by clearing the display to a known state       
 if( Mem.DisplayChoice == DIGITAL )  PrintDigital=true; 
 else                                         // If not a difital display 
   { 
    PrintDigital=false;
    switch(Mem.LanguageChoice)
     {
      case 0: 
      strncpy(Template,"HETVISOWASOVIJFQPRECIESZSTIENKPFKWARTSVOORSOVERAHALFSMIDDERTVIJFATWEESOEENOXVIERELFQTIENKTWAALFBHDRIECNEGENACHTFZESVZEVENOENVUUR",128);  
              ColorLeds(Template,0,127, DARKGRAY_16b);
               Dutch();    break;
      case 1: 
      strncpy(Template,"HITVISOWASOEXACTHALFITWENTYEFIVEQUARTERTENZPASTATOLFSMIDKSIXVTWOFIVEETWELVEXTENRELEVENENFOURAONETEIGHTHREENIGHTFNINESEVENOCLOCKR",128);  
              ColorLeds(Template,0,127, DARKGRAY_16b);
      English();  break;
      case 2: 
      strncpy(Template,"ESTISTWARSGENAUTZEHNFUNFVIERTELQZWANZIGTKURZAVORNACHYHALBKFUNFBOEINSEVIERZWOLFARMITTERNACHTNDREISECHHNSIEBENEUNBZWEIZEHNELFMKUHR",128);
              ColorLeds(Template,0,127, DARKGRAY_16b);              ColorLeds(Template,0,127, DARKGRAY_16b);
         German();   break;
      case 3:
      strncpy(Template,"ILWESTETAITEXACTSIXDEUXTROISONZEQUATRERMINUITDIXCINQNEUFKMIDISWOHUITESEPTUNEDOSRHEURESYETOMOINSELETDEMIENEQUARTNDIXEVINGTOCINQKR",128); 
              ColorLeds(Template,0,127, DARKGRAY_16b);
         French();   break;
     }
   }
}


//--------------------------- Time functions --------------------------
//--------------------------------------------
// DS3231 Get time from DS3231
//--------------------------------------------
void GetTijd(byte printit)
{
 Ihour   = RTCklok.getHours();
 Iminute = RTCklok.getMinutes();
 Isecond = RTCklok.getSeconds();
 Iday    = RTCklok.getDay();
 Imonth  = RTCklok.getMonth();
 Iyear   = RTCklok.getYear();
// if (Ihour > 24) { Ihour = random(12)+1; Iminute = random(60)+1; Isecond = 30;}  // set a time if time module is absent or defect
 if (printit)  Print_RTC_tijd(); 
}

//--------------------------------------------
// DS3231 utility function prints time to serial
//--------------------------------------------
void Print_RTC_tijd(void)
{
 sprintf(sptext,"%0.2d:%0.2d:%0.2d %0.2d-%0.2d-%0.4d",RTCklok.getHours(),RTCklok.getMinutes(),RTCklok.getSeconds(),RTCklok.getDay(),RTCklok.getMonth(),RTCklok.getYear()+2000);
 Tekstprintln(sptext);
}

//--------------------------------------------
// CLOCK utility function prints time to serial
//--------------------------------------------
void Print_tijd(void)
{
 sprintf(sptext,"%0.2d:%0.2d:%0.2d",Ihour,Iminute,Isecond);
 Tekstprintln(sptext);
}

//--------------------------------------------
// DS3231 Set time in module and print it
//--------------------------------------------
void SetRTCTime(void)
{ 
 Ihour   = constrain(Ihour  , 0,24);
 Iminute = constrain(Iminute, 0,59); 
 Isecond = constrain(Isecond, 0,59); 
 RTCklok.setTime(Ihour, Iminute, Isecond);
 GetTijd(0);                                     // synchronize time with RTC clock
 Displaytime();
 Print_tijd();
}
//--------------------------------------------
// DS3231 Convert compile DATA & TIME to time_t
//--------------------------------------------
time_t ConvertDateTime(char const *date, char const *time)
{
 char s_month[5];
 int year, day;
 int hour, minute, second;
 tmElements_t t;
 static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
 sscanf(date, "%s %d %d", s_month, &day, &year);                  // sscanf(time, "%2hhd %*c %2hhd %*c %2hhd", &t.Hour, &t.Minute, &t.Second);
 sscanf(time, "%2d %*c %2d %*c %2d", &hour, &minute, &second);   //  Find where is s_month in month_names. Deduce month value.
 t.Month = (strstr(month_names, s_month) - month_names) / 3 + 1;
 t.Day = day;         // year can be given as '2010' or '10'. It is converted to years since 1970
 if (year > 99) t.Year = year - 1970;
 else t.Year = year + 50;
 t.Hour = hour;
 t.Minute = minute;
 t.Second = second;
 return makeTime(t);
}

// ------------------- End  Time functions 

//--------------------------------------------
//  CLOCK Convert Hex to uint32
//--------------------------------------------
uint32_t HexToDec(String hexString) 
{
 uint32_t decValue = 0;
 int nextInt;
 for (int i = 0; i < hexString.length(); i++) 
  {
   nextInt = int(hexString.charAt(i));
   if (nextInt >= 48 && nextInt <= 57)  nextInt = map(nextInt, 48, 57, 0, 9);
   if (nextInt >= 65 && nextInt <= 70)  nextInt = map(nextInt, 65, 70, 10, 15);
   if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
   nextInt = constrain(nextInt, 0, 15);
   decValue = (decValue * 16) + nextInt;
  }
 return decValue;
}

//--------------------------------------------
//  CLOCK Dutch clock display
//--------------------------------------------
void Dutch(void)
{
HET;                                       // HET light is always on
 if (Ihour == 12 && Iminute == 0 && random(2)==0) { IS; NOEN; return; }
 if (Ihour == 00 && Iminute == 0 && random(2)==0) { IS; MIDDER; NACHT; return; } 
switch (Iminute)
 {
  case  0: IS;  PRECIES; break;
  case  1: IS;  break;
  case  2: 
  case  3: WAS; break;
  case  4: 
  case  5: 
  case  6: IS;  MVIJF; OVER; break;
  case  7: 
  case  8: WAS; MVIJF; OVER; break;
  case  9: 
  case 10: 
  case 11: IS;  MTIEN; OVER; break;
  case 12: 
  case 13: WAS; MTIEN; OVER; break;
  case 14: 
  case 15: 
  case 16: IS;  KWART; OVER; break;
  case 17: 
  case 18: WAS; KWART; OVER; break;
  case 19: 
  case 20: 
  case 21: IS;  MTIEN; VOOR; HALF; break;
  case 22: 
  case 23: WAS; MTIEN; VOOR; HALF; break;
  case 24: 
  case 25: 
  case 26: IS;  MVIJF; VOOR; HALF; break;
  case 27: 
  case 28: WAS; MVIJF; VOOR; HALF; break;
  case 29: IS;  HALF; break;
  case 30: IS;  PRECIES; HALF; break;
  case 31: IS;  HALF; break;
  case 32: 
  case 33: WAS; HALF; break;
  case 34: 
  case 35: 
  case 36: IS;  MVIJF; OVER; HALF; break;
  case 37: 
  case 38: WAS; MVIJF; OVER; HALF; break;
  case 39: 
  case 40: 
  case 41: IS;  MTIEN; OVER; HALF; break;
  case 42: 
  case 43: WAS; MTIEN; OVER; HALF; break;
  case 44: 
  case 45: 
  case 46: IS;  KWART; VOOR; break;
  case 47: 
  case 48: WAS; KWART; VOOR; break;
  case 49: 
  case 50: 
  case 51: IS;  MTIEN; VOOR;  break;
  case 52: 
  case 53: WAS; MTIEN; VOOR;  break;
  case 54: 
  case 55: 
  case 56: IS;  MVIJF; VOOR; break;
  case 57: 
  case 58: WAS; MVIJF; VOOR; break;
  case 59: IS;  break;
}
//if (Ihour >=0 && hour <12) digitalWrite(AMPMpin,0); else digitalWrite(AMPMpin,1);

 sayhour = Ihour;
 if (Iminute > 18 )  sayhour = Ihour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1: EEN; break;
  case 14:
  case 2: TWEE; break;
  case 15:
  case 3: DRIE; break;
  case 16:
  case 4: VIER; break;
  case 17:
  case 5: VIJF; break;
  case 18:
  case 6: ZES; break;
  case 19:
  case 7: ZEVEN; break;
  case 20:
  case 8: ACHT; break;
  case 21:
  case 9: NEGEN; break;
  case 22:
  case 10: TIEN; break;
  case 23:
  case 11: ELF; break;
  case 0:
  case 12: TWAALF; break;
 } 
 switch (Iminute)
 {
  case 59: 
  case  0: 
  case  1: 
  case  2: 
  case  3: UUR;  break; 
 }
}

//--------------------------------------------
//  CLOCK English clock display
//--------------------------------------------
void English(void)
{
 IT;                                       // HET light is always on
 if (Ihour == 00 && Iminute == 0 && random(2)==0) { ISUK; MID; NIGHT; return; } 
 switch (Iminute)
 {
  case  0: ISUK;  EXACTUK; break;
  case  1: ISUK;  break;
  case  2: 
  case  3: WASUK; break;
  case  4: 
  case  5: 
  case  6: ISUK;  MFIVE; PAST; break;
  case  7: 
  case  8: WASUK; MFIVE; PAST; break;
  case  9: 
  case 10: 
  case 11: ISUK;  MTEN; PAST; break;
  case 12: 
  case 13: WASUK; MTEN; PAST; break;
  case 14: 
  case 15: 
  case 16: ISUK;  QUARTER; PAST; break;
  case 17: 
  case 18: WASUK; QUARTER; PAST; break;
  case 19: 
  case 20: 
  case 21: ISUK;  TWENTY; PAST; break;
  case 22: 
  case 23: WASUK; TWENTY; PAST; break;
  case 24: 
  case 25: 
  case 26: ISUK;  TWENTY; MFIVE; PAST; break;
  case 27: 
  case 28: WASUK; TWENTY; MFIVE; PAST; break;
  case 29: ISUK;  HALFUK; PAST; break;
  case 30: ISUK;  EXACTUK; HALFUK; PAST; break;
  case 31: ISUK;  HALFUK; PAST; break;
  case 32: 
  case 33: WASUK; HALFUK; PAST; break;
  case 34: 
  case 35: 
  case 36: ISUK;  TWENTY; MFIVE; TO; break;
  case 37: 
  case 38: WASUK; TWENTY; MFIVE; TO; break;
  case 39: 
  case 40: 
  case 41: ISUK;  TWENTY; TO; break;
  case 42: 
  case 43: WASUK; TWENTY; TO break;
  case 44: 
  case 45: 
  case 46: ISUK;  QUARTER; TO; break;
  case 47: 
  case 48: WASUK; QUARTER; TO; break;
  case 49: 
  case 50: 
  case 51: ISUK;  MTEN; TO;  break;
  case 52: 
  case 53: WASUK; MTEN; TO;  break;
  case 54: 
  case 55: 
  case 56: ISUK;  MFIVE; TO; break;
  case 57: 
  case 58: WASUK; MFIVE; TO; break;
  case 59: ISUK;  break;
}
//if (Ihour >=0 && hour <12) digitalWrite(AMPMpin,0); else digitalWrite(AMPMpin,1);

 sayhour = Ihour;
 if (Iminute > 33 ) sayhour = Ihour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1:  ONE; break;
  case 14:
  case 2:  TWO; break;
  case 15:
  case 3:  THREE; break;
  case 16:
  case 4:  FOUR; break;
  case 17:
  case 5:  FIVE; break;
  case 18:
  case 6:  SIXUK; break;
  case 19:
  case 7:  SEVEN; break;
  case 20:
  case 8:  EIGHT; break;
  case 21:
  case 9:  NINE; break;
  case 22:
  case 10: TEN; break;
  case 23:
  case 11: ELEVEN; break;
  case 0:
  case 12: TWELVE; break;
 } 
 switch (Iminute)
 {
  case 59: 
  case  0: 
  case  1: 
  case  2: 
  case  3: OCLOCK;  break; 
 }
}
//--------------------------------------------
//  CLOCK German clock display
//--------------------------------------------
void German(void)
{
  ES;                                       // HET light is always on
 if (Ihour == 00 && Iminute == 0 && random(2)==0) {IST; MITTER; NACHTDE; return; } 
 switch (Iminute)
 {
  case  0: IST;  GENAU; break;
  case  1: IST; KURZ; NACH; break;
  case  2: 
  case  3: WAR; break;
  case  4: 
  case  5: 
  case  6: IST; MFUNF; NACH; break;
  case  7: 
  case  8: WAR; MFUNF; NACH; break;
  case  9: 
  case 10: 
  case 11: IST; MZEHN; NACH; break;
  case 12: 
  case 13: WAR; MZEHN; NACH; break;
  case 14: 
  case 15: 
  case 16: IST; VIERTEL; NACH; break;
  case 17: 
  case 18: WAR; VIERTEL; NACH; break;
  case 19: 
  case 20: 
  case 21: IST; MZEHN; VOR; HALB; break;
  case 22: 
  case 23: WAR; MZEHN; VOR; HALB; break;
  case 24: 
  case 25: 
  case 26: IST; MFUNF; VOR; HALB; break;
  case 27: 
  case 28: WAR; MFUNF; VOR; HALB; break;
  case 29: IST; KURZ;  VOR; HALB; break;
  case 30: IST; GENAU; HALB; break;
  case 31: IST; KURZ;  NACH; HALB; break;
  case 32: 
  case 33: WAR; HALB; break;
  case 34: 
  case 35: 
  case 36: IST; MFUNF; NACH; HALB; break;
  case 37: 
  case 38: WAR; MFUNF; NACH; HALB; break;
  case 39: 
  case 40: 
  case 41: IST; MZEHN; NACH; HALB; break;
  case 42: 
  case 43: WAR; MZEHN; NACH; HALB; break;
  case 44: 
  case 45: 
  case 46: IST; VIERTEL; VOR; break;
  case 47: 
  case 48: WAR; VIERTEL; VOR; break;
  case 49: 
  case 50: 
  case 51: IST; MZEHN; VOR;  break;
  case 52: 
  case 53: WAR; MZEHN; VOR;  break;
  case 54: 
  case 55: 
  case 56: IST; MFUNF; VOR; break;
  case 57: 
  case 58: WAR; MFUNF; VOR; break;
  case 59: IST;  break;
}
//if (Ihour >=0 && hour <12) digitalWrite(AMPMpin,0); else digitalWrite(AMPMpin,1);

 sayhour = Ihour;
 if (Iminute > 18 ) sayhour = Ihour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1: EINS; break;
  case 14:
  case 2: ZWEI; break;
  case 15:
  case 3: DREI; break;
  case 16:
  case 4: VIERDE; break;
  case 17:
  case 5: FUNF; break;
  case 18:
  case 6: SECHS; break;
  case 19:
  case 7: SIEBEN; break;
  case 20:
  case 8: ACHTDE; break;
  case 21:
  case 9: NEUN; break;
  case 22:
  case 10: ZEHN; break;
  case 23:
  case 11: ELFDE; break;
  case 0:
  case 12: ZWOLF; break;
 } 
 switch (Iminute)
 {
  case 59: 
  case  0: 
  case  1: 
  case  2: 
  case  3: UHR;  break; 
 }
}
//--------------------------------------------
//  CLOCK French clock display
//--------------------------------------------
void French(void)
{
 IL;                                       // HET light is always on
 switch (Iminute)
 {
  case  0: EST;   EXACT; DITLEHEURE; break;
  case  1: EST;   DITLEHEURE; break;
  case  2: 
  case  3: ETAIT; DITLEHEURE; break;
  case  4: 
  case  5: 
  case  6: EST;   DITLEHEURE; MCINQ; break;
  case  7: 
  case  8: ETAIT; DITLEHEURE; MCINQ; break;
  case  9: 
  case 10: 
  case 11: EST;   DITLEHEURE; MDIX;  break;
  case 12: 
  case 13: ETAIT; DITLEHEURE; MDIX;  break;
  case 14: 
  case 15: 
  case 16: EST;   DITLEHEURE; ET; QUART; break;
  case 17: 
  case 18: ETAIT; DITLEHEURE; ET; QUART; break;
  case 19: 
  case 20: 
  case 21: EST;   DITLEHEURE; VINGT; break;
  case 22: 
  case 23: ETAIT; DITLEHEURE; VINGT; break;
  case 24: 
  case 25: 
  case 26: EST;   DITLEHEURE; VINGT; MCINQ; break;
  case 27: 
  case 28: ETAIT; DITLEHEURE; VINGT; MCINQ; break;
  case 29: EST;   DITLEHEURE; ET; DEMI; break;
  case 30: EST;   EXACT; DITLEHEURE;  ET; DEMI; break;
  case 31: EST;   DITLEHEURE; ET; DEMI; break;
  case 32: 
  case 33: ETAIT; DITLEHEURE; ET; DEMI; break;
  case 34: 
  case 35: 
  case 36: EST;   DITLEHEURE; MOINS; VINGT; MCINQ; break;
  case 37: 
  case 38: ETAIT; DITLEHEURE; MOINS; VINGT; MCINQ; break;
  case 39: 
  case 40: 
  case 41: EST;   DITLEHEURE; MOINS; VINGT;  break;
  case 42: 
  case 43: ETAIT; DITLEHEURE; MOINS; VINGT;  break;
  case 44: 
  case 45: 
  case 46: EST;   DITLEHEURE; MOINS; LE; QUART; break;
  case 47: 
  case 48: ETAIT; DITLEHEURE; MOINS; LE; QUART; break;
  case 49: 
  case 50: 
  case 51: EST;   DITLEHEURE; MOINS; MDIX;   break;
  case 52: 
  case 53: ETAIT; DITLEHEURE; MOINS; MDIX;   break;
  case 54: 
  case 55: 
  case 56: EST;   DITLEHEURE; MOINS; MCINQ;  break;
  case 57: 
  case 58: ETAIT; DITLEHEURE; MOINS; MCINQ;  break;
  case 59: EST;   DITLEHEURE;  break;
 }
//if (Ihour >=0 && hour <12) digitalWrite(AMPMpin,0); else digitalWrite(AMPMpin,1);
// switch (Iminute)
// {
//  case 59: 
//  case  0: 
//  case  1: 
//  case  2: 
//  case  3: if(sayhour%12 == 1) {HEURE;} 
//           else                {HEURES;}  
//           break; 
// }
}

void DitLeHeure(void)
{
 byte sayhour = Ihour;
 if (Iminute > 33 ) sayhour = Ihour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1:  UNE;    HEURE;  break;
  case 14:
  case 2:  DEUX;   HEURES;  break;
  case 15:
  case 3:  TROIS;  HEURES;  break;
  case 16:
  case 4:  QUATRE; HEURES; break;
  case 17:
  case 5:  CINQ;   HEURES;   break;
  case 18:
  case 6:  SIX;    HEURES;   break;
  case 19:
  case 7:  SEPT;   HEURES;  break;
  case 20:
  case 8:  HUIT;   HEURES; break;
  case 21:
  case 9:  NEUF;   HEURES; break;
  case 22:
  case 10: DIX;    HEURES; break;
  case 23:
  case 11: ONZE;   HEURES; break;
  case 0:  MINUIT; break;
  case 12: MIDI;   break;
 } 
}

//--------------------------------------------
// POCKET SCREEN   Change the time with  Buttons
//--------------------------------------------
void ChangeTheTime(byte PressedButtons)
{ // button pressed 1= PIN_BUTTON_0; 2 = PIN_BUTTON_1, 4 PIN_BUTTON_2
 switch (PressedButtons)
 {
  case  0:  break;
  case  1:  Iminute++; Iminute%=60; SetRTCTime(); break;
  case  2:  break;   // acts as SHIFT key
  case  3:  PrintDigital!=PrintDigital; 
            PrintDigital?PrintTimeInScreen():Displaytime();
            break; 
  case  4:  Ihour++;   Ihour%=24;  SetRTCTime(); break; 
  case  5:  break;
  case  6:  ++Mem.LanguageChoice%=4;  Displaytime(); break;  // Press Key 2+4
  case  7:  break; 
 }
}
//--------------------------------------------
// POCKET SCREEN   Check Buttons
//--------------------------------------------
byte CheckButtons(void)
{
 byte ButtonPressed = 0;
 if (millis() > LastButtonTime + 250) 
  { 
   if (!digitalRead(PIN_BUTTON_0)) ButtonPressed+=1;
   if (!digitalRead(PIN_BUTTON_1)) ButtonPressed+=2;
   if (!digitalRead(PIN_BUTTON_2)) ButtonPressed+=4;   
  }
if(ButtonPressed) 
  {
   sprintf(sptext,"Button pressed: %d",ButtonPressed);  Tekstprintln(sptext);
   LastButtonTime = millis(); 
  }
return ButtonPressed;
}

//--------------------------------------------
// POCKET SCREEN   Print the digital time in screen
//--------------------------------------------
void PrintTimeInScreen() 
{
 if( RTCklok.getSeconds()==0)   pocketscreen.clearScreen();
 pocketscreen.setFontColor(ORANGE_16b, BLACK_16b);
 pocketscreen.setCursor(3, 16);
 sprintf(sptext,"%0.2d : %0.2d : %0.2d",RTCklok.getHours(),RTCklok.getMinutes(),RTCklok.getSeconds());
 pocketscreen.print(sptext);
 pocketscreen.setFontColor(RED_16b, BLACK_16b);
 pocketscreen.setCursor(3, 32);
 sprintf(sptext,"%0.2d-%0.2d-%0.4d",RTCklok.getDay(),RTCklok.getMonth(),RTCklok.getYear()+2000);
 pocketscreen.print(sptext);
}

//--------------------------------------------
// POCKET SCREEN   Print mebu entries in screen
//--------------------------------------------
void PrintMenuScreen() 
{
 pocketscreen.clearScreen();
 pocketscreen.setFontColor(ORANGE_16b, BLACK_16b);
 pocketscreen.setCursor(0, 8);
 pocketscreen.print("Select a pin:");

// ------nog maken
}
