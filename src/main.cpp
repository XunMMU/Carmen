/// @file main.cpp
/// @brief Malaysia EV Recommendation System — Carmen
/// @description CLI tool that gathers user criteria (budget, distance,
///              passengers, weather, charging, purpose), then filters
///              and scores 16 EV models to recommend the best match.
/// @author <XunMMU>
/// @author A-chan (opencode)
/// @date 2026-06-29
/// @partof LDCW6123-Project

#include <cctype>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

/// @enum Weather
/// @brief Driving weather conditions affecting SUV score adjustment.
enum Weather { Normal, Heavy_Rain, Flood_Prone };
/// @enum BodyType
/// @brief Vehicle body categories used for passenger-based recommendation.
enum BodyType { Hatchback, Sedan, SUV, MPV };
/** @enum Purpose
 *  @brief Driving-purpose personas used for purpose-matching score bonus.
 *  @var Student         Budget-conscious, typically first car
 *  @var City_Driving    Daily commute and urban use
 *  @var Family          Family-oriented, space and safety
 *  @var Long_Distance   Highway travel, range priority
 *  @var Performance     Speed and driving excitement
 *  @var Technology      Tech-savvy, latest features
 *  @var Luxury          Premium comfort and brand prestige
 */
enum Purpose {
  Student,
  City_Driving,
  Family,
  Long_Distance,
  Performance,
  Technology,
  Luxury
};

/** @struct EVModel
 *  @brief Data model for an electric vehicle entry in the catalogue.
 *  @var model    Display name of the EV
 *  @var price    Price in Malaysian Ringgit (RM)
 *  @var range    Driving range in kilometres
 *  @var body     Body type classification
 *  @var purpose  Intended driving-purpose segment
 *  @var best_for Short human-readable description of ideal use-case
 */
struct EVModel {
  std::string model;
  int price;
  int range;
  BodyType body;
  Purpose purpose;
  std::string best_for;
};

/** @brief Maps a BodyType enum to its human-readable string.
 *  @param b The body type value
 *  @return std::string representation (e.g. "SUV")
 */
std::string body_name(BodyType b) {
  switch (b) {
  case Hatchback:
    return "Hatchback";
  case Sedan:
    return "Sedan";
  case SUV:
    return "SUV";
  case MPV:
    return "MPV";
  }
  return "";
}

/** @brief Maps a Purpose enum to its human-readable string.
 *  @param p The purpose value
 *  @return std::string representation (e.g. "Long Distance")
 */
std::string purpose_name(Purpose p) {
  switch (p) {
  case Student:
    return "Students";
  case City_Driving:
    return "City Driving";
  case Family:
    return "Family";
  case Long_Distance:
    return "Long Distance";
  case Performance:
    return "Performance";
  case Technology:
    return "Technology Enthusiasts";
  case Luxury:
    return "Luxury";
  }
  return "";
}

/** @brief Maps a Weather enum to its human-readable string.
 *  @param w The weather value
 *  @return std::string representation (e.g. "Heavy Rain")
 */
std::string weather_name(Weather w) {
  switch (w) {
  case Normal:
    return "Normal";
  case Heavy_Rain:
    return "Heavy Rain";
  case Flood_Prone:
    return "Flood-Prone Area";
  }
  return "";
}

/** @brief Returns the full EV catalogue.
 *  @return Vector of 16 EVModel entries covering Malaysian-market EVs
 */
std::vector<EVModel> all_evs() {
  return {
      {"Proton e.MAS 5", 56800, 325, Hatchback, Student, "Students"},
      {"Proton e.MAS 7", 99800, 410, SUV, Family, "Family"},
      {"BYD Dolphin", 100530, 490, Hatchback, City_Driving, "City driving"},
      {"BYD Atto 3", 125800, 480, SUV, Family, "Family"},
      {"BYD Seal", 171800, 570, Sedan, Performance, "Performance"},
      {"Tesla Model 3", 189000, 629, Sedan, Long_Distance, "Long distance"},
      {"Tesla Model Y", 199000, 622, SUV, Family, "Family and travel"},
      {"MG4 EV", 103999, 520, Hatchback, Student, "Young users"},
      {"MG ZS EV", 125999, 440, SUV, City_Driving, "Daily commute"},
      {"ORA Good Cat", 109800, 500, Hatchback, City_Driving, "Urban driving"},
      {"Neta V", 100000, 384, Hatchback, Student, "Budget"},
      {"Neta X", 119888, 480, SUV, Family, "Family"},
      {"Xpeng G6", 166000, 570, SUV, Technology, "Technology enthusiasts"},
      {"Zeekr X", 155800, 440, SUV, Luxury, "Premium users"},
      {"BMW iX1", 280000, 440, SUV, Luxury, "Luxury"},
      {"Porsche Taycan", 575000, 500, Sedan, Performance, "Performance"},
  };
}

/** @brief Recommends a body type based on passenger count.
 *  @param n Number of passengers
 *  @return BodyType — MPV for 6+, SUV for 4–5, Sedan for 2–3, Hatchback for 1
 */
BodyType body_for_passengers(int n) {
  if (n >= 6)
    return MPV;
  if (n >= 4)
    return SUV;
  if (n >= 2)
    return Sedan;
  return Hatchback;
}

/** @brief Prompts the user and reads a non-negative integer with validation.
 *  @param prompt The message displayed to the user
 *  @return Validated non-negative integer input
 */
int get_int(const std::string &prompt) {
  int val;
  while (true) {
    std::cout << prompt;
    std::cin >> val;
    if (std::cin.fail() || val < 0) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "  (invalid input, try again)\n";
    } else {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return val;
    }
  }
}

/** @brief Removes leading and trailing whitespace (spaces/tabs) in-place.
 *  @param str String to trim (modified in-place)
 */
void trim(std::string &str) {
  str.erase(0, str.find_first_not_of(" \t"));
  str.erase(str.find_last_not_of(" \t") + 1);
}

/** @brief Prompts the user for weather conditions with case-insensitive
 * matching.
 *  @return Weather enum (Normal, Heavy_Rain, or Flood_Prone)
 */
Weather getWeather() {
  while (true) {
    std::cout << "Weather condition [Normal / Heavy Rain / Flood]: ";
    std::string weather_str;
    std::getline(std::cin, weather_str);
    trim(weather_str);
    {
      std::string lower;
      for (char c : weather_str)
        lower += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
      if (lower.find("normal") != std::string::npos)
        return Normal;
      else if (lower.find("heavy rain") != std::string::npos)
        return Heavy_Rain;
      else if (lower.find("flood") != std::string::npos)
        return Flood_Prone;
      else {
        std::cout << "  (invalid input, try again)\n";
      }
    }
  }
}

/** @brief Asks whether charging is available nearby (Y/n).
 *  @return true if charging available, false otherwise
 */
bool getCharging() {
  while (true) {
    std::cout << "Charging station available nearby? [Y/n]: ";
    std::string boolstr;
    std::getline(std::cin, boolstr);
    trim(boolstr);
    if (boolstr.size() != 1) {
      std::cout << "  (invalid input, try again)\n";
      continue;
    }
    switch (boolstr[0]) {
    case 'y':
    case 'Y':
      return true;
    case 'n':
    case 'N':
      return false;
    default:
      std::cout << "  (invalid input, try again)\n";
    }
  }
}

/** @brief Prompts for driving purpose and maps input to a Purpose enum.
 *  @return Purpose value matching the user's input
 */
Purpose parse_purpose() {
  while (true) {
    std::cout << "Driving purpose [Student / City / Family / Long Distance / "
                 "Performance / Technology / Luxury]: ";
    std::string purpose_str;
    std::getline(std::cin, purpose_str);
    trim(purpose_str);

    std::string lower;
    lower.reserve(purpose_str.size());
    for (char c : purpose_str)
      lower += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    if (lower == "student")
      return Student;
    if (lower == "city" || lower == "city driving")
      return City_Driving;
    if (lower == "family")
      return Family;
    if (lower == "long distance" || lower == "long")
      return Long_Distance;
    if (lower == "performance")
      return Performance;
    if (lower == "technology")
      return Technology;
    if (lower == "luxury")
      return Luxury;
    std::cout << "  (invalid input, try again)\n";
  }
}

/** @brief Entry point. Orchestrates user input, filtering, scoring, and output.
 *  @details Walks the user through budget, distance, passenger, weather,
 *           charging, and purpose questions. If no charging is available,
 *           recommends a hybrid/petrol vehicle. Otherwise scores eligible EVs
 *           and outputs the best match with reasons.
 *  @return 0 on successful completion
 */
int main() {
  std::cout << "=== Malaysia EV Recommendation System ===\n\n";

  int budget = get_int("Enter your budget (RM): ");
  int distance = get_int("Enter planned trip distance (km): ");
  int passengers = 0;
  while (true) {
    passengers = get_int("Enter number of passengers: ");
    if (passengers > 0) {
      break;
    }
    std::cout << "  (invalid input, try again)\n";
  }

  // numerical value

  Weather weather = getWeather();

  bool charging_available = getCharging();

  if (!charging_available) {
    std::cout << "\n--------------------------------------------------\n";
    std::cout << "No charging station available nearby.\n";
    std::cout << "Recommendation: Hybrid or Petrol vehicle\n";
    std::cout
        << "Reason: Limited charging infrastructure, high range anxiety.\n";
    std::cout << "--------------------------------------------------\n";
    return 0;
  }

  Purpose drv_purpose = parse_purpose();

  /// @section scoring Scoring Algorithm
  /// Each eligible EV starts at 60 base points, then:
  /// - +10 if body type matches passenger-based recommendation
  /// - +15 if purpose matches user's driving purpose
  /// - +15 if weather is non-normal and body is SUV (or -10 otherwise)
  /// Score is clamped to [0, 100]. Highest-scoring EV is recommended.

  BodyType desired_body = body_for_passengers(passengers);
  std::vector<EVModel> pool = all_evs();
  std::vector<const EVModel *> eligible;

  for (const auto &ev : pool) {
    if (ev.price > budget)
      continue;
    if (ev.range < distance)
      continue;
    eligible.push_back(&ev);
  }

  if (eligible.empty()) {
    std::cout << "\n--------------------------------------------------\n";
    std::cout << "No EV matches all your criteria.\n";
    std::cout << "Consider increasing budget, reducing trip distance, or "
                 "looking at Hybrid/Petrol vehicles.\n";
    std::cout << "--------------------------------------------------\n";
    return 0;
  }

  const EVModel *best = nullptr;
  int best_score = 0;
  std::vector<std::string> best_reasons;

  for (const auto *ev : eligible) {
    int score = 60;
    std::vector<std::string> reasons;
    reasons.push_back("Within budget (RM" + std::to_string(ev->price) +
                      " =< RM" + std::to_string(budget) + ")");
    reasons.push_back("Enough range (" + std::to_string(ev->range) +
                      " km >= " + std::to_string(distance) + " km)");
    reasons.push_back("Suitable for " + std::to_string(passengers) +
                      " passenger(s) (" + body_name(ev->body) + ")");

    if (ev->body == desired_body) {
      score += 10;
    }

    if (ev->purpose == drv_purpose) {
      score += 15;
      reasons.push_back("Matches your driving purpose (" +
                        purpose_name(ev->purpose) + ")");
    }

    if (weather != Normal && ev->body == SUV) {
      score += 15;
      reasons.push_back("SUV is suitable for " + weather_name(weather) +
                        " weather");
    } else if (weather != Normal && ev->body != SUV) {
      score -= 10;
      reasons.push_back("Not ideal for " + weather_name(weather) +
                        " weather (consider SUV)");
    }

    if (score > 100)
      score = 100;
    if (score < 0)
      score = 0;

    if (!best || score > best_score) {
      best = ev;
      best_score = score;
      best_reasons = reasons;
    }
  }

  std::cout << "\n==================================================\n";
  std::cout << "Recommended Vehicle Type: " << "EV" << "\n";
  std::cout << "Best Match: " << best->model << "\n";
  std::cout << "\nDetails:\n";
  std::cout << "  - Price: RM" << best->price << "\n";
  std::cout << "  - Range: " << best->range << " km\n";
  std::cout << "  - Body Type: " << body_name(best->body) << "\n";
  std::cout << "  - Best For: " << best->best_for << "\n";
  std::cout << "\nCompatibility Score: " << best_score << "%\n";
  std::cout << "\nReason:\n";
  for (const auto &r : best_reasons) {
    std::cout << "  - " << r << "\n";
  }
  std::cout << "==================================================\n";

  return 0;
}
