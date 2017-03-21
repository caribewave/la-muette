#include <N2kMessages.h>

double parse_depth(const tN2kMsg &N2kMsg) 
{
    unsigned char SID;
    double DepthBelowTransducer;
    double Offset;
    if (ParseN2kWaterDepth(N2kMsg,SID,DepthBelowTransducer,Offset) ) 
    {
      return DepthBelowTransducer + Offset;
    }
    return -1.0;
}

double parse_speed(const tN2kMsg &N2kMsg) 
{
    unsigned char SID;
    double WaterReferenced;
    double GroundReferenced;
    tN2kSpeedWaterReferenceType SWRT;
    if (ParseN2kBoatSpeed(N2kMsg,SID,WaterReferenced,GroundReferenced,SWRT) ) 
    {
      return WaterReferenced;
    }
    return -1.0;
}

double parse_distance(const tN2kMsg &N2kMsg) 
{
    uint16_t DaysSince1970;
    double SecondsSinceMidnight;
    uint32_t Log;
    uint32_t TripLog;
    if (ParseN2kDistanceLog(N2kMsg,DaysSince1970,SecondsSinceMidnight,Log,TripLog) ) 
    {
      return TripLog;
    }
    return -1.0;
}


double parse_temp(const tN2kMsg &N2kMsg) 
{
    unsigned char SID;
    unsigned char TempInstance;
    tN2kTempSource TempSource;
    double ActualTemperature;
    double SetTemperature;
    if (ParseN2kTemperatureExt(N2kMsg,SID,TempInstance,TempSource,ActualTemperature,SetTemperature)) 
    {
      return KelvinToC(ActualTemperature);
    }
    return -1.0;
}
