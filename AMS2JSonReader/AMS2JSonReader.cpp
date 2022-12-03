// AMS2JSonReader.cpp : Defines the entry point for the application.
//

#include "AMS2JSonReader.h"

using namespace std;
using json = nlohmann::json;

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

int main()
{
    std::ifstream conf("config.json");
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
                    std::list<Lap> v;
                }

                json members = (*it)["members"];
                for (auto memberit = members.begin(); memberit != members.end(); ++memberit)
                {
                    int chave = std::stoi(memberit.key());
                    std::string name = (*memberit).find("name").value();
                    refidMap.insert_or_assign(chave, name);
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

                                int s = *refid;
                                Lap lap = {};
                                lap.validLap = valid;
                                lap.sector1 = s1;
                                lap.sector2 = s2;
                                lap.sector3 = s3;
                                lap.lapTime = n;
                                lap.track = trackid;
                                lap.vehicle = refidVehicleMap[s];

                                nameLapMap[refidMap[s]].push_back(lap);
                            }
                        }
                    }
                }
            }

            json outjson;
            int indexNames = 0;
            for (auto i = nameLapMap.begin(); i != nameLapMap.end(); ++i)
            {
                //cout << "Name: " << i->first << endl;

                json laps;
                for (auto l = i->second.begin(); l != i->second.end(); ++l)
                {
                    int index = static_cast<int>(l - i->second.begin());
                    std::string valid;
                    if (l->validLap)
                        valid = "Valid Lap - ";
                    else
                        valid = "Invalid Lap - ";

                    //cout << valid  << "LapTime: " << formataVolta(l->lapTime) << " Sector1 : " << formataVolta(l->sector1) << " Sector2 : " << formataVolta(l->sector2) << " Sector3 : " << formataVolta(l->sector3) << " Vehicle : " << l->vehicle << endl;

                    laps[index] = json::object({ {"ValidLap", l->validLap}, {"LapTime", l->lapTime}, {"Sector1", l->sector1}, {"Sector2", l->sector2}, {"Sector3", l->sector3}, {"Vehicle", l->vehicle}, {"Track", l->track} });
                }
                outjson[indexNames]["Name"] = i->first;
                outjson[indexNames]["Laps"] = laps;
                //cout << endl;
                indexNames++;
            }

            outdir += "output.json";
            std::ofstream o(outdir);
            if (!o.is_open())
                cout << std::endl << "Ocorreu um erro ao criar o arquivo!" << std::endl;
            else
                cout << std::endl << "Arquivo criado com sucesso!" << std::endl;

            o << outjson;
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


void GenerateCSV()
{
    string inputString;

    cout << "Informe o nome do arquivo a ser criado: ";
    std::cin >> inputString;

    inputString += ".csv";

    std::ifstream f("jr_stats_data.json", std::ifstream::in | std::ifstream::binary);
    std::ofstream fileOut(inputString);
    fileOut.imbue(std::locale(fileOut.getloc(), new std::codecvt_utf8_utf16<wchar_t>));
    fileOut << "RefId;Nome;Volta" << std::endl;

    string out;
    string line;
    while (std::getline(f, line))
    {
        if (line.substr(0, 2) != "//" && line.substr(0, 2) != "\r\n")
            out += line;
    }

    f.close();

    try 
    {
        json data = json::parse(out);
        json hist = data["stats"]["history"];
        json players = data["stats"]["players"];
        //auto e = hist.find("members");
        //std::cout << e.value();
        std::map<int, std::string> refidMap;
        std::map<std::string, std::string> steamidMap;

        for (auto it = hist.begin(); it != hist.end(); ++it)
        {
            json participants = (*it)["participants"];
            for (auto partit = participants.begin(); partit != participants.end(); ++partit)
            {
                std::string name = (*partit).find("Name").value();
                auto ref = (*partit).find("RefId");
                int r = ref.value();
                //cout << " Nome: " << name.value() << " RefId: " << r << std::endl;
                refidMap.insert_or_assign( r, name );
            }

            json members = (*it)["members"];
            for (auto memberit = members.begin(); memberit != members.end(); ++memberit)
            {
                //cout << "Chave: " << memberit.key();
                int chave = std::stoi(memberit.key());
                std::string name = (*memberit).find("name").value();
                //cout << " Nome: " << name.value() << std::endl;
                refidMap.insert_or_assign( chave, name );

            }

            json eventos = (*it)["stages"]["practice1"]["events"];
            for (auto it2 = eventos.begin(); it2 != eventos.end(); ++it2)
            {
                json s = (*it2);
                auto n = *it2;
               // std::cout << "Tamanho: " << n.size() << std::endl;

                //std::cout << *it2 << std::endl;
                auto x = s.find("event_name");
                if (x.value() == "Lap") {
                    //cout << x.value() << std::endl;
                    auto refid = s.find("refid");
                    
                    for (auto it3 = s.begin(); it3 != s.end(); ++it3)
                    {
                        if (it3.key() == "attributes")
                        {
                            json v = (*it3)["CountThisLapTimes"];
                            int valid = *v.begin();
                            if (valid)
                            {
                                json l = (*it3)["LapTime"];
                                int n = *l.begin();
                                std::cout << "Refid: " << *refid << " Nome: " << refidMap[*refid] << " Volta: " << formataVolta(n) << std::endl;
                                int s = *refid;
                                fileOut << std::to_string(s).c_str() << ";" << refidMap[*refid].c_str() << ";" << formataVolta(n).c_str() <<  std::endl;
                            }
                        }
                    }
                }
            }
            
        }

    }
    catch (json::exception& e)
    {
        std::cout << "message: " << e.what() << '\n'
            << "exception id: " << e.id << std::endl;

        goto exit;
    }
    cout << std::endl << "Arquivo criado com sucesso!" << std::endl;

exit: 
    std::cin.get();
    std::cin.get();

    return;
}
