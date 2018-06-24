//============================================================================
// Name        : cppgps.cpp
// Author      : Wasim
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "gps.h"
#include <stdlib.h>
#include <fstream>
using namespace std;

int main() {
    // Open
    gps_init();

    loc_t data;

    ofstream out("log.txt",ios::out|ios::app);
    if(out.is_open())
    {
    	out<<"Time"<<"\t"<<"GPS Fix"<<"\t"<<"Satellites"<<"\t"<<"Latitude"<<"\t"<<"Longitude"<<"\t"<<"Speed"<<"\t"<<"Altitude"<<endl;

        while (1) {
            gps_location(&data);
            cout<<"Time:"<<data.time;
            cout<<"\t";
            cout<<"GPS Fix:"<<int(data.gps_fix);
            cout<<"\t";
            cout<<"Satellites Connected"<<int(data.satellites);
            cout<<"\t";
            cout<<"Latitude:"<<data.latitude;
            cout<<"\t";
            cout<<"Longitude"<<data.longitude;
            cout<<"\t";
            cout<<"Speed:"<<data.speed;
            cout<<"\t";
            cout<<"Altitude"<<data.altitude;
            cout<<endl;

            out<<data.time<<"\t"<<int(data.gps_fix)<<"\t"<<int(data.satellites)<<"\t"<<data.latitude<<"\t"<<data.longitude<<"\t"<<data.speed<<"\t"<<data.altitude<<endl;
        }

    }
    else
    {
    	cout<<"File opening Error"<<endl;
    }


    out.close();

    return EXIT_SUCCESS;
}



