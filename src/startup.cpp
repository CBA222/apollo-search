/* Start the search engine.
 *
 *
 *
 */

#include "search_index/index.h"
#include "ranker/pageranker.h"
#include "utils/vector_space.h"
#include "setup_service/setup_data_service.h"
#include <nlohmann/json.hpp>
#include <libpq-fe.h>
#include <vector>
#include <ifstream>
#include <iostream>

/* Master process to start the entire server
 * 
 */
int main(int argc, char* argv[]) {
    std::ifstream config_file;
    config_file.open(argv[1]);

    if (!config_file.is_open()) {
        std::cout << "ERROR: Config file not found. Exiting program." << std::endl;
        return 1;
    }

    SetupDataService setup_data_service = new SetupDataService();

    std::vector<char *> tokens = setup_data_service.get_tokens();
    std::unordered_map<char *, std::vector<int>> inverted_mapping = setup_data_service.get_inverted_index();

    Index *index;
    VectorSpace *vs;
    PageRanker *ranker;

    vs = new VectorSpace(tokens);
    if (!validate_vector_space()) {
        std::cout << "ERROR: Created index mappings do not match stored index mappings. Exiting program." << std::endl;
        return 1;
    }

    index = new Index();
    index->populate(tokens);
    for (auto it = tokens.begin();it != tokens.end();it++) {
        index->insert(*it, ,)
    }

    ranker = PageRanker(index);
}

bool validate_vector_space(VectorSpace *vs, json correct_mappings) {
    for (auto it = correct_mappings.begin();it != correct_mappings.end();it++) {
        if (vs->get_idx(it.key()) != it.value()) {
            return false;
        }
    }
    return true;
}