#include "NotificationService.h"
#include <iostream>

void NotificationService::notify(const std::string& userEmail,
                                 const std::string& message)
{
    // v1: log-based notification
    std::cout << "[NOTIFY] To: " << userEmail
              << " | Message: " << message << std::endl;
}
