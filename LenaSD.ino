/*
 * Connect the SD card to the following pins:
 *
 * SD Card | ESP32
 *    D2       -
 *    D3       SS
 *    CMD      MOSI
 *    VSS      GND
 *    VDD      3.3V
 *    CLK      SCK
 *    VSS      GND
 *    D0       MISO
 *    D1       -
 */
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include <FastLED.h>

#include <LEDMatrix.h>
#include <LEDSprites.h>

// Change the next 6 defines to match your matrix type and size

#define LED_PIN        4
#define COLOR_ORDER    RGB
#define CHIPSET        WS2812B

#define MATRIX_WIDTH   16
#define MATRIX_HEIGHT  -8
#define MATRIX_TYPE    HORIZONTAL_MATRIX

cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> leds;


#define SHAPE_WIDTH    16
#define SHAPE_HEIGHT   8

const uint8_t Spritelena1Data[] = {
  0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,
  0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,
  0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x30,
  0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,0x40,
  0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50,
  0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,0x60,
  0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,0x70,
  0x71,0x72,0x73,0x74,0x75,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F };
  //const 
  struct CRGB Spritelena1Cols[255]; 
  /*= { CRGB(197,120,121), CRGB(218,146,133), CRGB(215,161,153), CRGB(212,178,172), CRGB(196,136,133), CRGB(212,120,123), CRGB(229,138,138), CRGB(235,155,148), CRGB(235,155,145), CRGB(236,166,153), CRGB(237,180,166), CRGB(238,183,167), CRGB(242,192,176), CRGB(250,208,191), CRGB(243,168,151), CRGB(147,41,68),
   CRGB(222,164,144), CRGB(214,155,144), CRGB(218,174,167), CRGB(215,148,139), CRGB(200,97,102), CRGB(195,89,101), CRGB(173,71,88), CRGB(200,111,116), CRGB(233,152,142), CRGB(235,155,142), CRGB(237,174,158), CRGB(237,175,160), CRGB(242,184,163), CRGB(227,161,146), CRGB(199,106,107), CRGB(135,35,64),
   CRGB(224,181,165), CRGB(217,170,158), CRGB(187,112,110), CRGB(170,57,71), CRGB(163,62,78), CRGB(157,58,75), CRGB(156,50,74), CRGB(184,68,85), CRGB(218,110,113), CRGB(233,140,133), CRGB(238,172,159), CRGB(244,180,164), CRGB(206,117,115), CRGB(144,44,67), CRGB(141,43,70), CRGB(146,45,68),
   CRGB(227,197,179), CRGB(179,106,105), CRGB(113,13,49), CRGB(106,21,59), CRGB(98,17,60), CRGB(152,93,118), CRGB(120,39,74), CRGB(194,78,89), CRGB(219,96,102), CRGB(230,125,123), CRGB(248,191,173), CRGB(198,131,125), CRGB(102,21,62), CRGB(137,61,93), CRGB(115,32,66), CRGB(105,22,59),
   CRGB(203,138,129), CRGB(185,73,85), CRGB(144,44,72), CRGB(153,60,87), CRGB(143,73,108), CRGB(229,195,195), CRGB(209,119,125), CRGB(193,68,80), CRGB(210,85,95), CRGB(230,128,127), CRGB(245,195,177), CRGB(170,81,98), CRGB(130,56,95), CRGB(193,126,146), CRGB(149,54,77), CRGB(96,14,53),
   CRGB(202,86,96), CRGB(220,109,116), CRGB(214,107,114), CRGB(200,87,102), CRGB(204,103,117), CRGB(212,131,137), CRGB(220,134,140), CRGB(211,93,102), CRGB(211,85,96), CRGB(228,125,128), CRGB(240,198,187), CRGB(209,120,128), CRGB(201,105,123), CRGB(184,92,108), CRGB(144,43,65), CRGB(118,27,60),
   CRGB(215,99,108), CRGB(224,122,127), CRGB(228,131,131), CRGB(225,127,128), CRGB(221,122,127), CRGB(221,132,138), CRGB(222,135,138), CRGB(215,104,112), CRGB(216,90,100), CRGB(224,112,121), CRGB(240,201,192), CRGB(226,158,154), CRGB(218,111,117), CRGB(200,84,95), CRGB(184,73,85), CRGB(130,32,61),
   CRGB(217,97,105), CRGB(227,120,123), CRGB(232,141,137), CRGB(233,153,146), CRGB(232,153,147), CRGB(227,136,133), CRGB(221,111,116), CRGB(218,93,101), CRGB(221,102,110), CRGB(240,197,189), CRGB(228,168,160), CRGB(222,123,124), CRGB(219,97,107), CRGB(206,86,95), CRGB(130,36,67), CRGB(0,0,0),
   CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0),
   CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0),
   CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0),
   CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0),
   CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0),
   CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0),
   CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0),
   CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0) };
   */
cSprite Spritelena1(16, 8, Spritelena1Data, 1, _8BIT, Spritelena1Cols);



cLEDSprites Sprites(&leds);

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

#define BUFFPIXEL 1

void bmpDraw(char *filename, uint8_t x, uint8_t y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   
  uint8_t  bmpDepth;              
  uint32_t bmpImageoffset;        
  uint32_t rowSize;               
  uint8_t  sdbuffer[3*BUFFPIXEL]; 
  uint8_t  buffidx = sizeof(sdbuffer); 
  boolean  goodBmp = false;       
  boolean  flip    = true;        
  int      w, h, row, col, x2, y2, bx1, by1;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print(F("File not found"));
    return;
  }
  
  if(read16(bmpFile) == 0x4D42) { 
    read32(bmpFile);
    (void)read32(bmpFile); 
    bmpImageoffset = read32(bmpFile); 
    bmpImageoffset, DEC;
    read32(bmpFile);
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);

    Serial.println(bmpWidth);
    Serial.println(bmpHeight);
    
    if(read16(bmpFile) == 1) {
      bmpDepth = read16(bmpFile);
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) {

        goodBmp = true; 
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);


        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }
        
        w = bmpWidth;
        h = bmpHeight;
        bx1 = by1 = 0; // UL coordinate in BMP file

        for (row=0; row<h; row++) { 
            pos = bmpImageoffset + row * rowSize;

            if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
              pos = bmpImageoffset + (bmpHeight - 1 - (row + by1)) * rowSize;
            else     // Bitmap is stored top-to-bottom
              pos = bmpImageoffset + (row + by1) * rowSize;
            pos += bx1 * 3; // Factor in starting column (bx1)
            
            if(bmpFile.position() != pos) { // Need seek?
              bmpFile.seek(pos);
              buffidx = sizeof(sdbuffer); // Force buffer reload
            }

          for (col=0; col<w; col++) {
            if (buffidx >= sizeof(sdbuffer)) {
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; 
            }

       
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];

            Spritelena1Cols[row * w + col] = CRGB(r,g,b);

            Serial.print("(");
            Serial.print(b);Serial.print(", ");
            Serial.print(g);Serial.print(", ");
            Serial.print(r);Serial.print(") - ");
            Serial.println(row * w + col);
            //strip.SetPixelColor(row * w + col, RgbColor(r*0.05,g*0.05,r*0.05));
            //myScreen.point(row, col, rgb565(r,g,b));

          } 
        }
         
        //strip.Show();
        
        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } 
    }
  }

  bmpFile.close();
  if(!goodBmp) Serial.println(F("BMP format not recognized."));
}

uint16_t read16(File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); 
  ((uint8_t *)&result)[1] = f.read(); 
  return result;
}

uint32_t read32(File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); 
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); 
  return result;
}

unsigned short rgb565(int r, int g, int b)
{
  unsigned char rr = r >> 3;
  unsigned char gg = g >> 2;
  unsigned char bb = b >> 3;
  return (unsigned short)rr * 2048 + gg * 32 + bb;
}

void setup()
{

  Serial.begin(9600);
  if(!SD.begin()){
      Serial.println("Card Mount Failed");
      return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
      Serial.println("No SD card attached");
      return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
      Serial.println("MMC");
  } else if(cardType == CARD_SD){
      Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
      Serial.println("SDHC");
  } else {
      Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  delay(5000);
  bmpDraw("/lena1.bmp", 16, 8);  

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds[0], leds.Size());
  FastLED.setBrightness(8);
  FastLED.clear(true);
  delay(500);
  FastLED.showColor(CRGB::Blue);
  delay(1000);
  FastLED.show();

  Spritelena1.SetPositionFrameMotionOptions(0/*X*/, 0/*Y*/, 0/*Frame*/, 0/*FrameRate*/, -1/*XChange*/, 0/*XRate*/, 0/*YChange*/, 0/*YRate*/, SPRITE_DETECT_EDGE | SPRITE_X_KEEPIN | SPRITE_Y_KEEPIN);
  Sprites.AddSprite(&Spritelena1);
}


void loop()
{
  FastLED.clear();
  Sprites.UpdateSprites();
  Sprites.RenderSprites();
  FastLED.show();
  delay(50);
}
