## Table of Contents
1. MPU6050 & HMC5883L (Understanding Auxiliary I2C)
3. MS5611

## 1. MPU6050 & HMC5883L
MPU6050 Sensor is fairly simple to use, however it is quite difficult to use it together with HMC5883L. Without understanding the auxiliary I2C, you will not be able to use HMC5883L at all. </br>

The HMC5883L sensor is not directly connected to SDA and SCL lines going through external pins. HMC5883L sensor is connected to the MPU6050 through auxiliary lines, which is  physically inaccessible through external pins on the board. In order to properly set up the HMC5883L, the MPU6050 must be in bypass mode so that the micro controller can simply bypass MPU6050 and directly access to the HMC5883L. Only then the microcontroller can write registers to HMC5883L sensor. Once initialization of HMC5883L is complete, the MPU6050 should be brought back to the master mode and it should read the data from HMC5883L. The data sent by the HMC5883L will be read by the MPU6050, then it will be saved to the MPU6050's internal register address. When the user requires to read the data, the user can retrieve 9 axis values through reading the registers from MPU6050. </br>

Summary of HMC5883L </br>
1. Set MPU6050 to bypass mode (it is set to master mode by default, therefore needs to be changed) </br>
2. Directly access HMC5883L for initial setup </br>
3. Set MPU6050 to back to master mode </br>
4. Let MPU6050 to read HMC5883L through auxiliary I2C connection </br>
5. Read 9 axis values from MPU6050 </br>
