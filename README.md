# ObegransadPanel

This is a first rough cut of an Arduino library based sur la librairie Adafruit_GFX, for the IKEA OBEGRANSAD LED panels (inspired by https://github.com/frumperino/FrekvensPanel).

Tested with ESP32.

Contributions welcome
Happy hacking!

```
#include <Arduino.h>
#include <ObegransadPanel.h>

#define PIN_ENABLE 26   // Orange
#define PIN_DATA 27     // Yellow
#define PIN_CLOCK 14    // Green
#define PIN_LATCH 12    // Blue
#define PIN_BUTTON 25   // Purple

ObegransadPanel panel( PIN_LATCH, PIN_CLOCK, PIN_DATA);


void setup() {
  Serial.begin(115200);

  pinMode(PIN_ENABLE, OUTPUT); 
  analogWrite(PIN_ENABLE, 128); // full brightness
}



void scrollString(String str) {
    int y = floor((panel.height() - 7) / 2);
    panel.clear(); panel.scan();
    panel.setTextWrap(false);
    panel.setTextSize(0);
    panel.setTextColor( 1, 0);
    panel.setRotation(1);
    int charWidth = 5;
    int pxwidth = (str.length()*charWidth)+str.length()*2; 
    for (uint8_t x=0; x<pxwidth; x++) {
        panel.setCursor(x*-1,y);
        panel.print(str); 
        panel.scan();
        delay(150);
    }
}

void loop() {
  
  panel.clear();
  panel.scan();  
  scrollString( "Hello World !");
  //progressiveFillScreen();
/*
  panel.drawPixel(0, 0, 1);
  panel.scan();
  delay(1000);

  panel.drawLine( 0, 0, 0, 15, 1);
  panel.drawLine( 0, 0, 7, 15, 1);
  panel.drawLine( 0, 0, 15, 15, 1);
  panel.drawLine( 0, 0, 15, 7, 1);
  panel.drawLine( 0, 0, 15, 0, 1);
  panel.scan();
  delay(1000);

  panel.drawRect( 1, 1, 14, 14, 1);
  panel.scan();
  delay(1000);

  panel.fillRect( 3, 3, 10, 10, 1);
  panel.scan();
  delay(1000);

  panel.drawCircle( 8, 8, 5, 1);
  panel.scan();
  delay(1000);
  
  panel.fillCircle( 8, 8, 5, 1);
  panel.scan();
  delay(1000);

  panel.drawRoundRect( 1, 1, 14, 14, 4, 1);
  panel.scan();
  delay(1000); 

  panel.fillRoundRect( 1, 1, 14, 14, 4, 1);
  panel.scan();
  delay(1000);

  panel.drawTriangle( 1, 1, 15, 1, 7, 15, 1);
  panel.scan();
  delay(1000);

  panel.fillTriangle( 1, 1, 15, 1, 7, 15, 1);
  panel.scan();
  delay(1000);

  panel.drawChar( 1, 1, 'A', 1, 0, 1);
  panel.scan();
  delay(1000);

  panel.setCursor( 1, 1);
  //panel.setTextColor( 1);
  panel.setTextColor( 1, 0);
  panel.setTextSize( 1);
  panel.setTextWrap( false);
  panel.print("Hello world");
  panel.scan();
  delay(1000);

  panel.setRotation(0);
  panel.scan();
  delay(1000);

  panel.setRotation(1);
  panel.scan();
  delay(1000);

  panel.setRotation(2);
  panel.scan();
  delay(1000);

  panel.setRotation(3);
  panel.scan();
  delay(1000);
*/
}
```
