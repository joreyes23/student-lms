#pragma once
#include <string>

class NotificationService {
public:
    static void notify(const std::string& userEmail,
                       const std::string& message);
};
