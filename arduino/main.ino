// scrolltext demo for Adafruit RGBmatrixPanel library.
// Demonstrates double-buffered animation on our 16x32 RGB LED matrix:
// http://www.adafruit.com/products/420
// DOUBLE-BUFFERED ANIMATION DOES NOT WORK WITH ARDUINO UNO or METRO 328.

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include <RGBmatrixPanel.h>

// Most of the signal pins are configurable, but the CLK pin has some
// special constraints.  On 8-bit AVR boards it must be on PORTB...
// Pin 11 works on the Arduino Mega.  On 32-bit SAMD boards it must be
// on the same PORT as the RGB data pins (D2-D7)...
// Pin 8 works on the Adafruit Metro M0 or Arduino Zero,
// Pin A4 works on the Adafruit Metro M4 (if using the Adafruit RGB
// Matrix Shield, cut trace between CLK pads and run a wire to A4).

//#define CLK 8 // USE THIS ON ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE 9
#define LAT 10
#define A A0
#define B A1
#define C A2

// Last parameter = 'true' enables double-buffering, for flicker-free,
// buttery smooth animation.  Note that NOTHING WILL SHOW ON THE DISPLAY
// until the first call to swapBuffers().  This is normal.
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, true);

// Similar to F(), but for PROGMEM string pointers rather than literals
#define F2(progmem_ptr) (const __FlashStringHelper*)progmem_ptr

const char str[] PROGMEM = "Adafruit 16x32 RGB LED Matrix";
int16_t    textX = matrix.width(), textMin = sizeof(str) * -12, hue = 0;

int8_t ball[3][4] = {{3, 0, 1, 1}, // Initial X,Y pos & velocity for 3 bouncy balls
                     {17, 15, 1, -1},
                     {27, 4, -1, 1}};

static const uint16_t PROGMEM ballcolor[3] = {
    0x0080, // Green=1
    0x0002, // Blue=1
    0x1000  // Red=1
};

String nom = "Arduino";
String msg = "";

void readSerialPort()
{
    msg = "";
    if (Serial.available())
    {
        delay(10);
        while (Serial.available() > 0)
        {
            msg += (char)Serial.read();
        }
        Serial.flush();
    }
}

void sendData()
{
    Serial.print(nom);
    Serial.print(" received : ");
    Serial.print(msg);
}

void setup()
{
    Serial.begin(9600);

    matrix.begin();
    matrix.setTextWrap(false); // Allow text to run off right edge
    matrix.setTextSize(2);
}

void loop()
{
    readSerialPort();

    if (msg != "")
        sendData();

    byte i;

    // Clear background
    matrix.fillScreen(0);

    // Draw big scrolly text on top
    matrix.setTextColor(matrix.ColorHSV(hue, 255, 255, true));
    matrix.setCursor(textX, 1);
    //matrix.print(F2(str));
    matrix.print(msg.c_str());

    // Move text left (w/wrap), increase hue
    if ((--textX) < textMin)
        textX = matrix.width();
    hue += 7;
    if (hue >= 1536)
        hue -= 1536;

#if !defined(__AVR__)
    // On non-AVR boards, delay slightly so screen updates aren't too quick.
    delay(20);
#endif

    // Update display
    matrix.swapBuffers(false);
}