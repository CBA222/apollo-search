/* Setup Service
 * This service is queried for the data necessary to initialize the search engine.
 */

#include <vector>
#include <unordered_map>
#include <string>
#include <nlohmann/json.hpp>
#include <ifstream>

using json = nlohmann::json;

#ifndef STARTUP_DATA_SERVICE_H
#define STARTUP_DATA_SERVICE_H

class SetupDataService
{
private:

    std::unordered_map<string, int> vs_mapping;
    std::unordered_map<char *, std::vector<int>> inverted_index;
    std::vector<char *> tokens;

public:

    SetupDataService(char* files_location) {
        std::ifstream file;
        json j;
        file >> j;

        for (auto it = j.begin();it != j.end();it++) {
            vs_mapping[it.key()] = it.val();
        }
    }

    std::unordered_map<string, int> get_vs_mapping() {
        return vs_mapping;
    }

    std::unordered_map<char *, std::vector<int>> get_inverted_index() {
        return inverted_index;
    }

    std::vector<char *> get_tokens() {
        return tokens;
    }
};

#endif