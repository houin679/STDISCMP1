// config_reader.h
#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

unordered_map<string, int> read_config(const string& filename) {
    unordered_map<string, int> config;
    ifstream file(filename);
    string key;
    int value;
    while (file >> key >> value) {
        config[key] = value;
    }
    return config;
}

#endif // CONFIG_READER_H
