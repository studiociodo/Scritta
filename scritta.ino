
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include "Parola_Fonts_data.h"


MD_Parola P = MD_Parola(10, 8);  //(pin CS, numero elementi)
//CLK = 13, DIO = 11
SoftwareSerial mySerial(8, 9); // RX, TX (TX dell' HC-05 su D8)


const char* deb1 = "+READY";
const char* deb2 = "+DISC";
const char* deb3 = "+PAIRABLE";
const char* deb4 = "CONNECT";

#define  BUF_SIZE  140
char curMessage[BUF_SIZE] = { "" };
char newMessage[BUF_SIZE] = { "" };
bool newMessageAvailable = true;
bool endofstring = false;

void setup() {
  mySerial.begin(9600);
  P.begin();
  P.setFont(ExtASCII);
  P.displayText(curMessage, PA_LEFT, 25, 1000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);

}

void loop()
{
   if (P.displayAnimate())
  {
    if (newMessageAvailable)
    {
      if((strstr (newMessage, deb1)) ||(strstr (newMessage, deb2)) ||(strstr (newMessage, deb3)) ||(strstr (newMessage, deb4))) {
        newMessageAvailable = false;
      } else {
              strcpy(curMessage, newMessage);
      newMessageAvailable = false;
      }

    }
    P.displayReset();
  }
  readSerial();

}


void readSerial(void)
{
  String messaggio;
  endofstring = false;
  while (mySerial.available() > 0)
  {
    messaggio = mySerial.readString();
    delay(20);
    endofstring = true;
    newMessageAvailable = true;
  }
  mySerial.flush();
  if ((newMessageAvailable) && (endofstring))
  {
    //Serial.println(messaggio);
    while (messaggio.indexOf("à") > 0) {
    messaggio.setCharAt(messaggio.indexOf('à'), '^');
    }
    while (messaggio.indexOf("È") > 0) {
    messaggio.setCharAt(messaggio.indexOf('È'), '[');
    }
    while (messaggio.indexOf("è") > 0) {
    messaggio.setCharAt(messaggio.indexOf('è'), '[');
    }
    while (messaggio.indexOf("é") > 0) {
    messaggio.setCharAt(messaggio.indexOf('é'), ']');
    }
    while (messaggio.indexOf("ì") > 0) {
    messaggio.setCharAt(messaggio.indexOf('ì'), '{');
    }
    while (messaggio.indexOf("ò") > 0) {
    messaggio.setCharAt(messaggio.indexOf('ò'), '}');
    }
    while (messaggio.indexOf("ù") > 0) {
    messaggio.setCharAt(messaggio.indexOf('ù'), '|');
    }
    
    char c;
    char no = 0xC3; //character I want removed.

    for (int i=0; i<messaggio.length()-1;++i){
        c = messaggio.charAt(i);
        if(c==no){
            messaggio.remove(i, 1);
        }
    if ((messaggio == "vuoto") || (messaggio == "Vuoto")){
      messaggio = "";
    }
      messaggio.toCharArray(newMessage, BUF_SIZE);


    }

    }
}
