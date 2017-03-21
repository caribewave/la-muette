#include <HardwareSerial.h>
#include <NMEAGPS.h>
#include <N2kMsg.h>
#include "Parser.h"

void log_data(Stream &serial, gps_fix &fix, const tN2kMsg &N2kMsg)
{
  serial.print(fix.dateTime);
  serial.print(',');
  serial.print(fix.latitude(), 8);
  serial.print(',');
  serial.print(fix.longitude(), 8);
  serial.print(',');
  serial.print(N2kMsg.PGN);
  serial.print(',');
  switch (N2kMsg.PGN) {
    case 128267:
      serial.print("depth,");
      serial.print(parse_depth(N2kMsg));
      break;
    case 128259:
      serial.print("speed,");
      serial.print(parse_speed(N2kMsg));
      break;
    case 128275:
      serial.print("distance,");
      serial.print(parse_distance(N2kMsg));
      break;
    case 130316:
      serial.print("temperature,");
      serial.print(parse_temp(N2kMsg));
      break;
    default:
      serial.print("unknown,");
      serial.print("unknown");
      break;
  }
  serial.print('\n');
}
