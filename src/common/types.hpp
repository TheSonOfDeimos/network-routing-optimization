#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <cassert>

using guid_t = int;

using status_t = int;

// Not Errors
#define ERROR_OK            0
#define ERROR_RESULT_TRUE   3
#define ERROR_NO_EFFECT     12

// Errors
#define ERROR_UNKNOWN       -1
#define ERROR_LOGIC         -2
#define ERROR_RESULT_FALSE  -3

#define IS_SUCCESS(x)       ((static_cast<status_t>(x)) >= 0)
#define IS_UNSUCCESS(s)     (!IS_SUCCESS(s))
#define EXIT(s)             do { status = s; goto exit; } while (0)
#define EXIT_IF(cond, res)  do { if (cond) EXIT(res); } while(0)
#define EXIT_IF_ERROR       do { if (IS_UNSUCCESS(status)) goto exit; } while (0)
#define RUN(s)              do { status = s; EXIT_IF_ERROR; } while(0)

#define assertm(exp, msg) assert(((void)msg, exp))
#define NOT_IMPLEMENTED_YET do { assertm(false, "Not Implemented yet"); } while (0)

using hostAddress_t = int;
using hostName_t = std::string;
using port_t = int;
using packageId_t  = int;
using dataVolume_t  = int; // in Bytes
using ttl_t = int;

enum class TransportProto : int
{
    TCP = 0,
    UDP,
    ICMP
};

enum class Priority : int
{
    NONE    = -1,

    LOW     = 1,
    MEDIUM  = 2,
    HIGH    = 3
};

#endif