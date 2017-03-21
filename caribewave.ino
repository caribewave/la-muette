#define N2k_SPI_CS_PIN 10

#include <Arduino.h>
#include <NMEAGPS.h>
#include <SoftwareSerial.h>
#include "GPSport.h"
#include <SPI.h>
#include <SD.h>
#include <NMEA2000_CAN.h>  // This will automatically choose right CAN library and create suitable NMEA2000 object
#include <N2kMsg.h>
#include <NMEA2000.h>
#include <N2kMessages.h>


static NMEAGPS  gps;
static gps_fix  gps_data;

File LOG;

void getWaterDepth(const tN2kMsg &N2kMsg) {
    unsigned char SID;
    double DepthBelowTransducer;
    double Offset;

    if (ParseN2kWaterDepth(N2kMsg,SID,DepthBelowTransducer,Offset) ) {
      return DepthBelowTransducer+Offset;
    }
}

bool FAIL;

void set_fail() {
  FAIL = true;
  while (FAIL) {
    digitalWrite(8, HIGH);
    delay(500);
    digitalWrite(8, LOW);
    delay(500);
  }
}

void set_ok() {
  FAIL = false;
  if (gps.available(gps_port)) {
    digitalWrite(8, HIGH);
  }
}

void setup() {
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);

  if (!SD.begin(9)) {
    set_fail();
    return;
  }
  File LOG = SD.open("log.txt", FILE_WRITE);
  gps_port.begin(9600);
  NMEA2000.SetMsgHandler(HandleNMEA2000Msg);
  NMEA2000.Open();
  LOG.println("Initialization done.");
  LOG.flush();
  set_ok();
}

void HandleNMEA2000Msg(const tN2kMsg &N2kMsg) {
  if (!gps.available(gps_port)) {
    return;
  }
    File myFile = SD.open("data.txt", FILE_WRITE);
    if (myFile) {
      digitalWrite(8, HIGH);
      gps_data = gps.read();
      myFile.print(gps_data.dateTime_cs);
      myFile.print(',');
      myFile.print(gps_data.latitude());
      myFile.print(',');
      myFile.print(gps_data.longitude());
      myFile.print('\n');
      myFile.close();
    } else {
      digitalWrite(7, HIGH);
    }
}

void loop() {
  NMEA2000.ParseMessages();
}
