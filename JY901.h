#ifndef JY901_h
#define JY901_h
#include <stdint.h>

class CJY901 
{
  public: 
	
	CJY901(); 										//构造函数
	void     startIIC(uint8_t address = 0x50);		    //设定0x50地址的IIC初始化
	bool     copeSerialData(uint8_t data);			//处理接收的数据
	void     readData(uint8_t address,					//address地址
					  uint8_t length,					//长度length
					   int8_t data[]);				//手动读取ucLength长度的数据
	uint16_t getTime(const char*);				//获取时间，'Y'年，'M'月，'D'天，'h'时，'m'分，'s'秒，'l'毫秒
	double 	 getAcc(const char*);				//获取加速度
	double 	 getGyro(const char*);				//获取角速度
	double 	 getAngle(const char*);				//获取角度
	double 	 getMag(const char*);				//获取磁场
	int32_t getPressure(void);				//获取压力
	int32_t getAltitude(void);				//获取高度	
	int16_t getDStatus(const char*);			//获取端口输出	
	int32_t getLon(void);				//获取经度
	int32_t getLat(void);				//获取纬度
	double   getGPSH(void);					//GPS 高度
	double   getGPSY(void);					//GPS 航向角
	double 	 getGPSV(void);				//GPS 地速
	
  private: 
	uint8_t _address;
	bool    _transferMode;
	uint8_t rxBuffer[12];
	uint8_t rxCnt = 0;
	void readRegisters(uint8_t deviceAddr,uint8_t addressToRead, uint8_t bytesToRead, int8_t * dest);
	void writeRegister(uint8_t deviceAddr,uint8_t addressToWrite,uint8_t bytesToRead, int8_t *dataToWrite);
	struct
	{		
		struct 
		{
			uint8_t  year       :8;
			uint8_t  month      :8;
			uint8_t  day        :8;
			uint8_t  hour       :8;
			uint8_t  minute 	:8;
			uint8_t  second     :8;
			uint16_t milisecond :16;
		}time;
		struct
		{
			int16_t x 			 :16;
			int16_t y 			 :16;
			int16_t z 			 :16;
			int16_t temperature  :16;
		}acc;
		struct
		{
			int16_t x	         :16;
			int16_t y            :16;
			int16_t z 			 :16;
			int16_t temperature  :16;
		}gyro;
		struct
		{
			int16_t x 			 :16;
			int16_t y 			 :16;
			int16_t z 			 :16;
			int16_t temperature  :16;
		}angle;
		struct
		{
			int16_t x 			 :16;
			int16_t y 			 :16;
			int16_t z 			 :16;
			int16_t temperature  :16;
		}mag;
		struct
		{
			int16_t d_0 :16;
			int16_t d_1 :16;
			int16_t d_2 :16;
			int16_t d_3 :16;
		}dStatus;
		int32_t pressure;
		int32_t altitude;
		int32_t lon;
		int32_t lat;

		int16_t GPSHeight;
		int16_t GPSYaw;
		int32_t GPSVelocity;
	}JY901_data;
};
extern CJY901 JY901;
#endif
