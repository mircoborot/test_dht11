/* ---------------------------------------------------------
Package     :  wsn v.1
Module      :  dht11
Copyright   :  (c) 2016 by Antonio Curci 

Copyright 2014-2016 by Antonio Curci. All rights reserved.
Redistribution and use in source and binary forms,
with or without modification, are not permitted.
-------------------------------------------------------- */

// ******************** WARNINGS
/*
 Uso la libreria idDHT11 perchè gestisce la lettura con interrupt.
 Con la vecchia libreria HDT11 si blocca nel giro di due minuti.
 Utilizzando il solo sensore (senza il modulo contenente la resistenza di 10k)
 si può omettere la resistenza di 10k modificando il modulo idDHT11.cpp
 della libreria idDHT11 ne seguente modo:
 		//pinMode(pin, INPUT);
		pinMode(pin, INPUT_PULLUP); // PER EVITARE RESISTORE 10K

*/
// ******************** LIBRARIES
#include <idDHT11.h>

// ******************** DEFINES

// ******************** CONSTANTS
// PIN
// Collegare il ricevitore al pin di (interrupt) 2/3
const byte PIN_DHT = 2;   
const byte INT2 = 0; 

// Range for temp events
// ]0,TMIN] --> cold   [TMAX,50] --> hot
const byte 
  TMIN = 8,  
  TMAX = 18;

// Range for hum events
// [20,HMIN] --> wet   [HMAX,90] --> dry
const byte 
  HMIN = 40,  
  HMAX = 60;

// ******************** VARIABLES
boolean den;    // enabled
byte 
  hint,  // parte intera di H  
  tint,  // parte intera di T
  err;   // reading error

/* codici di errore
IDDHTLIB_ERROR_CHECKSUM		1
IDDHTLIB_ERROR_ISR_TIMEOUT	2
IDDHTLIB_ERROR_RESPONSE_TIMEOUT	3
IDDHTLIB_ERROR_DATA_TIMEOUT	4
IDDHTLIB_ERROR_ACQUIRING	5
IDDHTLIB_ERROR_DELTA		6
IDDHTLIB_ERROR_NOTSTARTED	7
*/  

//int idDHT11pin = PIN_DHT; //Digital pin for comunications
//int idDHT11intNumber = 0; //interrupt number (must be the one that use the previus defined pin (see table above)

//declaration
void dht11_wrapper(); // must be declared before the lib initialization

// Lib instantiate
idDHT11 DHT11(PIN_DHT,INT2,dht11_wrapper); // interrupt#: 0 se pin=2, 1 se pin=3

// ******************** FUNCTIONS

// This wrapper is in charge of calling must be defined like this for the lib work
void dht11_wrapper() {DHT11.isrCallback();}

void dhtsetup() {dhten(true);}
void dhten(boolean f) {den=f;}  
boolean dhtena()   {return den;}  

byte dhterr() {return err;}

// gradi °C [0,50]
int dhtint() {return (!den || err) ? 0 : tint;}

// % RH [20,90]
int dhtinh() {return (!den || err) ? 0 : hint;}

// r: reference
boolean dhthot(unsigned long r) {
  if (!r) r=TMAX; 
  return (!den || err) ? false : tint>=r;
} 

// r: reference
boolean dhtcold(unsigned long r) {
  if (!r) r=TMIN; 
  return (!den || err) ? false : tint<=r;
}

// r: reference
boolean dhtdry(unsigned long r) {
  if (!r) r=HMAX;
  return (!den || err) ? false : hint>=r;
}

// r: reference
boolean dhtwet(unsigned long r) {
  if (!r) r=HMIN; 
  return (!den || err) ? false : hint<=r;
}

void dhtin() {
  DHT11.acquire();
  while (DHT11.acquiring());
  err=(byte)-DHT11.getStatus();  // status è negativo se errore, 0 se ok
  if (!err) {
    hint=(byte)DHT11.getHumidity();
    tint=(byte)DHT11.getCelsius();
  }
}
