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
    string inputString;

    cout << "Informe o nome do arquivo a ser criado: ";
    cin >> inputString;

    inputString += ".csv";

    std::ifstream f("jr_stats_data.json", std::ifstream::in | std::ifstream::binary);
    std::ofstream fileOut(inputString);
    fileOut.imbue(std::locale(fileOut.getloc(), new std::codecvt_utf8_utf16<wchar_t>));
    fileOut << "RefId;Nome;Volta" << std::endl;

    try 
    {
        json data = json::parse(f);
        json hist = data["stats"]["history"];
        //auto e = hist.find("members");
        //std::cout << e.value();
        for (auto it = hist.begin(); it != hist.end(); ++it)
        {
            std::map<int, std::string> players;

            json participants = (*it)["participants"];
            for (auto partit = participants.begin(); partit != participants.end(); ++partit)
            {
                std::string name = (*partit).find("Name").value();
                auto ref = (*partit).find("RefId");
                int r = ref.value();
                //cout << " Nome: " << name.value() << " RefId: " << r << std::endl;
                players.insert_or_assign( r, name );
            }

            json members = (*it)["members"];
            for (auto memberit = members.begin(); memberit != members.end(); ++memberit)
            {
                //cout << "Chave: " << memberit.key();
                int chave = std::stoi(memberit.key());
                std::string name = (*memberit).find("name").value();
                //cout << " Nome: " << name.value() << std::endl;
                players.insert_or_assign( chave, name );

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
                                std::cout << "Refid: " << *refid << " Nome: " << players[*refid] << " Volta: " << formataVolta(n) << std::endl;
                                int s = *refid;
                                fileOut << std::to_string(s).c_str() << ";" << players[*refid].c_str() << ";" << formataVolta(n).c_str() <<  std::endl;
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
    cin.get();
    cin.get();

    return 0;
}
