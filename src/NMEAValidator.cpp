#include "../include/NMEADefinitions.h"
#include "../include/NMEAvalidator.h"
#include "../include/HelperFunctions.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<bool> validateCheckSum(vector<string> nmeaLogs){
    vector<bool> validateResults;
    for(const string& log : nmeaLogs){
        size_t asteriskPos = log.find('*');
        if(asteriskPos != string::npos && asteriskPos + 2 < log.size()){
            string checkSumStr = log.substr(asteriskPos + 1, 2);
            int checkSumValue = stoi(checkSumStr, nullptr, 16);
            int calculatedChecksum = 0;
            for(size_t i=1; i < asteriskPos; i++){
                calculatedChecksum ^= log[i];
            }
            cout<<calculatedChecksum<<" vs "<<checkSumValue<<endl;
            validateResults.push_back(calculatedChecksum == checkSumValue);
        } else {
            validateResults.push_back(false);
        }
    }    
    return validateResults;
}