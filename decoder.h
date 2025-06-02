#ifndef DECODER_H
#define DECODER_H

#include <vector>
#include <string>
#include <cstdint>

class Decoder {
protected:
    bool testMode = false;
    
public:
    void setTestMode(bool mode);
    bool isTestMode() const;
    virtual std::string getUserInput() const;
    std::vector<uint8_t> decodeData(const std::string& input) const;
    void displayResult(const std::vector<uint8_t>& result) const;
};

#endif
