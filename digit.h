#pragma once

namespace AlphaNumeric{

bool inline isLowerAlphabetic(uint8_t digit){ return digit >= 'a' && digit <= 'z'; };
bool inline isUpperAlphabetic(uint8_t digit){ return digit >= 'A' && digit <= 'Z'; };
bool inline isAlphabetic(uint8_t digit){ return isLowerAlphabetic(digit) || isUpperAlphabetic(digit); };
bool inline isNumeric(uint8_t digit){ return digit >= '0' && digit <= '9'; };
bool inline isAlphaNumeric(uint8_t digit){ return isNumeric(digit) || isAlphabetic(digit); };


}