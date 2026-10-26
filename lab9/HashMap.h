//
// Created by Daniel Devadas on 10/25/26.
//

#ifndef LAB9_HASHMAP_H
#define LAB9_HASHMAP_H
#include <array>
#include <cstddef>
#include <string>

class HashMap {
private:
    std::array<std::size_t, 128> counts{};
public:
    void add(char character);
    void add(const std::string& text);
    void remove(char character);
    std::size_t count(char character) const;
    static bool supports(char character);
};


#endif //LAB9_HASHMAP_H
