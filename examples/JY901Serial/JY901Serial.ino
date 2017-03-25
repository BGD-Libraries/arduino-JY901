#include <JY901.h>
/*
Test on Uno R3.
JY901   UnoR3
TX <---> 0(Rx)
*/
void setup() 
{
	Serial.begin(9600);
	JY901.attach(Serial);
}

void loop() 
{
	JY901.receiveSerialData();


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
	Serial.print(JY901.getAccX());
	Serial.print(" ");
	Serial.print(JY901.getAccY());
	Serial.print(" ");
	Serial.print(JY901.getAccZ());
	Serial.print("\n");

	
	Serial.print("Gyro:");
	Serial.print(JY901.getGyroX());
	Serial.print(" ");
	Serial.print(JY901.getGyroY());
	Serial.print(" ");
	Serial.print(JY901.getGyroZ());
	Serial.print("\n");
	
	Serial.print("Mag:");
	Serial.print(JY901.getMagX());
	Serial.print(" ");
	Serial.print(JY901.getMagY());
	Serial.print(" ");
	Serial.print(JY901.getMagZ());
	Serial.print("\n");

	Serial.print("Angle:");
	Serial.print(JY901.getRoll());
	Serial.print(" ");
	Serial.print(JY901.getPitch());
	Serial.print(" ");
	Serial.print(JY901.getYaw());
	Serial.print("\n");

	Serial.print("Pressure:");
	Serial.println(JY901.getPressure());
	Serial.print("Altitude:");
	Serial.println(JY901.getAltitude()/100.0);

	
	Serial.print("DStatus:");
	Serial.print(JY901.getD0Status());
	Serial.print(" ");
	Serial.print(JY901.getD1Status());
	Serial.print(" ");
	Serial.print(JY901.getD2Status());
	Serial.print(" ");
	Serial.print(JY901.getD3Status());
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


