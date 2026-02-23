#include "../include/NMEADefinitions.h"
#include "../include/NMEAExporter.h"
#include "../include/HelperFunctions.h"
#include "kml/dom.h"
#include "kml/base/file.h"
#include "kml/engine.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include "kml/base/color32.h"

using namespace std;

void exportParsedMessagesToCSV(const vector<NMEAMessage*>& validMessages, const string& outputFileName) {
    ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        cerr << "Error opening file: " << outputFileName << endl;
        return;
    }

    for (const auto& msg : validMessages) {
        if (!msg) continue; // Safety check

        outputFile << "New NMEA Log\n";
        msg->writeToCSV(outputFile);
        outputFile << "\n";
    }

    outputFile.close();
    cout << "Parsed messages exported to CSV: " << outputFileName << endl;
}

void exportErrorLogsToCSV(const vector<string>& errorMessages, const string& outputFileName) {
    ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        cerr << "Error opening file: " << outputFileName << endl;
        return;
    }
    
    outputFile << "Error Logs\n";
    for (const string& errorMessage : errorMessages) {
        outputFile << errorMessage << "\n";
    }
    
    outputFile.close();
    cout << "Error logs exported to CSV: " << outputFileName << endl;
}

using kmldom::KmlFactory;
using kmldom::KmlPtr;
using kmldom::DocumentPtr;
using kmldom::PlacemarkPtr;
using kmldom::LineStringPtr;
using kmldom::CoordinatesPtr;
using kmldom::StylePtr;
using kmldom::LineStylePtr;

void exportParsedMessagesToKML(const vector<NMEAMessage*>& validMessages, const string& outputFileName) {
    KmlFactory* factory = KmlFactory::GetFactory();
    
    KmlPtr kml = factory->CreateKml();
    DocumentPtr document = factory->CreateDocument();
    kml->set_feature(document);
    
    StylePtr style = factory->CreateStyle();
    style->set_id("blueLine");

    LineStylePtr lineStyle = factory->CreateLineStyle();
    
    lineStyle->set_color(kmlbase::Color32("ffff0000")); 
    lineStyle->set_width(4.0);
    style->set_linestyle(lineStyle);
    
    document->add_styleselector(style); 

    PlacemarkPtr placemark = factory->CreatePlacemark();
    placemark->set_name("GPS Path");
    placemark->set_styleurl("#blueLine");

    LineStringPtr lineString = factory->CreateLineString();
    lineString->set_extrude(true);
    lineString->set_tessellate(true);

    CoordinatesPtr coordinates = factory->CreateCoordinates();

    for (const auto& msg : validMessages) {
        if (!msg) continue;

        string latStr = "";
        string lonStr = "";

        if (msg->msgId == "GGA") {
            auto gga = static_cast<NmeaGGA*>(msg);
            if (gga->qualityInd > 0) { 
                latStr = gga->latitude;
                lonStr = gga->longitude;
            }
        } 
        else if (msg->msgId == "RMC") {
            auto rmc = static_cast<NmeaRMC*>(msg);
            if (rmc->status == 'A') { 
                latStr = rmc->latitude;
                lonStr = rmc->longitude;
            }
        }
        else if (msg->msgId == "GLL") {
            auto gll = static_cast<NmeaGLL*>(msg);
            if (gll->status == 'A') {
                latStr = gll->latitude;
                lonStr = gll->longitude;
            }
        }

        if (!latStr.empty() && !lonStr.empty()) {
            try {
                double lat = stod(latStr);
                double lon = stod(lonStr);
                coordinates->add_latlng(lat, lon);
            } catch (const exception& e) {
                cerr << "Skipping coordinate due to stod conversion error: " << e.what() << endl;
            }
        }
    }

    lineString->set_coordinates(coordinates);
    placemark->set_geometry(lineString);
    document->add_feature(placemark);

    string xml_output = kmldom::SerializePretty(kml);
    
    if (!xml_output.empty()) {
        ofstream outFile(outputFileName);
        outFile << xml_output;
        outFile.close();
        cout << "Successfully exported tracklog via libkml to " << outputFileName << endl;
    } else {
        cerr << "Error: libkml failed to generate KML data." << endl;
    }
}