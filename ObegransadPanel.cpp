// By /u/frumperino
// goodwires.org

#include <ObegransadPanel.h>

void init(int p_latch, int p_clock, int p_data, int p_enable);

ObegransadPanel::ObegransadPanel(int p_latch, int p_clock, int p_data, int bitDepth,
                             int numPanels) : Adafruit_GFX(16, numPanels * 16)
{
    init(p_latch, p_clock, p_data, bitDepth, numPanels);
}

ObegransadPanel::ObegransadPanel(int p_latch, int p_clock, int p_data) :
ObegransadPanel(p_latch, p_clock, p_data, 0, 1) { }

void ObegransadPanel::init(int p_latch, int p_clock, int p_data, int bitDepth, int numPanels)
{
    //Serial.println( "=> ObegransadPanel::init");
    _pLatch = p_latch;
    _pClock = p_clock;
    _pData = p_data;
    pinMode(_pLatch, OUTPUT);
    pinMode(_pClock, OUTPUT);
    pinMode(_pData, OUTPUT);
    _bitDepth = bitDepth;
    _numPages = (1 << bitDepth);
    _pageMask = _numPages - 1;
    _numPanels = numPanels;
    _pageStride = 4 * numPanels;
    _numWords = _numPages * _pageStride;
    _numPixels = 16 * 16 * numPanels;
    _addressMask = _numPixels-1;
    _width = 16;
    _height = 16 * _numPanels;
    buf = (unsigned short*) malloc(_numWords * sizeof(unsigned short));
}

void ObegransadPanel::clear()
{
  //Serial.println( "=> ObegransadPanel::clear");
  for (int i=0;i<16*2;i++) {
      buf[i] = 0;
  }
}

/*
void ObegransadPanel::writeDeepPixel(unsigned short x, unsigned short y, unsigned short value)
{
    //ToDo:
    //Serial.println( "!!! ObegransadPanel::writeDeepPixel");
    if (x > 7) { y += 0x10; x &= 0x07; }
    unsigned int address = (x + (y << 3)) & _addressMask;
    unsigned short ba = address >> 4;
    unsigned short br = address & 0x0F;
    unsigned short ms = (1 << br);
    unsigned short mc = 0xFFFF ^ ms;
    unsigned short* wp = &buf[ba];
    unsigned short ofs = (x+y) & _pageMask;
    for (unsigned int i=0;i<_numPages;i++)
    {
        ofs++;
        ofs &= _pageMask;
        char b = ((value >> ofs) & 0x01);
        if (b)
        {
            *wp |= ms;
        }
        else
        {
            *wp &= mc;
        }
        wp+=_pageStride;
    }
}
*/

void ObegransadPanel::scan()
{
  //Serial.println( "=> ObegransadPanel::scan");
  uint8_t val;
  uint8_t addr = 1;
  for( uint8_t i = 1; i <= 16 * 2; i++) {
    if( i % 2 == 1) {
        //Serial.print( i-1);Serial.print( "-");Serial.print( addr); Serial.print( "\t");
    }
    for( uint8_t j = 0; j < 8; j++) {
      if( j == 4) {
        //Serial.print( " ");
      }
      if( i % 2 == 0) {
        val = ( buf[addr] & (1 << 7-j) ) ? HIGH : LOW;
      }
      else {
        val = ( buf[addr] & (1 << j) ) ? HIGH : LOW;
      }
      digitalWrite(_pData, val);
      //Serial.print( val);
      digitalWrite(_pClock, HIGH);
      delayMicroseconds(1);
      digitalWrite(_pClock, LOW);
    }
    
    if( i % 2 == 0) {
        //Serial.println(" ");
    }
    else {
        //Serial.print( " - ");
    }
    if( i % 8 == 4 + 2 ) { addr -= 1;}
    else if( i % 4 == 0 + 2 ) { addr -= 3;}
    else { addr += 2; }    
  }
  digitalWrite(_pLatch,HIGH);
  delayMicroseconds(1);
  digitalWrite(_pLatch,LOW);
}

unsigned short ObegransadPanel::width()
{
  //Serial.println( "=> ObegransadPanel::width");
    return this->_width;
}

unsigned short ObegransadPanel::height()
{
  //Serial.println( "=> ObegransadPanel::height");
    return this->_height;
}

boolean ObegransadPanel::getPixel(int16_t x, int16_t y)
{
  //Serial.println( "=> ObegransadPanel::getPixel");
  int16_t t;
  switch ( getRotation()) {
    case 1:
      t = x;
      x = WIDTH - 1 - y;
      y = t;
      break;
    case 2:
      x = WIDTH - 1 - x;
      y = HEIGHT - 1 - y;
      break;
    case 3:
      t = x;
      x = y;
      y = HEIGHT - 1 - t;
      break;
  }

  return getRawPixel(x, y);
}

boolean ObegransadPanel::getRawPixel(int16_t x, int16_t y) {
  //Serial.println( "=> ObegransadPanel::getRawPixel");
  uint8_t addr = floor( (x + ( y * 16)) / 8);
  if( x > 7 ) { x %= 8; }
  return buf[addr] &= 1 << ( 7 - x);
}

void ObegransadPanel::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  //Serial.println( "=> ObegransadPanel::drawPixel");
  int16_t t;
  switch ( getRotation()) {
    case 1:
      t = x;
      x = WIDTH - 1 - y;
      y = t;
      break;
    case 2:
      x = WIDTH - 1 - x;
      y = HEIGHT - 1 - y;
      break;
    case 3:
      t = x;
      x = y;
      y = HEIGHT - 1 - t;
      break;
  }

  //Serial.print( "x: ");Serial.print( x);Serial.print( " - y: ");Serial.print( y); Serial.println( " ");
  if( x >= 0 && y >= 0 && x < height() && y<width() ) {
    uint8_t addr = floor( (x + ( y * 16)) / 8);
    if( x > 7 ) { x %= 8; }
    if( color == 0) { buf[addr] &= ~( 1 << ( 7 - x));}
    else { buf[addr] |= 1 << ( 7 - x);}
  }
}

void ObegransadPanel::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  //Serial.println( "=> ObegransadPanel::drawFastVLine");
  for (int i=0;i<h;i++)
  {
      drawPixel(x,y+i,color);
  }
}

void ObegransadPanel::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  //Serial.println( "=> ObegransadPanel::drawFastHLine");
  for (int i=0;i<w;i++)
  {
      drawPixel(x+i,y,color);
  }
}

void ObegransadPanel::fillScreen(uint16_t color)
{
  //Serial.println( "=> ObegransadPanel::fillScreen");
  for( uint8_t y = 0; y < 16; y++) {
    for( uint8_t x = 0; x < 16; x++) {
      drawPixel( x, y, color);
    }
  }
}

/* -------------------------------------------------- */

void ObegransadPanel::printBits( uint8_t val) {
  for( unsigned short i = 0; i < 8; i++) {
    if( i == 4 ) { Serial.print( " "); }
    Serial.print( ( (1 << 7 - i) & val) != 0 );
  }
}

void ObegransadPanel::debugMatrix() {
  for( uint8_t i = 0; i < 16; i+=2) {
    Serial.print( i);
    Serial.print( "\t");
    printBits(buf[i]);
    Serial.print( " - ");
    printBits(buf[i+1]);
    Serial.println( " ");
  }
}

