#include "Read_ini.h"
#include <cstring>

void ini_file_line_parse(char* line, std::string &key, std::string &value)
{   // 拆分一行字符串  
    char * pch_1;
    char * pch_2;
    const char delimit[6]{"=[]\r\n"};
    pch_1 = strtok(line, delimit);
    if (pch_1 != NULL)
        key = pch_1;
    else
        key = "";

    pch_2 = strtok(NULL, delimit);
    if (pch_2 != NULL)
        value = pch_2;
    else
        value = "";
}


IniType Read_ini(const char* filename)
{   // 读取配置文件
    const size_t line_size{ 1024 };
    char line[line_size];
    std::string key, value, temp_key{""};
    std::map<std::string, std::map<std::string, std::string>> config;
    std::ifstream input;

    input.open(filename);
    if (input.fail() || input.bad())
    {   
        char s[line_size];
        sprintf(s, "%s Line %d error in read %s\n", __func__, __LINE__, filename);
        printf(s);
        throw runtime_error(s);
    }
    while(!input.eof())
    {
        input.getline(line, line_size);
        key.clear();
        value.clear();
        ini_file_line_parse(line, key, value);
        if (key == "")
        {
            continue;
        }
        if (value == "")
        {   
            std::map<std::string, std::string> temp_map;
            config[key] = temp_map;
            temp_key = key;
        }
        else
        {
            config[temp_key][key] = value;
        }
    }
    input.close();
    return config;
}

void show_ini(IniType config)
{   //显示配置文件的内容
    char buffer[100];
    for (auto i= config.begin(); i!= config.end(); i++)
    {   
        std::string key = i->first;
        auto value      = i->second;
        sprintf(buffer, "[%s]:\n", key.data());
        fputs (buffer , stdout);  
        for (auto ii= value.begin(); ii != value.end(); ii++)
        {
            std::string key_   = ii->first;
            std::string value_ = ii->second;
            sprintf(buffer, "        <%s : %s>\n", key_.data(), value_.data());
            fputs (buffer , stdout);  
        }
    }
}

vector<double> read_vector_from_ini(string linestr)
{   
    vector<double> vec_d;
    char *word, *next_token = NULL, line_c[1024];
    const char delimit[7]{",=[]\r\n"};
    strncpy(line_c, linestr.c_str(), 1024);
    word = strtok(line_c, delimit);
    while (word != NULL)
    {   
        vec_d.push_back(atof(word));
        word = strtok(NULL, delimit);
    }
    return vec_d;
}

int GetIntFromKey(std::map<std::string, std::string> &strmap, std::string key)
{
    string value = strmap[key];
    char s[128];
    int i;
    if (value.empty())
    {
        sprintf(s, "No Key Named >>>%s<<<\n", key.c_str());
        printf(s);
        i = 0;
        throw(runtime_error(s));
    }
    else
    {   
        i = stoi(value);
        sprintf(s, "Set %s:%d\n", key.c_str(), i);
        printf(s);
    }
    return i;
}

double GetDoubleFromKey(std::map<std::string, std::string> &strmap, std::string key)
{
    string value = strmap[key];
    char s[128];
    double d;
    if (value.empty())
    {
        sprintf(s, "No Key Named >>>%s<<<\n", key.c_str());
        printf(s);
        d = 0;
    }
    else
    {
        d = stof(value);
        printf("Set %s:%.4lf\n", key.c_str(), d);
    }
    return d;
}

vector<double> GetVectorDoubleFromKey(std::map<std::string, std::string> &strmap, std::string key)
{
    string value = strmap[key];
    char s[128];
    vector<double> vec_d;
    if (value.empty())
    {
        sprintf(s, "No Key Named >>>%s<<<\n", key.c_str());
        printf(s);
    }
    else
    {
        vec_d = read_vector_from_ini(value);
        printf("Set %s:", key.c_str());
        for (double d : vec_d)
        {
            printf("%.4lf, ", d);
        }
        printf("\n");
    }
    return vec_d;
}
