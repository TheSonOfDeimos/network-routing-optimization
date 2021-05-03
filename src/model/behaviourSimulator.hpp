#ifndef BEHAVIOUR_SIMULATOR_HPP
#define BEHAVIOUR_SIMULATOR_HPP

#include <memory>
#include <vector>

#include "types.hpp"
#include "time.hpp"
#include "package.hpp"

enum class RoleType : int
{
    CONSUMER = 0,
    PRODUCER,
    PROVIDER
};

#endif
