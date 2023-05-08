#include "EggTurner.h"
MCUFRIEND_kbv tft;

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GRAY    0x8410

File root;
String chickenPic = "Hen.bmp";
String settingsIcon = "Settings.bmp";
String emptyString = "";
unsigned long millisFromStart;

uint16_t version = MCUFRIEND_KBV_H_;

#define MINPRESSURE 200
#define MAXPRESSURE 1000

const int XP = 8, XM = A2, YP = A3, YM = 9; //ID=0x9341
const int TS_LEFT = 907, TS_RT = 136, TS_TOP = 942, TS_BOT = 139;

int pixel_x;
int pixel_y;

//TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

void setup()
{
    Serial.begin(9600);
    if (!Serial) delay(5000);           //allow some time for Leonardo
    uint16_t ID = tft.readID();
    tft.begin(ID);
      
    bool good = SD.begin(SD_CS);
    if (!good) {
        Serial.print(F("cannot start SD"));
        while (1);
    }
    tft.setRotation(3);
    //bmpDraw(namebuf, 0, 0);

}

void loop()
{
    ReadConfig();
    MainScreen();
    delay(5000);
  Serial.print("\n--------------Out of while-----------------\n");  
    tft.fillScreen(YELLOW);
    Serial.print("into selection");
    MonthSelectionScreen();
    Serial.print("Let's wait here for 5 seconds");
    delay(5000);
    delay(5000);
    DaySelectionScreen();
    delay(5000);
    HourSelectionScreen();
    delay(5000);
    MinuteSelectionScreen();
    delay(5000);
}
void ReadConfig(){
  String filename = "DateTimeConfig.txt";
     Serial.print("Loading config '");
     
   Serial.println('\'');
   // Open requested file on SD card
   SPCR = spi_save;
   if ((SD.open(filename)) == NULL) {
	 Serial.print("File not found ");
   Serial.print(filename);
   Serial.print("\n");
	 return;
   }


}
void MainScreen(){
  
    tft.fillScreen(BLUE);
      static uint8_t aspect = 3;
    const char *aspectname[] = {
        "PORTRAIT", "LANDSCAPE", "PORTRAIT_REV", "LANDSCAPE_REV"
    };
    //bmpDraw(namebuf, 0, 0);
    EggTurnerButton chickenButton = EggTurnerButton(0,0,0,0,emptyString,emptyString,chickenPic,tft);
    EggTurnerButton settingsButton = EggTurnerButton(0,0,50,50,emptyString,emptyString,settingsIcon,tft);
    
    //chickenButton.PlaceButton();
    //settingsButton.PlaceButton();
    
    const char *colorname[] = { "BLUE", "GREEN", "RED", "GRAY" };
    uint16_t colormask[] = { BLUE, GREEN, RED, GRAY };
    uint16_t ID = tft.readID(); //
    tft.setRotation(aspect);
    int width = tft.width();
    int height = tft.height();
    //tft.fillScreen(colormask[aspect]);
    tft.drawRect(0, 0, width, height, WHITE);
    tft.drawRect(32, 52, width - 64, height - 64, WHITE);
    tft.setTextSize(3);

    //Day X of Y
    tft.setTextColor(YELLOW);
    tft.setCursor(50, 75);
    tft.print("Day");
    tft.setTextColor(WHITE);
    tft.setCursor(130, 75);
    tft.print("10 of 21");

    //Remaining
    tft.setTextColor(YELLOW);
    tft.setCursor(50, 125);
    tft.print("Left");
    tft.setTextColor(WHITE);
    tft.setCursor(200, 125);
    tft.print("11");

    //Candle warning
    // tft.fillRoundRect(50, 170, 220, 30, 5, RED);
    // tft.setTextColor(WHITE);
    // tft.setTextSize(2);
    // tft.setCursor(57, 177);
    // tft.print("Candle the eggs!");


    
}
void MonthSelectionScreen(){
  const char *monthname[] = { "Jan", "Feb", "Mar", "Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };
  int upperX = 10;
  int upperY = 50;
  int margin = 10;
  int boxHeight = 30;
  int boxWidth = 60;
  int colWidth = boxWidth+margin;
  int rowHeight = boxHeight+margin;
  int textOffsetX = 12;
  int textOffsetY= 8;
  int textSize = 2;
  uint16_t textColor = WHITE;
  uint16_t buttonColor = GRAY;
  int row = 0;
  int col = 0;

  int boxx = 0;
  int boxy = 0;
   
  tft.fillScreen(BLUE);

  Serial.print("MonthSelectionScreen()");
  tft.setTextColor(textColor);
  tft.setTextSize(textSize);
  for(int monthIndex=0;monthIndex<12;monthIndex++){
    boxx = upperX+margin+(col*colWidth);
    boxy = upperY+margin+(row*rowHeight);

    col++;
    
    if((col%4)==0){
        row++;
        col = 0;
    }
    tft.fillRoundRect(boxx,boxy,boxWidth,boxHeight, 5, buttonColor);
    tft.setCursor(boxx+textOffsetX, boxy+textOffsetY);
    tft.print(monthname[monthIndex]);
  }
}

void DaySelectionScreen(){
  int upperX = 10;
  int upperY = 50;
  int margin = 10;
  int boxHeight = 22;
  int boxWidth = 30;
  int colWidth = boxWidth+margin;
  int rowHeight = boxHeight+margin;
  int textOffsetX = 4;
  int textOffsetY = 4;
  int textSize = 2;
  uint16_t textColor = WHITE;
  uint16_t buttonColor = GRAY;
  int row = 0;
  int col = 0;

  int boxx = 0;
  int boxy = 0;
  
  int extraXOffset = 0;

  tft.fillScreen(BLUE);
  Serial.print("DaySelectionScreen()");
  tft.setTextColor(textColor);
  tft.setTextSize(textSize);
  for(int dayIndex=1;dayIndex<32;dayIndex++){
    boxx = upperX+margin+(col*colWidth);
    boxy = upperY+margin+(row*rowHeight);

    col++;
    
    if((col%7)==0){
        row++;
        col = 0;
    }

    extraXOffset = 0;
    if(dayIndex<10){
      extraXOffset = boxWidth/4;
    }
    
    tft.fillRoundRect(boxx,boxy,boxWidth,boxHeight, 5, buttonColor);
    tft.setCursor(boxx+textOffsetX+extraXOffset, boxy+textOffsetY);
    tft.print(dayIndex);
  }
}
void HourSelectionScreen(){
  int upperX = 10;
  int upperY = 50;
  int margin = 10;
  int boxHeight = 22;
  int boxWidth = 30;
  int colWidth = boxWidth+margin;
  int rowHeight = boxHeight+margin;
  int textOffsetX = 4;
  int textOffsetY = 4;
  int textSize = 2;
  uint16_t textColor = WHITE;
  uint16_t buttonColor = GRAY;
  int row = 0;
  int col = 0;

  int boxx = 0;
  int boxy = 0;
  
  int extraXOffset = 0;

  tft.fillScreen(GREEN);
  Serial.print("DaySelectionScreen()");
  tft.setTextColor(textColor);
  tft.setTextSize(textSize);
  for(int dayIndex=1;dayIndex<25;dayIndex++){
    boxx = upperX+margin+(col*colWidth);
    boxy = upperY+margin+(row*rowHeight);

    col++;
    
    if((col%6)==0){
        row++;
        col = 0;
    }

    extraXOffset = 0;
    if(dayIndex<10){
      extraXOffset = boxWidth/4;
    }
    
    tft.fillRoundRect(boxx,boxy,boxWidth,boxHeight, 5, buttonColor);
    tft.setCursor(boxx+textOffsetX+extraXOffset, boxy+textOffsetY);
    tft.print(dayIndex);
  }
}
void MinuteSelectionScreen(){
  int upperX = 10;
  int upperY = 50;
  int margin = 10;
  int boxHeight = 22;
  int boxWidth = 30;
  int colWidth = boxWidth+margin;
  int rowHeight = boxHeight+margin;
  int textOffsetX = 4;
  int textOffsetY = 4;
  int textSize = 2;
  uint16_t textColor = WHITE;
  uint16_t buttonColor = GRAY;
  int row = 0;
  int col = 0;

  int boxx = 0;
  int boxy = 0;
  
  int extraXOffset = 0;

  tft.fillScreen(YELLOW);
  Serial.print("DaySelectionScreen()");
  tft.setTextColor(textColor);
  tft.setTextSize(textSize);
  for(int dayIndex=0;dayIndex<61;dayIndex+=5){
    boxx = upperX+margin+(col*colWidth);
    boxy = upperY+margin+(row*rowHeight);

    col++;
    
    if((col%7)==0){
        row++;
        col = 0;
    }

    extraXOffset = 0;
    if(dayIndex<10){
      extraXOffset = boxWidth/4;
    }
    
    tft.fillRoundRect(boxx,boxy,boxWidth,boxHeight, 5, buttonColor);
    tft.setCursor(boxx+textOffsetX+extraXOffset, boxy+textOffsetY);
    tft.print(dayIndex);
  }
}
