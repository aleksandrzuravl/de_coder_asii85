#include <gtest/gtest.h>
#include "decoder.h"
#include "coder.h"
#include <sstream>

class CodecTest : public ::testing::Test {
protected:
    void SetUp() override {
        encoder.setTestMode(true);
        decoder.setTestMode(true);
    }

    Coder encoder;
    Decoder decoder;
};

// ===================== Decoder Tests =====================

TEST_F(CodecTest, DecodesValidASCII85) {
    std::string input = "87cURD]j,BOQ79R";
    std::vector<uint8_t> expected = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
    
    auto result = decoder.decodeData(input);
    EXPECT_EQ(result, expected);
}

TEST_F(CodecTest, DecodesZeroSequence) {
    auto result = decoder.decodeData("z");
    std::vector<uint8_t> expected(4, 0);
    EXPECT_EQ(result, expected);
}

TEST_F(CodecTest, ThrowsOnInvalidCharacters) {
    EXPECT_THROW(decoder.decodeData("87cUR!@#"), std::runtime_error);
}

TEST_F(CodecTest, ThrowsOnIncorrectZposition) {
    EXPECT_THROW(decoder.decodeData("Azzz"), std::runtime_error);
}

TEST_F(CodecTest, HandlesPartialBlocks) {
    std::string input = "6Zm!";
    std::vector<uint8_t> expected = {'t', 'e', 's', 't'};
    
    auto result = decoder.decodeData(input);
    EXPECT_EQ(result, expected);
}

// ===================== Coder Tests =====================

TEST_F(CodecTest, EncodesBasicString) {
    std::string input = "test";
    std::string expected = "6Zm!";
    
    auto result = encoder.encodeString(input);
    EXPECT_EQ(result, expected);
}

TEST_F(CodecTest, EncodesZeroSequence) {
    std::string input(4, '\0');
    std::string expected = "z";
    
    auto result = encoder.encodeString(input);
    EXPECT_EQ(result, expected);
}

TEST_F(CodecTest, EncodesPartialBlocks) {
    std::string input = "Hello";
    std::string expected = "87cURD]j";
    
    auto result = encoder.encodeString(input);
    EXPECT_EQ(result, expected);
}

// ===================== Roundtrip Tests =====================

TEST_F(CodecTest, EncodesDecodesRoundtrip) {
    std::string original = "The quick brown fox jumps over the lazy dog";
    
    std::string encoded = encoder.encodeString(original);
    auto decoded = decoder.decodeData(encoded);
    std::string result(decoded.begin(), decoded.end());
    
    EXPECT_EQ(original, result);
}

// ===================== IO Tests =====================

TEST_F(CodecTest, PrintsEncodedResult) {
    std::string testData = "6Zm!";
    std::string expected = "\nРезультат кодирования: 6Zm!\n";
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    encoder.displayResult(testData);
    std::cout.rdbuf(old);
    
    EXPECT_EQ(buffer.str(), expected);
}

TEST_F(CodecTest, PrintsDecodedResult) {
    std::vector<uint8_t> testData = {'t', 'e', 's', 't'};
    std::string expected = "\nРезультат декодирования: test\n";
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    decoder.displayResult(testData);
    std::cout.rdbuf(old);
    
    EXPECT_EQ(buffer.str(), expected);
}

TEST_F(CodecTest, ThrowsOnEmptyDecodedData) {
    std::vector<uint8_t> emptyData;
    EXPECT_THROW(decoder.displayResult(emptyData), std::invalid_argument);
}

// ===================== Main =====================

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
