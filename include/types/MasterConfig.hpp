#pragma once
#include <string>
#include <vector>
#include <map>
#include <optional>
#include "lib/json.hpp"

using namespace std;

struct IndexConfig {
    string field;
    bool unique;
};

struct CollectionConfig {
    string path;
    string schema;
    int last_id;
    vector<IndexConfig> indexes;
};

class MasterConfig {
    public:
        MasterConfig() = default;
    
        int getLastId(const string& collectionName) const;
        int incrementLastId(const string& collectionName);
        int decrementLastId(const string& collectionName);
        void saveToJson(const string& path) const;
        void setLastId(const string& collectionName, int newId);
        bool hasCollection(const string& collectionName) const;
        map<string, CollectionConfig> collections;
    }; 