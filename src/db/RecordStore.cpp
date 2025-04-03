#include <iostream>
#include <fstream>
#include <string>
#include <format>
#include "lib/json.hpp"
#include "schema/SchemaValidator.hpp"
#include "db/RecordStore.hpp"

using json = nlohmann::json;
using namespace std;

RecordStore::RecordStore(const string& collectionName) {
    loadMaster();
    this->collectionPath = format("data/{}", collectionName);
    this->collectionName = collectionName;
}

bool RecordStore::createRecord(const json& record) {
    int lastId = master.incrementLastId(this->collectionName);
    this->recordId = lastId;

    json modified = record;
    if (!validateAgainstSchema(modified)) {
        cerr << "Record creation failed: invalid schema.\n";
        master.decrementLastId(this->collectionName);
        return false;
    }

    ofstream file = createRecordFile(modified, lastId);
    cout << "Record created successfully.\n";
    return true;
}

void RecordStore::applySchemaDefaults(json& record, const json& schema) {
    if (!schema.contains("properties")) {
        cout << "[SchemaDefaults] No properties found in schema. Skipping default application.\n";
        return;
    }

    cout << "[SchemaDefaults] Applying defaults to record...\n";

    for (auto& [key, prop] : schema["properties"].items()) {
        if (!record.contains(key) && prop.contains("default")) {
            record[key] = prop["default"];
            cout << "[SchemaDefaults] Set default for key '" << key << "' -> " << prop["default"] << "\n";
        }
    }
}

bool RecordStore::validateAgainstSchema(json& record) {
    ifstream schemaFile(format("data/{}/schema.json", this->collectionName));
    if (!schemaFile.is_open()) {
        throw runtime_error("Failed to open schema file.");
    }

    json schema;
    schemaFile >> schema;

    applySchemaDefaults(record, schema);
    return SchemaValidator::validate(record, schema);
}

void RecordStore::loadMaster() {
    ifstream file("data/master.json");
    if (!file.is_open()) {
        throw runtime_error("Failed to open data/master.json. Current dir: " + filesystem::current_path().string());
    }

    json j;
    file >> j;

    MasterConfig master;
    auto collectionsJson = j["collections"];
    for (auto& [collName, collData] : collectionsJson.items()) {
        CollectionConfig config;
        config.path = collData["path"];
        config.schema = collData["schema"];
        config.last_id = collData["last_id"];

        if (collData.contains("indexes") && collData["indexes"].is_array()) {
            for (auto& idxData : collData["indexes"]) {
                IndexConfig idx;
                idx.field = idxData["field"];
                idx.unique = idxData["unique"];
                config.indexes.push_back(idx);
            }
        }

        master.collections[collName] = config;
    }

    this->master = master;
}

ofstream RecordStore::createRecordFile(const json& record, int& lastId) {
    string filePath = format("{}/{}.json", this->collectionPath, lastId);
    ofstream file(filePath);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file for writing: " + filePath);
    }

    json modified_record = record;
    modified_record["id"] = lastId;
    file << modified_record.dump(4);
    return file;
}
