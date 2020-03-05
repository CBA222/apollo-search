#include "utils/vector_space.h"
#include <vector>
#include <iostream>

bool test_case_1();

int main(int argc, char* argv[]) {
    if (!test_case_1()) {
        std::cout << "TEST FAILED" << std::endl;
        return 1;
    }

    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
}

bool test_case_1() {
    std::vector<char *> tokens;
    tokens.push_back("apple");
    tokens.push_back("pear");
    tokens.push_back("orange");
    tokens.push_back("grape");
    tokens.push_back("banana");

    VectorSpace *vs = new VectorSpace(tokens);

    bool in_set;
    unsigned int idx;

    for (auto it = tokens.begin();it != tokens.end();it++) {
        idx = vs->get_idx(*it, in_set);
        std::cout << *it << " " << idx << std::endl;
        if (!in_set) { return false; }
    }

    idx = vs->get_idx("not in set", in_set);
    if (in_set) { return false; }

    delete vs;

    return true;
}