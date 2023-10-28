
#include "I2Cdev.h"
#include "MPU6050.h"
#include "HMC5883L.h"


#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif


MPU6050 gy86;
HMC5883L mag;
void read_mag_gy86 (int16_t *mx,int16_t *my,int16_t *mz);

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

#define OUTPUT_READABLE_ACCELGYRO

#define LED_PIN 2

bool blinkState = false;

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    Serial.begin(115200);
    Serial.println("Initializing I2C devices...");
    gy86.initialize();
    gy86.setI2CMasterModeEnabled(false);
    gy86.setI2CBypassEnabled(true);
    mag.initialize();
    gy86.setI2CMasterModeEnabled(true);
    gy86.setSlaveAddress(0,HMC5883L_ADDRESS | 0x80);
    gy86.setSlaveRegister(0,HMC5883L_RA_DATAX_H);
    gy86.setSlaveEnabled(0,true);
    gy86.setSlaveDataLength(0,6);
    
    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(gy86.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    // configure Arduino LED pin for output
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // read raw accel/gyro measurements from device
    gy86.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    read_mag_gy86(&mx,&my,&mz);
    #ifdef OUTPUT_READABLE_ACCELGYRO
        // display tab-separated accel/gyro x/y/z values
        Serial.print("a/g:\t");
        Serial.print(ax); Serial.print("\t");
        Serial.print(ay); Serial.print("\t");
        Serial.print(az); Serial.print("\t");
        Serial.print(gx); Serial.print("\t");
        Serial.print(gy); Serial.print("\t");
        Serial.print(gz); Serial.print("\t");
        Serial.print(mx); Serial.print("\t");
        Serial.print(my); Serial.print("\t");
        // Serial.print(mz);
        Serial.println(mz);
    #endif

    #ifdef OUTPUT_BINARY_ACCELGYRO
        Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
        Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
        Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
        Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
        Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
        Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
    #endif

    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
    delay(100);
}
void read_mag_gy86 (int16_t *mx,int16_t *my,int16_t *mz){
    *mx=gy86.getExternalSensorWord(0);
    *mz=gy86.getExternalSensorWord(2);
    *my=gy86.getExternalSensorWord(4);
}