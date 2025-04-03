#include <iostream>
#include <fstream>
#include "types/MasterConfig.hpp"

using json = nlohmann::json;
using namespace std;

namespace nlohmann {
    template<>
    struct adl_serializer<IndexConfig> {
        static void from_json(const json& j, IndexConfig& idx) {
            j.at("field").get_to(idx.field);
            j.at("unique").get_to(idx.unique);
        }
        
        static void to_json(json& j, const IndexConfig& idx) {
            j = json{{"field", idx.field}, {"unique", idx.unique}};
        }
    };
    
    template<>
    struct adl_serializer<CollectionConfig> {
        static void from_json(const json& j, CollectionConfig& config) {
            j.at("path").get_to(config.path);
            j.at("schema").get_to(config.schema);
            j.at("last_id").get_to(config.last_id);
            j.at("indexes").get_to(config.indexes);
        }
        
        static void to_json(json& j, const CollectionConfig& config) {
            j = json{
                {"path", config.path},
                {"schema", config.schema},
                {"last_id", config.last_id},
                {"indexes", config.indexes}
            };
        }
    };
    
    template<>
    struct adl_serializer<MasterConfig> {
        static void from_json(const json& j, MasterConfig& config) {
            j.at("collections").get_to(config.collections);
        }
        
        static void to_json(json& j, const MasterConfig& config) {
            j = json{{"collections", config.collections}};
        }
    };
}

void MasterConfig::saveToJson(const string& path) const {
    json j = *this;
    ofstream out(path);
    if (!out.is_open()) {
        throw runtime_error("Failed to open file for writing: " + path);
    }
    out << j.dump(4);
}

int MasterConfig::getLastId(const string& collectionName) const {
    if (auto it = collections.find(collectionName); it != collections.end()) {
        return it->second.last_id;
    }

    return -1;
}

int MasterConfig::incrementLastId(const string& collectionName) {
    if (auto it = collections.find(collectionName); it != collections.end()) {
        it->second.last_id++;
        this->saveToJson("data/master.json");
        return it->second.last_id;
    }

    return -1;
}

int MasterConfig::decrementLastId(const string& collectionName) {
    if (auto it = collections.find(collectionName); it != collections.end()) {
        it->second.last_id--;
        this->saveToJson("data/master.json");
        return it->second.last_id;
    }

    return -1;
}

void MasterConfig::setLastId(const string& collectionName, int newId) {
    collections[collectionName].last_id = newId;
}

bool MasterConfig::hasCollection(const string& collectionName) const {
    return collections.find(collectionName) != collections.end();
}