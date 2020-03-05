#include "data_service/ranker_service.h"
#include <unordered_map>
//#include <gtest/gtest.h>
//#include <assert.h>
#include <iostream>
#include <string>

using std::string;

#define ASSERT(x) (std::cout << (x ? "Passed" : "Failed") << std::endl)

void test1() {
    std::unordered_map<string, string> config;
    config["rabbitmq_host"] = "1.0.1";
    RankerService *ranker = new RankerService(config);
}

/*
class RankerServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        ranker = new RankerService();
    }

    RankerService *ranker;
}

TEST_F(RankServiceTest, )
*/

int main(int argc, char *argv[]) {
    test1();
}