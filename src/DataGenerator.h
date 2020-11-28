#include "Particle.h"
#include "Arduino.h"

class DataGenerator {
  public:
    int get();

  private:
    int generateRandomInt();
};