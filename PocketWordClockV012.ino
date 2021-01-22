// =============================================================================================================================
/* 
This Arduino code controls the Pocketscreen

Load and install in IDE:
http://files.zepsch.com/arduino/package_zepsch_index.json
Board: Pocketscreen
Built: type default

******>>> RENAME Time.h --> xTime.h in folder ..\Arduino\libraries\Time\


 Author .    : Ed Nieuwenhuys
 Changes-V007: Adapted from Character_Colour_Clock_V066
 Changes V008: Added button control
 Changes V009: Try to install own font
 Changes V010: NTP time server attempt
 Changes V011: Web page attempt
 Changes V012: V010-V011 are dead ends. Added the other three languages
 Todo V013: EEPROM storage. Clean up code. Make several display choices available
*/
// =============================================================================================================================
//pocketScreenMS7pt  LITERAL1
char VERSION[] = "PocketWordClockV012";

//--------------------------------------------
// ARDUINO Includes defines and initialisations
//--------------------------------------------
#include <RTCZero.h>
#include <TimeLib.h>             // For time management  REMOVE Time.h from Arduino\libraries\Time\
//#include <EEPROM.h>

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
// PIN Assigments
//-------------------------------------------- 
enum DigitalPinAssignments {      // Digital hardware constants ATMEGA 328 ----
 RX           = 0,                // Connects to Bluetooth TX
 TX           = 1,                // Connects to Bluetooth RX
 PIN02        = 2,                // DCFPulse on interrupt  pin
 PIN03        = 3,                // right (labeled DT on decoder)on interrupt  pin
 PIN04        = 4,                // switch (labeled SW on decoder)
 LED_PIN      = 5,                // Pin to control colour SK6812/WS2812 LEDs
 secondsPin   = 3
 };
 
enum AnaloguePinAssignments {     // Analogue hardware constants ----
 EmptyA0      = 0,                // Empty
 EmptyA1      = 1,                // Empty
 PhotoCellPin = 2                // LDR pin
 };

                      
//--------------------------------------------
// Pocketscreen
//--------------------------------------------// we need the library
#include <PocketScreen.h>
PocketScreen pocketscreen = PocketScreen();
bool     PrintDigital=false;
char*    PocketScreenTekst;
byte     ButtonsPressed = 0;                    // contains button pressed 1= PIN_BUTTON_0; 2 = PIN_BUTTON_1, 4 PIN_BUTTON_2
byte     Language = 0;                          //0=NL,1=UK,2=DE,3=FR
char     *pinNames[] = {"D0", "D1", "D2", "D3", "D4", "A0", "A1", "A2"};
int      pins[] = {D0, D1, D2, D3, D4, A0, A1, A2};  
uint32_t LastButtonTime = 0;
/*   // defined 16 bit color
const uint16_t BLACK_16b        = 0x0000;
const uint16_t DARKGRAY_16b     = 0x4208;
const uint16_t GRAY_16b         = 0x8410;
const uint16_t LIGHTGRAY_16b    = 0xBDF7;
const uint16_t WHITE_16b        = 0xFFFF;
const uint16_t RED_16b          = 0xF800;
const uint16_t ORANGE_16b       = 0xFC00;
const uint16_t YELLOW_16b       = 0xFFE0;
const uint16_t GREENYELLOW_16b  = 0x87E0;
const uint16_t GREEN_16b        = 0x07E0;
const uint16_t CYAN_16b         = 0x07FF;
const uint16_t BLUE_16b         = 0x001F;
const uint16_t PURPLE_16b       = 0x801F;
const uint16_t MAGENTA_16b      = 0xF81F;
const uint16_t DARKRED_16b      = 0x8000;
const uint16_t DARKGREEN_16b    = 0x0400;
const uint16_t DARKBLUE_16b     = 0x0010;
*/
//--------------------------------------------
// RTC CLOCK
//--------------------------------------------

//RTC_DS3231 RTCklok;    //RTC_DS1307 RTC; 
RTCZero RTCklok;

//--------------------------------------------
// CLOCK
//--------------------------------------------                                 
#define MAXTEXT 80
static  uint32_t msTick;                  // the number of millisecond ticks since we last incremented the second counter
int     count; 
int     Delaytime = 200;
byte    Isecond, Iminute, Ihour, Iday, Imonth, Iyear; 
byte    lastminute = 0, lasthour = 0, sayhour = 0;
bool    ChangeTime           = false;
bool    ChangeLightIntensity = false;
bool    Demo                 = false;
bool    Zelftest             = false;
bool    Is                   = true;      // toggle of displaying Is or Was
bool    ZegUur               = true;      // Say or not say Uur in NL clock
//--------------------------------------------
// LED
//--------------------------------------------
const byte NUM_LEDS      = 144;         // How many leds in  strip?
const byte MATRIX_WIDTH  = 12;
const byte MATRIX_HEIGHT = 12;
const byte BRIGHTNESS    = 32;         // BRIGHTNESS 0 - 255

bool     LEDsAreOff            = false;     // If true LEDs are off except time display
bool     NoTextInColorLeds     = false;     // Flag to control printing of the text in function ColorLeds()
byte     BrightnessCalcFromLDR = BRIGHTNESS;// Initial brightness value The intensity send to the LEDs (0-255)

int      Previous_LDR_read     = 512;       // The actual reading from the LDR + 4x this value /5
uint16_t MINColor      = RED_16b;      //0X00FFDD;
uint16_t SECColor      = GREEN_16b;      //0X00FFDD;
uint16_t LetterColor   = YELLOW_16b;      //0X00FFDD;       
uint16_t DefaultColor  = 0X00FFDD;        // Yellow
uint16_t OwnColour     = 0X002345;        // Blue
uint16_t WhiteColour   , white = 0XFFFF;
uint16_t WheelColor    = 0X00FF;
uint16_t HourColor[24] ={white ,0X8000,0XFFD5,0X00B8,0XFF62,0X80FF,
                         0XFF80,0X00C8,0X7500,0XFFD0,0X00FF,0XFF0050,
                         0XFF00,0XFFFF,0XFF40,0XFF00,0XF7FF,0XFF00,
                         0X00FF,0X00F2,0X6FFF,0X0073,0X70ff,0X0000 };  
//----------------------------------------
// Common
//----------------------------------------

char sptext[82];                    // For common print use    
int  MilliSecondValue     = 1000;   // The duration of a second  minus 1 ms. Used in Demo mode
struct EEPROMstorage {              // Data storage in EEPROM to maintain them after power loss
  byte LightReducer;
  byte LowerBrightness;
  byte DisplayChoice;
  byte TurnOffLEDsAtHH;
  byte TurnOnLEDsAtHH;
  byte LanguageChoice;
  uint16_t OwnColour;               // Self defined colour for clock display
} Mem = {0};

//--------------------------------------------
// Menu
//--------------------------------------------  

 char menu[][MAXTEXT] = {
 "Pocket screen",
 "Enter time as: hhmmss (132145)",
 "D D15012021 is date 15 Jan 2021",
 "E Language E0=NL,E1=UK,E2=DE,E3=FR",
 "L (L5) Min lightintensity(0-255 bits)",
 "M (M90)Max lightintensity(1%-250%)",
 "N (N2208)Turn On/OFF LEDs between Nhhhh",
 "P (P00234F8A) own colour (n=0-F)",
 "Q Display Choice (Q0-6)",
 "  Q0= Yellow colour",
 "  Q1= Hourly colour",
 "  Q2= All white",
 "  Q3= All Own colour",
 "  Q4= Own colour",
 "  Q5= Wheel colour",
 "  Q6= Digital display",
 "I For this info",
 "R Reset to default settings",
 "S Self test",
 "T Language T1=NL, T2=UK,T3=DE,T4=FR",
 "X (X50) Demo mode. ms delay (0-9999)",
 "Ed Nieuwenhuys Jan 2021" };
 
//  -------------------------------------   End Definitions  ---------------------------------------
//--------------------------------------------
// POCKET SCREEN   Print the digital time in screen
//--------------------------------------------
void PrintTimeInScreen() 
{
 pocketscreen.clearScreen();
 pocketscreen.setFontColor(ORANGE_16b, BLACK_16b);
 pocketscreen.setCursor(0, 16);
 sprintf(sptext,"%0.2d : %0.2d : %0.2d",RTCklok.getHours(),RTCklok.getMinutes(),RTCklok.getSeconds());
 pocketscreen.print(sptext);
 pocketscreen.setFontColor(RED_16b, BLACK_16b);
 pocketscreen.setCursor(0, 32);
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

 
//--------------------------------------------
// ARDUINO Setup
//--------------------------------------------
void setup() 
{
 for (int i=0; i<5; i++)  pinMode(pins[i], OUTPUT);                // the digital pins must be declared as output, while the analog pins must not
 SerialUSB.begin(115200);                                             // Setup the serial port to 9600 baud // while (!SerialUSB);   
 Tekstprintln("\n*********\nSerial started"); 
 pocketscreen.begin();
 pocketscreen.setBitDepth(BitDepth16);   // set font
 pocketscreen.setFont(pocketScreen7pt);
 pocketscreen.drawRect(0, 0, 96, 64, true, LIGHTGRAY_16b);           // BLACK_16b);     // clear screen
 pocketscreen.setFontColor(WHITE_16b, BLACK_16b);                    // Set color
 Tekstprintln("Pocket screen started");
 RTCklok.begin();                                                    // Start the RTC-module 
 Tekstprintln("RTC enabled");
 RTCklok.setEpoch(ConvertDateTime(__DATE__, __TIME__));              // Set clock to compile DATE TIME
 Displaytime(); 
 SWversion(); 
// PrintTimeInScreen();  delay(5000);
 msTick = LastButtonTime = millis();
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
// POCKET SCREEN   Change the time with  Buttons
//--------------------------------------------
void ChangeTheTime(byte PressedButtons)
{ // button pressed 1= PIN_BUTTON_0; 2 = PIN_BUTTON_1, 4 PIN_BUTTON_2
 switch (PressedButtons)
 {
  case  0:  break;
  case  1:  Iminute++; Iminute%=60; SetRTCTime(); break;
  case  2:  break;   // acts as SHIFT key
  case  3:  PrintDigital = 1-PrintDigital; 
            if(!PrintDigital) Displaytime(); 
            else PrintTimeInScreen(); 
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
// POCKET SCREEN Print screen
//--------------------------------------------
void PrintTemplateScreen(uint16_t color) 
{
 pocketscreen.setFontColor(color, BLACK_16b);      // set color
 pocketscreen.setCursor(0, 0);

 switch(Mem.LanguageChoice)
 {
  case 0:
         pocketscreen.print("HETVISOWASOVIJFQ");   //   pocketscreen.setCursor(0, 0); pocketscreen.print(__TIME__);
         pocketscreen.setCursor(0, 8);
         pocketscreen.print("PRECIESZSTIENKPF");
         pocketscreen.setCursor(0, 16);
         pocketscreen.print("KWARTSVOORSOVERA");
         pocketscreen.setCursor(0, 24);
         pocketscreen.print("HALFSMIDDERTVIJF");    
         pocketscreen.setCursor(0, 32);
         pocketscreen.print("ATWEESOEENOXVIER");
         pocketscreen.setCursor(0, 40);
         pocketscreen.print("ELFQTIENKTWAALFB");
         pocketscreen.setCursor(0, 48);
         pocketscreen.print("HDRIECNEGENACHTF");
         pocketscreen.setCursor(0, 56);
         pocketscreen.print("ZESVZEVENOENVUUR");  
         break;  
  case 1:
         pocketscreen.print("HITVISOWASOEXACT");  
         pocketscreen.setCursor(0, 8);
         pocketscreen.print("HALFITWENTYEFIVE");
         pocketscreen.setCursor(0, 16);
         pocketscreen.print("QUARTERTENZPASTA");
         pocketscreen.setCursor(0, 24);
         pocketscreen.print("TOLFSMIDKSIXVTWO");    
         pocketscreen.setCursor(0, 32);
         pocketscreen.print("FIVEETWELVEXTENR");
         pocketscreen.setCursor(0, 40);
         pocketscreen.print("ELEVENENFOURAONE");
         pocketscreen.setCursor(0, 48);
         pocketscreen.print("TEIGHTHREENIGHTF");
         pocketscreen.setCursor(0, 56);
         pocketscreen.print("NINESEVENOCLOCKR");  
         break;
  case 2:
         pocketscreen.print("ESTISTWARRGENAUT"); 
         pocketscreen.setCursor(0, 8);
         pocketscreen.print("ZEHNFUNFVIERTELQ");
         pocketscreen.setCursor(0, 16);
         pocketscreen.print("ZWANZIGTKURZAVOR");
         pocketscreen.setCursor(0, 24);
         pocketscreen.print("NACHYHALBKFUNFBO");    
         pocketscreen.setCursor(0, 32);
         pocketscreen.print("EINSEVIERZWOLFAR");
         pocketscreen.setCursor(0, 40);
         pocketscreen.print("MITTERNACHTNDREI");
         pocketscreen.setCursor(0, 48);
         pocketscreen.print("SECHHNSIEBENEUNB");
         pocketscreen.setCursor(0, 56);
         pocketscreen.print("ZWEIZEHNELFMKUHR");
         break;
  case 3:
         pocketscreen.print("ILWESTETAITEXACT");  
         pocketscreen.setCursor(0, 8);
         pocketscreen.print("SIXDEUXTROISONZE");
         pocketscreen.setCursor(0, 16);
         pocketscreen.print("QUATRERMINUITDIX");
         pocketscreen.setCursor(0, 24);
         pocketscreen.print("CINQNEUFKMIDISWO");    
         pocketscreen.setCursor(0, 32);
         pocketscreen.print("HUITESEPTUNEDOSR");
         pocketscreen.setCursor(0, 40);
         pocketscreen.print("HEURESYETOMOINSE");
         pocketscreen.setCursor(0, 48);
         pocketscreen.print("LETDEMIENEQUARTN");
         pocketscreen.setCursor(0, 56);
         pocketscreen.print("DIXEVINGTOCINQKR");
         break;   
 }
}

//--------------------------------------------
// CLOCK Update routine done every second
//--------------------------------------------
void EverySecondCheck(void)
{
 int32_t msLeap       = millis()- msTick;
 int32_t msLeapButton = millis()- LastButtonTime;
// if (msLeap >100)  pocketscreen.setLEDColor(LEDRed);      // Turn OFF the second on SecondsPin. Minimize DigitalWrites.
 if (msLeap > 999)                                          // Every second enter the loop
  {
   GetTijd(0);                                             // Update Isecond, Iminute, Ihour, Iday, Imonth, Iyear
   msTick = millis();
   if (Iminute != lastminute)   EveryMinuteUpdate();        // Enter the every minute routine after one minute
   if(PrintDigital)    PrintTimeInScreen();                //  show the digital time in the screen
// pocketscreen.setLEDColor(LEDBlue);                      // turn ON the second on pin 
//   if(Isecond % 30 == 0) DimLeds(true);                  // Text LED intensity control + seconds tick print every 30 seconds    
//   DimLeds(false);                                       // Read the LDR and set intensity for the LEDs
                              #if defined(WIFI)
   WIFIServercheck(); 
                              #endif
   SerialCheck();                                          // Check serial port every second
  }
  
 }
//--------------------------------------------
// CLOCK Update routine done every minute
//--------------------------------------------
 void EveryMinuteUpdate(void)
 {
  static byte lastday = 0;
  lastminute = Iminute;  
  if(LEDsAreOff) { LedsOff(); ShowLeds();}                // Turn the LEDs off
   else if(!PrintDigital) Displaytime();                  // Turn the LEDs on                 
  Print_RTC_tijd();
  if(Ihour != lasthour) 
    {
      lasthour = Ihour;
      if(Ihour == Mem.TurnOffLEDsAtHH) {LEDsAreOff = true;  LedsOff(); ShowLeds();}         // is it time to turn off the LEDs?
      if(Ihour == Mem.TurnOnLEDsAtHH)   LEDsAreOff = false;                                 // or on?
     }
   if(Iday != lastday) 
    {
      lastday = Iday; 
     }
 }

//--------------------------------------------
// CLOCK check for serial input
//--------------------------------------------
void SerialCheck(void)
{
 String  SerialString; 
 while (SerialUSB.available())
    { 
     char c = SerialUSB.read();
//     SerialUSB.write(c);
     if (c>31 && c<128) SerialString += c;                 // allow input from Space - Del
     else c = 0;                                           // delete a CR
    }
 if (SerialString.length()>0) 
    {
      ReworkInputString(SerialString);                     // Rework ReworkInputString();
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
// String temp;
 InputString.toCharArray(sptext, MAXTEXT-1);    // Tekstprintln(sptext);
  SerialUSB.println(InputString);
  if (InputString.length() >10) return;

 if(  InputString[0] > 64 && InputString[0] <123 )                     // Does the string start with a letter?
  { 
//   Tekstprintln(sptext);
   switch ((byte)InputString[0])
   {
    case 'A':
    case 'a':
            if (InputString.length() == 1)
              {
              }
             else Tekstprintln("**** Length fault. Enter A ****");
             break;  
   
   
 case 'C':
    case 'c':
             int i,n;         // i = 0,1,2 -> DCFtiny, DCF77,Both
             if(InputString.length() == 1)
               {
                Mem.LightReducer    = 40;
                Mem.LowerBrightness = 5;  
//                Tekstprintln("Statistics data erased. EEPROM data will be cleared at midnight"); 
               }
              if(InputString.length() == 3)
               {
//                for (i=0 ; i<EEPROM.length(); i++) { EEPROM.write(i, 0); }
                Tekstprintln("EEPROM data were erased"); 
                setup();
               }
                                                    break;
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
               Print_tijd();  //Tekstprintln(sptext);   //Print_Tijd() fills sptext with time string
              }
            else Tekstprintln("**** Length fault. Enter ddmmyyyy ****");
            break;
    case 'e':
    case 'E':  
             if (InputString.length() == 2 )
               {
                byte res = (byte) InputString.substring(1,2).toInt();   
                Mem.LanguageChoice = res%4;  // Result between 0 and 3 , LEDsAreOff?"OFF":"ON" );
                byte ch = Mem.LanguageChoice;                
                sprintf(sptext,"Language choice:%s",ch==0?"NL":ch==1?"UK":ch==2?"DE":ch==3?"FR":"NOP"); 
                Tekstprintln(sptext);
                lastminute = 99;                                      // Force a minute update
               }
             else Tekstprintln("**** Display choice length fault. Enter E0 - E3"); 
            break;     

    case 'I':
    case 'i': 
            if (InputString.length() == 1)
            {  
             SWversion();
            }
            break;

/*
    case 'L':                                                         // Lowest value for Brightness
    case 'l':
             if (InputString.length() < 5)
               {      
 //               temp = InputString.substring(1);
                Mem.LowerBrightness   = (byte) constrain(InputString.substring(1).toInt(),0,255);
                sprintf(sptext,"Lower brightness changed to: %d bits",Mem.LowerBrightness);
                Tekstprintln(sptext);
               }
             else Tekstprintln("**** Length fault. Enter Lnnn ****");
             break;
    case 'M':                                                         // factor ( 0 - 1) to multiply brighness (0 - 255) with 
    case 'm':
             if (InputString.length() < 5)
               {    
                temp = InputString.substring(1);
                Mem.LightReducer = constrain(temp.toInt(),1,255);
                sprintf(sptext,"Max brightness changed to: %d%%",Mem.LightReducer);
                Tekstprintln(sptext);
               }
             else Tekstprintln("**** Length fault. Enter Mnnn ****");
              break;
    case 'N':
    case 'n':
             if (InputString.length() == 1 )         Mem.TurnOffLEDsAtHH = Mem.TurnOnLEDsAtHH = 0;
             if (InputString.length() == 5 )
              {
               temp   = InputString.substring(1,3);  Mem.TurnOffLEDsAtHH = (byte) temp.toInt(); 
               temp   = InputString.substring(3,5);  Mem.TurnOnLEDsAtHH = (byte) temp.toInt(); 
              }
             Mem.TurnOffLEDsAtHH = constrain(Mem.TurnOffLEDsAtHH, 0, 23);
             Mem.TurnOnLEDsAtHH  = constrain(Mem.TurnOnLEDsAtHH,  0, 23); 
             sprintf(sptext,"LEDs are OFF between %2d:00 and %2d:00", Mem.TurnOffLEDsAtHH,Mem.TurnOnLEDsAtHH );
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
//                else {DimLeds(true); Displaytime();}                   // Turn the LEDs on                
               }
                                                                  break;                                                                   
    case 'P':
    case 'p':  
//             if (InputString.length() == 9 )
//               {
//                temp = InputString.substring(1,9);
//                LetterColor = Mem.OwnColour = HexToDec(temp);               // Display letter color 
//                sprintf(sptext,"Own colour stored0X%X", Mem.OwnColour);
//                Tekstprintln(sptext); 
//                Tekstprintln("**** Own colour changed ****"); 
//                Displaytime();
//               }
//             else Tekstprintln("**** Length fault. Enter Pwwrrggbb ****");            
//             break;
    case 'q':
    case 'Q':  
             if (InputString.length() == 2 )
               {
                temp   = InputString.substring(1,2);     
                 Mem.DisplayChoice = (byte) temp.toInt(); 
                sprintf(sptext,"Display choice: Q%d", Mem.DisplayChoice);
                Tekstprintln(sptext);
                lastminute = 99;                                      // Force a minute update
               }
             else Tekstprintln("**** Display choice length fault. Enter Q0 - Q6"); 
             Displaytime();                                           // Turn on the LEDs with proper time             
            break;     


    case 'R':
    case 'r':
            if (InputString.length() == 1)
              {
//               Reset();                                               // Reset all settings 
               Tekstprintln("**** Resetted to default settings ****"); 
              }
            break;
*/
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
    case 'X':
    case 'x':    
             if (InputString.length() >1 && InputString.length() < 6 )                
                    MilliSecondValue = InputString.substring(1,5).toInt();                
             Demo = 1 - Demo;                                          // Toggle Demo mode
             if (!Demo)  MilliSecondValue = 1000;                      // So clock runs again at normal speed minus 1 ms
             sprintf(sptext,"Demo mode: %d MillisecondTime=%d",Demo,MilliSecondValue);
             Tekstprintln(sptext);
             break;        

default:
            break;
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
// EEPROM.put(0,Mem);                                                    // Update EEPROM                                                
 InputString = "";

}

//--------------------------------------------
//  LED Set color for LED
//--------------------------------------------
void ColorLeds(char* Tekst, int FirstLed, int LastLed, uint16_t RGBWColor)
{ 

 //if (!NoTextInColorLeds && strlen(Tekst) > 0 )
 {sprintf(sptext,"%s ",Tekst); Tekstprint(sptext); }   // Print the text 
 pocketscreen.setFontColor(RGBWColor, BLACK_16b);      // set color
 pocketscreen.setCursor(6*(FirstLed%16), 8*(FirstLed/16));
 sprintf(sptext,"%s",Tekst);
 to_upper(sptext); 
 pocketscreen.print(sptext); 
 ShowLeds(); 
}

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
 pocketscreen.drawRect(0, 0, 96, 64, true, LIGHTGRAY_16b);
}

//--------------------------------------------
// LED Turn On and Off the LED's after Delaytime is milliseconds
//--------------------------------------------
void Laatzien()
{ 
 ShowLeds();
 LedsOff(); 
}

//--------------------------------------------
//  LED Push data in LED strip to commit the changes
//--------------------------------------------
void ShowLeds(void)
{

}
//--------------------------------------------
//  LED Set brighness of LEDs
//--------------------------------------------  
void SetBrightnessLeds( byte Bright)
{
// ShowLeds();
}

//--------------------------------------------
// CLOCK Version info
//--------------------------------------------
void SWversion(void) 
{ 
PrintLine(40);
for (int i = 0; i < sizeof(menu) / sizeof(menu[0]); Tekstprintln(menu[i++]));
PrintLine(40);
// sprintf(sptext,"  Brightness Min: %3d bits  Max: %3d%%",Mem.LowerBrightness, Mem.LightReducer);      Tekstprintln(sptext);
// sprintf(sptext,"    LDR read Min:%4d bits  Max: %3d bits",MinPhotocell, MaxPhotocell);           Tekstprintln(sptext); 
// sprintf(sptext,"LEDs off between: %0.2d - %0.2d",Mem.TurnOffLEDsAtHH, Mem.TurnOnLEDsAtHH);       Tekstprintln(sptext);
// sprintf(sptext,"  Display choice: %3d", Mem.DisplayChoice);                                       Tekstprintln(sptext);
 sprintf(sptext,"Software: %s",VERSION);                                                          Tekstprintln(sptext); 
 GetTijd(1);  
 PrintLine(40);
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
// LedsOff();                                 // Start by clearing the display to a known state   

 PrintTemplateScreen(DARKGRAY_16b); //0x1010); //colors[random(NUM_COLORS)]);
// if( Mem.DisplayChoice == DIGITAL ) { TimePlaceDigit(Ihour,Iminute); }
// else                                      // If not a difital display 
   {
   switch(Mem.LanguageChoice)
     {
      case 0: Dutch();    break;
      case 1: English();  break;
      case 2: German();   break;
      case 3: French();   break;
     }

// SetSecondColour();                         // Set the colour per second of 'IS' and 'WAS'   
    ShowLeds();                                // and turn on the LEDs
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
