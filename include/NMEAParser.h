#ifndef NMEA_PARSER_H
#define NMEA_PARSER_H

#include "NMEADefinitions.h"
#include <vector>
#include <string>
#include <functional>
#include <map>

class NMEAParser {
private:
    std::map<std::string, std::function<NMEAMessage*(std::vector<std::string>&)>> dispatcher;

    NMEAMessage* parseDTM(std::vector<std::string>& fields);
    NMEAMessage* parseGBS(std::vector<std::string>& fields);
    NMEAMessage* parseGGA(std::vector<std::string>& fields);
    NMEAMessage* parseGLL(std::vector<std::string>& fields);
    NMEAMessage* parseGNS(std::vector<std::string>& fields);
    NMEAMessage* parseGRS(std::vector<std::string>& fields);
    NMEAMessage* parseGSA(std::vector<std::string>& fields);
    NMEAMessage* parseGST(std::vector<std::string>& fields);
    NMEAMessage* parseGSV(std::vector<std::string>& fields);
    NMEAMessage* parseRMC(std::vector<std::string>& fields);
    NMEAMessage* parseTXT(std::vector<std::string>& fields);
    NMEAMessage* parseVLW(std::vector<std::string>& fields);
    NMEAMessage* parseVTG(std::vector<std::string>& fields);
    NMEAMessage* parseZDA(std::vector<std::string>& fields);

public:
    NMEAParser(){
        dispatcher["DTM"] = [this](auto& fields) { return parseDTM(fields); };
        dispatcher["GBS"] = [this](auto& fields) { return parseGBS(fields); };
        dispatcher["GGA"] = [this](auto& fields) { return parseGGA(fields); };
        dispatcher["GLL"] = [this](auto& fields) { return parseGLL(fields); };
        dispatcher["GNS"] = [this](auto& fields) { return parseGNS(fields); };
        dispatcher["GRS"] = [this](auto& fields) { return parseGRS(fields); };
        dispatcher["GSA"] = [this](auto& fields) { return parseGSA(fields); };
        dispatcher["GST"] = [this](auto& fields) { return parseGST(fields); };
        dispatcher["GSV"] = [this](auto& fields) { return parseGSV(fields); };
        dispatcher["RMC"] = [this](auto& fields) { return parseRMC(fields); };
        dispatcher["TXT"] = [this](auto& fields) { return parseTXT(fields); };
        dispatcher["VLW"] = [this](auto& fields) { return parseVLW(fields); };
        dispatcher["VTG"] = [this](auto& fields) { return parseVTG(fields); };
        dispatcher["ZDA"] = [this](auto& fields) { return parseZDA(fields); };
    }
    NMEAMessage* processSentence(std::string& msgId, std::vector<std::string>& fields);

};


std::string parseLonData(std::string& lon);
std::string parseLatData(std::string& lat);
std::string parseDateData(std::string& date);
std::string parseTimeData(std::string& time);


#endif