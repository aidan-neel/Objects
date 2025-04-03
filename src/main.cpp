#include <iostream>
#include "db/RecordStore.hpp"
#include "lib/json.hpp"

int main() {
    RecordStore users("users");

    nlohmann::json newUser = {
        { "email", "alice@example.com" },
        { "password", "hashed123" },
        { "age", 30 },
    };

    if (users.createRecord(newUser)) {
        std::cout << "Record created successfully.\n";
    } else {
        std::cout << "Failed to create record.\n";
    }

    return 0;
}
