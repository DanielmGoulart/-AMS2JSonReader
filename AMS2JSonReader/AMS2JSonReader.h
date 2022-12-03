// AMS2JSonReader.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <codecvt>
#include <cassert>
#include <locale>

// TODO: Reference additional headers your program requires here.
#include "json.hpp"

struct Lap
{
    int lapTime;
    int sector1;
    int sector2;
    int sector3;
    bool validLap;
    int64_t vehicle;
    int64_t track;
};

struct Player
{
    std::string name;
    int64_t vehicle;
    std::vector<Lap> laps;
};

struct Participant
{
    std::string name;
    int64_t vehicle;
};