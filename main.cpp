#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <fstream>

using namespace std;

string messageLayoutsFileName = "NMEAMessageLayouts.txt";
map<MessageID, vector<string>> messageLayoutsMap; 

enum class FieldType{
    TIME, LAT, LON, INT, FLOAT, CHAR, DATE, STRING, UNKNOWN
};
enum class MessageID{
    UNKNOWN = 0,
    GGA, GLL, GNS, GBS, RMC, VTG, DTM, VLW, GSV, GSA, GRS, ZDA, GST, TXT, GBQ, GLQ, GNQ, GPQ
};
void loadMessageLayouts();
vector<string> readNmeaLogs(const string& fileName);
string extractMessageType(string nmeaLog);
vector<string> getMessageLayoutbyMessageType(string messageType);
vector<bool> validateCheckSum(vector<string> nmeaLog);
vector<string> isolateErrorLogs(vector<string>& nmeaLogs, vector<bool> validationResults);
vector<string> parseNmeaLog(string nmeaLog, vector<string> messageLayout);
void exportParsedMessagesToCSV(const vector<vector<string>>& allParsedMessages, const vector<vector<string>>& allMessageLayouts, const string& outputFileName);
void exportParsedMessagesToKML(const vector<vector<string>>& allParsedMessages, const vector<vector<string>>& allMessageLayouts, const string& outputFileName);
void exportErrorLogsToCSV(const vector<string>& errorLogs, const string& outputFileName);
// Helper function
vector<string> splitByDelimeter(string str, char delimiter);
FieldType stringToFieldType(string fieldTypeString);
MessageID stringToMessageID(string messageTypeString);

int main(){
    vector<vector<string>> allParsedMessages;
    vector<vector<string>> allMessageLayouts;
    loadMessageLayouts();
    vector<string> nmeaLogs = readNmeaLogs("NMEALogs.txt");
    vector<bool> validationResults = validateCheckSum(nmeaLogs);
    vector<string> errorLogs = isolateErrorLogs(nmeaLogs, validationResults);
    for(const string& log : nmeaLogs){
        vector<string> messageLayout = getMessageLayoutbyMessageType(extractMessageType(log));
        vector<string> parsedMessage = parseNmeaLog(log, messageLayout);
        allParsedMessages.push_back(parsedMessage);
        allMessageLayouts.push_back(messageLayout);
    }
    exportParsedMessagesToKML(allParsedMessages, allMessageLayouts, "ParsedNMEAMessages.kml");
    exportParsedMessagesToCSV(allParsedMessages, allMessageLayouts, "ParsedNMEAMessages.csv");
    exportErrorLogsToCSV(errorLogs, "ErrorNMEALogs.csv");
    return 0;
}
