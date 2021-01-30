//select either one of ACCELEROMETER_OR_GYROSCOPE for sensing movements along the axes
#define ACCELEROMETER_OR_GYROSCOPE 0   //0 for Both, 1 for only ACCELEROMETER, 2 for only GYROSCOPE
#define ACCEL_GYRO_PREFER_WEIGHT 75  //when using both, how much preference should be given to ACCELEROMETER or Gyro, set between 0 to 100, where 50 being equal, 100 means high preference for accel, 0 means high preference for gyro
#define RIGHT_LEFT_SWITCH 0     //if servo righ-left direction(x-axis) is switched, change it to 1
#define FWD_BWD_SWITCH 0     //if servo forward-backward direction(z-axis) is switched, change it to 1
#define SERVO_LAG_DELAY 0 //time delay given for rocket to adjust to new postions before taking new reading from IMU sensor. In milliseconds



#include "libs\\MPU6050\\MPU6050.h"
#include "libs\\MPU6050\\MPU6050.cpp"
#include "Servo.h"


// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

int16_t ax, ay, az;
int16_t gx, gy, gz;
#define AXIS_SERVO_COUNT 2
Servo servo[AXIS_SERVO_COUNT];
const int servo_pins[AXIS_SERVO_COUNT] = {8, 9};  //x-axis, z-axis servo pins


//#define OUTPUT_READABLE_ACCELGYRO
#define OUTPUT_BINARY_ACCELGYRO
#define ACCEL_PREFER_WEIGHT ACCEL_GYRO_PREFER_WEIGHT*1
#define GYRP_PREFER_WEIGHT (100-ACCEL_GYRO_PREFER_WEIGHT)

#define LED_PIN 13
int gyro_val[2];
int accel_val[2];
int servo_now[2];

int last_gyro_val[2];
int last_accel_val[2];

const float radToDeg = 180.0 / 3.1415926535897932384626433832795;
float yScaleOffset = 1;

void setup()
{
  // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  pinMode(LED_PIN, OUTPUT);
  for (int i = 0; i < AXIS_SERVO_COUNT; i++)
  {
    servo[i].attach(servo_pins[i]);
    servo_now[i] = 90;
    servo[i].write(servo_now[i]);
    gyro_val[i] = 0;
    accel_val[i] = 0;
    last_gyro_val[i] = 0;
    last_accel_val[i] = 0;
  }

  // initialize serial communication
  // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
  // it's really up to you depending on your project)
  Serial.begin(38400);

  Serial.println(F("Servos at 90'"));
  // initialize device
  Serial.println(F("Initializing I2C devices..."));
  accelgyro.initialize();

  // verify connection
  Serial.println(F("Testing device connections..."));
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  Serial.print(F("ACCEL_OR_GYRO:\t"));
  Serial.println(ACCELEROMETER_OR_GYROSCOPE);
  Serial.print(F("PREFER_WEIGHT:\t"));
  Serial.println(ACCEL_GYRO_PREFER_WEIGHT);
  Serial.println(F("Boot done"));
}



void loop()
{
  // read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

#if RIGHT_LEFT_SWITCH
  ax = -1 * ax;
  gx = -1 * gx;
#endif
#if FWD_BWD_SWITCH
  az = -1 * az;
  gz = -1 * gz;
#endif
  //adjust acording to gyro
#if (ACCELEROMETER_OR_GYROSCOPE!=1)
  gyro_val[0] = map(gx, -28000, 28000, 0, 180);   //rotation along x-axis. i.e Lean sideways, left right
  gyro_val[1] = map(gz, -28000, 28000, 0, 180);   //rotation along z-axis. i.e. Lean back and fwd

  //rotation along y-axis is rotation along the cylindrical axis. Its not controlled.

  for (int i = 0; i < AXIS_SERVO_COUNT; i++)
  {
    if (last_gyro_val[i] != gyro_val[i])
    {
      servo_now[i] = gyro_val[i];
      last_gyro_val[i] = gyro_val[i];
    }
  }
#endif


  //adjust acording to acceleration
#if (ACCELEROMETER_OR_GYROSCOPE!=2)

  accel_val[0] = map(atan((float)ax / (ay * yScaleOffset)) * 1000, -1571, 1571, 0, 180); // calculate for X axis, 1571 is (Pi/2)*1000
  accel_val[1] = map(atan((float)az / (ay * yScaleOffset)) * 1000, -1571, 1571, 0, 180); // calculate for Z axis


  for (int i = 0; i < AXIS_SERVO_COUNT; i++)
  {
    if (last_accel_val[i] != accel_val[i])
    {
      servo_now[i] = accel_val[i];
      last_accel_val[i] = accel_val[i];
    }
  }
#endif

  for (int i = 0; i < AXIS_SERVO_COUNT; i++)
  {
#if(ACCELEROMETER_OR_GYROSCOPE==0)
    servo_now[i] = (((long)accel_val[i] * ACCEL_PREFER_WEIGHT) + ((long)gyro_val[i] * GYRP_PREFER_WEIGHT)) / 100;
#endif
    servo[i].write(servo_now[i]);
  }

  delay(SERVO_LAG_DELAY); //give time to adjust servos

}
