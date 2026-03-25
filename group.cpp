#include "group.hpp"
#include <memory>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <map>
#include <iomanip>
#include <vector>
#include <random>
#include <string>

const int BASE_LVL = 1;
const int LIMIT_SUB_GROUPS_PER_GROUP = 5;
const int BASE_COST = 10;
std::vector<long> ids;
std::vector<std::string> roundLogs;

int generateRandom(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

void insertId(long id) {
    ids.push_back(id);
}

long getRandomId() {
    int randomIndex = generateRandom(0, ids.size()-1);
    return ids.at(randomIndex);
}

bool hasReachedLimitSubGroups(group& root, int lv, int currentLvl) {
    if(currentLvl == lv) return root.branchs.size() >= LIMIT_SUB_GROUPS_PER_GROUP;
    if(currentLvl != lv && root.branchs.empty()) return false;
    bool hasReached;
    for(auto& branch: root.branchs) {
        hasReached = hasReachedLimitSubGroups(branch, lv, currentLvl + 1);
        if(hasReached) break;
    }
    return hasReached;
}

bool hasReachedLimit(group& root, int lv) {
    return hasReachedLimitSubGroups(root, lv, 1);
}

bool isFirstRound(group& root) {
    return root.branchs.empty();
}

long calculateCost(group& root, long id, int size, int lv) {
    long total = 0;
    if(root.id == id) {
        return BASE_COST * size * lv;
    }
    for(auto& branch: root.branchs) {
        total = calculateCost(branch, id, size, lv+1);
        if(total > 0) break;
    }
    return total;
}

long generateId() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    return ms.count();
}

int getMaxLvl(group& root, int lv) {
    if(root.branchs.empty())
        return lv;
    for(auto& branch : root.branchs) {
        int nextMaxLv = getMaxLvl(branch, lv+1);
        lv = lv > nextMaxLv? lv : nextMaxLv;
    }
    return lv;
}

int totalLvs(group& root) {
    return getMaxLvl(root, 1);
}

int getLvl(group& root, long id, int lv) {
    if(root.id == id) {
        return lv;
    }
    for(auto& branch : root.branchs) {
        int result = getLvl(branch, id, lv + 1);
        if(result != -1) {
            return result;
        }
    }
    return -1;
}

void insertLog(std::string log) {
    roundLogs.push_back(log);
}

void clearLogs() {
    roundLogs.clear();
}

void insertGroup(group& root, group& current, group& newGroup, resource& res, long idToInsert) {
    if(current.id == idToInsert) {
        newGroup.cost = calculateCost(root, current.id, newGroup.size, BASE_LVL);
        res.value -= BASE_COST;
        current.branchs.push_back(newGroup);
        return;
    }
    for(auto& branch: current.branchs) {
        insertGroup(root, branch, newGroup, res, idToInsert);
    }
}

void createGroup(group&root, resource& res) {
    group newGroup;
    newGroup.id = generateId();
    newGroup.size = generateRandom(1, 10);
    long idToInsert = getRandomId();
    insertGroup(root, root, newGroup, res, idToInsert);
    insertId(newGroup.id);
    insertLog("Grupo criado: [ID: " + std::to_string(newGroup.id) + "] | Tamanho: " + std::to_string(newGroup.size) + " | Custo: " + std::to_string(newGroup.cost) + " | Recursos restantes: " + std::to_string(res.value));
}



void consumeResource(group& root, group& current, resource& res) {
    if(current.branchs.empty()) {
        return;
    }
    for(auto& branch: current.branchs) {
        int currentLvl = getLvl(root, branch.id, 1);
        res.value -= branch.cost;
        consumeResource(root, branch, res);
    }
}

void updateGroups(group& root, resource& res, int round) {
    if(round == 1) {
        return;
    }
    consumeResource(root, root, res);
}

long calculateIncreaseResource(group& root, int lv = 1) {
    if(root.branchs.empty()) {
        return BASE_COST * root.size * lv;
    }
    long totalIncrease = 0;
    for(auto& branch: root.branchs) {
        totalIncrease += calculateIncreaseResource(branch, lv + 1);
    }
    return totalIncrease;
}

void updateResource(group& root, resource& res, int round) {
    if(round == 1) {
        return;
    }
    long increase = calculateIncreaseResource(root);
    res.increase = increase;
    res.value += increase;
    insertLog("Recursos aumentados: " + std::to_string(increase) + " | Recursos atuais: " + std::to_string(res.value));
}

void printLogs() {
    std::cout << "\n==========================================" << std::endl;
    std::cout << "                LOGS DA RODADA             " << std::endl;
    std::cout << "==========================================" << std::endl;
    for(const auto& log : roundLogs) {
        std::cout << log << std::endl;
    }
    std::cout << "==========================================\n" << std::endl;
}

void clearTerminal() {
    #ifdef _WIN32
        std::system("cls");
    #elif __linux__
        std::cout << "\033[H\033[J";
    #endif
}

void printGroupByLvl(group& current, std::string prefix, bool isLast) {
    std::cout << prefix;
    std::cout << (isLast ? "└── " : "├── ");

    std::cout << "\033[1;32m[ID: " << current.id << "]\033[0m " // Verde para ID
              << "Amt: " << current.size 
              << " | Cost: \033[1;31m" << current.cost << "\033[0m" << std::endl; // Vermelho para Cost

    std::string newPrefix = prefix + (isLast ? "    " : "│   ");

    for (size_t i = 0; i < current.branchs.size(); ++i) {
        bool lastChild = (i == current.branchs.size() - 1);
        printGroupByLvl(current.branchs[i], newPrefix, lastChild);
    }
}

void printGroup(group& root, resource& res) {
    std::cout << "\n==========================================" << std::endl;
    std::cout << " RESOURCES AVAILABLE: [ \033[1;34m" << res.value << "\033[0m ]" << std::endl;
    std::cout << "==========================================" << std::endl;
    
    std::cout << "ROOT" << std::endl;
    for (size_t i = 0; i < root.branchs.size(); ++i) {
        bool lastChild = (i == root.branchs.size() - 1);
        printGroupByLvl(root.branchs[i], "", lastChild);
    }
}



void countNodesByLvl(group& current, int lv, std::map<int, int>& counts) {
    counts[lv]++;
    for (auto& branch : current.branchs) {
        countNodesByLvl(branch, lv + 1, counts);
    }
}

void printGroupSummary(group& root, resource& res) {
    std::map<int, int> counts;
    countNodesByLvl(root, 1, counts);

    std::cout << "==========================================" << std::endl;
    std::cout << "          RESUMO DA ESTRUTURA             " << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "RECURSOS: [ " << res.value << " ]" << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << std::left << std::setw(12) << "NIVEL" << "QNT. DE NOS" << std::endl;

    int totalNos = 0;
    for (auto const& [lv, qtd] : counts) {
        std::cout << "Lvl " << std::left << std::setw(8) << lv << ": " << qtd << std::endl;
        totalNos += qtd;
    }

    std::cout << "------------------------------------------" << std::endl;
    std::cout << "TOTAL DE NOS NA ARVORE: " << totalNos << std::endl;
    std::cout << "==========================================\n" << std::endl;
}