#include <Wire.h>
#include <JY901.h>
/*
Test on Uno R3.
JY901    UnoR3
SDA <---> SDA
SCL <---> SCL
*/
void setup() 
{
	Serial.begin(115200);
	JY901.startIIC();
} 

void loop() 
{
	//print received data. Data was received in serialEvent;
	Serial.print("Time:20");
	Serial.print(JY901.getTime("year"));
	Serial.print("-");
	Serial.print(JY901.getTime("month"));
	Serial.print("-");
	Serial.print(JY901.getTime("day"));
	Serial.print(" ");
	Serial.print(JY901.getTime("hour"));
	Serial.print(":");
	Serial.print(JY901.getTime("minute"));
	Serial.print(":");
	Serial.println((float)JY901.getTime("second")+(float)JY901.getTime("milisecond")/1000);

							 
	Serial.print("Acc:");
	Serial.print(JY901.getAcc("x"));
	Serial.print(" ");
	Serial.print(JY901.getAcc("y"));
	Serial.print(" ");
	Serial.print(JY901.getAcc("z"));
	Serial.print("\n");

	
	Serial.print("Gyro:");
	Serial.print(JY901.getGyro("x"));
	Serial.print(" ");
	Serial.print(JY901.getGyro("y"));
	Serial.print(" ");
	Serial.print(JY901.getGyro("z"));
	Serial.print("\n");

	
	Serial.print("Angle:");
	Serial.print(JY901.getAngle("x"));
	Serial.print(" ");
	Serial.print(JY901.getAngle("y"));
	Serial.print(" ");
	Serial.print(JY901.getAngle("z"));
	Serial.print("\n");

	
	Serial.print("Mag:");
	Serial.print(JY901.getMag("x"));
	Serial.print(" ");
	Serial.print(JY901.getMag("y"));
	Serial.print(" ");
	Serial.print(JY901.getMag("z"));
	Serial.print("\n");

	
	Serial.print("Pressure:");
	Serial.println(JY901.getPressure());
	Serial.print("Altitude:");
	Serial.println(JY901.getAltitude()/100.0);

	
	Serial.print("DStatus:");
	Serial.print(JY901.getDStatus("0"));
	Serial.print(" ");
	Serial.print(JY901.getDStatus("1"));
	Serial.print(" ");
	Serial.print(JY901.getDStatus("2"));
	Serial.print(" ");
	Serial.print(JY901.getDStatus("3"));
	Serial.print("\n");

	
	Serial.print("Longitude:");
	Serial.print(JY901.getLon()/10000000);
	Serial.print("Deg");
	Serial.print((double)(JY901.getLon() % 10000000)/1e5);
	Serial.println("m");

	Serial.print("Lattitude:");
	Serial.print(JY901.getLat()/10000000);
	Serial.print("Deg");
	Serial.print((double)(JY901.getLat() % 10000000)/1e5);
	Serial.println("m");

	
	Serial.print("GPSHeight:");
	Serial.print(JY901.getGPSH());
	Serial.println("m");
	Serial.print("GPSYaw:");
	Serial.print(JY901.getGPSY());
	Serial.println("Deg:");
	Serial.print("GPSV:");
	Serial.print(JY901.getGPSV());
	Serial.println("km/h");

	
	Serial.println("");

	delay(500);
}



