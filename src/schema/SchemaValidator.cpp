#include "schema/SchemaValidator.hpp"
#include <iostream>

using json = nlohmann::json;
using namespace std;

bool SchemaValidator::validate(json& record, const json& schema) {
    if (!schema.contains("type") || schema["type"] != "object") {
        throw runtime_error("Invalid schema: root type must be object.");
    }

    if (!schema.contains("properties") || !schema["properties"].is_object()) {
        throw runtime_error("Invalid schema: missing 'properties'");
    }

    const json& props = schema["properties"];
    const set<string> required = schema.contains("required")
        ? set<string>(schema["required"].begin(), schema["required"].end())
        : set<string>{};

    for (auto& [key, rules] : props.items()) {
        bool exists = record.contains(key);

        if (required.contains(key) && !exists) {
            cerr << "[Schema] Missing required field: " << key << "\n";
            return false;
        }

        if (!exists && rules.contains("default")) {
            record[key] = rules["default"];
            continue;
        }

        if (!exists)
            continue;

        const string type = rules["type"];

        if (type == "string" && !record[key].is_string()) {
            cerr << "[Schema] '" << key << "' must be a string\n";
            return false;
        }
        if (type == "number" && !record[key].is_number()) {
            cerr << "[Schema] '" << key << "' must be a number\n";
            return false;
        }
        if (type == "boolean" && !record[key].is_boolean()) {
            cerr << "[Schema] '" << key << "' must be a boolean\n";
            return false;
        }
        if (type == "object" && !record[key].is_object()) {
            cerr << "[Schema] '" << key << "' must be an object\n";
            return false;
        }
        if (type == "array" && !record[key].is_array()) {
            cerr << "[Schema] '" << key << "' must be an array\n";
            return false;
        }
    }

    return true;
}
