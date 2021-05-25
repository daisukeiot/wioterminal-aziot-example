#include <azure/core/az_json.h>
#include <azure/core/az_result.h>

#define TELEMETRY_GPS_LOCATION    "gps"
#define TELEMETRY_GPS_TYPE        "type"
#define TELEMETRY_GPS_POINT       "point"
#define TELEMETRY_GPS_COORDINATE  "coordinates"

typedef struct {
    double lat = 0.0;
    double lng = 0.0;
} GPS_DATA;

void InitGps();
void GetGpsData(GPS_DATA* gps_data);
az_result Append_Gps_Telemetry(az_json_writer *json_builder, GPS_DATA* gps_data);