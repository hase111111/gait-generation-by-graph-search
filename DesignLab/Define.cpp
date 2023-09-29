#include "define.h"

#include <filesystem>


const int Define::kSimurateNum = 5;

const int Define::kGaitPatternGenerationLimit = 1000;

const int Define::kGoalTape = 1200;


const std::string Define::kResultFilePath = std::filesystem::current_path().string() + "/result";