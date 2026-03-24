#include <iostream>
#include <memory>
#include <random>
#include <thread>
#include "group.hpp"

const int TOTAL_ROUNDS = 100;


int main() {
    group root;
    root.id = 1;
    extern std::vector<long> ids;
    ids.push_back(root.id);
    root.amount = 1;
    root.cost = 1;
    resource res = {100, 10};
    int choosedLvl = 1;
    for(int round=1; round<TOTAL_ROUNDS; round++) {
        std::cout << "ROUND: " << round << std::endl;
        int amount = generateRandom(1, 10);
        createGroup(root, res, amount);
        printGroupSummary(root, res);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "\033[H\033[J";
        res.value += res.increase;
    }
    
    return 0;
}