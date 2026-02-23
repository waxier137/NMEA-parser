#include "../include/NMEAParser.h"
#include "../include/HelperFunctions.h"
#include "../include/NMEADefinitions.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

string parseLonData(string& lon){
    if (lon.empty()||lon.size()<4){
        cerr << "Longtitude value is invalid: " + lon << "\n";
        return "";
    }
    double degrees = stod(lon.substr(0,3));
    double minutes = stod(lon.substr(3));
    double decimalDegree = degrees + (minutes / 60.0);

    return to_string(decimalDegree);
}

string parseLatData(string& lat){
    if (lat.empty()||lat.size()<4){
        cerr << "Lattitude value is invalid: " + lat << "\n";
        return "";
    }
    double degrees = stod(lat.substr(0,2));
    double minutes = stod(lat.substr(2));
    double decimalDegree = degrees + (minutes / 60.0);

    return to_string(decimalDegree);
}

string parseDateData(string& date){
    if(date.length()<6){
        cerr << "Date value is invalid: " + date << "\n";
        return "";
    }
    string day = date.substr(0,2);
    string month = date.substr(2,2);
    string year = date.substr(4,2);

    string fullYear = "20" + year;
    return fullYear + "-" + month + "-" + day;
}

string parseTimeData(string& time){
    if(time.size()<6){
        cerr << "Time value is invalid: " + time << "\n";
        return "";
    }
    string hours = time.substr(0,2);
    string minutes = time.substr(2,2);
    string seconds = time.substr(4);
    return hours + ":" + minutes + ":" + seconds;
}

NMEAMessage* NMEAParser::parseDTM(vector<string>& fields){
    NmeaDTM* dtm = new NmeaDTM();
    auto getChar = [](const string& str) -> CHAR {
        return str.empty() ? '\0' : str[0]; 
    };
    auto getFloat = [](const string& str) -> FLOAT {
        if (str.empty()) return 0.0;
        return stof(str);
    };

    try {
        if (fields.size() > 1) dtm->localDatum  = fields[1];
        if (fields.size() > 2) dtm->subDatum    = fields[2];
        
        if (fields.size() > 3) dtm->latOffset   = getFloat(fields[3]);
        if (fields.size() > 4) dtm->nsIndicator = getChar(fields[4]);
        
        if (fields.size() > 5) dtm->lonOffset   = getFloat(fields[5]);
        if (fields.size() > 6) dtm->ewIndicator = getChar(fields[6]);
        
        if (fields.size() > 7) dtm->altOffset   = getFloat(fields[7]);
        
        if (fields.size() > 8) dtm->refDatum    = fields[8];

    } catch (const std::exception& e) {
        cerr << "Warning: Failed to parse numeric value in DTM sentence: " << e.what() << "\n";
    }

    return dtm;
}

NMEAMessage* NMEAParser::parseGBS(vector<string>& fields) {
    NmeaGBS* gbs = new NmeaGBS();
    auto getChar = [](const string& str) -> CHAR {
        return str.empty() ? '\0' : str[0];
    };

    auto getFloat = [](const string& str) -> FLOAT {
        if (str.empty()) return 0.0f;
        try { return stof(str); } catch (...) { return 0.0f; }
    };

    auto getInt = [](const string& str) -> INT {
        if (str.empty()) return 0;
        try { return stoi(str); } catch (...) { return 0; }
    };

    try {
        if (fields.size() > 1 && !fields[1].empty()) {
            gbs->utcTime = parseTimeData(fields[1]);
        }
        if (fields.size() > 2) gbs->errLat = getFloat(fields[2]);
        if (fields.size() > 3) gbs->errLon = getFloat(fields[3]);
        if (fields.size() > 4) gbs->errAlt = getFloat(fields[4]);
        if (fields.size() > 5) gbs->svid = getInt(fields[5]);
        if (fields.size() > 6) gbs->prob = getFloat(fields[6]);
        if (fields.size() > 7) gbs->bias = getFloat(fields[7]);
        if (fields.size() > 8) gbs->stddev = getFloat(fields[8]);
        if (fields.size() > 9) gbs->systemId = getInt(fields[9]);
        if (fields.size() > 10) gbs->signalId = getInt(fields[10]);

    } catch (const std::exception& e) {
        cerr << "Warning: Failed to parse numeric value in GBS sentence: " << e.what() << "\n";
    }

    return gbs;
}

NMEAMessage* NMEAParser::parseGGA(vector<string>& fields) {
    NmeaGGA* gga = new NmeaGGA();
    auto getChar = [](const string& str) -> CHAR {
        return str.empty() ? '\0' : str[0];
    };
    auto getFloat = [](const string& str) -> FLOAT {
        if (str.empty()) return 0.0f;
        try { return stof(str); } catch (...) { return 0.0f; }
    };
    auto getInt = [](const string& str) -> INT {
        if (str.empty()) return 0;
        try { return stoi(str); } catch (...) { return 0; }
    };

    try {
        if (fields.size() > 1 && !fields[1].empty()) {
            gga->utcTime = parseTimeData(fields[1]);
        }

        if (fields.size() > 3 && !fields[2].empty()) {
            gga->latitude = parseLatData(fields[2]);
            gga->nsIndicator = getChar(fields[3]);
            if(gga->nsIndicator == 'S' || gga->nsIndicator == 's')
                gga->latitude = '-' + gga->latitude;
        }

        if (fields.size() > 5 && !fields[4].empty()) {
            gga->longitude = parseLonData(fields[4]);
            gga->ewIndicator = getChar(fields[5]);
            if(gga->ewIndicator == 'W' || gga->ewIndicator == 'w')
                gga->longitude = '-' + gga->longitude;
        }

        if (fields.size() > 6) gga->qualityInd = getInt(fields[6]);

        if (fields.size() > 7) gga->numSV = getInt(fields[7]);

        if (fields.size() > 8) gga->hdop = getFloat(fields[8]);

        if (fields.size() > 9) gga->altitude = getFloat(fields[9]);
        if (fields.size() > 10) gga->altUnit = getChar(fields[10]);

        if (fields.size() > 11) gga->geoidSep = getFloat(fields[11]);
        if (fields.size() > 12) gga->sepUnit = getChar(fields[12]);

        if (fields.size() > 13) gga->diffAge = getFloat(fields[13]);

        if (fields.size() > 14) gga->diffStation = getInt(fields[14]);

    } catch (const std::exception& e) {
        cerr << "Warning: Failed to parse numeric value in GGA sentence: " << e.what() << "\n";
    }

    return gga;
}

NMEAMessage* NMEAParser::parseGLL(vector<string>& fields) {
    NmeaGLL* gll = new NmeaGLL();
    auto getChar = [](const string& str) -> CHAR {
        return str.empty() ? '\0' : str[0];
    };

    try {
        if (fields.size() > 2 && !fields[1].empty()) {
            gll->latitude = parseLatData(fields[1]);
            gll->nsIndicator = getChar(fields[2]);
            if(gll->nsIndicator == 'S' || gll->nsIndicator == 's')
                gll->latitude = '-' + gll->latitude;
        }

        if (fields.size() > 4 && !fields[3].empty()) {
            gll->longitude = parseLonData(fields[3]);
            gll->ewIndicator = getChar(fields[4]);
            if(gll->ewIndicator == 'W' || gll->ewIndicator == 'w')
                gll->longitude = '-' + gll->longitude;
        }

        if (fields.size() > 5 && !fields[5].empty()) {
            gll->utcTime = parseTimeData(fields[5]);
        }

        if (fields.size() > 6) {
            gll->status = getChar(fields[6]);
        }

        if (fields.size() > 7) {
            gll->posMode = getChar(fields[7]);
        }

    } catch (const std::exception& e) {
        cerr << "Warning: Failed to parse GLL sentence: " << e.what() << "\n";
    }

    return gll;
}

NMEAMessage* NMEAParser::parseGNS(vector<string>& fields) {
    NmeaGNS* gns = new NmeaGNS();
    auto getChar = [](const string& str) -> CHAR {
        return str.empty() ? '\0' : str[0];
    };

    auto getFloat = [](const string& str) -> FLOAT {
        if (str.empty()) return 0.0f;
        try { return stof(str); } catch (...) { return 0.0f; }
    };

    auto getInt = [](const string& str) -> INT {
        if (str.empty()) return 0;
        try { return stoi(str); } catch (...) { return 0; }
    };

    try {
        if (fields.size() > 1 && !fields[1].empty()) {
            gns->utcTime = parseTimeData(fields[1]);
        }

        if (fields.size() > 3 && !fields[2].empty()) {
            gns->latitude = parseLatData(fields[2]);
            gns->nsIndicator = getChar(fields[3]);
            if(gns->nsIndicator == 'S' || gns->nsIndicator == 's')
                gns->latitude = '-' + gns->latitude;
        }

        if (fields.size() > 5 && !fields[4].empty()) {
            gns->longitude = parseLonData(fields[4]);
            gns->ewIndicator = getChar(fields[5]);
            if(gns->ewIndicator == 'W' || gns->ewIndicator == 'w')
                gns->longitude = '-' + gns->longitude;
        }

        if (fields.size() > 6) gns->posMode = fields[6];

        if (fields.size() > 7) gns->numSV = getInt(fields[7]);

        if (fields.size() > 8) gns->hdop = getFloat(fields[8]);

        if (fields.size() > 9) gns->altitude = getFloat(fields[9]);

        if (fields.size() > 10) gns->geoidSep = getFloat(fields[10]);

        if (fields.size() > 11) gns->diffAge = getFloat(fields[11]);

        if (fields.size() > 12) gns->diffStation = getInt(fields[12]);

        if (fields.size() > 13) gns->navStatus = getChar(fields[13]);

    } catch (const std::exception& e) {
        cerr << "Warning: Failed to parse GNS sentence: " << e.what() << "\n";
    }

    return gns;
}

NMEAMessage* NMEAParser::parseGRS(vector<string>& fields){
    NmeaGRS* grs = new NmeaGRS();
    try{
        if (fields.size() > 1) grs->utcTime = fields[1];
        if (fields.size() > 2 && !fields[2].empty()) {
            grs->mode = std::stoi(fields[2]);
        }
        for (int i = 3; i <= 14; ++i) {
            if (fields.size() > i && !fields[i].empty()) {
                grs->residuals.push_back(std::stof(fields[i]));
            } else {
                break; 
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "GRS Parse Error: " << e.what() << std::endl;
    }
    return grs;
}

NMEAMessage* NMEAParser::parseGSA(vector<string>& fields) {
    NmeaGSA* gsa = new NmeaGSA();
    auto getChar = [](const string& str) -> CHAR {
        return str.empty() ? '\0' : str[0];
    };

    auto getFloat = [](const string& str) -> FLOAT {
        if (str.empty()) return 0.0f;
        try { return stof(str); } catch (...) { return 0.0f; }
    };

    auto getInt = [](const string& str) -> INT {
        if (str.empty()) return 0;
        try { return stoi(str); } catch (...) { return 0; }
    };

    try {
        if (fields.size() > 1) gsa->opMode = getChar(fields[1]);
        if (fields.size() > 2) gsa->navMode = getInt(fields[2]);
        for (int i = 3; i <= 14; ++i) {
            if (fields.size() > i && !fields[i].empty()) {
                gsa->svIds.push_back(getInt(fields[i]));
            }
        }

        if (fields.size() > 15) gsa->pdop = getFloat(fields[15]);

        if (fields.size() > 16) gsa->hdop = getFloat(fields[16]);

        if (fields.size() > 17) gsa->vdop = getFloat(fields[17]);

        if (fields.size() > 18) gsa->systemId = getInt(fields[18]);

    } catch (const std::exception& e) {
        std::cerr << "GSA Parse Error: " << e.what() << std::endl;
    }

    return gsa;
}

NMEAMessage* NMEAParser::parseGST(vector<string>& fields) {
    NmeaGST* gst = new NmeaGST();
    
    auto getFloat = [](const string& str) -> FLOAT {
        if (str.empty()) return 0.0f;
        try { return std::stof(str); } catch (...) { return 0.0f; }
    };

    try {
        if (fields.size() > 1 && !fields[1].empty()) {
            gst->utcTime = parseTimeData(fields[1]);
        }
        if (fields.size() > 2) gst->rangeRms = getFloat(fields[2]);
        if (fields.size() > 3) gst->stdMajor = getFloat(fields[3]);
        if (fields.size() > 4) gst->stdMinor = getFloat(fields[4]);
        if (fields.size() > 5) gst->orient = getFloat(fields[5]);
        if (fields.size() > 6) gst->stdLat = getFloat(fields[6]);
        if (fields.size() > 7) gst->stdLong = getFloat(fields[7]);
        if (fields.size() > 8) gst->stdAlt = getFloat(fields[8]);

    } catch (const std::exception& e) {
        std::cerr << "GST Parse Error: " << e.what() << std::endl;
    }

    return gst;
}

NMEAMessage* NMEAParser::parseGSV(vector<string>& fields) {
    NmeaGSV* gsv = new NmeaGSV();
    
    auto getInt = [](const string& str) -> INT {
        if (str.empty()) return 0;
        try { return stoi(str); } catch (...) { return 0; }
    };

    try {
        if (fields.size() > 1) gsv->numMsg = getInt(fields[1]);

        if (fields.size() > 2) gsv->msgNum = getInt(fields[2]);

        if (fields.size() > 3) gsv->numSV = getInt(fields[3]);

        for (size_t i = 4; i + 3 < fields.size() && i < 20; i += 4) {
            if (!fields[i].empty()) {
                GSVSatelliteData sat;
                sat.svId      = getInt(fields[i]);
                sat.elevation = getInt(fields[i+1]);
                sat.azimuth   = getInt(fields[i+2]);
                sat.cno       = getInt(fields[i+3]);
                gsv->satellites.push_back(sat);
            }
        }
        size_t signalIdIdx = 4 + (gsv->satellites.size() * 4);
        if (fields.size() > signalIdIdx) {
            gsv->signalId = getInt(fields[signalIdIdx]);
        }

    } catch (const std::exception& e) {
        std::cerr << "GSV Parse Error: " << e.what() << std::endl;
    }

    return gsv;
}

NMEAMessage* NMEAParser::parseRMC(vector<string>& fields) {
    NmeaRMC* rmc = new NmeaRMC();
    
    // Helper lambdas for consistent data extraction
    auto getChar = [](const string& str) -> CHAR {
        return str.empty() ? '\0' : str[0];
    };

    auto getFloat = [](const string& str) -> FLOAT {
        if (str.empty()) return 0.0f;
        try { return stof(str); } catch (...) { return 0.0f; }
    };

    try {
        if (fields.size() > 1 && !fields[1].empty()) {
            rmc->utcTime = parseTimeData(fields[1]);
        }

        if (fields.size() > 2) rmc->status = getChar(fields[2]);

        if (fields.size() > 4 && !fields[3].empty()) {
            rmc->latitude = parseLatData(fields[3]);
            rmc->nsIndicator = getChar(fields[4]);
            if(rmc->nsIndicator == 'S' || rmc->nsIndicator == 's')
                rmc->latitude = '-' + rmc->latitude;
        }

        if (fields.size() > 6 && !fields[5].empty()) {
            rmc->longitude = parseLonData(fields[5]);
            rmc->ewIndicator = getChar(fields[6]);
            if(rmc->ewIndicator == 'W' || rmc->ewIndicator == 'w')
                rmc->longitude = '-' + rmc->longitude;
        }

        if (fields.size() > 7) rmc->speedGnd = getFloat(fields[7]);
        if (fields.size() > 8) rmc->courseGnd = getFloat(fields[8]);

        if (fields.size() > 9 && !fields[9].empty()) {
            rmc->utcDate = parseDateData(fields[9]);
        }

        if (fields.size() > 10) rmc->magVar = getFloat(fields[10]);
        if (fields.size() > 11) rmc->magVarEW = getChar(fields[11]);

        if (fields.size() > 12) rmc->posMode = getChar(fields[12]);

        if (fields.size() > 13) rmc->navStatus = getChar(fields[13]);

    } catch (const std::exception& e) {
        cerr << "Warning: Failed to parse RMC sentence: " << e.what() << "\n";
    }

    return rmc;
}

NMEAMessage* NMEAParser::parseZDA(vector<string>& fields) {
    NmeaZDA* zda = new NmeaZDA();
    
    auto getInt = [](const string& str) -> INT {
        if (str.empty()) return 0;
        try { return stoi(str); } catch (...) { return 0; }
    };

    try {
        if (fields.size() > 1 && !fields[1].empty()) {
            zda->utcTime = parseTimeData(fields[1]);
        }

        if (fields.size() > 2) zda->day = getInt(fields[2]);

        if (fields.size() > 3) zda->month = getInt(fields[3]);

        if (fields.size() > 4) zda->year = getInt(fields[4]);

        if (fields.size() > 5) zda->localTimeHrs = getInt(fields[5]);

        if (fields.size() > 6) zda->localTimeMins = getInt(fields[6]);

    } catch (const std::exception& e) {
        cerr << "Warning: Failed to parse ZDA sentence: " << e.what() << "\n";
    }

    return zda;
}

NMEAMessage* NMEAParser::parseVLW(vector<string>& fields) {
    NmeaVLW* vlw = new NmeaVLW();
    
    // Helper lambdas for consistent data extraction
    auto getChar = [](const string& str) -> CHAR {
        return str.empty() ? '\0' : str[0];
    };

    auto getFloat = [](const string& str) -> FLOAT {
        if (str.empty()) return 0.0f;
        try { return stof(str); } catch (...) { return 0.0f; }
    };

    try {
        if (fields.size() > 1) vlw->totalWaterDist = getFloat(fields[1]);

        if (fields.size() > 2) vlw->twdUnit = getChar(fields[2]);

        if (fields.size() > 3) vlw->waterDist = getFloat(fields[3]);

        if (fields.size() > 4) vlw->wdUnit = getChar(fields[4]);

        if (fields.size() > 5) vlw->totalGndDist = getFloat(fields[5]);

        if (fields.size() > 6) vlw->tgdUnit = getChar(fields[6]);

        if (fields.size() > 7) vlw->gndDist = getFloat(fields[7]);

        if (fields.size() > 8) vlw->gdUnit = getChar(fields[8]);

    } catch (const std::exception& e) {
        cerr << "Warning: Failed to parse VLW sentence: " << e.what() << "\n";
    }

    return vlw;
}

NMEAMessage* NMEAParser::parseTXT(vector<string>& fields) {
    NmeaTXT* txt = new NmeaTXT();
    auto getInt = [](const string& str) -> INT {
        if (str.empty()) return 0;
        try { return stoi(str); } catch (...) { return 0; }
    };

    try {
        if (fields.size() > 1) txt->totalMsgs = getInt(fields[1]);

        if (fields.size() > 2) txt->msgNum = getInt(fields[2]);

        if (fields.size() > 3) txt->msgType = getInt(fields[3]);

        if (fields.size() > 4) txt->textMsg = fields[4];

    } catch (const std::exception& e) {
        cerr << "Warning: Failed to parse TXT sentence: " << e.what() << "\n";
    }

    return txt;
}

NMEAMessage* NMEAParser::parseVTG(vector<string>& fields) {
    NmeaVTG* vtg = new NmeaVTG();
    
    auto getChar = [](const string& str) -> CHAR {
        return str.empty() ? '\0' : str[0];
    };

    auto getFloat = [](const string& str) -> FLOAT {
        if (str.empty()) return 0.0f;
        try { return stof(str); } catch (...) { return 0.0f; }
    };

    try {
        if (fields.size() > 1) vtg->courseTrue = getFloat(fields[1]);
        if (fields.size() > 2) vtg->trueInd = getChar(fields[2]);

        if (fields.size() > 3) vtg->courseMag = getFloat(fields[3]);
        if (fields.size() > 4) vtg->magInd = getChar(fields[4]);

        if (fields.size() > 5) vtg->speedKnots = getFloat(fields[5]);
        if (fields.size() > 6) vtg->knotsInd = getChar(fields[6]);

        if (fields.size() > 7) vtg->speedKph = getFloat(fields[7]);
        if (fields.size() > 8) vtg->kphInd = getChar(fields[8]);

        if (fields.size() > 9) vtg->posMode = getChar(fields[9]);

    } catch (const std::exception& e) {
        cerr << "Warning: Failed to parse VTG sentence: " << e.what() << "\n";
    }

    return vtg;
}

NMEAMessage* NMEAParser::processSentence(string& msgId, vector<string>& fields) {
    auto it = dispatcher.find(msgId);
    
    if (it != dispatcher.end()) {
        return it->second(fields); 
    }
    return nullptr; 
}