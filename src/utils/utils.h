#include <fstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "utils/vector_space.h"

using json = nlohmann::json;

#ifndef UTILS_H
#define UTILS_H

std::vector<char *> read_lines_from_file(std::ifstream *file) {

    std::string line;
    std::vector<char *> holding;

    while (getline(*file, line)) {
        const char * line_chars = line.c_str();
        char* temp = (char *)malloc((strlen(line_chars) + 1) * sizeof(char));
        strcpy(temp, line_chars);

        holding.push_back(temp);
    }

    return holding;
};

json generate_mappings(json json_data) {
    std::string line;
    std::vector<char *> tokens;

    for (json::iterator it=json_data.begin();it!=json_data.end();it++) {
        std::string to_insert = (std::string)(it.key());
        const char * line_chars = to_insert.c_str();
        char* temp = (char *)malloc((strlen(line_chars) + 1) * sizeof(char));
        strcpy(temp, line_chars);

        tokens.push_back(temp);
    }

    VectorSpace *vs = new VectorSpace(tokens);
    json json_mappings;

    for (auto it = tokens.begin();it != tokens.end();it++) {
        bool in_set;
        json_mappings[*it] = vs->get_idx(*it, in_set);
    }

    return json_mappings;
};

#endif