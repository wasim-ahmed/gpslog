all:gpsplus

gpsplus:cppgps.o gps.o nmea.o serial.o
	g++ -o gpsplus cppgps.o gps.o nmea.o serial.o

cppgps.o:cppgps.cpp
	g++ -c cppgps.cpp

gps.o:gps.cpp
	g++ -c gps.cpp

nmea.o:nmea.cpp
	g++ -c nmea.cpp

serial.o:serial.cpp
	g++ -c serial.cpp

clean:
	rm serial.o nmea.o gps.o cppgps.o gpsplus
