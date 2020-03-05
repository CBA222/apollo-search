/*
 * This file helps determine the index positions for a list of words
 */

#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "utils/utils.h"

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    std::ifstream file(argv[1]);
    std::vector<char *> tokens;

    json j;

    if (file.is_open()) {
        //tokens = read_lines_from_file(&file);
        file >> j;
        file.close();
    } else {
        std::cout << "FILE NOT FOUND" << std::endl;
        return 0;
    }

    json mappings = generate_mappings(j);

    std::cout << mappings;
}