//
// Created by Luis Cardoza Bird on 10/4/24.
//

#include <iostream>
#include "library.h"

int main() {
const char *sentence = "luz alta";

    char* m = stringToMorse(sentence);
    printf("Morse: %s\n", m);
    free(m);


    size_t count;

int* haptics = stringToHapticMorse(sentence, &count);
    printf("Haptic (%zu durations):", count);
    for (size_t i = 0; i < count; ++i)
        printf(" %d", haptics[i]);
    printf("\n");
    free(haptics);
return 0;

}
