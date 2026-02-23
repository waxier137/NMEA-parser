#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include <vector>
#include <string>

std::vector<std::string> readNMEALogs(const std::string& fileName);

std::string extractMessageID(const std::string& nmeaLog);
std::vector<std::string> splitByDelimiter(const std::string& str, char delimiter);
void trimWhitespace(std::string& str);

bool validateChecksum(const std::string& nmeaLog);

#endif