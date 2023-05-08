
#include <EggTurner.h>

//char[50] *chickenPic = "Hen.bmp";
String settingsIcon = "Settings.bmp";
String emptyString = "";

uint16_t version = MCUFRIEND_KBV_H_;

#define MINPRESSURE 200
#define MAXPRESSURE 1000
#define SD_CS 10

#define EGG_TURN_INTERVAL 60*60*4

const int XP = 8, XM = A2, YP = A3, YM = 9; //ID=0x9341
const int TS_LEFT = 907, TS_RT = 136, TS_TOP = 942, TS_BOT = 139;

const String monthNames[13] {"","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
const String dayOfWeekNames[8] {"","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
int pixel_x, pixel_y;     //Touch_getXY() updates global vars

time_t initTime;
time_t eggsSetTime;
time_t currTime;
time_t hatchTime;
time_t eggInPositionTime;
time_t lastTurned;
bool isLeft = false;

//TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
//EggTurnerButton settingsButton = EggTurnerButton(10,10,105,25,2,"Change Settings","Settings","",tft);
//EggTurnerButton homeButton = EggTurnerButton(10,10,55,25,2,"Go Home Blacky","Home","",tft);

int buttonStartX = 35;
int buttonStartY = 73;
int buttonH = 15;
int buttonW = 20;
int buttonColSpacing = 53;
int buttonRowSpacing = 45;
int buttonTextSize = 1;
int titleTextH = 25;

void setup()
{
  //Serial.print("buttonStartY+buttonRowSpacing+3+buttonH"); Serial.println(buttonStartY+buttonRowSpacing+3+buttonH);
  //Serial.print("buttonStartY+buttonRowSpacing+3+buttonH+buttonRowSpacing"); Serial.println(buttonStartY+buttonRowSpacing+3+buttonH+buttonRowSpacing);
    Serial.begin(9600);
    if (!Serial) delay(5000);           //allow some time for Leonardo
    
    uint16_t ID = tft.readID();
    tft.begin(ID);

    // bool good = SD.begin(SD_CS);
    // if (!good) {
    //     Serial.print(F("cannot start SD\n"));
    //     while (1);
    // }
    // else{
    //   Serial.print("Good is good\n");
    // }

    tft.setRotation(1);
    //bmpDraw(namebuf, 0, 0);
    
    //ReadConfig();
    Serial.print("currTime before assignment: "); Serial.println(String(currTime));
    //currTime = 1695570251;
    currTime = 1678651276;
    initTime = currTime;
    //eggsSetTime = initTime-518400; //518400 = 6 days of seconds
    eggsSetTime = initTime; 
    //setTime(currTime);
    //setSyncProvider(getTimeFunction);
    //setSyncInterval(1);
    lastTurned = currTime;
    Serial.print("currTime: "); Serial.println(currTime);
    Serial.print("currTime: "); Serial.println(GetDateStringFromEpoch(currTime));
    Serial.print("lastTurned: "); Serial.println(lastTurned);
}

void loop()
{
  SettingsScreen();
  delay(35000);
    //MainScreen();
   // delay(10000);
    //EggCandleScreen();
  //  delay(5000);
}

void MainScreen(){
    static uint8_t aspect = 1;
    const char *aspectname[] = {
        "PORTRAIT", "LANDSCAPE", "PORTRAIT_REV", "LANDSCAPE_REV"
    };
    
    const char *colorname[] = { "BLUE", "GREEN", "RED", "GRAY" };
    uint16_t colormask[] = { BLUE, GREEN, RED, GRAY };
    uint16_t ID = tft.readID(); 
    
    


    //bmpDraw(namebuf, 0, 0);
    //EggTurnerButton chickenButton = EggTurnerButton(0,0,0,0,emptyString,emptyString,chickenPic,tft);
    
    //chickenButton.PlaceButton();
    
    tft.fillScreen(BLUE);    
    //Remaining
    tft.setRotation(aspect);
    int width = tft.width();
    int height = tft.height();
    //tft.fillScreen(colormask[aspect]);

    tft.drawRect(0, 0, width, height, WHITE);
    tft.drawRect(25, 50, 275, 175, WHITE);

    Serial.print("\nBefore Assignement hatchTime "); Serial.print(String(hatchTime));
    Serial.print("\tBefore Assignement eggsSetTime "); Serial.println(String(eggsSetTime));
    long unsigned int twentyOneDaysOfSeconds = 1814400;//(21*24*60*60)
    Serial.print("\ttwentyOneDaysOfSeconds "); Serial.println(String(twentyOneDaysOfSeconds));
    hatchTime = eggsSetTime + twentyOneDaysOfSeconds;
    Serial.print("\nhatchTime "); Serial.print(String(hatchTime));
    Serial.print("\teggsSetTime "); Serial.println(String(eggsSetTime));
    UpdateTimeDisplay();
    UpdateTimeToTurnDisplay();

    //Day X of Y
    tft.setTextSize(3);
    tft.setTextColor(YELLOW);
    tft.setCursor(50, 60);
    tft.print("Day");
    tft.setTextColor(WHITE);
    tft.setCursor(140, 60);
    int64_t bighatchTime = hatchTime;
    int64_t bigcurrTime = currTime;
    int64_t bigeggsSetTime = eggsSetTime;
    
    // Serial.print("bighatchTime "); Serial.print(int64String(bighatchTime)); 
    // Serial.print("\tbigcurrTime "); Serial.print(int64String(bigcurrTime)); 
    
    tft.print(String(String(floor((bigcurrTime-bigeggsSetTime)/(86400))).toInt()+1)); //24*60*60
    tft.print(" of 21");
  
    //Days Left
    tft.setTextColor(YELLOW);
    tft.setCursor(50, 105);
    tft.print("Left");
    tft.setTextColor(WHITE);
    tft.setCursor(175, 105);
    tft.print(String(String(floor((bighatchTime-bigcurrTime)/(86400))).toInt())); //86400 = 24*60*60
    
  
  
  //Hatch Date
    tft.setTextColor(YELLOW);
    tft.setCursor(50, 150);
    tft.print("Hatch");
    tft.setTextColor(WHITE);
    tft.setCursor(165, 150);
    String hatchTimeString = GetShortDateStringFromEpoch(hatchTime);
    tft.print(hatchTimeString);

    //Candle warning
    // tft.fillRoundRect(50, 170, 220, 30, 5, RED);
    // tft.setTextColor(WHITE);
    // tft.setTextSize(2);
    // tft.setCursor(57, 177);
    // tft.print("Candle the eggs!");

        //Show Time Elapsed
    // unsigned long elapsed;
    // elapsed = millis()/1000;
    // tft.fillRoundRect(50, 170, 220, 30, 5, RED);
    // tft.setTextColor(WHITE);
    //  tft.setTextSize(2);
    //  tft.setCursor(57, 177);
    //  tft.print("Elapased: ");
    //  tft.print(elapsed);
    //  tft.print(" seconds");

    
    //settingsButton.PlaceButton();

    time_t ticsFromUpdate = 0;
    time_t updateInterval = 1000;
    
    bool leaveScreen = false;

    while(!leaveScreen){
    TSPoint clicked = Touch_getXY();
    ticsFromUpdate++;
    if(ticsFromUpdate>updateInterval){
      ticsFromUpdate = 0;
      UpdateTimeDisplay();
      UpdateTimeToTurnDisplay();
    }
    // if(settingsButton.ButtonIsHit(pixel_x,pixel_y)){
    //   leaveScreen = true;
    // }
  if (clicked.z > ts.pressureThreshhold) {
  //    Serial.print("X = "); Serial.print(clicked.x);
  //    Serial.print("\tY = "); Serial.print(clicked.y);
  //    Serial.print("\tPressure = "); Serial.println(clicked.z);
  //    Serial.print("\tpixel_x = "); Serial.print(pixel_x);
  //    Serial.print("\tpixel_y = "); Serial.print(pixel_y);
  // Serial.print("\tupdateInterval = "); Serial.print(updateInterval);
  // Serial.print("\tticsFromUpdate = "); Serial.println(ticsFromUpdate);
  }
    }
    pixel_x =- 100;
    pixel_y = -100;
    SettingsScreen();
    delay(5000);
} 
void SettingsScreen(){
    static uint8_t aspect = 1;
    tft.fillScreen(YELLOW);
      tft.setRotation(aspect);
    int width = tft.width();
    int height = tft.height();
    //tft.fillScreen(colormask[aspect]);

    tft.drawRect(0, 0, width, height, WHITE);
    tft.drawRect(25, 50, 275, 175, WHITE);

    tft.setTextSize(2);
    tft.setCursor(buttonStartX,buttonStartY-buttonH-2);
    tft.setTextColor(BLACK);
    tft.print("Current Time");

    tft.setCursor(buttonStartX,buttonStartY+buttonRowSpacing+buttonH+5);
    tft.setTextColor(BLACK);
    tft.print("Eggs Set Time");

    
    //Year
    PlaceButton(buttonStartX,buttonStartY,buttonW,buttonH,buttonTextSize,"Increment Current Year","Up","");
    PlaceButton(buttonStartX,buttonStartY+buttonRowSpacing,buttonW,buttonH,buttonTextSize,"Decrement Current Year","Dn","");

    PlaceButton(buttonStartX,buttonStartY+buttonRowSpacing+3+buttonH+titleTextH,buttonW,buttonH,buttonTextSize,"Increment Set Year","Up","");
    PlaceButton(buttonStartX,buttonStartY+buttonRowSpacing+3+buttonH+buttonRowSpacing+titleTextH,buttonW,buttonH,buttonTextSize,"Decrement Set Year","Dn","");

    //Month
    PlaceButton(buttonStartX+buttonColSpacing,buttonStartY,buttonW,buttonH,buttonTextSize,"Increment Current Month","Up","");
    PlaceButton(buttonStartX+buttonColSpacing,buttonStartY+buttonRowSpacing,buttonW,buttonH,buttonTextSize,"Decrement Current Month","Dn","");

    PlaceButton(buttonStartX+buttonColSpacing,buttonStartY+buttonRowSpacing+3+buttonH+titleTextH,buttonW,buttonH,buttonTextSize,"Increment Set Month","Up","");
    PlaceButton(buttonStartX+buttonColSpacing,buttonStartY+buttonRowSpacing+3+buttonH+buttonRowSpacing+titleTextH,buttonW,buttonH,buttonTextSize,"Decrement Set Month","Dn","");

    //Day
    PlaceButton(buttonStartX+2*buttonColSpacing,buttonStartY,buttonW,buttonH,buttonTextSize,"Increment Current Day","Up","");
    PlaceButton(buttonStartX+2*buttonColSpacing,buttonStartY+buttonRowSpacing,buttonW,buttonH,buttonTextSize,"Decrement Current Day","Dn","");

    PlaceButton(buttonStartX+2*buttonColSpacing,buttonStartY+buttonRowSpacing+3+buttonH+titleTextH,buttonW,buttonH,buttonTextSize,"Increment Set Day","Up","");
    PlaceButton(buttonStartX+2*buttonColSpacing,buttonStartY+buttonRowSpacing+3+buttonH+buttonRowSpacing+titleTextH,buttonW,buttonH,buttonTextSize,"Decrement Set Day","Dn","");

    //Hour
    PlaceButton(buttonStartX+3*buttonColSpacing,buttonStartY,buttonW,buttonH,buttonTextSize,"Increment Current Day","Up","");
    PlaceButton(buttonStartX+3*buttonColSpacing,buttonStartY+buttonRowSpacing,buttonW,buttonH,buttonTextSize,"Decrement Current Day","Dn","");

    PlaceButton(buttonStartX+3*buttonColSpacing,buttonStartY+buttonRowSpacing+3+buttonH+titleTextH,buttonW,buttonH,buttonTextSize,"Increment Set Day","Up","");
    PlaceButton(buttonStartX+3*buttonColSpacing,buttonStartY+buttonRowSpacing+3+buttonH+buttonRowSpacing+titleTextH,buttonW,buttonH,buttonTextSize,"Decrement Set Day","Dn","");

    //Minute
    PlaceButton(buttonStartX+4*buttonColSpacing,buttonStartY,buttonW,buttonH,buttonTextSize,"Increment Current Minute","Up","");
    PlaceButton(buttonStartX+4*buttonColSpacing,buttonStartY+buttonRowSpacing,buttonW,buttonH,buttonTextSize,"Decrement Current Minute","Dn","");

    PlaceButton(buttonStartX+4*buttonColSpacing,buttonStartY+buttonRowSpacing+3+buttonH+titleTextH,buttonW,buttonH,buttonTextSize,"Increment Set Minute","Up","");
    PlaceButton(buttonStartX+4*buttonColSpacing,buttonStartY+buttonRowSpacing+3+buttonH+buttonRowSpacing+titleTextH,buttonW,buttonH,buttonTextSize,"Decrement Set Minute","Dn","");
}
// void ReadConfig(){
//   String fileContents = "";
//   char nextChar;
//   int charIndex;
//   int commaIndex;
//   int commaPos[20];

//   // open the file for reading:
//   myFile = SD.open("dateConf.txt");
//   if (myFile) {
//     Serial.println("dateConf.txt:");

//     // read from the file until there's nothing else in it:
//     charIndex = 0;
//     commaIndex = 0;
//     while (myFile.available()) {
//       nextChar = myFile.read();
//       //Serial.write(nextChar);
//       fileContents.concat(nextChar);
//       if(nextChar == ',' || nextChar == ':' || nextChar == '\r' || nextChar == '\n'){
//         commaPos[commaIndex++] = charIndex;
//         Serial.println(charIndex);
//       }
//       charIndex++;
//     }
 
//  // close the file:
//     myFile.close();

//     String initYear = fileContents.substring(commaPos[0]+2,commaPos[1]);
//     String initMonth = fileContents.substring(commaPos[1]+1,commaPos[2]);
//     String initDay = fileContents.substring(commaPos[2]+1,commaPos[3]);
//     String initHour = fileContents.substring(commaPos[3]+1,commaPos[4]);
//     String initMinute = fileContents.substring(commaPos[4]+1,commaPos[5]);
//     // Serial.println("initYear:");
//     // Serial.println(initYear);
//      Serial.println("initMonth:");
//      Serial.println(initMonth);
//     // Serial.println("initDay:");
//     // Serial.println(initDay);
//     // Serial.println("initHour:");
//     // Serial.println(initHour);
//     // Serial.println("initMinute");
//     // Serial.println(initMinute);

//     String setYear = fileContents.substring(commaPos[7]+2,commaPos[8]);
//     String setMonth = fileContents.substring(commaPos[8]+1,commaPos[9]);
//     String setDay = fileContents.substring(commaPos[9]+1,commaPos[10]);
//     String setHour = fileContents.substring(commaPos[10]+1,commaPos[11]);
//     String setMinute = fileContents.substring(commaPos[11]+1,commaPos[12]);
//     // Serial.println("setYear:");
//     // Serial.println(setYear);
//     // Serial.println("setMonth:");
//     // Serial.println(setMonth);
//     // Serial.println("setDay:");
//     // Serial.println(setDay);
//     // Serial.println("setHour:");
//     // Serial.println(setHour);
//     // Serial.println("setMinute");
//     // Serial.println(setMinute);
   
//     // Serial.println("This is what I read from the config:");
//     // Serial.println(fileContents);

//   initTime = tmConvert_t(initYear.toInt(),(byte)initMonth.toInt(),initDay.toInt(),initHour.toInt(),initMinute.toInt(),00);
//   eggsSetTime = tmConvert_t(setYear.toInt(),setMonth.toInt(),setDay.toInt(),setHour.toInt(),setMinute.toInt(),00);

//   } else {
//     // if the file didn't open, print an error:
//     Serial.println("error opening dateConf.txt");
//   }



// 	 return;
   

   
// }

void PlaceButton(int x,int y,int width,int height,int textSize,String value,String buttonText,String imageFilePath){
  Serial.print("Placing Button X: ");
  Serial.print(x);
  Serial.print(" Y: ");
  Serial.print(y);
  Serial.print(buttonText);
  Serial.println("");
  if(imageFilePath != NULL){
  	//BmpDraw(imageFilePath,x,y);
  }
  if(buttonText != NULL){
	    tft.setTextSize(textSize);
    tft.setTextColor(WHITE);
	tft.fillRoundRect(x, y, width, height, 4,GREEN);
    tft.setCursor(x+5, y+5);
    tft.print(buttonText);
  }
}

void EggCandleScreen(){
  
    //EggTurnerButton settingsButton = EggTurnerButton(0,0,50,50,emptyString,emptyString,settingsIcon,tft);
    //settingsButton.PlaceButton();
}

time_t tmConvert_t(int YYYY, byte MM, byte DD, byte hh, byte mm, byte ss)
{
  tmElements_t tmSet;
  tmSet.Year = YYYY - 1970;
  tmSet.Month = MM;
  tmSet.Day = DD;
  tmSet.Hour = hh;
  tmSet.Minute = mm;
  tmSet.Second = ss;
  return makeTime(tmSet); 
}

String GetDateStringFromEpoch(time_t convTime){
  String timeString = "";
  
  // Serial.println("\n---------------------------------");
  // Serial.print("convTime");  Serial.println(convTime);
  // Serial.print("year(convTime)");  Serial.println(year(convTime));
  // Serial.print("month(convTime)");  Serial.println(monthNames[month(convTime)]);
  // Serial.print("day(convTime)");  Serial.println(day(convTime));
  // Serial.print("hour(convTime)");  Serial.println(hour(convTime));
  // Serial.println("---------------------------------\n");

  timeString += String(monthNames[month(convTime)]);
  timeString += " ";
  timeString += ZeroPad(String(day(convTime)));
  timeString += ", ";
  timeString += String(year(convTime));
  timeString += " ";
  timeString += ZeroPad(String(hour(convTime)));
  timeString += ":";
  timeString += ZeroPad(String(minute(convTime)));
  timeString += ":";
  timeString += ZeroPad(String(second(convTime)));
  return timeString;
}
String GetShortDateStringFromEpoch(time_t convTime){
  String timeString = "";
  timeString += String(monthNames[month(convTime)]);
  timeString += " ";
  timeString += ZeroPad(String(day(convTime)));
  return timeString;
}

String GetTimeStringFromSeconds(time_t seconds){
  unsigned long hours = floor(seconds/(60*60));
  unsigned long minutes = floor((seconds-(hours*60*60))/(60));
  unsigned long daSeconds = floor((seconds-(hours*60*60)-(minutes*60)));
  
  // Serial.println("---------------------------------");
  // Serial.print("convTime: "); Serial.println(convTime);
  // Serial.print("hours: ");  Serial.println(String(hour(convTime)));
  // Serial.print("minutes: ");  Serial.println(String(minute(convTime)));
  // Serial.print("seconds: ");  Serial.println(String(second(convTime)));
  // Serial.println("---------------------------------");

  String timeString = "";
  timeString += SpacePad(String(hours));
  timeString += ":";
  timeString += ZeroPad(String(minutes));
  timeString += ":";
  timeString += ZeroPad(String(daSeconds));
  return timeString;
}

TSPoint Touch_getXY(void)
{
  TSPoint p;
  bool pressed = false;

  //while(!pressed){
  p = ts.getPoint();
  pinMode(YP, OUTPUT);      //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);
  digitalWrite(XM, HIGH);
  
  pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
    if (tft.width() <= tft.height()) { //Portrait
      pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
      pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
    } else {
      pixel_x = map(p.y, TS_BOT, TS_TOP, 0, tft.width());
      pixel_y = map(p.x, TS_RT, TS_LEFT, 0, tft.height());
    }
  }
  //}
  return p;
}

time_t getTheSortOfCurrentTime(){
  return initTime + millis()/1000;
}

void UpdateTimeDisplay(){
    currTime = getTheSortOfCurrentTime();
    //tft.fillRect(35,195, 263, 20, BLUE);
    tft.setTextSize(2);
    tft.setCursor(40, 195);
    tft.setTextColor(BLACK,BLUE);
    
    String currTimeString = GetDateStringFromEpoch(currTime);
    tft.print(currTimeString);
}

void UpdateTimeToTurnDisplay(){
    //display time
    currTime = getTheSortOfCurrentTime();

    eggInPositionTime = currTime-lastTurned;
    // Serial.print("Current Time"); Serial.println(currTime);
    // Serial.print("lastTurned"); Serial.println(lastTurned);
    // Serial.print("Egg In Position Time"); Serial.println(eggInPositionTime);

    if(eggInPositionTime > EGG_TURN_INTERVAL){
      isLeft = !isLeft;
      lastTurned = currTime;
    }

    //Left Right Indicator
    tft.setTextSize(2);
    tft.setCursor(185,18);
    tft.setTextColor(ORANGE,BLUE);
    if(isLeft){
      tft.print("< ");
    }
    else{
      tft.print("> ");
    }
    String inPosString = GetTimeStringFromSeconds(EGG_TURN_INTERVAL-eggInPositionTime);
    tft.print(inPosString);
}

String SpacePad(String integerString){
  if(integerString.toInt() < 10){
    integerString = " " + integerString;
  }
  return integerString;
}
String ZeroPad(String integerString){
  String returnString = integerString;
  if(integerString.toInt() < 10){
    returnString = "0" + integerString;
  }

  return returnString;
}
