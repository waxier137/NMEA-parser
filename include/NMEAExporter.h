#ifndef NMEA_EXPORTER_H
#define NMEA_EXPORTER_H

#include "NMEADefinitions.h"
#include <vector>
#include <string>

void exportParsedMessagesToCSV(const std::vector<NMEAMessage*>& validMessages, const std::string& outputFileName);

void exportErrorLogsToCSV(const std::vector<std::string>& errorMessages, const std::string& outputFileName);

void exportParsedMessagesToKML(const std::vector<NMEAMessage*>& validMessages, const std::string& outputFileName);

#endif