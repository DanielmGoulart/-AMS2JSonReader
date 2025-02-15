﻿// AMS2JSonReader.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <codecvt>
#include <cassert>
#include <locale>
#include <thread>
#include <chrono>
#include <algorithm>
#include <curl/curl.h>

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

    bool operator < (const Lap& str) const
    {
        return (lapTime < str.lapTime);
    }
};

struct FastestLap
{
    std::string name;
    Lap lap;
    int totalLaps;
    int allLapsIndex;

    bool operator < (const FastestLap& str) const
    {
        return (lap.lapTime < str.lap.lapTime);
    }
};

char vehicles[] = R"({
    "result" : "ok",
    "response" : {
      "description" : "All known vehicles. Each structure contains the vehicle's id, name and (optionally) class.",
      "list" : [
        {
          "id" : 1368036017,
          "name" : "BMW M1 Procar",
          "class" : "Procar"
        },
        {
          "id" : -934098507,
          "name" : "BMW M3 Sport Evo Group A",
          "class" : "Group A"
        },
        {
          "id" : 262982797,
          "name" : "Mercedes-Benz 190E 2.5-16 Evo II DTM",
          "class" : "Group A"
        },
        {
          "id" : 1330326301,
          "name" : "MINI Cooper S 1965",
          "class" : "TC60S2"
        },
        {
          "id" : 400701189,
          "name" : "MINI Cooper S 1965 B",
          "class" : "CopaClassicB"
        },
        {
          "id" : -830134672,
          "name" : "Lotus 23",
          "class" : "TC60S2"
        },
        {
          "id" : 1818602836,
          "name" : "MINI Cooper JCW",
          "class" : "MiniChallenge"
        },
        {
          "id" : 912279169,
          "name" : "Cadillac DPi-VR",
          "class" : "DPI"
        },
        {
          "id" : -950775810,
          "name" : "Caterham Academy",
          "class" : "Cat_Academy"
        },
        {
          "id" : 1864701845,
          "name" : "Caterham 620R",
          "class" : "Cat620R"
        },
        {
          "id" : -1660644383,
          "name" : "Caterham Superlight",
          "class" : "Cat_Superlight"
        },
        {
          "id" : 851522805,
          "name" : "Caterham Supersport",
          "class" : "Cat_Supersport"
        },
        {
          "id" : -2123166417,
          "name" : "Chevrolet Camaro SS",
          "class" : "Street"
        },
        {
          "id" : 428770333,
          "name" : "Dallara F301",
          "class" : "F-3"
        },
        {
          "id" : 302183120,
          "name" : "Dallara F309",
          "class" : "F-3"
        },
        {
          "id" : 697161929,
          "name" : "Formula Classic Gen1 Model1",
          "class" : "F-Classic_Gen1"
        },
        {
          "id" : -1819510420,
          "name" : "Formula Classic Gen1 Model2",
          "class" : "F-Classic_Gen1"
        },
        {
          "id" : -1667467124,
          "name" : "Formula Classic Gen2 Model1",
          "class" : "F-Classic_Gen2"
        },
        {
          "id" : -1004811218,
          "name" : "Formula Classic Gen2 Model2",
          "class" : "F-Classic_Gen2"
        },
        {
          "id" : -494068343,
          "name" : "Formula Classic Gen2 Model3",
          "class" : "F-Classic_Gen2"
        },
        {
          "id" : -1700189536,
          "name" : "Formula Classic Gen3 Model1",
          "class" : "F-Classic_Gen3"
        },
        {
          "id" : -1662617552,
          "name" : "Formula Classic Gen3 Model2",
          "class" : "F-Classic_Gen3"
        },
        {
          "id" : -1828044943,
          "name" : "Formula Classic Gen3 Model3",
          "class" : "F-Classic_Gen3"
        },
        {
          "id" : -1261768631,
          "name" : "Formula Vintage Gen1 Model1",
          "class" : "F-Vintage_Gen1"
        },
        {
          "id" : 318400650,
          "name" : "Formula Vintage Gen1 Model2",
          "class" : "F-Vintage_Gen1"
        },
        {
          "id" : -1988395354,
          "name" : "Formula Vintage Gen2 Model1",
          "class" : "F-Vintage_Gen2"
        },
        {
          "id" : 617290135,
          "name" : "Formula Vintage Gen2 Model2",
          "class" : "F-Vintage_Gen2"
        },
        {
          "id" : -1201605905,
          "name" : "Brabham BT26A",
          "class" : "F-Vintage_Gen2"
        },
        {
          "id" : 1061494025,
          "name" : "Lotus Type 49C Cosworth",
          "class" : "F-Vintage_Gen2"
        },
        {
          "id" : 435621847,
          "name" : "Lotus Type 72E",
          "class" : "F-Retro_Gen1"
        },
        {
          "id" : -1025098540,
          "name" : "Brabham BT44",
          "class" : "F-Retro_Gen1"
        },
        {
          "id" : 1437730287,
          "name" : "Formula Retro V8",
          "class" : "F-Retro_Gen1"
        },
        {
          "id" : -532210519,
          "name" : "Formula Retro V12",
          "class" : "F-Retro_Gen1"
        },
        {
          "id" : 553963368,
          "name" : "McLaren M23",
          "class" : "F-Retro_Gen1"
        },
        {
          "id" : 369722929,
          "name" : "Formula Retro Gen2",
          "class" : "F-Retro_Gen2"
        },
        {
          "id" : 1439296671,
          "name" : "Lotus 79",
          "class" : "F-Retro_Gen2"
        },
        {
          "id" : 476615325,
          "name" : "Brabham BT46B",
          "class" : "F-Retro_Gen2"
        },
        {
          "id" : 2005556145,
          "name" : "McLaren MP4/1C",
          "class" : "F-Retro_Gen3"
        },
        {
          "id" : -953316099,
          "name" : "Formula Retro Gen3 DFY",
          "class" : "F-Retro_Gen3"
        },
        {
          "id" : 1408558396,
          "name" : "Formula Retro Gen3 Turbo",
          "class" : "F-Retro_Gen3"
        },
        {
          "id" : -2140090167,
          "name" : "Formula Vee",
          "class" : "F-Vee"
        },
        {
          "id" : -186413128,
          "name" : "Formula Vee Fin",
          "class" : "F-Vee"
        },
        {
          "id" : 575788923,
          "name" : "Formula Trainer",
          "class" : "F-Trainer"
        },
        {
          "id" : 1982976051,
          "name" : "Formula Trainer Advanced",
          "class" : "F-Trainer_A"
        },
        {
          "id" : 1948072701,
          "name" : "Formula Reiza",
          "class" : "F-Reiza"
        },
        {
          "id" : -1142039519,
          "name" : "Formula Ultimate 2019",
          "class" : "F-Ultimate"
        },
        {
          "id" : -377858367,
          "name" : "Formula Ultimate Gen2",
          "class" : "F-Ultimate_Gen2"
        },
        {
          "id" : 523915852,
          "name" : "Formula V10 Gen2",
          "class" : "F-V10_Gen2"
        },
        {
          "id" : 884102371,
          "name" : "Formula V10 Gen1",
          "class" : "F-V10_Gen1"
        },
        {
          "id" : -1766950841,
          "name" : "McLaren MP4/12",
          "class" : "F-V10_Gen1"
        },
        {
          "id" : -2053858829,
          "name" : "Formula V12",
          "class" : "F-V12"
        },
        {
          "id" : -487937394,
          "name" : "Iveco Stralis",
          "class" : "CopaTruck"
        },
        {
          "id" : -819133010,
          "name" : "MAN TGX",
          "class" : "CopaTruck"
        },
        {
          "id" : 1647822272,
          "name" : "Mercedes-Benz Actros",
          "class" : "CopaTruck"
        },
        {
          "id" : -2086797102,
          "name" : "VW Constellation",
          "class" : "CopaTruck"
        },
        {
          "id" : -619309786,
          "name" : "Vulkan Truck",
          "class" : "CopaTruck"
        },
        {
          "id" : 310900789,
          "name" : "Ginetta G40",
          "class" : "GT5"
        },
        {
          "id" : -751207847,
          "name" : "Ginetta G40 Cup",
          "class" : "G40Cup"
        },
        {
          "id" : 2091910841,
          "name" : "Ginetta G55 GT4 Supercup",
          "class" : "G55Supercup"
        },
        {
          "id" : 379055283,
          "name" : "Ginetta G55 GT4",
          "class" : "GT4"
        },
        {
          "id" : -1170674276,
          "name" : "Ginetta G55 GT3",
          "class" : "GTOpen"
        },
        {
          "id" : 1433352906,
          "name" : "Ginetta G58",
          "class" : "P1"
        },
        {
          "id" : -215037480,
          "name" : "Chevrolet Corvette C3",
          "class" : "TC60S"
        },
        {
          "id" : 1965836946,
          "name" : "Chevrolet Corvette C3-R",
          "class" : "TC70S"
        },
        {
          "id" : -1729418598,
          "name" : "Chevrolet Corvette C8.R",
          "class" : "GTE"
        },
        {
          "id" : 373176631,
          "name" : "Chevrolet Corvette GTP",
          "class" : "Group C"
        },
        {
          "id" : 24108228,
          "name" : "Porsche 911 RSR GTE",
          "class" : "GTE"
        },
        {
          "id" : 901145560,
          "name" : "BMW M8 GTE",
          "class" : "GTE"
        },
        {
          "id" : -241187148,
          "name" : "BMW M6 GT3",
          "class" : "GT3"
        },
        {
          "id" : -1966031044,
          "name" : "BMW M4 GT4",
          "class" : "GT4"
        },
        {
          "id" : 143364290,
          "name" : "BMW 2002 Turbo",
          "class" : "TC60S"
        },
        {
          "id" : 1353949246,
          "name" : "Mercedes-AMG GT3",
          "class" : "GT3"
        },
        {
          "id" : -855144880,
          "name" : "Mercedes-AMG GT4",
          "class" : "GT4"
        },
        {
          "id" : 809291220,
          "name" : "Porsche 911 GT3 R",
          "class" : "GT3"
        },
        {
          "id" : 2144142462,
          "name" : "Porsche 911 GT3 Cup 3.8",
          "class" : "Carrera Cup"
        },
        {
          "id" : -109705417,
          "name" : "Porsche 911 GT3 Cup 4.0",
          "class" : "Carrera Cup"
        },
        {
          "id" : 1464988033,
          "name" : "Porsche Cayman GT4 Clubsport MR",
          "class" : "GT4"
        },
        {
          "id" : 850609487,
          "name" : "McLaren 570S GT4",
          "class" : "GT4"
        },
        {
          "id" : -310556497,
          "name" : "McLaren 720S GT3",
          "class" : "GT3"
        },
        {
          "id" : -2019695308,
          "name" : "Chevrolet Camaro GT4.R",
          "class" : "GT4"
        },
        {
          "id" : -1001569309,
          "name" : "McLaren F1 GTR",
          "class" : "GT1"
        },
        {
          "id" : 1979398129,
          "name" : "Mercedes-Benz CLK LM",
          "class" : "GT1"
        },
        {
          "id" : 1076438091,
          "name" : "Porsche 911 GT1-98",
          "class" : "GT1"
        },
        {
          "id" : 957632269,
          "name" : "Porsche 962C",
          "class" : "Group C"
        },
        {
          "id" : 65306143,
          "name" : "Sauber Mercedes C9",
          "class" : "Group C"
        },
        {
          "id" : 844159614,
          "name" : "Kart 125cc 2STROKE",
          "class" : "Kart125cc"
        },
        {
          "id" : -2116593279,
          "name" : "Kart GX390 Rental",
          "class" : "KartRental"
        },
        {
          "id" : 374810616,
          "name" : "Kart GX390 Race",
          "class" : "KartGX390"
        },
        {
          "id" : -739789710,
          "name" : "Kart 125cc Shifter",
          "class" : "KartShifter"
        },
        {
          "id" : 95104745,
          "name" : "MCR S2000",
          "class" : "P4"
        },
        {
          "id" : -204135982,
          "name" : "McLaren F1 LM",
          "class" : "Street"
        },
        {
          "id" : 1932261404,
          "name" : "MetalMoro AJR Chevrolet",
          "class" : "P1"
        },
        {
          "id" : -1236687924,
          "name" : "MetalMoro AJR Honda",
          "class" : "P1"
        },
        {
          "id" : -1745773963,
          "name" : "MetalMoro AJR Judd",
          "class" : "P1"
        },
        {
          "id" : 1618401665,
          "name" : "MetalMoro AJR Nissan",
          "class" : "P1"
        },
        {
          "id" : -50694644,
          "name" : "Mitsubishi Lancer R",
          "class" : "LancerCup"
        },
        {
          "id" : 213908738,
          "name" : "Mitsubishi Lancer RS",
          "class" : "LancerCup"
        },
        {
          "id" : 979672157,
          "name" : "Copa Montana",
          "class" : "Montana"
        },
        {
          "id" : -1404228714,
          "name" : "Sprint Race",
          "class" : "SprintRace"
        },
        {
          "id" : 253111186,
          "name" : "Super V8",
          "class" : "SuperV8"
        },
        {
          "id" : 1323381033,
          "name" : "Chevrolet Cruze Stock Car 2019",
          "class" : "StockCarV8"
        },
        {
          "id" : 1836524676,
          "name" : "Chevrolet Cruze Stock Car 2020",
          "class" : "StockCarV8_2020"
        },
        {
          "id" : -1357079805,
          "name" : "Chevrolet Cruze Stock Car 2021",
          "class" : "StockCarV8_2021"
        },
        {
          "id" : 1767659669,
          "name" : "Chevrolet Cruze Stock Car 2022",
          "class" : "StockCarV8_2022"
        },
        {
          "id" : 619110280,
          "name" : "Toyota Corolla Stock Car 2020",
          "class" : "StockCarV8_2020"
        },
        {
          "id" : 806954123,
          "name" : "Toyota Corolla Stock Car 2021",
          "class" : "StockCarV8_2021"
        },
        {
          "id" : 539628044,
          "name" : "Toyota Corolla Stock Car 2022",
          "class" : "StockCarV8_2022"
        },
        {
          "id" : -371227432,
          "name" : "Chevrolet Omega Stock Car 1999",
          "class" : "StockCar99"
        },
        {
          "id" : 306371028,
          "name" : "Superkart 250cc",
          "class" : "SuperKart"
        },
        {
          "id" : 1560162507,
          "name" : "Ultima GTR",
          "class" : "Street"
        },
        {
          "id" : 306785397,
          "name" : "Ultima GTR Race",
          "class" : "GTOpen"
        },
        {
          "id" : 703591920,
          "name" : "MetalMoro MRX Duratec Turbo P2",
          "class" : "P2"
        },
        {
          "id" : 65202613,
          "name" : "MetalMoro MRX Honda P3",
          "class" : "P3"
        },
        {
          "id" : -181636428,
          "name" : "MetalMoro MRX Duratec P4",
          "class" : "P4"
        },
        {
          "id" : -1834081784,
          "name" : "MetalMoro MRX Duratec Turbo P3",
          "class" : "P3"
        },
        {
          "id" : 802736208,
          "name" : "Roco 001",
          "class" : "P3"
        },
        {
          "id" : -1870819346,
          "name" : "Sigma P1",
          "class" : "P2"
        },
        {
          "id" : -1201567586,
          "name" : "Chevrolet Opala Stock Cars 1979",
          "class" : "Opala79"
        },
        {
          "id" : 245459304,
          "name" : "Chevrolet Opala Stock Cars 1986",
          "class" : "Opala86"
        },
        {
          "id" : -62148492,
          "name" : "Chevrolet Opala Old Stock Race",
          "class" : "OldStock"
        },
        {
          "id" : 2016280350,
          "name" : "Porsche 911 RSR 74",
          "class" : "TC70S"
        },
        {
          "id" : -609305506,
          "name" : "Puma GTB",
          "class" : "CopaClassicFL"
        },
        {
          "id" : 1031807465,
          "name" : "Puma GTE",
          "class" : "CopaClassicB"
        },
        {
          "id" : -93205368,
          "name" : "Puma P052",
          "class" : "GT5"
        },
        {
          "id" : -1155860123,
          "name" : "Chevrolet Chevette",
          "class" : "CopaClassicB"
        },
        {
          "id" : 1775576087,
          "name" : "Copa Fusca",
          "class" : "CopaFusca"
        },
        {
          "id" : -815324367,
          "name" : "Fusca Classic FL",
          "class" : "CopaClassicFL"
        },
        {
          "id" : -844211966,
          "name" : "Copa Uno",
          "class" : "CopaUno"
        },
        {
          "id" : 311092313,
          "name" : "Uno Classic B",
          "class" : "CopaClassicB"
        },
        {
          "id" : -45848101,
          "name" : "Gol Classic FL",
          "class" : "CopaClassicFL"
        },
        {
          "id" : -333355648,
          "name" : "Gol Classic B",
          "class" : "CopaClassicB"
        },
        {
          "id" : 1661024873,
          "name" : "Passat Classic FL",
          "class" : "CopaClassicFL"
        },)"
        R"(
        {
          "id" : 1700425966,
          "name" : "Passat Classic B",
          "class" : "CopaClassicB"
        },
        {
          "id" : -1086802614,
          "name" : "Fusca 1 Hot Cars",
          "class" : "Hot Cars"
        },
        {
          "id" : 150641251,
          "name" : "Fusca 2 Hot Cars",
          "class" : "Hot Cars"
        },
        {
          "id" : 390318757,
          "name" : "Passat Hot Cars",
          "class" : "Hot Cars"
        },
        {
          "id" : 1653755268,
          "name" : "Gol Hot Cars",
          "class" : "Hot Cars"
        },
        {
          "id" : 1785300635,
          "name" : "Aussie Racing Camaro",
          "class" : "ARC_Cam"
        },
        {
          "id" : -1170240161,
          "name" : "VW Polo",
          "class" : "TSICup"
        },
        {
          "id" : -1762971386,
          "name" : "VW Polo GTS",
          "class" : "TSICup"
        },
        {
          "id" : -1137512839,
          "name" : "VW Virtus",
          "class" : "TSICup"
        },
        {
          "id" : -1310963279,
          "name" : "VW Virtus GTS",
          "class" : "TSICup"
        },
        {
          "id" : -1477576480,
          "name" : "Lola T95/00 Ford-Cosworth",
          "class" : "F-USA_Gen1"
        },
        {
          "id" : 1701517873,
          "name" : "Lola T95/00 Mercedes-Benz",
          "class" : "F-USA_Gen1"
        },
        {
          "id" : 1408764300,
          "name" : "Lola T98/00 Ford-Cosworth",
          "class" : "F-USA_Gen2"
        },
        {
          "id" : -845636149,
          "name" : "Lola B2K00 Ford-Cosworth",
          "class" : "F-USA_Gen3"
        },
        {
          "id" : 1305671580,
          "name" : "Lola B2K00 Mercedes-Benz",
          "class" : "F-USA_Gen3"
        },
        {
          "id" : -1883533152,
          "name" : "Lola B2K00 Toyota",
          "class" : "F-USA_Gen3"
        },
        {
          "id" : -794072085,
          "name" : "Reynard 95i Ford-Cosworth",
          "class" : "F-USA_Gen1"
        },
        {
          "id" : 243649723,
          "name" : "Reynard 95i Honda",
          "class" : "F-USA_Gen1"
        },
        {
          "id" : -832846400,
          "name" : "Reynard 95i Mercedes-Benz",
          "class" : "F-USA_Gen1"
        },
        {
          "id" : -406287657,
          "name" : "Reynard 98i Ford-Cosworth",
          "class" : "F-USA_Gen2"
        },
        {
          "id" : 2011027431,
          "name" : "Reynard 98i Honda",
          "class" : "F-USA_Gen2"
        },
        {
          "id" : 174857498,
          "name" : "Reynard 98i Mercedes-Benz",
          "class" : "F-USA_Gen2"
        },
        {
          "id" : -1212556546,
          "name" : "Reynard 98i Toyota",
          "class" : "F-USA_Gen2"
        },
        {
          "id" : 990042821,
          "name" : "Reynard 2Ki Ford-Cosworth",
          "class" : "F-USA_Gen3"
        },
        {
          "id" : 1530597907,
          "name" : "Reynard 2Ki Honda",
          "class" : "F-USA_Gen3"
        },
        {
          "id" : 290327508,
          "name" : "Reynard 2Ki Mercedes-Benz",
          "class" : "F-USA_Gen3"
        },
        {
          "id" : -564338836,
          "name" : "Reynard 2Ki Toyota",
          "class" : "F-USA_Gen3"
        },
        {
          "id" : 943684424,
          "name" : "Swift 009c Ford-Cosworth",
          "class" : "F-USA_Gen2"
        },
        {
          "id" : -67190685,
          "name" : "Formula Inter MG-15",
          "class" : "F-Inter"
        },
        {
          "id" : -1416203489,
          "name" : "Nissan GT-R Nismo GT3",
          "class" : "GT3"
        },
        {
          "id" : -343023508,
          "name" : "Nissan R390 GT1",
          "class" : "GT1"
        },
        {
            "id" : 1989434463,
            "name" : "Ginetta G58 Gen2",
            "class" : "P1Gen2"
        },
        {
            "id" : -1164814162,
            "name" : "MetalMoro AJR Gen2 Chevrolet",
            "class" : "P1Gen2"
        },
        {
            "id" : 995681332,
            "name" : "MetalMoro AJR Gen2 Honda",
            "class" : "P1Gen2"
        },
        {
            "id" : -68656752,
            "name" : "MetalMoro AJR Gen2 Nissan",
            "class" : "P1Gen2"
        },
        {
            "id" : -1720554236,
            "name" : "Sigma P1 G5",
            "class" : "P1Gen2"
        },
        {
            "id" : 1311912387,
            "name" : "BMW M4 GT3",
            "class" : "GT3_Gen2"
        },
        {
            "id" : 160008140,
            "name" : "Mercedes-AMG GT3 Evo",
            "class" : "GT3_Gen2"
        },
        {
            "id" : -519566358,
            "name" : "Porsche 992 GT3 R",
            "class" : "GT3_Gen2"
        },
        {
            "id" : -1026941607,
            "name" : "McLaren 720S GT3 Evo",
            "class" : "GT3_Gen2"
        },
        {
            "id" : -842343170,
            "name" : "BMW M Hybrid V8",
            "class" : "LMDh"
        },
        {
            "id" : -811404968,
            "name" : "Porsche 963",
            "class" : "LMDh"
        },
        {
            "id" : 16119375,
            "name" : "Cadillac V-Series.R",
            "class" : "LMDh"
        }
      ]
    }
  })";

char tracks[] = R"({
  "result" : "ok",
  "response" : {
    "description" : "All known track ids",
    "list" : [
      {
        "id" : -559709709,
        "name" : "Adelaide_Historic",
        "gridsize" : 26
      },
      {
        "id" : 827815091,
        "name" : "Adelaide_Modern",
        "gridsize" : 39
      },
      {
        "id" : -1939104917,
        "name" : "Azure_Circuit_2021",
        "gridsize" : 24
      },
      {
        "id" : 1080325116,
        "name" : "Bathurst_2020",
        "gridsize" : 36
      },
      {
        "id" : 1534602052,
        "name" : "BrandsHatch_GP",
        "gridsize" : 32
      },
      {
        "id" : -572148012,
        "name" : "BrandsHatch_Indy",
        "gridsize" : 32
      },
      {
        "id" : 202837760,
        "name" : "Brasilia_Full",
        "gridsize" : 30
      },
      {
        "id" : 1828328431,
        "name" : "Brasilia_Outer",
        "gridsize" : 30
      },
      {
        "id" : -1914387303,
        "name" : "Buenos_Aires_Circuito_6",
        "gridsize" : 32
      },
      {
        "id" : -44643975,
        "name" : "Buenos_Aires_Circuito_6T",
        "gridsize" : 32
      },
      {
        "id" : -1297377774,
        "name" : "Buenos_Aires_Circuito_7",
        "gridsize" : 32
      },
      {
        "id" : -444279793,
        "name" : "Buenos_Aires_Circuito_8",
        "gridsize" : 32
      },
      {
        "id" : 325651039,
        "name" : "Buenos_Aires_Circuito_9",
        "gridsize" : 32
      },
      {
        "id" : 799677855,
        "name" : "Buenos_Aires_Circuito_12",
        "gridsize" : 32
      },
      {
        "id" : -430748509,
        "name" : "Buenos_Aires_Circuito_15",
        "gridsize" : 32
      },
      {
        "id" : -1786068114,
        "name" : "Buskerud_Long",
        "gridsize" : 24
      },
      {
        "id" : 2097280990,
        "name" : "Buskerud_Short",
        "gridsize" : 24
      },
      {
        "id" : 1910889511,
        "name" : "CadwellPark",
        "gridsize" : 20
      },
      {
        "id" : 2135405654,
        "name" : "CampoGrande1",
        "gridsize" : 46
      },
      {
        "id" : -916478809,
        "name" : "Cascavel2",
        "gridsize" : 38
      },
      {
        "id" : -2123543761,
        "name" : "Cleveland_GP",
        "gridsize" : 32
      },
      {
        "id" : -1142444519,
        "name" : "Cordoba_International",
        "gridsize" : 24
      },
      {
        "id" : -1043857231,
        "name" : "Cordoba_GP",
        "gridsize" : 24
      },
      {
        "id" : -171682166,
        "name" : "Cordoba_NATL",
        "gridsize" : 24
      },
      {
        "id" : 844082431,
        "name" : "CuritibaInternacional",
        "gridsize" : 30
      },
      {
        "id" : -549646259,
        "name" : "Curitiba_outer",
        "gridsize" : 30
      },
      {
        "id" : -923996694,
        "name" : "CurveloLong",
        "gridsize" : 38
      },
      {
        "id" : -203742941,
        "name" : "CurveloShort",
        "gridsize" : 38
      },
      {
        "id" : 2054003546,
        "name" : "Daytona_Speedway_Tri_Oval",
        "gridsize" : 32
      },
      {
        "id" : 467707118,
        "name" : "Daytona_Road_Course",
        "gridsize" : 32
      },
      {
        "id" : 705412912,
        "name" : "Daytona_Nascar_Road_Course",
        "gridsize" : 32
      },
      {
        "id" : 1497365379,
        "name" : "Donington_GP",
        "gridsize" : 38
      },
      {
        "id" : -865646115,
        "name" : "Donington_Nat",
        "gridsize" : 38
      },
      {
        "id" : 1650761166,
        "name" : "Cascais",
        "gridsize" : 30
      },
      {
        "id" : -1015082583,
        "name" : "Cascais_Alternate",
        "gridsize" : 30
      },
      {
        "id" : 1602044389,
        "name" : "Fontana_OVAL",
        "gridsize" : 32
      },
      {
        "id" : -1899621736,
        "name" : "Fontana_SCC",
        "gridsize" : 30
      },
      {
        "id" : 1674501695,
        "name" : "Galeao_Airport",
        "gridsize" : 32
      },
      {
        "id" : 2044979547,
        "name" : "Gateway_OVAL",
        "gridsize" : 32
      },
      {
        "id" : -385022794,
        "name" : "Gateway_RC1",
        "gridsize" : 32
      },
      {
        "id" : 175033766,
        "name" : "Gateway_RC2",
        "gridsize" : 32
      },
      {
        "id" : -438882031,
        "name" : "GoianiaMixed",
        "gridsize" : 36
      },
      {
        "id" : 693994049,
        "name" : "GoianiaOuter",
        "gridsize" : 36
      },
      {
        "id" : -916412256,
        "name" : "GoianiaShort",
        "gridsize" : 36
      },
      {
        "id" : -939269561,
        "name" : "CopaSaoPauloStage2",
        "gridsize" : 28
      },
      {
        "id" : -844021865,
        "name" : "GranjaVianaKart101",
        "gridsize" : 28
      },
      {
        "id" : 553029608,
        "name" : "GranjaVianaKart102",
        "gridsize" : 28
      },
      {
        "id" : 1153901510,
        "name" : "GranjaVianaKart121",
        "gridsize" : 28
      },
      {
        "id" : 2058166835,
        "name" : "Guapore",
        "gridsize" : 40
      },
      {
        "id" : -435924753,
        "name" : "Hockenheim_GP",
        "gridsize" : 30
      },
      {
        "id" : 239659483,
        "name" : "Hockenheim_ShortA",
        "gridsize" : 30
      },
      {
        "id" : 230784137,
        "name" : "Hockenheim_ShortB",
        "gridsize" : 30
      },
      {
        "id" : -327427534,
        "name" : "Hockenheim_National",
        "gridsize" : 30
      },
      {
        "id" : -108270200,
        "name" : "Hockenheim_2001",
        "gridsize" : 30
      },
      {
        "id" : 534374248,
        "name" : "Hockenheim_1988",
        "gridsize" : 36
      },
      {
        "id" : -543681041,
        "name" : "Hockenheim_1988_short",
        "gridsize" : 36
      },
      {
        "id" : 473366003,
        "name" : "Hockenheim_1977",
        "gridsize" : 36
      },
      {
        "id" : 2111076703,
        "name" : "Ibarra2",
        "gridsize" : 30
      },
      {
        "id" : -676667056,
        "name" : "Ibarra_Reverse",
        "gridsize" : 30
      },
      {
        "id" : 731129913,
        "name" : "Imola_GP_2018",
        "gridsize" : 32
      },
      {
        "id" : 1003427592,
        "name" : "Imola_GP_1972",
        "gridsize" : 20
      },
      {
        "id" : 1544603199,
        "name" : "Imola_GP_1988",
        "gridsize" : 26
      },
      {
        "id" : -29732804,
        "name" : "Imola_GP_2001",
        "gridsize" : 26
      },
      {
        "id" : -468654879,
        "name" : "Indianapolis_2022_Oval",
        "gridsize" : 32
      },
      {
        "id" : 328837350,
        "name" : "Indianapolis_2022_RC",
        "gridsize" : 32
      },
      {
        "id" : -1478712571,
        "name" : "Interlagos_GP",
        "gridsize" : 46
      },
      {
        "id" : 420324528,
        "name" : "Interlagos_SCB",
        "gridsize" : 46
      },
      {
        "id" : 1312214486,
        "name" : "Interlagos_Historic",
        "gridsize" : 32
      },
      {
        "id" : -1704124105,
        "name" : "Interlagos_Historic_Outer",
        "gridsize" : 32
      },
      {
        "id" : 228315736,
        "name" : "InterlagosKart1",
        "gridsize" : 38
      },
      {
        "id" : 870961348,
        "name" : "InterlagosKart2",
        "gridsize" : 38
      },
      {
        "id" : 1869056157,
        "name" : "InterlagosKart3",
        "gridsize" : 38
      },
      {
        "id" : -89774641,
        "name" : "Jacarepagua_Historic",
        "gridsize" : 40
      },
      {
        "id" : 393495474,
        "name" : "Jacarepagua_2005",
        "gridsize" : 40
      },
      {
        "id" : -1081969582,
        "name" : "Jacarepagua_OVAL",
        "gridsize" : 32
      },
      {
        "id" : -467386624,
        "name" : "Jacarepagua_SCB",
        "gridsize" : 40
      },
      {
        "id" : 1891554116,
        "name" : "Jacarepagua_SHORT",
        "gridsize" : 40
      },
      {
        "id" : 1406264747,
        "name" : "Jerez_GP2019",
        "gridsize" : 42
      },
      {
        "id" : -1602971785,
        "name" : "Jerez_Standard",
        "gridsize" : 42
      },
      {
        "id" : 1261622488,
        "name" : "Kansai_GP",
        "gridsize" : 52
      },
      {
        "id" : 1035236174,
        "name" : "Kansai_Classic",
        "gridsize" : 52
      },
      {
        "id" : 530399487,
        "name" : "Kansai_East",
        "gridsize" : 52
      },
      {
        "id" : 85029775,
        "name" : "Kansai_West",
        "gridsize" : 18
      },
      {
        "id" : 2018595322,
        "name" : "Kyalami_2019",
        "gridsize" : 40
      },
      {
        "id" : -1384297883,
        "name" : "Kyalami_Historic",
        "gridsize" : 25
      },
      {
        "id" : 568559152,
        "name" : "Laguna_Seca_2020",
        "gridsize" : 30
      },
      {
        "id" : -1540556268,
        "name" : "Londrina_short",
        "gridsize" : 46
      },
      {
        "id" : -1959616750,
        "name" : "Londrina_long",
        "gridsize" : 46
      },
      {
        "id" : 1373891276,
        "name" : "LondrinaKart1",
        "gridsize" : 28
      },
      {
        "id" : 16295271,
        "name" : "LondrinaKart2",
        "gridsize" : 28
      },
      {
        "id" : 1731699995,
        "name" : "Long_Beach",
        "gridsize" : 32
      },
      {
        "id" : -696853932,
        "name" : "Montreal_Historic",
        "gridsize" : 40
      },
      {
        "id" : -1239363445,
        "name" : "Montreal_Modern",
        "gridsize" : 40
      },
      {
        "id" : -2054918047,
        "name" : "Monza_1971_10k",
        "gridsize" : 22
      },
      {
        "id" : 612695202,
        "name" : "Monza_1971_10knc",
        "gridsize" : 22
      },
      {
        "id" : -332593300,
        "name" : "Monza_1971",
        "gridsize" : 22
      },
      {
        "id" : 1003665316,
        "name" : "Monza_1991",
        "gridsize" : 32
      },
      {
        "id" : -1956398437,
        "name" : "Monza_1971_Junior",
        "gridsize" : 22
      },
      {
        "id" : -1257095693,
        "name" : "Monza_2020",
        "gridsize" : 32
      },
      {
        "id" : -2098177408,
        "name" : "Monza_2020_Junior",
        "gridsize" : 32
      },
      {
        "id" : 884472481,
        "name" : "Nordschleife_2020",
        "gridsize" : 28
      },
      {
        "id" : -472366150,
        "name" : "Nordschleife_2020_24hr",
        "gridsize" : 30
      },
      {
        "id" : 899109770,
        "name" : "Nurb_GP_2020",
        "gridsize" : 30
      },
      {
        "id" : -581740816,
        "name" : "Nurb_GP_2020_Veedol",
        "gridsize" : 30
      },
      {
        "id" : 1133983668,
        "name" : "Nurb_GP_2020_Sprint",
        "gridsize" : 30
      },
      {
        "id" : 1819021861,
        "name" : "Nurb_GP_2020_Sprint_S",
        "gridsize" : 30
      },
      {
        "id" : 809214670,
        "name" : "Ortona1",
        "gridsize" : 30
      },
      {
        "id" : 1549441566,
        "name" : "Ortona2",
        "gridsize" : 30
      },
      {
        "id" : -1213903939,
        "name" : "Ortona3",
        "gridsize" : 30
      },
      {
        "id" : 59749392,
        "name" : "Ortona4",
        "gridsize" : 30
      },
      {
        "id" : -498735748,
        "name" : "OultonPark_International",
        "gridsize" : 28
      },
      {
        "id" : -914088887,
        "name" : "OultonPark_Classic",
        "gridsize" : 28
      },
      {
        "id" : -92069206,
        "name" : "OultonPark_Fosters",
        "gridsize" : 28
      },
      {
        "id" : 1820168870,
        "name" : "OultonPark_Island",
        "gridsize" : 28
      },
      {
        "id" : -398562049,
        "name" : "Road_America_RC",
        "gridsize" : 32
      },
      {
        "id" : 372107672,
        "name" : "Road_America_RCB",
        "gridsize" : 32
      },
      {
        "id" : 761562120,
        "name" : "Salvador_Street_Circuit",
        "gridsize" : 34
      },
      {
        "id" : 1932830334,
        "name" : "SantaCruzDoSul",
        "gridsize" : 36
      },
      {
        "id" : -526206945,
        "name" : "Silverstone_1975",
        "gridsize" : 36
      },
      {
        "id" : -533867030,
        "name" : "Silverstone_1975_No_Chicane",
        "gridsize" : 36
      },
      {
        "id" : -797317755,
        "name" : "Silverstone_1991",
        "gridsize" : 36
      },
      {
        "id" : 509991425,
        "name" : "Silverstone_2001",
        "gridsize" : 36
      },
      {
        "id" : 1648317775,
        "name" : "Silverstone_2001_International",
        "gridsize" : 36
      },
      {
        "id" : -507834810,
        "name" : "Silverstone_2001_National",
        "gridsize" : 36
      },
      {
        "id" : -931849903,
        "name" : "Silverstone_GP",
        "gridsize" : 32
      },
      {
        "id" : 964004535,
        "name" : "Silverstone_Intl_2019",
        "gridsize" : 32
      },
      {
        "id" : -1061474453,
        "name" : "Silverstone_Natl_2019",
        "gridsize" : 36
      },
      {
        "id" : -1470317805,
        "name" : "Snetterton_100",
        "gridsize" : 20
      },
      {
        "id" : -1338478783,
        "name" : "Snetterton_200",
        "gridsize" : 26
      },
      {
        "id" : 987592900,
        "name" : "Snetterton_300",
        "gridsize" : 26
      },
      {
        "id" : -1736505524,
        "name" : "Spa_Francorchamps_1970",
        "gridsize" : 22
      },
      {
        "id" : 1170932587,
        "name" : "Spa_Francorchamps_1970_1000km",
        "gridsize" : 22
      },
      {
        "id" : 1283905272,
        "name" : "Spa_Francorchamps_1993",
        "gridsize" : 30
      },
      {
        "id" : -1262750090,
        "name" : "Spa_Francorchamps_2020",
        "gridsize" : 30
      },
      {
        "id" : 775712153,
        "name" : "Spa_Francorchamps_2022",
        "gridsize" : 30
      },
      {
        "id" : 756011139,
        "name" : "Speedland1",
        "gridsize" : 14
      },
      {
        "id" : -588458518,
        "name" : "Speedland2",
        "gridsize" : 14
      },
      {
        "id" : -327486946,
        "name" : "Speedland3",
        "gridsize" : 14
      },
      {
        "id" : -1941017299,
        "name" : "Speedland4",
        "gridsize" : 14
      },
      {
        "id" : -100668052,
        "name" : "Spielberg_Historic",
        "gridsize" : 20
      },
      {
        "id" : -213305159,
        "name" : "Spielberg_Vintage",
        "gridsize" : 20
      },
      {
        "id" : 735562025,
        "name" : "Spielberg_Modern",
        "gridsize" : 32
      },
      {
        "id" : 1819432538,
        "name" : "Spielberg_Short",
        "gridsize" : 32
      },
      {
        "id" : 2074495683,
        "name" : "Taruma_Internacional",
        "gridsize" : 32
      },
      {
        "id" : -108853074,
        "name" : "Taruma_Chicane",
        "gridsize" : 32
      },
      {
        "id" : 163877389,
        "name" : "Termas_Rio_Hondo",
        "gridsize" : 34
      },
      {
        "id" : 1882187011,
        "name" : "VeloCitta1",
        "gridsize" : 34
      },
      {
        "id" : 1557615576,
        "name" : "VeloCittaTD",
        "gridsize" : 34
      },
      {
        "id" : 1956507207,
        "name" : "VeloCittaClubDay",
        "gridsize" : 34
      },
      {
        "id" : 193535285,
        "name" : "Velopark_2010",
        "gridsize" : 34
      },)"
      R"({
        "id" : -1642426225,
        "name" : "Velopark_2017",
        "gridsize" : 34
      },
      {
        "id" : 1063112912,
        "name" : "Virginia_Full",
        "gridsize" : 28
      },
      {
        "id" : -1592912773,
        "name" : "Virginia_Grand",
        "gridsize" : 28
      },
      {
        "id" : 2134032188,
        "name" : "Virginia_North",
        "gridsize" : 28
      },
      {
        "id" : 1284894334,
        "name" : "Virginia_Patriot",
        "gridsize" : 14
      },
      {
        "id" : -1235504884,
        "name" : "Virginia_South",
        "gridsize" : 16
      },
      {
        "id" : -875185854,
        "name" : "Watkins_Glen_GP",
        "gridsize" : 32
      },
      {
        "id" : -191952188,
        "name" : "Watkins_Glen_GPIL",
        "gridsize" : 32
      },
      {
        "id" : 2035789624,
        "name" : "Watkins_Glen_S",
        "gridsize" : 32
      },
      {
        "id" : -619438500,
        "name" : "Watkins_Glen_SIL",
        "gridsize" : 32
      },
      {
        "id" : 788137081,
        "name" : "Circuit_de_Catalunya_GP",
        "gridsize" : 32
      },
      {
        "id" : -2045930240,
        "name" : "Circuit_de_Catalunya_GP_NC",
        "gridsize" : 32
      },
      {
        "id" : -869897529,
        "name" : "Cascais_1988",
        "gridsize" : 30
      },
      {
        "id" : -1548942089,
        "name" : "Jerez_GP1988",
        "gridsize" : 32
      },
      {
        "id" : 1060182346,
        "name" : "Le_Mans_24h",
        "gridsize" : 60
      },
      {
        "id" : 1446740417,
        "name" : "Le_Mans_Circuit_Bugatti",
        "gridsize" : 32
      }
    ]
  }
})";
