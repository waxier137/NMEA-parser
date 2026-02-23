#include "../include/HelperFunctions.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

vector<string> readNMEALogs(const string& fileName){
    vector<string> nmeaLogs;
    ifstream file(fileName);
    if(!file.is_open()){
        cerr << "Error opening NMEA logs file: " << fileName << endl;
        return nmeaLogs;
    }
    string line;
    while(getline(file, line)){
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if(line.empty()) continue; 
        
        nmeaLogs.push_back(line);
    }
    file.close();
    return nmeaLogs;
}

string extractMessageID(const string& nmeaLog){
    size_t dollarPos = nmeaLog.find('$');
    size_t commaPos = nmeaLog.find(',');
    
    if(dollarPos == string::npos || commaPos == string::npos || commaPos <= dollarPos) {
        return "UNKNOWN";
    }
    
    string address = nmeaLog.substr(dollarPos + 1, commaPos - dollarPos - 1); 
    
    if (address.length() >= 3) {
        return address.substr(address.length() - 3); 
    }
    return "UNKNOWN";
}

vector<string> splitByDelimiter(const string& str, char delimiter){
    vector<string> tokens;
    stringstream ss(str);
    string token;
    
    while(getline(ss, token, delimiter)){
        tokens.push_back(token);
    }
    if (!str.empty() && str.back() == delimiter) {
        tokens.push_back("");
    }
    
    return tokens;
}

void trimWhitespace(string& str){
    str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
}

bool validateChecksum(const string& nmeaLog) {
    size_t asteriskPos = nmeaLog.find('*');
    if (asteriskPos == string::npos || asteriskPos < 2) return false;

    int calculatedChecksum = 0;
    for (size_t i = 1; i < asteriskPos; ++i) {
        calculatedChecksum ^= nmeaLog[i];
    }

    string providedHex = nmeaLog.substr(asteriskPos + 1, 2);
    try {
        int providedChecksum = stoi(providedHex, nullptr, 16);
        return calculatedChecksum == providedChecksum;
    } catch (...) {
        return false;
    }
}