//
// Created by Daniel Devadas on 10/25/26.
//

#include "HashMap.h"

#include <limits>

namespace {

bool keyFor(char character, std::size_t& key) {
    unsigned char ascii = static_cast<unsigned char>(character);
    if (ascii >= 'A' && ascii <= 'Z') {
        ascii = static_cast<unsigned char>(ascii - 'A' + 'a');
    }

    const bool lowercase = ascii >= 'a' && ascii <= 'z';
    const bool digit = ascii >= '0' && ascii <= '9';
    const bool punctuationOrSymbol =
        (ascii >= '!' && ascii <= '/') ||
        (ascii >= ':' && ascii <= '@') ||
        (ascii >= '[' && ascii <= '`') ||
        (ascii >= '{' && ascii <= '~');
    if (!lowercase && !digit && !punctuationOrSymbol) {
        return false;
    }

    key = ascii;
    return true;
}

}

bool HashMap::supports(char character) {
    std::size_t key;
    return keyFor(character, key);
}

void HashMap::add(char character) {
    std::size_t key;
    if (keyFor(character, key) && counts[key] < std::numeric_limits<std::size_t>::max()) {
        ++counts[key];
    }
}

void HashMap::add(const std::string& text) {
    for (char character : text) {
        add(character);
    }
}

void HashMap::remove(char character) {
    std::size_t key;
    if (keyFor(character, key) && counts[key] > 0) {
        --counts[key];
    }
}

std::size_t HashMap::count(char character) const {
    std::size_t key;
    return keyFor(character, key) ? counts[key] : 0;
}
