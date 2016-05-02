#ifndef JY901_dfs_h
#define JY901_dfs_h

#define JY_SAVE 		0x00
#define JY_CALSW 		0x01
#define JY_RSW 			0x02
#define JY_RRATE		0x03
#define JY_BAUD 		0x04
#define JY_AXOFFSET		0x05
#define JY_AYOFFSET		0x06
#define JY_AZOFFSET		0x07
#define JY_GXOFFSET		0x08
#define JY_GYOFFSET		0x09
#define JY_GZOFFSET		0x0a
#define JY_HXOFFSET		0x0b
#define JY_HYOFFSET		0x0c
#define JY_HZOFFSET		0x0d
#define JY_D0MODE		0x0e
#define JY_D1MODE		0x0f
#define JY_D2MODE		0x10
#define JY_D3MODE		0x11
#define JY_D0PWMH 		0x12
#define JY_D1PWMH	  	0x13
#define JY_D2PWMH		0x14
#define JY_D3PWMH		0x15
#define JY_D0PWMT		0x16
#define JY_D1PWMT		0x17
#define JY_D2PWMT		0x18
#define JY_D3PWMT		0x19
#define JY_IICADDR		0x1a
#define JY_LEDOFF 		0x1b
#define JY_GPSBAUD		0x1c

#define JY_YYMM			0x30
#define JY_DDHH			0x31
#define JY_MMSS			0x32
#define JY_MS			0x33
#define JY_AX			0x34
#define JY_AY			0x35
#define JY_AZ			0x36
#define JY_GX			0x37
#define JY_GY			0x38
#define JY_GZ			0x39
#define JY_HX			0x3a
#define JY_HY			0x3b
#define JY_HZ			0x3c
#define JY_Roll			0x3d
#define JY_Pitch		0x3e
#define JY_Yaw			0x3f
#define JY_TEMP			0x40
#define JY_D0Status		0x41
#define JY_D1Status		0x42
#define JY_D2Status		0x43
#define JY_D3Status		0x44
#define JY_PressureL	0x45
#define JY_PressureH	0x46
#define JY_HeightL		0x47
#define JY_HeightH		0x48
#define JY_LonL			0x49
#define JY_LonH			0x4a
#define JY_LatL			0x4b
#define JY_LatH			0x4c
#define JY_GPSHeight   	0x4d
#define JY_GPSYAW      	0x4e
#define JY_GPSVL		0x4f
#define JY_GPSVH		0x50
	  
#define JY_DIO_MODE_AIN		0
#define JY_DIO_MODE_DIN		1
#define JY_DIO_MODE_DOH		2
#define JY_DIO_MODE_DOL		3
#define JY_DIO_MODE_DOPWM	4
#define JY_DIO_MODE_GPS		5

#endif