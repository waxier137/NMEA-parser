#ifndef NMEA_DEFINITIONS_H
#define NMEA_DEFINITIONS_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <iomanip>

using TIME = std::string;
using LAT = std::string;
using LON = std::string;
using INT = int;
using FLOAT = float;
using CHAR = char;
using DATE = std::string;
using STRING = std::string;

struct NMEAMessage {
    STRING msgId;
    
    virtual ~NMEAMessage() = default;

    virtual void writeToCSV(std::ofstream& out) const = 0;
};

struct NmeaDTM : public NMEAMessage {
    NmeaDTM() {
        msgId = "DTM";
    };
    STRING localDatum;
    STRING subDatum;
    FLOAT latOffset;
    CHAR nsIndicator;
    FLOAT lonOffset;
    CHAR ewIndicator;
    FLOAT altOffset;
    STRING refDatum;
    void writeToCSV(std::ofstream& out) const {
        out << "DTM,"
            << "Local Datum Code (W84=WGS84; 999=User): " << localDatum << ","
            << "Sub-datum Field: " << subDatum << ","
            << "Offset in Latitude (min): " << latOffset << ","
            << "North/South: " << nsIndicator << ","
            << "Offset in Longitude (min): " << lonOffset << ","
            << "East/West: " << ewIndicator << ","
            << "Offset in Altitude (m): " << altOffset << ","
            << "Reference Datum Code (W84): " << refDatum << "\n";
    }
};

struct NmeaGBS : public NMEAMessage {
    NmeaGBS() {
        msgId = "GBS";
    };
    TIME utcTime;
    FLOAT errLat;
    FLOAT errLon;
    FLOAT errAlt;
    INT svid;
    FLOAT prob;
    FLOAT bias;
    FLOAT stddev;
    INT systemId;
    INT signalId;
    void writeToCSV(std::ofstream& out) const {
        out << "GBS,"
            << "UTC Time: " << utcTime << ","
            << "Expected Lat Error (m): " << errLat << ","
            << "Expected Lon Error (m): " << errLon << ","
            << "Expected Alt Error (m): " << errAlt << ","
            << "Failed SV ID: " << svid << ","
            << "Prob of missed detection: " << prob << ","
            << "Estimate of Bias (m): " << bias << ","
            << "Std Dev of Bias (m): " << stddev << ","
            << "NMEA GNSS System ID: " << systemId << ","
            << "NMEA GNSS Signal ID: " << signalId << "\n";
    }
};

struct NmeaGGA : public NMEAMessage {
    NmeaGGA() {
        msgId = "GGA";
    };
    TIME utcTime;
    LAT latitude;
    CHAR nsIndicator;
    LON longitude;
    CHAR ewIndicator;
    INT qualityInd;
    INT numSV;
    FLOAT hdop;
    FLOAT altitude;
    CHAR altUnit;
    FLOAT geoidSep;
    CHAR sepUnit;
    FLOAT diffAge;
    INT diffStation;
    void writeToCSV(std::ofstream& out) const {
        out << "GGA,"
            << "UTC Time: " << utcTime << ","
            << "Latitude: " << latitude << "," << nsIndicator << ","
            << "Longitude: " << longitude << "," << ewIndicator << ","
            << "Quality (0=Inv; 1=GPS; 2=DGPS; 4=RTK Fixed; 5=RTK Float; 6=DR): " << qualityInd << ","
            << "Sats Used: " << numSV << ","
            << "HDOP: " << hdop << ","
            << "Altitude (m): " << altitude << "," << altUnit << ","
            << "Geoid Sep (m): " << geoidSep << "," << sepUnit << ","
            << "Age of Diff Corr (s): " << diffAge << ","
            << "DGPS Station ID: " << diffStation << "\n";
    }
};

struct NmeaGLL : public NMEAMessage {
    NmeaGLL() {
        msgId = "GLL";
    };
    LAT latitude;
    CHAR nsIndicator;
    LON longitude;
    CHAR ewIndicator;
    TIME utcTime;
    CHAR status;
    CHAR posMode;
    void writeToCSV(std::ofstream& out) const {
        out << "GLL,"
            << "Latitude: " << latitude << "," << nsIndicator << ","
            << "Longitude: " << longitude << "," << ewIndicator << ","
            << "UTC Time: " << utcTime << ","
            << "Status (A=Valid; V=Invalid): " << status << ","
            << "Positioning Mode: " << posMode << "\n";
    }
};

struct NmeaGNS : public NMEAMessage {
    NmeaGNS() {
        msgId = "GNS";
    };
    TIME utcTime;
    LAT latitude;
    CHAR nsIndicator;
    LON longitude;
    CHAR ewIndicator;
    STRING posMode;
    INT numSV;
    FLOAT hdop;
    FLOAT altitude;
    FLOAT geoidSep;
    FLOAT diffAge;
    INT diffStation;
    CHAR navStatus;
    void writeToCSV(std::ofstream& out) const {
        out << "GNS,"
            << "UTC Time: " << utcTime << ","
            << "Latitude: " << latitude << "," << nsIndicator << ","
            << "Longitude: " << longitude << "," << ewIndicator << ","
            << "Mode (GPS;GLONASS): " << posMode << ","
            << "Sats Used: " << numSV << ","
            << "HDOP: " << hdop << ","
            << "Altitude (m): " << altitude << ","
            << "Geoid Sep (m): " << geoidSep << ","
            << "Age of Diff Corr (s): " << diffAge << ","
            << "DGPS Station ID: " << diffStation << ","
            << "Nav Status (V=Equipment not providing data): " << navStatus << "\n";
    }
};

struct NmeaGRS : public NMEAMessage {
    NmeaGRS() {
        msgId = "GRS";
    };
    TIME utcTime;
    INT mode;
    std::vector<FLOAT> residuals;
    INT systemId;
    INT signalId;
    void writeToCSV(std::ofstream& out) const {
        out << "GRS,"
            << "UTC Time: " << utcTime << ","
            << "Mode (0=Calc; 1=Recomputed): " << mode << ",";
        out << "Residuals (m): ";
        for (const auto& res : residuals) out << res << ";";
        out << ",NMEA GNSS System ID: " << systemId << ","
                << "NMEA GNSS Signal ID: " << signalId << "\n";
    }
};

struct NmeaGSA : public NMEAMessage {
    NmeaGSA() {
        msgId = "GSA";
    };
    CHAR opMode;
    INT navMode;
    std::vector<INT> svIds;
    FLOAT pdop;
    FLOAT hdop;
    FLOAT vdop;
    INT systemId;
    void writeToCSV(std::ofstream& out) const {
        out << "GSA,"
            << "Op Mode (M=Manual; A=Auto): " << opMode << ","
            << "Nav Mode (1=No fix; 2=2D; 3=3D): " << navMode << ",";
        out << "SV IDs: ";
        for (const auto& id : svIds) out << id << ";";
        out << ",PDOP: " << pdop << ","
            << "HDOP: " << hdop << ","
            << "VDOP: " << vdop << ","
            << "NMEA GNSS System ID: " << systemId << "\n";
    }
};

struct NmeaGST : public NMEAMessage {
    NmeaGST() {
        msgId = "GST";
    };
    TIME utcTime;
    FLOAT rangeRms;
    FLOAT stdMajor;
    FLOAT stdMinor;
    FLOAT orient;
    FLOAT stdLat;
    FLOAT stdLong;
    FLOAT stdAlt;
    void writeToCSV(std::ofstream& out) const {
        out << "GST,"
            << "UTC Time: " << utcTime << ","
            << "RMS deviation of ranges: " << rangeRms << ","
            << "Std Dev Semi-major axis: " << stdMajor << ","
            << "Std Dev Semi-minor axis: " << stdMinor << ","
            << "Orientation of semi-major axis: " << orient << ","
            << "Std Dev Lat Error: " << stdLat << ","
            << "Std Dev Lon Error: " << stdLong << ","
            << "Std Dev Alt Error: " << stdAlt << "\n";
    }
};

struct GSVSatelliteData {
    INT svId;
    INT elevation;
    INT azimuth;
    INT cno;
};

struct NmeaGSV : public NMEAMessage {
    NmeaGSV() {
        msgId = "GSV";
    };
    INT numMsg;
    INT msgNum;
    INT numSV;
    std::vector<GSVSatelliteData> satellites;
    INT signalId;
    void writeToCSV(std::ofstream& out) const {
        out << "GSV,"
            << "Total Msg count: " << numMsg << ","
            << "Msg Number: " << msgNum << ","
            << "Sats in view: " << numSV << ",";
        for (const auto& sat : satellites) {
            out << "ID:" << sat.svId << "|El:" << sat.elevation << "|Az:" << sat.azimuth << "|CNo:" << sat.cno << ";";
        }
        out << ",NMEA GNSS Signal ID: " << signalId << "\n";
    }
};

struct NmeaRMC : public NMEAMessage {
    NmeaRMC() {
        msgId = "RMC";
    };
    TIME utcTime;
    CHAR status;
    LAT latitude;
    CHAR nsIndicator;
    LON longitude;
    CHAR ewIndicator;
    FLOAT speedGnd;
    FLOAT courseGnd;
    DATE utcDate;
    FLOAT magVar;
    CHAR magVarEW;
    CHAR posMode;
    CHAR navStatus;
    void writeToCSV(std::ofstream& out) const {
        out << "RMC,"
            << "UTC Time: " << utcTime << ","
            << "Status (A=Data Valid; V=Warning): " << status << ","
            << "Latitude: " << latitude << "," << nsIndicator << ","
            << "Longitude: " << longitude << "," << ewIndicator << ","
            << "Speed Over Ground (knots): " << speedGnd << ","
            << "Course Over Ground (true): " << courseGnd << ","
            << "Date (ddmmyy): " << utcDate << ","
            << "Mag Variation: " << magVar << "," << magVarEW << ","
            << "Pos Mode: " << posMode << ","
            << "Nav Status: " << navStatus << "\n";
    }
};

struct NmeaTXT : public NMEAMessage {
    NmeaTXT() {
        msgId = "TXT";
    };
    INT totalMsgs;
    INT msgNum;
    INT msgType;
    STRING textMsg;
    void writeToCSV(std::ofstream& out) const {
        out << "TXT,"
            << "Total Msgs: " << totalMsgs << ","
            << "Msg Num: " << msgNum << ","
            << "Msg Type (00=Err; 01=Warn; 02=Notice; 07=User): " << msgType << ","
            << "Text: " << textMsg << "\n";
    }
};

struct NmeaVLW : public NMEAMessage {
    NmeaVLW() {
        msgId = "VLW";
    };
    FLOAT totalWaterDist;
    CHAR twdUnit;
    FLOAT waterDist;
    CHAR wdUnit;
    FLOAT totalGndDist;
    CHAR tgdUnit;
    FLOAT gndDist;
    CHAR gdUnit;
    void writeToCSV(std::ofstream& out) const {
        out << "VLW,"
            << "Total Water Dist (nm): " << totalWaterDist << ","
            << "Water Dist Since Reset (nm): " << waterDist << ","
            << "Total Gnd Dist (nm): " << totalGndDist << ","
            << "Gnd Dist Since Reset (nm): " << gndDist << "\n";
    }
};

struct NmeaVTG : public NMEAMessage {
    NmeaVTG() {
        msgId = "VTG";
    };
    FLOAT courseTrue;
    CHAR trueInd;
    FLOAT courseMag;
    CHAR magInd;
    FLOAT speedKnots;
    CHAR knotsInd;
    FLOAT speedKph;
    CHAR kphInd;
    CHAR posMode;
    void writeToCSV(std::ofstream& out) const {
        out << "VTG,"
            << "Course (true): " << courseTrue << ","
            << "Course (magnetic): " << courseMag << ","
            << "Speed (knots): " << speedKnots << ","
            << "Speed (km/h): " << speedKph << ","
            << "Mode: " << posMode << "\n";
    }
};

struct NmeaZDA : public NMEAMessage {
    NmeaZDA() {
        msgId = "ZDA";
    };
    TIME utcTime;
    INT day;
    INT month;
    INT year;
    INT localTimeHrs;
    INT localTimeMins;
    void writeToCSV(std::ofstream& out) const {
        out << "ZDA,"
            << "UTC Time: " << utcTime << ","
            << "Day: " << day << ","
            << "Month: " << month << ","
            << "Year: " << year << ","
            << "Local Zone Offset (hrs): " << localTimeHrs << ","
            << "Local Zone Offset (min): " << localTimeMins << "\n";
    }
};

#endif
