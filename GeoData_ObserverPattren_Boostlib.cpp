/******************************************************************************
	Author : Chandana Jayashekar
	Customer : VWIF
	Task : Access Geo-Data source. 
    ECU contains a geo-data source (assume a library with a convenient interface) 
    that provides points, lines and areas at a high frequency. 
    Sketch an access library that gives several consumers (think about different 
    maprenderers that are in the same process) access to this geo-data source by 
    implementing the observer pattern.

*******************************************************************************/
#include <iostream>
#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <boost/bind/placeholders.hpp>


struct GeoData {

    std::string location;
    double latitude;
    double longitude;
};

class AccessLibrary {

public:

    using GeoDataSignal = boost::signals2::signal<void(const GeoData&)>;

    boost::signals2::connection registerSubscriber(GeoDataSignal::slot_type subscriber) {

        return subscribers.connect(subscriber);
    }

    void deregisterSubscriber(boost::signals2::connection& connection) {

        connection.disconnect();
    }

    void notifySubscribers(const GeoData& data) {

        subscribers(data);
    }

private:

    GeoDataSignal subscribers;
};

class MapRenderer {

public:

    void processGeoData(const GeoData& data) {

        std::cout << "Updated GeoData Location: \n" << data.location << " \nLatitude in degree: " << data.latitude <<  "\nLongitude in degree: " << data.longitude <<"\n"<< std::endl;
    }
};

int main() {

    GeoData geoData1{"Location:  RensingStraße VWIF Bochum", 51.515522, 7.207843};
    GeoData geoData2{"Location:  WittenerStraße VWIF Bochum", 51.473019, 7.242687};

    AccessLibrary accessLibrary;
    MapRenderer renderer1, renderer2;

    // Subscribe to the publisher for geo data signal in the access library
    boost::signals2::connection connection1 = accessLibrary.registerSubscriber(boost::bind(&MapRenderer::processGeoData, &renderer1, _1));
    boost::signals2::connection connection2 = accessLibrary.registerSubscriber(boost::bind(&MapRenderer::processGeoData, &renderer2, _1));

    // Notify subscribers about new geo data    
    accessLibrary.notifySubscribers(geoData1);
    accessLibrary.notifySubscribers(geoData2);

    // Disconnect the consumer from receiving updates
    accessLibrary.deregisterSubscriber(connection1);
    accessLibrary.deregisterSubscriber(connection2);

    return 0;
}
