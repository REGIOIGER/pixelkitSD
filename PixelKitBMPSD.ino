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


const uint16_t PixelCount = 128; // make sure to set this to the number of pixels in your strip
const uint16_t PixelPin = 4;  // make sure to set this to the correct pin, ignored for Esp8266


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

            Serial.print("(");
            Serial.print(b);Serial.print(", ");
            Serial.print(g);Serial.print(", ");
            Serial.print(r);Serial.print(") - ");
            Serial.println(row * w + col);
            strip.SetPixelColor(row * w + col, RgbColor(r*0.05,g*0.05,r*0.05));
            //myScreen.point(row, col, rgb565(r,g,b));

          } 
        }
         
        strip.Show();
        
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

void setup(){
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

    
    readFile(SD, "/hello.txt");
    
    Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
    Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));

    strip.Begin();
    strip.Show();
    
    delay(5000);
    bmpDraw("/lena1.bmp", 16, 8);

}

void loop(){
  delay(5000);
  //bmpDraw("/lena1.bmp", 16, 8);
}
