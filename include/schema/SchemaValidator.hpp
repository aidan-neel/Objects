#pragma once

#include <string>
#include <set>
#include "lib/json.hpp"

class SchemaValidator {
public:
    static bool validate(nlohmann::json& record, const nlohmann::json& schema);
};
