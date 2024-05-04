#include <GY_85.h>
#include <Wire.h>

GY_85 GY85;     //create the object
//float pitch;
//float yaw;
float roll;
//float mag_x;
//float mag_y;
float cos_roll;
float sin_roll;
//float cos_pitch;
//float sin_pitch;

#define TO_DEG(x) (x * 57.2957795131)  // *180/pi
  
void setup()
{
    Serial.begin(9600);
    delay(6000);
    Wire.begin();
//    delay(10);
    delay(10);
    GY85.init();
//    delay(10);
}


void loop()
{
    int ax = GY85.accelerometer_x( GY85.readFromAccelerometer() );
//    int ay = GY85.accelerometer_y( GY85.readFromAccelerometer() );
    int az = GY85.accelerometer_z( GY85.readFromAccelerometer() );
    
//    int cx = GY85.compass_x( GY85.readFromCompass() );
//    int cy = GY85.compass_y( GY85.readFromCompass() );
//    int cz = GY85.compass_z( GY85.readFromCompass() );

//    float gx = GY85.gyro_x( GY85.readGyro() );
//    float gy = GY85.gyro_y( GY85.readGyro() );
//    float gz = GY85.gyro_z( GY85.readGyro() );
//    float gt = GY85.temp  ( GY85.readGyro() );

    roll= atan2(-ax,az);
//    pitch= -atan2(ay, sqrt(ax * ax + az * az));

//    cos_roll = cos(roll);
//    sin_roll = sin(roll);
//    cos_pitch = cos(pitch);
//    sin_pitch = sin(pitch);
//    mag_x = cx* cos_pitch + cy * sin_roll * sin_pitch + cz * cos_roll * sin_pitch;
//    mag_y =cy * cos_roll - cz * sin_roll;
//    yaw = atan2(-mag_y, mag_x);
    
//    Serial.print  ( "," );
//    Serial.print  ( TO_DEG(yaw) );
//    Serial.print  ( "," );
//    Serial.print  ( TO_DEG(pitch) );
//    Serial.print  ( "," );
      Serial.println  ( TO_DEG(roll) );
  
    if (TO_DEG(roll) >= -90 && TO_DEG(roll) <= 180){
        Serial.println(TO_DEG(roll) - 90);
    }

    if (TO_DEG(roll) > -180 && TO_DEG(roll) < -90){
        Serial.println(TO_DEG(roll) + 270);
    }            
}
