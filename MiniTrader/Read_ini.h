#pragma once
#include <cstdio>
#include <map>
#include <fstream>
#include <exception>
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <exception>
using std::runtime_error;
typedef std::map<std::string, std::map<std::string, std::string>> IniType;

void show_ini(IniType config);
IniType Read_ini(const char* filename);                         // 读取配置文件

int GetIntFromKey(std::map<std::string, std::string> &strmap, std::string key);
double GetDoubleFromKey(std::map<std::string, std::string> &strmap, std::string key);
vector<double> GetVectorDoubleFromKey(std::map<std::string, std::string> &strmap, std::string key);
