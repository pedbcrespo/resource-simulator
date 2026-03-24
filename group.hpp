#include<vector>
#include<memory>

typedef struct group{
    long id;
    int amount;
    long cost;
    std::vector<struct group> branchs;
} group;

typedef struct resource {
    long value;
    long increase;
} resource;

void createGroup(group&root, resource& res, int amount);
int totalLvs(group& root);
void printGroup(group& root, resource& res);
void printGroupSummary(group& root, resource& res);
bool hasReachedLimit(group& root, int lv);
int generateRandom(int min, int max);