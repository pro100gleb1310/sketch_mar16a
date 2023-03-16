/*************************************************************

 *************************************************************/

/* Данные девайса блинк*/
#define BLYNK_TEMPLATE_ID "TMPLiSvctxsq"
#define BLYNK_TEMPLATE_NAME "ЧС7"
#define BLYNK_AUTH_TOKEN "A74pCZkB31vHvOmB2Mlz-brVn6uEKL_Q"

/* Comment this out to disable prints and save space */
//#define BLYNK_PRINT Serial

/*Подключение библиотек блинк и вайфай*/
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

/*Подключение библиотеки расширения порта*/

#include "Arduino.h"
#include "PCF8575.h"

/*Инвертирую положения для реле*/
#define ON LOW
#define OFF HIGH

// Данные подключения к вайфай
char ssid[] = "1F13";
char pass[] = "1310dbnr";
bool inPossition = true;
int switch_304 = 1;  //  304 защелка
int counter = 0; // ограничитель набора
bool b = false; // какая-то хуйня

//Назначаю адрес i2c
PCF8575 pins_ext(0x20);
//Функции блинк

//Токоприемник 1
BLYNK_WRITE(V0) {
  int tokopr1 = param.asInt();
  if (tokopr1 == 0) {
    pins_ext.digitalWrite(P0, OFF);
  }
  if (tokopr1 == 1) {
    pins_ext.digitalWrite(P0, ON);
  }
}

//Токоприемник 2
BLYNK_WRITE(V1) {
  int tokopr2 = param.asInt();
  if (tokopr2 == 0) {
    pins_ext.digitalWrite(P1, OFF);
  }
  if (tokopr2 == 1) {
    pins_ext.digitalWrite(P1, ON);
  }
}

//Включение БВ
BLYNK_WRITE(V2) {
  int BV_ON = param.asInt();
  if (BV_ON == 0) {
    pins_ext.digitalWrite(P2, OFF);
  }
  if (BV_ON == 1) {
    pins_ext.digitalWrite(P2, ON);
    delay(2000);
    pins_ext.digitalWrite(P2, ON);
  }
}


//Выключение БВ
BLYNK_WRITE(V3) {
  int BV_OFF = param.asInt();
  if (BV_OFF == 0) {
    pins_ext.digitalWrite(P3, OFF);
  }
  if (BV_OFF == 1) {
    pins_ext.digitalWrite(P3, ON);
    pins_ext.digitalWrite(P13, ON);
    pins_ext.digitalWrite(P14, ON);
    delay(8000);
    pins_ext.digitalWrite(P3, OFF);
  }
}

//Вентиляторы
BLYNK_WRITE(V4) {
  int MV = param.asInt();
  if (MV == 0) {
    pins_ext.digitalWrite(P4, OFF);
  }
  if (MV == 1) {
    pins_ext.digitalWrite(P4, ON);
  }
}

//Компрессоры
BLYNK_WRITE(V5) {
  int MK = param.asInt();
  if (MK == 0) {
    pins_ext.digitalWrite(P5, OFF);
  }
  if (MK == 1) {
    pins_ext.digitalWrite(P5, ON);
  }
}

//Свисток
BLYNK_WRITE(V6) {
  int svistok = param.asInt();
  if (svistok == 0) {
    pins_ext.digitalWrite(P6, OFF);
  }
  if (svistok == 1) {
    pins_ext.digitalWrite(P6, ON);
    delay(150);
  }
}
//Сброс -

BLYNK_WRITE(V10) {
  int position_down = param.asInt();
  if (position_down == 0) {
    pins_ext.digitalWrite(P9, OFF);
  }
  if (position_down == 1 && switch_304 == 0) {
    pins_ext.digitalWrite(P9, ON);
    delay(4000);
    counter = 0;
  }
}

//Набор +1

// BLYNK_WRITE(V11) {
//   bool w = true;
//   int position_up_plus1 = param.asInt();


//   if (position_up_plus1 == 1 && switch_304 == 0) {
//     inPossition = false;

//     if (counter < 3) {
//       relayON(10);
//       relayON(11);
//       relayON(12);
//       delay(1000);

//     }
//   } else {
//     relayOFF(10);
//     relayOFF(11);
//     relayOFF(12);

//   }

// }


BLYNK_WRITE(V11) {
  int position_up_plus1 = param.asInt();

  if (position_up_plus1 == 0) {
    relayOFF(10);
    relayOFF(11);
    relayOFF(12);
    delay(1500);
  }
  if (position_up_plus1 == 1 && switch_304 == 0 && counter < 2) {
    relayON(10);
    relayON(11);
    relayON(12);
    inPossition = false;
    delay(1000);
    relayON(15);
    Serial.println(digitalRead(D7));
    if(digitalRead(D7) == 1){
      counter++;
    }
    relayOFF(15);
  }
}





//Реверсивка
BLYNK_WRITE(V7) {
  int revers = param.asInt();

  if (revers == 0) {
    if (inPossition) {  
      switch_304 = 0;  // 304 Off
      pins_ext.digitalWrite(P7, ON);
      pins_ext.digitalWrite(P8, OFF);
    }
  }
  if (revers == 1) {
    switch_304 = 1; // 304 On
    inPossition = true;
    relayON(P9);
    delay(2000);
    relayOFF(P9);
    relayOFF(P7);
    relayOFF(P8);
    counter = 0;
  }
  if (revers == 2) {
    if (inPossition) {
      switch_304 = 0; // 304 Off
      pins_ext.digitalWrite(P8, ON);
      pins_ext.digitalWrite(P7, OFF);
    }
  }
}


//Тормоз отпустить
BLYNK_WRITE(V8) {
  int brake_off = param.asInt();
  if (brake_off == 0) {
    pins_ext.digitalWrite(P14, OFF);
  }
  if (brake_off == 1) {
    relayON(P14);
    relayOFF(P13);
  }
}

//Тормоз применить
BLYNK_WRITE(V9) {
  int brake_on = param.asInt();
  if (brake_on == 0) {
    relayOFF(P13);  //13
  }
  if (brake_on == 1) {
    relayON(P13);
    relayON(P14);
  }
}

// прибор




void relayON(int blinkVar) {
  pins_ext.digitalWrite(blinkVar, ON);
}
void relayOFF(int blinkVar) {
  pins_ext.digitalWrite(blinkVar, OFF);
}





void setup() {

  Serial.begin(115200);

  //Назначаем режим порта
  pins_ext.pinMode(P0, OUTPUT);
  pins_ext.pinMode(P1, OUTPUT);
  pins_ext.pinMode(P2, OUTPUT);
  pins_ext.pinMode(P3, OUTPUT);
  pins_ext.pinMode(P4, OUTPUT);
  pins_ext.pinMode(P5, OUTPUT);
  pins_ext.pinMode(P6, OUTPUT);
  pins_ext.pinMode(P7, OUTPUT);
  pins_ext.pinMode(P8, OUTPUT);
  pins_ext.pinMode(P9, OUTPUT);
  pins_ext.pinMode(P10, OUTPUT);
  pins_ext.pinMode(P11, OUTPUT);
  pins_ext.pinMode(P12, OUTPUT);
  pins_ext.pinMode(P13, OUTPUT);
  pins_ext.pinMode(P14, OUTPUT);
  pins_ext.pinMode(P15, OUTPUT);
  pinMode(D7, INPUT_PULLUP);
  //Указываем состояние по умолчанию
  pins_ext.digitalWrite(P0, OFF);
  pins_ext.digitalWrite(P1, OFF);
  pins_ext.digitalWrite(P2, OFF);
  pins_ext.digitalWrite(P3, OFF);
  pins_ext.digitalWrite(P4, OFF);
  pins_ext.digitalWrite(P5, OFF);
  pins_ext.digitalWrite(P6, OFF);
  pins_ext.digitalWrite(P7, OFF);
  pins_ext.digitalWrite(P8, OFF);
  pins_ext.digitalWrite(P9, OFF);
  pins_ext.digitalWrite(P10, OFF);
  pins_ext.digitalWrite(P11, OFF);
  pins_ext.digitalWrite(P12, OFF);
  pins_ext.digitalWrite(P13, OFF);
  pins_ext.digitalWrite(P14, OFF);
  pins_ext.digitalWrite(P15, OFF);


  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  pins_ext.begin();
}

void loop() {
  //Запуск блинк
  Blynk.run();
}