#include <Arduino.h>
#include <TinyGPS++.h>
#include <wiring_private.h>
#include "Gps.h"

static const uint32_t GPSBaud = 9600;
static Uart GpsOnSerial(&sercom3, PIN_WIRE_SCL, PIN_WIRE_SDA, SERCOM_RX_PAD_1, UART_TX_PAD_0);
TinyGPSPlus gps;
TinyGPSCustom ggaLat(gps, "GPGGA", 2);
TinyGPSCustom ggaLng(gps, "GPGGA", 4);
static double lastLat = 0.0;
static double lastLng = 0.0;

#define AZ_RETURN_IF_FAILED(exp)         \
    do                                   \
    {                                    \
        az_result const _result = (exp); \
        if (az_result_failed(_result))   \
        {                                \
            return _result;              \
        }                                \
    } while (0)

void GetGpsData(GPS_DATA* gps_data)
{
    gps_data->lat = 0.0;
    gps_data->lng = 0.0;

    while (GpsOnSerial.available() > 0)
    {
        unsigned char cc = GpsOnSerial.read();
        if (gps.encode(cc))
        {
            if (ggaLng.isValid() && ggaLng.isUpdated())
            {
                ggaLng.value();
                gps_data->lng = gps.location.lng();
                gps_data->lat = gps.location.lat();

                if (gps_data->lng != lastLng)
                {
                    lastLng = gps_data->lng;
                }
                else
                {
                    //gps_data->lng = 0.0;
                }

                if (gps_data->lat != lastLat)
                {
                    lastLat = gps_data->lat;
                }
                else
                {
                    //gps_data->lat = 0.0;
                }
            }
        }
    }
}

az_result Append_Gps_Telemetry(az_json_writer *json_builder, GPS_DATA* gps_data)
{
    az_result rc = AZ_OK;

    if (gps_data->lat != 0.0)
    {
        AZ_RETURN_IF_FAILED(az_json_writer_append_property_name(json_builder, AZ_SPAN_LITERAL_FROM_STR(TELEMETRY_GPS_LOCATION)));
        AZ_RETURN_IF_FAILED(az_json_writer_append_begin_object(json_builder));
        AZ_RETURN_IF_FAILED(az_json_writer_append_property_name(json_builder, AZ_SPAN_LITERAL_FROM_STR(TELEMETRY_GPS_TYPE)));
        AZ_RETURN_IF_FAILED(az_json_writer_append_string(json_builder, AZ_SPAN_LITERAL_FROM_STR(TELEMETRY_GPS_POINT)));
        AZ_RETURN_IF_FAILED(az_json_writer_append_property_name(json_builder, AZ_SPAN_LITERAL_FROM_STR(TELEMETRY_GPS_COORDINATE)));
        AZ_RETURN_IF_FAILED(az_json_writer_append_begin_array(json_builder));
        AZ_RETURN_IF_FAILED(az_json_writer_append_double(json_builder, gps_data->lng, 6));
        AZ_RETURN_IF_FAILED(az_json_writer_append_double(json_builder, gps_data->lat, 6));
        AZ_RETURN_IF_FAILED(az_json_writer_append_end_array(json_builder));
        AZ_RETURN_IF_FAILED(az_json_writer_append_end_object(json_builder));
    }

    return rc;
}

////////////////////////////////////////////////////////////////////////////////
// For GPS connected to left side Grove connector
void SERCOM3_0_Handler()
{
  GpsOnSerial.IrqHandler();
}
void SERCOM3_1_Handler()
{
  GpsOnSerial.IrqHandler();
}
void SERCOM3_2_Handler()
{
  GpsOnSerial.IrqHandler();
}
void SERCOM3_3_Handler()
{
  GpsOnSerial.IrqHandler();
}

void InitGps()
{
    GpsOnSerial.begin(GPSBaud);
    pinPeripheral(PIN_WIRE_SCL, PIO_SERCOM_ALT);
    pinPeripheral(PIN_WIRE_SCL, PIO_SERCOM_ALT);
}

