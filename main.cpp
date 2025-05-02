#include "src/utils/Logger.hpp"
#include "src/Controller.hpp"

int main(const int argc, char *argv[])
{
    if(argc <= 1) {
        LOG("No filename provided!");
        return 1;
    }
    Controller ctrl(argv[1]);
    ctrl.process();
    return 0;
}
