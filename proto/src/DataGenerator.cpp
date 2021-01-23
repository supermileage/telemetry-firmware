#include "DataGenerator.h"

// Returns a random number from 0 to 9
int DataGenerator::get() {
    return generateRandomInt();
}  

int DataGenerator::generateRandomInt() {
    return random(0,10);
}

