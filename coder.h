#ifndef CODER_H
#define CODER_H

#include "decoder.h"
#include <string>

class Coder : public Decoder {
public:
    std::string getUserInput() const override;
    std::string encodeString(const std::string& input) const;
    void displayResult(const std::string& result) const;

private:
    std::string encodeBlock(uint32_t block, int bytesCount = 4) const;
};

#endif
