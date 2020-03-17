#include <vector>
#include <string>

using std::vector;
using std::string;

#ifndef TOKENIZER_H
#define TOKENIZER_H

vector<string> tokenize(string s);

inline vector<string> tokenize(string s) {
    vector<string> tokens;

    string token = "";
    for (auto it = s.begin();it != s.end();it++) {
        char c = *it;
        if (c == ' ') {
            tokens.push_back(token);
            token = "";
        } else {
            token.push_back(c);
        }
    }
    tokens.push_back(token);

    return tokens;
}

#endif