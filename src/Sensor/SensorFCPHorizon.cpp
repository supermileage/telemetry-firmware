#include "SensorFcpHorizon.h"
#include "fcp-common.h"
#include "settings.h"

// #define DEBUG_FCP_Horizon

#ifdef DEBUG_FCP_Horizon
#define FC_DEBUG_INTERVAL 100
uint32_t sensor_fcp_horizon_last_debug_output = 0;
#endif

#define FC_PACKET_LENGTH FC_NUM_HEADERS + FC_NUM_DATA * 2
