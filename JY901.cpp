#include "JY901.h"
#include "JY901_dfs.h"
#include <Arduino.h>
#include <string.h>
#include <Wire.h>
#include <stdint.h>

CJY901::CJY901()
{
	_address = 0x50;
	_transferMode = 0;
}

void CJY901::startIIC(uint8_t address)
{
	_address = address;
	_transferMode = 1;
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
	return 1;
}

void CJY901::readRegisters(uint8_t deviceAddr, uint8_t addressToRead, uint8_t bytesToRead, int8_t * dest)
{
	Wire.beginTransmission(deviceAddr);
	Wire.write(addressToRead);
	Wire.endTransmission(false); //endTransmission but keep the connection active

	Wire.requestFrom(deviceAddr, bytesToRead); //Ask for bytes, once done, bus is released by default

	while (Wire.available() < bytesToRead); //Hang out until we get the # of bytes we expect

	for (int x = 0; x < bytesToRead; x++)
		dest[x] = Wire.read();
}

void CJY901::writeRegister(uint8_t deviceAddr, uint8_t addressToWrite, uint8_t bytesToRead, int8_t *dataToWrite)
{
	Wire.beginTransmission(deviceAddr);
	Wire.write(addressToWrite);
	for (int i = 0; i < bytesToRead; i++)
		Wire.write(dataToWrite[i]);
	Wire.endTransmission(); //Stop transmitting
}


void CJY901::readData(uint8_t address, uint8_t length, int8_t data[])
{
	readRegisters(_address, address, length, data);
}

uint16_t CJY901::getTime(const char* str)
{
	if (_transferMode)
		readRegisters(_address, JY_YYMM, 8, (int8_t*)&JY901_data.time);

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

double CJY901::getAcc(const char* str)
{
	if (_transferMode)
		readRegisters(_address, JY_AX, 8, (int8_t *)&JY901_data.acc);

	if (strcmp(str, "x") == 0 || strcmp(str, "X") == 0)     //x
		return JY901_data.acc.x / (32768.0/16.0);

	if (strcmp(str, "y") == 0 || strcmp(str, "Y") == 0)     //y
		return JY901_data.acc.y / (32768.0/16.0);

	if (strcmp(str, "z") == 0 || strcmp(str, "Z") == 0)     //z
		return JY901_data.acc.z / (32768.0/16.0);

	if (strcmp(str, "t") == 0 || strcmp(str, "T") == 0)     //温度
		return JY901_data.acc.temperature / 340.0 + 36.53;
}

double CJY901::getGyro(const char* str)
{
	if (_transferMode)
		readRegisters(_address, JY_GX, 8, (int8_t *)&JY901_data.gyro);

	if (strcmp(str, "x") == 0 || strcmp(str, "X") == 0)     //x
		return JY901_data.gyro.x / (32768.0/2000.0);

	if (strcmp(str, "y") == 0 || strcmp(str, "Y") == 0)     //y
		return JY901_data.gyro.y / (32768.0/2000.0);

	if (strcmp(str, "z") == 0 || strcmp(str, "Z") == 0)     //z
		return JY901_data.gyro.z / (32768.0/2000.0);

	if (strcmp(str, "t") == 0 || strcmp(str, "T") == 0)     //温度
		return JY901_data.gyro.temperature / 340.0 + 36.53;
}

double CJY901::getAngle(const char* str)
{
	if (_transferMode)
		readRegisters(_address, JY_Roll, 8, (int8_t *)&JY901_data.angle);

	if (strcmp(str, "x") == 0 || strcmp(str, "X") == 0)     //x
		return JY901_data.angle.x / (32768.0/180.0);

	if (strcmp(str, "y") == 0 || strcmp(str, "Y") == 0)     //y
		return JY901_data.angle.y / (32768.0/180.0);

	if (strcmp(str, "z") == 0 || strcmp(str, "Z") == 0)     //z
		return JY901_data.angle.z / (32768.0/180.0);

	if (strcmp(str, "t") == 0 || strcmp(str, "T") == 0)     //温度
		return JY901_data.angle.temperature / 340.0 + 36.53;
}

double CJY901::getMag(const char* str)
{
	if (_transferMode)
		readRegisters(_address, JY_HX, 8, (int8_t *)&JY901_data.mag);

	if (strcmp(str, "x") == 0 || strcmp(str, "X") == 0)     //x
		return JY901_data.mag.x / (32768.0/180.0);

	if (strcmp(str, "y") == 0 || strcmp(str, "Y") == 0)     //y
		return JY901_data.mag.y / (32768.0/180.0);

	if (strcmp(str, "z") == 0 || strcmp(str, "Z") == 0)     //z
		return JY901_data.mag.z / (32768.0/180.0);

	if (strcmp(str, "t") == 0 || strcmp(str, "T") == 0)     //温度
		return JY901_data.mag.temperature / 340.0 + 36.53;
}

int32_t CJY901::getPressure(void)
{
	if (_transferMode)
		readRegisters(_address, JY_PressureL, 4, (int8_t *)&JY901_data.pressure);

	return JY901_data.pressure; //Pa
}

int32_t CJY901::getAltitude(void)
{
	if (_transferMode)
		readRegisters(_address, JY_HeightL, 4, (int8_t *)&JY901_data.altitude);

	return JY901_data.altitude; //cm

}


int16_t CJY901::getDStatus(const char* str)
{
	if (_transferMode)
		readRegisters(_address, JY_D0Status, 8, (int8_t *)&JY901_data.dStatus);

	if (strcmp(str, "0") == 0)      //
		return JY901_data.dStatus.d_0;

	if (strcmp(str, "1") == 0)      //
		return JY901_data.dStatus.d_1;

	if (strcmp(str, "2") == 0)      //
		return JY901_data.dStatus.d_2;

	if (strcmp(str, "3") == 0)      //
		return JY901_data.dStatus.d_3;
}

int32_t CJY901::getLon(void)
{
	if (_transferMode)
		readRegisters(_address, JY_LonL, 4, (int8_t *)&JY901_data.lon);

	return JY901_data.lon;
}

int32_t CJY901::getLat(void)
{
	if (_transferMode)
		readRegisters(_address, JY_LatL, 4, (int8_t *)&JY901_data.lat);

	return JY901_data.lat;
}

double CJY901::getGPSH(void)
{
	if (_transferMode)
		readRegisters(_address, JY_GPSHeight, 2, (int8_t *)&JY901_data.GPSHeight);

	return JY901_data.GPSHeight / 10.0;
}

double CJY901::getGPSY(void)    //度
{
	if (_transferMode)
		readRegisters(_address, JY_GPSYAW, 2, (int8_t *)&JY901_data.GPSYaw);

	return JY901_data.GPSYaw / 10.0;
}

double CJY901::getGPSV(void)    //km/h
{
	if (_transferMode)
		readRegisters(_address, JY_GPSVL, 4, (int8_t *)&JY901_data.GPSVelocity);

	return JY901_data.GPSVelocity / 1000.0;
}

void CJY901::saveConf(void)
{
	int8_t cmd[2] = {0x00,0x00};
	if (_transferMode) {
		writeRegister(_address, JY_SAVE, 2, cmd);
	}
}

void CJY901::quitCali(void)
{
	int8_t cmd[2] = {0x00,0x00};
	if (_transferMode) {
		writeRegister(_address, JY_CALSW, 2, cmd);
	}
}

void CJY901::caliIMU(void)
{
	int8_t cmd[2] = {0x01,0x00};
	if (_transferMode) {
		writeRegister(_address, JY_CALSW, 2, cmd);
	}
}

void CJY901::caliMag(void)
{
	int8_t cmd[2] = {0x02,0x00};
	if (_transferMode) {
		writeRegister(_address, JY_CALSW, 2, cmd);
	}
}

CJY901 JY901 = CJY901();
