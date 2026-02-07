#include <drogon/drogon.h>

int main() {
    drogon::app()
        .loadConfigFile("/home/jo/student-lms/backend/config.json")
        .run();
}
