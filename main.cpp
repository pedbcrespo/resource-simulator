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
    root.size = 1;
    root.cost = 1;
    resource res = {100, 50};
    int choosedLvl = 1;
    for(int round=1; round<TOTAL_ROUNDS; round++) {
        std::cout << "ROUND: " << round << std::endl;
        
        createGroup(root, res);
        updateGroups(root, res);
        printGroupSummary(root, res);
        printLogs();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        clearTerminal();
        res.value += res.increase;
    }
    
    return 0;
}