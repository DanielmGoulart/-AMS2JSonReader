// AMS2JSonReader.cpp : Defines the entry point for the application.
//

#include "AMS2JSonReader.h"

using namespace std;
using json = nlohmann::ordered_json;

std::string formataVolta(int n)
{
    int v = n;
    std::string s;
    int mili = v % 1000;
    v -= mili;
    v /= 1000;
    if (v > 0)
    {
        int min = v % 60;
        v -= min;
        v /= 60;
        if (v > 0)
        {
            if (v < 10)
                s = "0";
            s += std::to_string(v);
            s += ":";
        }
        else
            s = "00:";

        if (min < 10)
            s += "0";
        s += std::to_string(min);
        s += ".";
    }
    else
    {
        s = "0:00.";
    }

    if (mili < 100)
        s += "0";
    if (mili < 10)
        s += "0";
    
    s += std::to_string(mili);

    return s;
}

std::string formataSegundos(int n)
{
    int v = n;
    std::string s;
    if (n > 1000)
    {
        int mili = v % 1000;
        v -= mili;

        s += std::to_string(v);
        s += ".";
    }
    else
        s = "0.";

    if (v < 100)
        s += "0";
    if (v < 10)
        s += "0";

    s += std::to_string(v);

    return s;
}


int main()
{
    std::ifstream conf("json_reader_config.json");
    if (!conf.is_open())
    {
        cout << "Erro Fatal! Arquivo de configuração não encontrado." << endl << "Encerrando aplicação!";

        return 0;
    }

    try
    {
        json data = json::parse(conf);
        int time = data["Delay"];
        std::string outdir = data["OutputDir"];
        outdir += "output.json";
        bool fastestLapOnly = data["OnlyFastestLap"];
        bool lapInMinutes = data["LapInMinutes"];

        json vehiclesjson = json::parse(std::string(vehicles));
        json tracksjson = json::parse(std::string(tracks));

        json tracklistjson = tracksjson["response"]["list"];
        json vehiclelistjson = vehiclesjson["response"]["list"];

        std::map<int64_t, std::string> vehicleList;
        std::map<int64_t, std::string> vehicleClassList;
        std::map<int64_t, std::string> trackList;

        for (auto trackit = tracklistjson.begin(); trackit != tracklistjson.end(); ++trackit)
        {
            int64_t id = (*trackit)["id"];
            std::string name = (*trackit)["name"];
            trackList.insert_or_assign(id, name);
        }
        for (auto vehicleit = vehiclelistjson.begin(); vehicleit != vehiclelistjson.end(); ++vehicleit)
        {
            int64_t id = (*vehicleit)["id"];
            std::string name = (*vehicleit)["name"];
            vehicleList.insert_or_assign(id, name);
            std::string vehicleClass = (*vehicleit)["class"];
            vehicleClassList.insert_or_assign(id, vehicleClass);
        }


        int notFirstBatch = 0;

        while (true)
        {
            if (notFirstBatch) {
                cout << "Aguardando " << time << " segundos para a próxima execução" << endl;
                std::this_thread::sleep_for(std::chrono::seconds(time));
            }

            notFirstBatch = 1;

            std::ifstream f("jr_stats_data.json", std::ifstream::in | std::ifstream::binary);
            if (!f.is_open())
            {
                cout << "Arquivo json não encontrado, é necessário colocar o arquivo jr_stats_data.json no mesmo diretório da aplicação";

                return 0;
            }

            cout << "Formatando JSON" << endl;
            string out;
            string line;
            while (std::getline(f, line))
            {
                if (line.substr(0, 2) != "//" && line.substr(0, 2) != "\r\n")
                    out += line;
            }

            f.close();

            json data = json::parse(out);
            json hist = data["stats"]["history"];
            json players = data["stats"]["players"];

            std::map<std::string, std::vector<Lap>> nameLapMap;
            cout << "Extraindo dados do JSON." << endl;
            for (auto it = hist.begin(); it != hist.end(); ++it)
            {
                std::map<int, std::string> refidMap;
                std::map<int, int64_t> refidVehicleMap;
                int64_t trackid = (*it)["setup"]["TrackId"];
                json participants = (*it)["participants"];
                for (auto partit = participants.begin(); partit != participants.end(); ++partit)
                {
                    std::string name = (*partit).find("Name").value();
                    auto ref = (*partit).find("RefId");
                    int r = ref.value();
                    int64_t vehicleid = (*partit).find("VehicleId").value();
                    refidMap.insert_or_assign(r, name);
                    refidVehicleMap.insert_or_assign(r, vehicleid);
                }

                json members = (*it)["members"];
                for (auto memberit = members.begin(); memberit != members.end(); ++memberit)
                {
                    int chave = std::stoi(memberit.key());
                    std::string name = (*memberit).find("name").value();

                    refidMap.insert_or_assign(chave, name);
                    int64_t vehicleid = (*memberit)["setup"]["VehicleId"];
                    refidVehicleMap.insert_or_assign(chave, vehicleid);
                }

                json eventos = (*it)["stages"]["practice1"]["events"];
                for (auto it2 = eventos.begin(); it2 != eventos.end(); ++it2)
                {
                    json s = (*it2);
                    auto n = *it2;

                    auto x = s.find("event_name");
                    if (x.value() == "Lap") {
                        auto refid = s.find("refid");

                        for (auto it3 = s.begin(); it3 != s.end(); ++it3)
                        {
                            if (it3.key() == "attributes")
                            {
                                json v = (*it3)["CountThisLapTimes"];
                                int valid = *v.begin();

                                int s1 = *((*it3)["Sector1Time"]).begin();
                                int s2 = *((*it3)["Sector2Time"]).begin();
                                int s3 = *((*it3)["Sector3Time"]).begin();

                                json l = (*it3)["LapTime"];
                                int n = *l.begin();

                                if (s2 == 0 && valid)
                                    valid = 0;

                                int valorrefid = (int)*refid;
                                Lap lap = {};
                                lap.validLap = valid;
                                lap.sector1 = s1;
                                lap.sector2 = s2;
                                lap.sector3 = s3;
                                lap.lapTime = n;
                                lap.track = trackid;
                                lap.vehicle = refidVehicleMap[valorrefid];

                                nameLapMap[refidMap[valorrefid]].push_back(lap);
                            }
                        }
                    }
                }
            }

            json outjson;
            json lapsjson;
            int indexNames = 0;
            std::map <std::string, std::vector<FastestLap>> fastestLapListPerClass;
            for (auto i = nameLapMap.begin(); i != nameLapMap.end(); ++i)
            {
                //cout << "Name: " << i->first << endl;
                std::map<std::string, int> classFastestLapIndexMap;
                int fastestLapIndex = 0;
                int totalLaps = 0;
                json laps;

                for (auto l = i->second.begin(); l != i->second.end(); ++l)
                {
                    int index = static_cast<int>(l - i->second.begin());
                    std::string classLap = vehicleClassList[l->vehicle];
                    std::string classFastestLap = vehicleClassList[i->second[classFastestLapIndexMap[classLap]].vehicle];

                    //Esta condicional consiste em 3 partes:
                    // 1 - Verificar se a volta mais rápida é da mesma classe que a volta atual, se não for vai substituir
                    // 2 - Se a volta mais rápida não for válida irá substituir
                    // 3 - Se a volta atual for menor que a volta mais rápida irá substituir
                    if (((classLap != classFastestLap) || (!i->second[classFastestLapIndexMap[vehicleClassList[l->vehicle]]].validLap && l->validLap)) || (l->validLap && l->lapTime < i->second[classFastestLapIndexMap[vehicleClassList[l->vehicle]]].lapTime)) {
                        classFastestLapIndexMap[vehicleClassList[l->vehicle]] = index;
                        fastestLapIndex = index;
                    }

                    totalLaps++;
                    
                    if (!fastestLapOnly) {
                        if(lapInMinutes)
                            laps[index] = json::object({ {"ValidLap", l->validLap}, {"LapTime", formataVolta(l->lapTime)}, {"Sector1", formataVolta(l->sector1)}, {"Sector2", formataVolta(l->sector2)}, {"Sector3", formataVolta(l->sector3)}, {"Vehicle", vehicleList[l->vehicle]}, {"Track", trackList[l->track]} });
                        else
                            laps[index] = json::object({ {"ValidLap", l->validLap}, {"LapTime", l->lapTime}, {"Sector1", l->sector1}, {"Sector2", l->sector2}, {"Sector3", l->sector3}, {"Vehicle", vehicleList[l->vehicle]}, {"Track", trackList[l->track]} });
                    }
                }

                for (auto f = classFastestLapIndexMap.begin(); f != classFastestLapIndexMap.end(); ++f)
                {
                    cout << "Nome: " << i->first << endl;
                    cout << "Categoria: " << f->first << endl;
                    FastestLap fl = {};
                    fl.name = i->first;
                    fl.lap = i->second[f->second];
                    fl.totalLaps = totalLaps;
                    if (!fastestLapOnly)
                        fl.allLapsIndex = indexNames;
                    if(fl.lap.validLap)
                        fastestLapListPerClass[f->first].push_back(fl);
                }


                if (!fastestLapOnly) {
                    lapsjson[indexNames]["Name"] = i->first;
                    lapsjson[indexNames]["Laps"] = laps;

                    indexNames++;
                }
            }

            json fastestlapperclassjson;
            int indexClass = 0;
            for (auto fastestLapList = fastestLapListPerClass.begin(); fastestLapList != fastestLapListPerClass.end(); ++fastestLapList)
            {
                json fastestlapjson;
                std::sort(fastestLapList->second.begin(), fastestLapList->second.end());
                for (auto i = fastestLapList->second.begin(); i != fastestLapList->second.end(); ++i)
                {
                    int index = static_cast<int>(i - fastestLapList->second.begin());
                    fastestlapjson[index]["Name"] = i->name;
                    fastestlapjson[index]["TotalLaps"] = i->totalLaps;

                    if (!fastestLapOnly)
                        fastestlapjson[index]["Index"] = i->allLapsIndex;

                    if (lapInMinutes)
                        fastestlapjson[index]["Lap"] = json::object({ {"ValidLap", i->lap.validLap}, {"LapTime", formataVolta(i->lap.lapTime)}, {"Sector1", formataVolta(i->lap.sector1)}, {"Sector2", formataVolta(i->lap.sector2)}, {"Sector3", formataVolta(i->lap.sector3)}, {"Vehicle", vehicleList[i->lap.vehicle]}, {"Track", trackList[i->lap.track]}, {"RawTime", i->lap.lapTime} });
                    else
                        fastestlapjson[index]["Lap"] = json::object({ {"ValidLap", i->lap.validLap}, {"LapTime", i->lap.lapTime}, {"Sector1", i->lap.sector1}, {"Sector2", i->lap.sector2}, {"Sector3", i->lap.sector3}, {"Vehicle", vehicleList[i->lap.vehicle]}, {"Track", trackList[i->lap.track]} });
                }

                //cout << "Categoria Loop 2: " << fastestLapList->first << endl;
                fastestlapperclassjson[indexClass]["Category"] = fastestLapList->first;
                fastestlapperclassjson[indexClass]["LapsPerCategory"] = fastestlapjson;
                indexClass++;
            }
            
            

            outjson["stats"] = fastestlapperclassjson;
            
            if (!fastestLapOnly)
                outjson["overall"] = lapsjson;

            std::ofstream o(outdir);
            if (!o.is_open())
                cout << std::endl << "Ocorreu um erro ao criar o arquivo!" << std::endl;
            else
                cout << std::endl << "Arquivo criado com sucesso!" << std::endl;

            o << outjson.dump(4);
            o.close();

        }
    }
    catch (json::exception& e)
    {
        std::cout << "message: " << e.what() << '\n'
            << "exception id: " << e.id << std::endl;

        goto exit;
    }
    
exit:
    std::cin.get();
    std::cin.get();

    return 0;
}

void UploadFile()
{
    CURL* curl = curl_easy_init();


}
