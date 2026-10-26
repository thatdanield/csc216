#include <iostream>
#include <string>

#include "HashMap.h"

bool canCreateNote(const std::string& note, const std::string articles[], int articleCount) {
    HashMap availableCharacters;
    for (int articleIndex = 0; articleIndex < articleCount; articleIndex++) {
        availableCharacters.add(articles[articleIndex]);
    }

    for (std::string::size_type characterIndex = 0; characterIndex < note.size(); characterIndex++) {
        char character = note[characterIndex];
        if (!HashMap::supports(character)) {
            continue;
        }
        if (availableCharacters.count(character) == 0) {
            return false;
        }
        availableCharacters.remove(character);
    }

    return true;
}

#ifndef NOTE_TESTS
int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
#endif
