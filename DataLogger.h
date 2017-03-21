#include <HardwareSerial.h>
#include <NMEAGPS.h>
#include <N2kMsg.h>

void log_data(Stream &serial, gps_fix &fix, const tN2kMsg &N2kMsg);

