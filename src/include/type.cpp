#include <cstdint>
#include <string>

#pragma once

namespace Vechicle {

enum Fuel {
  petrol,
  hybird,
  ev,
};

enum Type {
  Hatchback, // 1-3
  Sedan,     // 2-5
  SUV,       // 4-7
  MPV,       // 6-8
};

enum Purpose {
  Student,       // Proton e.MAS 5
  City_Driving,  // BYD Dolphin
  Family,        // Proton e.MAS 7, BYD Atto 3
  Long_Distance, // Tesla Model 3, Tesla Model Y
  Performance,   // BYD Seal, Porsche Taycan
  Technology,    // Xpeng G6
  Luxury,        //	BMW iX1
};

struct VL {
  std::string model; // dynamic range of the model
  Type BodyType;     // vehicle size
  double price;      // price (rm)
  uint64_t range;    //  distance range (km)
  Purpose purpose;   // purpose
};
} // namespace Vechicle
