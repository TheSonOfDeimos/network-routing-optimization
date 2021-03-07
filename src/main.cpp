#include <iostream>

#include "core/time.hpp"
#include "core/core.hpp"
#include "model/node.hpp"
#include "model/routingTable.hpp"
#include "model/modelCore.hpp"

int main()
{

    ModelCore core;
    core.buildNetwork(100);

    core.start();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    core.stop();
    std::cout << Time::instance().get() << "\n";

    return 0;
}