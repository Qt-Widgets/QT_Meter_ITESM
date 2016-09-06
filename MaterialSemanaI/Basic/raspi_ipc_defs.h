#ifndef RASPI_IPC_DEFS_H_
#define RASPI_IPC_DEFS_H_

// local socket  for server app -> client gui comm
#define SOCKET_NAME_SERVER "/home/sergio/ITESMSemanaI/gageSocket"
//#define SOCKET_NAME_SERVER "/home/pi/gageSocket"
// server accept only one cliens connection
#define MAX_SERVER_CONNECTIONS 1

#define IPC_SET_GAGE_SOF  0xAA
#define IPC_SET_GAGE_FRAME_SZ   0x05


#define IPC_ID_IDX              1
#define IPC_DATA_IDX_H          3
#define IPC_DATA_IDX_L          2
#define IPC_UNIT_IDX            4

#endif
