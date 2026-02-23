#include <iostream>
#include <vector>
#include <string>

#include "../include/NMEADefinitions.h"
#include "../include/NMEAParser.h"
#include "../include/HelperFunctions.h"
#include "../include/NMEAExporter.h"

std::string nmeaLogsFileName = "../data/nmea_logs.txt";

using namespace std;

int main() {
    vector<NMEAMessage*> validMessages;
    vector<string> errorMessages;

    cout << "Reading logs from: " << nmeaLogsFileName << endl;
    vector<string> nmeaLogs = readNMEALogs(nmeaLogsFileName);
    
    if (nmeaLogs.empty()) {
        cerr << "No logs found or file could not be opened." << endl;
        return 1;
    }
    NMEAParser parser;

    for (const string& rawLog : nmeaLogs) {
        
        if (!validateChecksum(rawLog)) {
            errorMessages.push_back("Checksum Failed: " + rawLog);
            continue;
        }

        vector<string> fields = splitByDelimiter(rawLog, ',');
        
        string msgId = extractMessageID(rawLog);

        NMEAMessage* parsedMessage = parser.processSentence(msgId, fields);

        if (parsedMessage != nullptr) {
            validMessages.push_back(parsedMessage);
        } else {
            errorMessages.push_back("Unsupported Message Type: " + rawLog);
        }
    }

    cout << "Successfully parsed " << validMessages.size() << " messages." << endl;
    cout << "Found " << errorMessages.size() << " errors logs." << endl;

    exportParsedMessagesToCSV(validMessages, "parsed_messages.csv");
    exportErrorLogsToCSV(errorMessages, "error_logs.csv");
    exportParsedMessagesToKML(validMessages, "parsed_messages.kml"); 

    for (NMEAMessage* msg : validMessages) {
        delete msg;
    }
    validMessages.clear();

    return 0;
}