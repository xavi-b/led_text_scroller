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
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, true, 3);

int16_t textX = matrix.width();
int16_t textMin = 0;
int16_t x1 = 0;
int16_t y1 = 0;
uint16_t w = 0;
uint16_t h = 0;

String name = "Arduino";
String msg  = "Salut !";

void readSerialPort()
{
    if (Serial.available())
    {
        textX = matrix.width();
        msg = "";
        delay(10);
        msg = Serial.readStringUntil('\n');
        matrix.getTextBounds(msg, 0, 0, &x1, &y1, &w, &h);
        textMin = w * -1;
        Serial.flush();
        sendData();
    }
}

void sendData()
{
    Serial.print(name);
    Serial.print(" received : ");
    Serial.print(msg);
    Serial.print('\n');
}

void setup()
{
    Serial.begin(9600);

    matrix.begin();
    matrix.setTextWrap(false); // Allow text to run off right edge
    matrix.setTextSize(2);
    matrix.getTextBounds(msg, 0, 0, &x1, &y1, &w, &h);
    textMin = w * -1;
}

void loop()
{
    readSerialPort();

    // Clear background
    matrix.fillScreen(0);
//    matrix.fillRect(0, 0, 33, 16, 0x00ff);
//    matrix.fillRect(32, 0, 32, 16, 0x0ff0);
//    matrix.fillRect(64, 0, 32, 16, 0xff00);

    // Draw big scrolly text on top
    matrix.setTextColor(matrix.Color888(0, 0, 255));
    matrix.setCursor(textX, 1);
    matrix.print(msg);
    matrix.drawPixel(0, 0, 0);

    // Move text left (w/wrap)
    if ((--textX) < textMin)
        textX = matrix.width();

    delay(50);

    // Update display
    matrix.swapBuffers(true);
}
