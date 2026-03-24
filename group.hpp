#include<vector>
#include<memory>

typedef struct group{
    long id;
    int size;
    long cost;
    int strikes;
    std::vector<struct group> branchs;
} group;

typedef struct resource {
    long value;
    long increase;
} resource;

void createGroup(group&root, resource& res);
void updateGroups(group& root, resource& res);
void printGroup(group& root, resource& res);
void printGroupSummary(group& root, resource& res);
void insertLog(std::string log);
void clearLogs();
void printLogs();
void clearTerminal();


int totalLvs(group& root);
int generateRandom(int min, int max);

bool hasReachedLimit(group& root, int lv);