
// Alias for debug_port bound to Serial
#define debug_port Serial
// Alias for gps_port bound to Serial1
#define gps_port Serial1 // Works only on MEGA 2560
// N2k pin
#define N2k_SPI_CS_PIN 10 // Works only on MEGA 2560
// SDCard pin
#define SDCARD_PIN 9 // Works only on MEGA 2560

#include <Arduino.h>
#include <NMEAGPS.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include <SD.h>
// This will automatically choose right CAN library and create suitable NMEA2000 object
#include <NMEA2000_CAN.h>
#include <N2kMsg.h>
#include <NMEA2000.h>
#include <N2kMessages.h>
#include "Parser.h"
#include "DataLogger.h"


// GPS related variables
static NMEAGPS  gps;
static gps_fix  fix;
static String log_file;

/**
 * Setup function
 */
void setup() 
{
  debug_port.begin(9600);
  debug_port.println("Initializing La-Muette");
  // Init debug LEDS
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  debug_port.println("LEDs initialized");
  // Init SD
  if (!SD.begin(SDCARD_PIN)) {
    debug_port.println("Failed to initialize SD Card");
    return;
  }
  // Init log file
  log_file = "data.log";
  File LOG = SD.open(log_file, FILE_WRITE);
  debug_port.println("SD Card initialized with file " + log_file);
  // Init GPS
  gps_port.begin(9600);
  debug_port.println("GPS initialized");
  NMEA2000.SetMsgHandler(handle_n2k_msg);
  NMEA2000.Open();
  debug_port.println("CAN Bus initialized");
  debug_port.println("Initialization done.");
}

/**
 * Main Event Loop
 */
void loop() 
{
  handle_gps_msg();
  NMEA2000.ParseMessages();
}

/**
 * N2K Message handler. Is handled automatically by NMEA2000 API
 */
void handle_n2k_msg(const tN2kMsg &N2kMsg) 
{
  if (N2kMsg.PGN == 130311)
  {
    return;
  }
  debug_port.println("CAN Data received:");
  File myFile = SD.open(log_file, FILE_WRITE);
  if (myFile) {
    log_data(myFile, fix, N2kMsg);
    myFile.close();
    log_data(debug_port, fix, N2kMsg);
  }
}

/**
 * GPS Handler. Is called directly from main event loop
 */
void handle_gps_msg() 
{
  if (!gps.available(gps_port)) 
  {
    return;
  }
  debug_port.println("GPS Fix available");
  fix = gps.read();
}

