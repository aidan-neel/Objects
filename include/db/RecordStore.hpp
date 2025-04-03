#pragma once
#include <string>
#include <fstream>
#include <unordered_map>
#include <types/MasterConfig.hpp>

// https://github.com/nlohmann/json
#include "lib/json.hpp"

using json = nlohmann::json;

using namespace std;

class RecordStore {
public:
    RecordStore(const string& collectionName);

    bool createRecord(const json& record);
    
private:
    string collectionPath;
    string collectionName;
    string schemaPath;
    int recordId;
    MasterConfig master;

    bool validateAgainstSchema(json& record);
    void applySchemaDefaults(json& record, const json& schema);
    ofstream createRecordFile(const json& record, int& lastId);
    void updateIndexes(const json& record);
    void persistRecord(const json& record);
    void loadSchema();
    void loadMaster();
};
