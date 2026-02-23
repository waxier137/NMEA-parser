#ifndef NMEA_VALIDATOR_H
#define NMEA_VALIDATOR_H

#include "NMEADefinitions.h"
#include <iostream>
#include <string>
#include <vector>

std::vector<bool> validateCheckSum(std::vector<std::string> nmeaLog);

#endif