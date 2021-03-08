#include <iostream>

#include "core/time.hpp"
#include "core/core.hpp"
#include "model/node.hpp"
#include "model/routingTable.hpp"
#include "model/modelCore.hpp"
#include "model/packageQueue.hpp"

int main()
{

    status_t status = ERROR_OK;

    PackageQueue q(100, QueuePushRule::BACK, QueuePopRule::FRONT, QueueDropRule::NEW);
    RUN(q.push(std::make_unique<Package>("192.168.1.1", "192.168.1.2", 150)));
    RUN(q.push(std::make_unique<Package>("192.168.1.1", "192.168.1.2", 0)));
    RUN(q.push(std::make_unique<Package>("192.168.1.1", "192.168.1.2", 10)));
    RUN(q.push(std::make_unique<Package>("192.168.1.1", "192.168.1.2", 10)));
    RUN(q.push(std::make_unique<Package>("192.168.1.1", "192.168.1.2", 10)));
    RUN(q.push(std::make_unique<Package>("192.168.1.1", "192.168.1.2", 10)));
    RUN(q.push(std::make_unique<Package>("192.168.1.1", "192.168.1.2", 61)));

    {
        auto res_1 = q.pop();
        auto res_2 = q.pop();
        auto res_3 = q.pop();
        auto res_4 = q.pop();
        auto res_5 = q.pop();
        auto res_6 = q.pop();
        auto res_7 = q.pop();
    }

    // {
    //     ModelCore core;
    //     core.buildNetwork(100);

    //     core.start();
    //     std::this_thread::sleep_for(std::chrono::seconds(5));
    //     core.stop();
    //     std::cout << Time::instance().get() << "\n";
    // }

exit:
    return status;
}