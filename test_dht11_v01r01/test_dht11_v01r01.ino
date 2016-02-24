
/* ---------------------------------------------------------
Package     :  wifs v.1
Module      :  test dht11 sensor
Copyright   :  (c) 2016 by Antonio Curci 

Copyright 2014-2016 by Antonio Curci. All rights reserved.
Redistribution and use in source and binary forms,
with or without modification, are not permitted.
-------------------------------------------------------- */

#define pr(a,b) {Serial.print(a);Serial.print(b);}


void setup() {
  dhtsetup();
  Serial.begin(9600);
  pr("\r\nTester dht11 sensor v.01.01 \n(C) Copyright 2014-2016 by Curci\r\n","\nCommands: none\n"); 
} 

void loop() {
  static unsigned long n=1;
  
  dhtin();  
  if (dhterr()) {pr("\n",n++);pr("  error=",dhterr())}
  else          {pr("\n",n++);pr("  T=",dhtint());pr(" H=",dhtinh());}
  delay(1000);
}

