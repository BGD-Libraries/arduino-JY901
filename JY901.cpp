#include "JY901.h"
#include "JY901_dfs.h"
#include <string.h>
#include <Wire.h>

CJY901::CJY901()
{
	address_ = 0x50;
	transferMode_ = 0;
}

void CJY901::attach(Stream & Serial_temp)
{
	Serial_ = &Serial_temp;
}

void CJY901::startIIC(uint8_t address)
{
	address_ = address;
	transferMode_ = 1;
	Wire.begin();
}

bool CJY901::copeSerialData(uint8_t data)
{
	rxBuffer[rxCnt] = data;
	rxCnt++;
	if (rxBuffer[0] != 0x55) {
		rxCnt = 0;
		return 0;
	}
	if (rxCnt<11) {
		return 0;
	}
	uint8_t sum = 0;
	for (uint8_t cnt = 0; cnt<10; cnt++) {
		sum += rxBuffer[cnt];
	}
	while (sum>255) {
		sum -= 256;
	}
	if (sum != rxBuffer[10]) {
		rxCnt = 0;
		return 0;
	}
	switch (rxBuffer[1])
	{
		case 0x50:  memcpy(&JY901_data.time,    &rxBuffer[2], 8); break;    // 时间
		case 0x51:  memcpy(&JY901_data.acc,     &rxBuffer[2], 8); break;    //加速度
		case 0x52:  memcpy(&JY901_data.gyro,    &rxBuffer[2], 8); break;    //角速度
		case 0x53:  memcpy(&JY901_data.angle,   &rxBuffer[2], 8); break;    //角度
		case 0x54:  memcpy(&JY901_data.mag,     &rxBuffer[2], 8); break;    //磁场
		case 0x55:  memcpy(&JY901_data.dStatus, &rxBuffer[2], 8); break;    //端口状态
		case 0x56:  memcpy(&JY901_data.pressure,&rxBuffer[2], 4);           //气压
		            memcpy(&JY901_data.altitude,&rxBuffer[6], 4);           //高度
		            break;
		case 0x57:  memcpy(&JY901_data.lon,     &rxBuffer[2], 4);           //经度
		            memcpy(&JY901_data.lat,     &rxBuffer[6], 4);           //纬度
		            break;
		case 0x58:  memcpy(&JY901_data.GPSHeight,   &rxBuffer[2], 2);       //地速
		            memcpy(&JY901_data.GPSYaw,      &rxBuffer[4], 2);
		            memcpy(&JY901_data.GPSVelocity, &rxBuffer[6], 4);
		            break;
	}
	rxCnt = 0;
	lastTime = millis();
	return 1;
}

bool CJY901::receiveSerialData(void)
{
	bool status = false;
	if (Serial_->available()) {
		do {
			if (Serial_->peek() != 0x55) {  //第一个字节是否为0x55,如果不是丢弃
				Serial_->read();
			} else {
				if (Serial_->available() >= 11) {  //可接收数据是否达到要求
					Serial_->readBytes(rxBuffer, 11); //读取数据
					uint8_t sum = 0;
					for (uint8_t cnt = 0; cnt<10; cnt++) {
						sum += rxBuffer[cnt];
					}
					if (sum == rxBuffer[10]) {
						switch (rxBuffer[1])
						{
							case 0x50:  memcpy(&JY901_data.time,    &rxBuffer[2], 8); break;    // 时间
							case 0x51:  memcpy(&JY901_data.acc,     &rxBuffer[2], 8); break;    //加速度
							case 0x52:  memcpy(&JY901_data.gyro,    &rxBuffer[2], 8); break;    //角速度
							case 0x53:  memcpy(&JY901_data.angle,   &rxBuffer[2], 8); break;    //角度
							case 0x54:  memcpy(&JY901_data.mag,     &rxBuffer[2], 8); break;    //磁场
							case 0x55:  memcpy(&JY901_data.dStatus, &rxBuffer[2], 8); break;    //端口状态
							case 0x56:  memcpy(&JY901_data.pressure,&rxBuffer[2], 4);           //气压
							            memcpy(&JY901_data.altitude,&rxBuffer[6], 4);           //高度
							            break;
							case 0x57:  memcpy(&JY901_data.lon,     &rxBuffer[2], 4);           //经度
							            memcpy(&JY901_data.lat,     &rxBuffer[6], 4);           //纬度
							            break;
							case 0x58:  memcpy(&JY901_data.GPSHeight,   &rxBuffer[2], 2);       //地速
							            memcpy(&JY901_data.GPSYaw,      &rxBuffer[4], 2);
							            memcpy(&JY901_data.GPSVelocity, &rxBuffer[6], 4);
							            break;
						}
						status = true;
						lastTime = millis();
					}
				}
			}
		}while(Serial_->available() >= 11);
	}
	return status;
}

void CJY901::readData(uint8_t address, uint8_t length, int8_t data[])
{
	readRegisters(address_, address, length, data);
}

uint16_t CJY901::getTime(const char* str)
{
	if (transferMode_)
		readRegisters(address_, JY_YYMM, 8, (int8_t*)&JY901_data.time);

	if (strcmp(str, "year") == 0)       //年
		return JY901_data.time.year;

	if (strcmp(str, "month") == 0)      //月
		return JY901_data.time.month;

	if (strcmp(str, "day") == 0)        //日
		return JY901_data.time.day;

	if (strcmp(str, "hour") == 0)       //时
		return JY901_data.time.hour;

	if (strcmp(str, "minute") == 0)     //分
		return JY901_data.time.minute;

	if (strcmp(str, "second") == 0)     //秒
		return JY901_data.time.second;

	if (strcmp(str, "milisecond") == 0) //毫秒
		return JY901_data.time.milisecond;
}

double CJY901::getAccX()
{
	if (transferMode_)
		readRegisters(address_, JY_AX, 2, (int8_t *)&JY901_data.acc.x);
	return JY901_data.acc.x / (32768.0/16.0);
}

double CJY901::getAccY()
{
	if (transferMode_)
		readRegisters(address_, JY_AY, 2, (int8_t *)&JY901_data.acc.y);
	return JY901_data.acc.y / (32768.0/16.0);
}

double CJY901::getAccZ()
{
	if (transferMode_)
		readRegisters(address_, JY_AZ, 2, (int8_t *)&JY901_data.acc.z);
	return JY901_data.acc.z / (32768.0/16.0);
}

double CJY901::getGyroX()
{
	if (transferMode_)
		readRegisters(address_, JY_GX, 2, (int8_t *)&JY901_data.gyro.x);
	return JY901_data.gyro.x / (32768.0/2000.0);
}

double CJY901::getGyroY()
{
	if (transferMode_)
		readRegisters(address_, JY_GY, 2, (int8_t *)&JY901_data.gyro.y);
	return JY901_data.gyro.y / (32768.0/2000.0);
}

double CJY901::getGyroZ()
{
	if (transferMode_)
		readRegisters(address_, JY_GZ, 2, (int8_t *)&JY901_data.gyro.z);
	return JY901_data.gyro.z / (32768.0/2000.0);
}

double CJY901::getMagX()
{
	if (transferMode_)
		readRegisters(address_, JY_HX, 2, (int8_t *)&JY901_data.mag.x);
	return JY901_data.mag.x / (32768.0/180.0);
}

double CJY901::getMagY()
{
	if (transferMode_)
		readRegisters(address_, JY_HY, 2, (int8_t *)&JY901_data.mag.y);
	return JY901_data.mag.y / (32768.0/180.0);
}

double CJY901::getMagZ()
{
	if (transferMode_)
		readRegisters(address_, JY_HZ, 2, (int8_t *)&JY901_data.mag.z);
	return JY901_data.mag.z / (32768.0/180.0);
}

int16_t CJY901::getAccRawX()
{
	if (transferMode_)
		readRegisters(address_, JY_AX, 2, (int8_t *)&JY901_data.acc.x);
	return JY901_data.acc.x;
}

int16_t CJY901::getAccRawY()
{
	if (transferMode_)
		readRegisters(address_, JY_AY, 2, (int8_t *)&JY901_data.acc.y);
	return JY901_data.acc.y;
}

int16_t CJY901::getAccRawZ()
{
	if (transferMode_)
		readRegisters(address_, JY_AZ, 2, (int8_t *)&JY901_data.acc.z);
	return JY901_data.acc.z;
}

int16_t CJY901::getGyroRawX()
{
	if (transferMode_)
		readRegisters(address_, JY_GX, 2, (int8_t *)&JY901_data.gyro.x);
	return JY901_data.gyro.x;
}

int16_t CJY901::getGyroRawY()
{
	if (transferMode_)
		readRegisters(address_, JY_GY, 2, (int8_t *)&JY901_data.gyro.y);
	return JY901_data.gyro.y;
}

int16_t CJY901::getGyroRawZ()
{
	if (transferMode_)
		readRegisters(address_, JY_GZ, 2, (int8_t *)&JY901_data.gyro.z);
	return JY901_data.gyro.z;
}

int16_t CJY901::getMagRawX()
{
	if (transferMode_)
		readRegisters(address_, JY_HX, 2, (int8_t *)&JY901_data.mag.x);
	return JY901_data.mag.x;
}

int16_t CJY901::getMagRawY()
{
	if (transferMode_)
		readRegisters(address_, JY_HY, 2, (int8_t *)&JY901_data.mag.y);
	return JY901_data.mag.y;
}

int16_t CJY901::getMagRawZ()
{
	if (transferMode_)
		readRegisters(address_, JY_HZ, 2, (int8_t *)&JY901_data.mag.z);
	return JY901_data.mag.z;
}

double CJY901::getRoll()
{
		if (transferMode_)
			readRegisters(address_, JY_Roll, 2, (int8_t *)&JY901_data.angle.roll);
		return JY901_data.angle.roll / (32768.0/180.0);
}

double CJY901::getPitch()
{
		if (transferMode_)
			readRegisters(address_, JY_Pitch, 2, (int8_t *)&JY901_data.angle.pitch);
		return JY901_data.angle.pitch / (32768.0/180.0);
}
double CJY901::getYaw()
{
		if (transferMode_)
			readRegisters(address_, JY_Yaw, 2, (int8_t *)&JY901_data.angle.yaw);
		return JY901_data.angle.yaw / (32768.0/180.0);
}


// double CJY901::getTemp()
// {
// 	if (transferMode_)
// 		readRegisters(address_, JY_HX, 8, (int8_t *)&JY901_data.mag);
// 	if (strcmp(str, "t") == 0 || strcmp(str, "T") == 0) {    //温度
// 		return JY901_data.acc.temperature / 340.0 + 36.53;
// 	}
// }

int32_t CJY901::getPressure(void)
{
	if (transferMode_)
		readRegisters(address_, JY_PressureL, 4, (int8_t *)&JY901_data.pressure);

	return JY901_data.pressure; //Pa
}

int32_t CJY901::getAltitude(void)
{
	if (transferMode_)
		readRegisters(address_, JY_HeightL, 4, (int8_t *)&JY901_data.altitude);

	return JY901_data.altitude; //cm

}


int16_t CJY901::getD0Status()
{
	if (transferMode_)
		readRegisters(address_, JY_D0Status, 2, (int8_t *)&JY901_data.dStatus.d_0);
		return JY901_data.dStatus.d_0;
}

int16_t CJY901::getD1Status()
{
	if (transferMode_)
		readRegisters(address_, JY_D1Status, 2, (int8_t *)&JY901_data.dStatus.d_1);
		return JY901_data.dStatus.d_1;
}

int16_t CJY901::getD2Status()
{
	if (transferMode_)
		readRegisters(address_, JY_D2Status, 2, (int8_t *)&JY901_data.dStatus.d_2);
		return JY901_data.dStatus.d_2;
}

int16_t CJY901::getD3Status()
{
	if (transferMode_)
		readRegisters(address_, JY_D3Status, 2, (int8_t *)&JY901_data.dStatus.d_3);
		return JY901_data.dStatus.d_3;
}

int32_t CJY901::getLon(void)
{
	if (transferMode_)
		readRegisters(address_, JY_LonL, 4, (int8_t *)&JY901_data.lon);

	return JY901_data.lon;
}

int32_t CJY901::getLat(void)
{
	if (transferMode_)
		readRegisters(address_, JY_LatL, 4, (int8_t *)&JY901_data.lat);

	return JY901_data.lat;
}

double CJY901::getGPSH(void)
{
	if (transferMode_)
		readRegisters(address_, JY_GPSHeight, 2, (int8_t *)&JY901_data.GPSHeight);

	return JY901_data.GPSHeight / 10.0;
}

double CJY901::getGPSY(void)    //度
{
	if (transferMode_)
		readRegisters(address_, JY_GPSYAW, 2, (int8_t *)&JY901_data.GPSYaw);

	return JY901_data.GPSYaw / 10.0;
}

double CJY901::getGPSV(void)    //km/h
{
	if (transferMode_)
		readRegisters(address_, JY_GPSVL, 4, (int8_t *)&JY901_data.GPSVelocity);

	return JY901_data.GPSVelocity / 1000.0;
}

void CJY901::saveConf(void)
{
	int8_t cmd[2] = {0x00,0x00};
	if (transferMode_) {
		writeRegister(address_, JY_SAVE, 2, cmd);
	}
}

void CJY901::quitCali(void)
{
	int8_t cmd[2] = {0x00,0x00};
	if (transferMode_) {
		writeRegister(address_, JY_CALSW, 2, cmd);
	}
}

void CJY901::caliIMU(void)
{
	int8_t cmd[2] = {0x01,0x00};
	if (transferMode_) {
		writeRegister(address_, JY_CALSW, 2, cmd);
	}
}

void CJY901::caliMag(void)
{
	int8_t cmd[2] = {0x02,0x00};
	if (transferMode_) {
		writeRegister(address_, JY_CALSW, 2, cmd);
	}
}


unsigned long CJY901::getLastTime(void)
{
	return lastTime;
}

void CJY901::readRegisters(uint8_t deviceAddr, uint8_t addressToRead, uint8_t bytesToRead, int8_t * dest)
{
	Wire.beginTransmission(deviceAddr);
	Wire.write(addressToRead);
	Wire.endTransmission(false); //endTransmission but keep the connection active

	Wire.requestFrom(deviceAddr, bytesToRead); //Ask for bytes, once done, bus is released by default

	if (Wire.available() >= bytesToRead) {//Hang out until we get the # of bytes we expect
		for (int x = 0; x < bytesToRead; x++)
			dest[x] = Wire.read();
		lastTime = millis();
	}
}

void CJY901::writeRegister(uint8_t deviceAddr, uint8_t addressToWrite, uint8_t bytesToRead, int8_t *dataToWrite)
{
	Wire.beginTransmission(deviceAddr);
	Wire.write(addressToWrite);
	for (int i = 0; i < bytesToRead; i++)
		Wire.write(dataToWrite[i]);
	Wire.endTransmission(); //Stop transmitting
}

CJY901 JY901 = CJY901();
