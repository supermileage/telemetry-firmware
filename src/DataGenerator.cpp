/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/silviu/github/SMC-practice/src/DataGenerator.ino"
#include "DataGenerator.h"

// Returns a random number from 0 to 9
#line 4 "/Users/silviu/github/SMC-practice/src/DataGenerator.ino"
int DataGenerator::get() {
    return generateRandomInt();
}  

int DataGenerator::generateRandomInt() {
    return random(0,10);
}

