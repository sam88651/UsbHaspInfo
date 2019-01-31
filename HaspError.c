/*
 * Copyright (C) 2018 Sam88651.
 * 
 * Module Name:
 *     HaspError.c
 * Abstract:
 *     Handle hasp API errors messages
 * Notes:
 * Revision History:
 */
#include <stdio.h>
#include <stdlib.h>
#include "hasp.h"


static KeyValue errorMessages[] = {
    { HASPERR_SUCCESS,                      "Operation successful" },
    { HASPERR_INVALID_SERVICE,              "HASP Invalid Service" },
    { HASPERR_CANT_FREE_DOSMEM,             "Cannot free DOS Memory" },
    { HASPERR_CANT_ALLOC_DOSMEM,            "Cannot allocate DOS Memory" },
    { HASPERR_DOS_CANT_CLOSE_HDD,           "Cannot close the HASP Device Driver" },
    { HASPERR_DOS_CANT_READ_HDD,            "Cannot read the HASP Device Driver" },
    { HASPERR_DOS_CANT_OPEN_HDD,            "Cannot open the HASP Device Driver" },
    { HASPERR_CANT_CLOSE_HDD,               "Cannot close the HASP Device Driver" },
    { HASPERR_CANT_READ_HDD,                "Cannot read the HASP Device Driver" },
    { HASPERR_CANT_OPEN_HDD,                "Cannot open the HASP Device Driver" },
    { HASPERR_NOT_A_TIMEHASP,               "A HASP was found but it is not a TimeHASP" },
    { HASPERR_TH_INVALID_PASSWORDS,         "A HASP with specified passwords was not found" },
    { HASPERR_TH_TIMEOUT,                   "Timeout - Write operation failed" },
    { HASPERR_TH_INVALID_ADDRESS,           "Invalid address - Address is not in 0 - 15" },
    { HASPERR_TH_INVALID_HOUR,              "Invalid Hour" },
    { HASPERR_TH_INVALID_MINUTE,            "Invalid Minute" },
    { HASPERR_TH_INVALID_SECOND,            "Invalid Second" },
    { HASPERR_TH_INVALID_YEAR,              "Invalid year" },
    { HASPERR_TH_INVALID_MONTH,             "Invalid month" },
    { HASPERR_TH_INVALID_DAY,               "Invalid day" },
    { HASPERR_VERSION_MISMATCH,             "Driver version mismatch" },
    { HASPERR_INVALID_PARAMETER,            "Invalid parameter" },
    { HASPERR_TS_SP3_FOUND,                 "Terminal Server under SP3 is not supported" },
    { HASPERR_TS_FOUND,                     "Terminal Server was found" },
    { HASPERR_INVALID_POINTER,              "Invalid pointer used by Encode Data" },
    { HASPERR_HARDWARE_NOT_SUPPORTED,       "The hardware does not support the service" },
    { HASPERR_DATA_TOO_SHORT,               "The data length is too short" },
    { HASPERR_PORT_BUSY,                    "Parallel port is busy" },
    { HASPERR_MH_WRITE_FAIL,                "Unsuccessful Write operation" },
    { HASPERR_NOT_A_MEMOHASP,               "A HASP was found but it is not a MemoHASP" },
    { HASPERR_MH_INVALID_PASSWORDS,         "A HASP with specified passwords was not found" },
    { HASPERR_HASP_NOT_FOUND,               "A HASP with specified passwords was not found" },
    { HASPERR_MH_INVALID_ADDRESS,           "Address out of range" },
    { HASPERR_MH_TIMEOUT,                   "Timeout - Write operation failed" },
    { HASPERR_NO_PROTOCOLS,                 "IPX, NetBIOS, or TCP/IP protocols have not been installed properly" },
    { HASPERR_NO_SOCKET_NUMBER,             "Communication Error - unable to get the socket number (TCP/IP, IPX)" },
    { HASPERR_COMM_ERROR,                   "Communication Error" },
    { HASPERR_NO_NHLM,                      "No NetHASP License Manager was found" },
    { HASPERR_NO_NHLM_ADDRFILE,             "Cannot read NetHASP License Manager address file" },
    { HASPERR_CANT_CLOSE_NHLM_ADDRFILE,     "Cannot close NetHASP License Manager address file" },
    { HASPERR_CANT_SEND_PACKET,             "Communication error - failed to send packet (IPX, NetBIOS, TCP/IP)" },
    { HASPERR_SILENT_NHLM,                  "No answer from the NetHASP License Manager" },
    { HASPERR_NO_LOGIN,                     "Service requested before LOGIN" }, 
    { HASPERR_ADAPTER_ERROR,                "NetBIOS: Communication error - adapter error" },
    { HASPERR_NO_ACTIVE_NHLM,               "No active NetHASP License Manager was found" },
    { HASPERR_SSBN_FAILED,                  "Cannot perform LOGIN - SetServerByName failed" },
    { HASPERR_NHCF_SYNTAX_ERROR,            "NetHASP configuration file syntax error" },
    { HASPERR_NHCF_GENERIC_ERROR,           "Error handling NetHASP configuration file" },
    { HASPERR_NH_ENOMEM,                    "Memory allocation error" },
    { HASPERR_NH_CANT_FREE_MEM,             "Memory release error" },
    { HASPERR_NH_INVALID_ADDRESS,           "Invalid NetHASP memory address" },
    { HASPERR_NH_ENCDEC_ERR,                "Error trying to Encrypt/Decrypt" },
    { HASPERR_CANT_LOAD_WINSOCK,            "TCP/IP: failed to load WINSOCK.DLL" },
    { HASPERR_CANT_UNLOAD_WINSOCK,          "TCP/IP: failed to unload WINSOCK.DLL" },
    { HASPERR_WINSOCK_ERROR,                "TCP/IP: WINSOCK.DLL startup error" },
    { HASPERR_CANT_CLOSE_SOCKET,            "TCP/IP: Failed to close socket" },
    { HASPERR_SETPROTOCOL_FAILED,           "SetProtocol service requested without performing LOGOUT" },
    { HASPERR_NH_NOT_SUPPORTED,             "NetHASP services are not supported" },
    { HASPERR_NH_HASPNOTFOUND,              "NetHASP key is not connected to the NetHASP License Manager" },
    { HASPERR_INVALID_PROGNUM,              "Program Number is not in the Program List of the NetHASP memory" },
    { HASPERR_NH_READ_ERROR,                "Error reading NetHASP memory" },
    { HASPERR_NH_WRITE_ERROR,               "Error writing NetHASP memory" },
    { HASPERR_NO_MORE_STATIONS,             "Number of stations exceeded" },
    { HASPERR_NO_MORE_ACTIVATIONS,          "Number of activations exceeded" },
    { HASPERR_LOGOUT_BEFORE_LOGIN,          "LOGOUT called without LOGIN" },
    { HASPERR_NHLM_BUSY,                    "NetHASP License Manager is busy" },
    { HASPERR_NHLM_FULL,                    "No space in NetHASP Log Table" },
    { HASPERR_NH_INTERNAL_ERROR,            "NetHASP Internal error" },
    { HASPERR_NHLM_CRASHED,                 "NetHASP License Manager crashed and reactivated" },
    { HASPERR_NHLM_UNSUPPORTED_NETWORK,     "NetHASP License Manager does not support the station's network" },
    { HASPERR_NH_INVALID_SERVICE_2,         "Invalid Service NetHASP License Manager does not support the station's network" },
    { HASPERR_NHCF_INVALID_NHLM,            "Invalid NetHASP License Manager name in configuration file" },
    { HASPERR_SSBN_INVALID_NHLM,            "Invalid NetHASP License Manager name in SetServerByName" },
    { HASPERR_ENC_ERROR_NHLM,               "Error trying to encrypt by the LM" },
    { HASPERR_DEC_ERROR_NHLM,               "Error trying to decrypt by the LM" },
    { HASPERR_OLD_LM_VERSION_NHLM,          "LM old version was found" },
    { HASPERR_NH_BUFFER_TOO_SMALL,          "Specified buffer size is too small for requested function" },
    { HASPERR_IPX_UNAVAILABLE,              "IPX is enabled but it is not installed" },
    { HASPERR_NETBIOS_UNAVAILABLE,          "NetBIOS is enabled but it is not installed" },
    { HASPERR_IPX_NETBIOS_UNAVAILABLE,      "IPX and NetBIOS are enabled but both are not installed" },
    { HASPERR_TCPIP_UNAVAILABLE,            "TCP/IP is enabled but it is not installed" },
    { HASPERR_IPX_TCPIP_UNAVAILABLE,        "IPX and TCP/IP are enabled but both are not installed" },
    { HASPERR_TCPIP_NETBIOS_UNAVAILABLE,    "TCP/IP and NetBIOS are enabled but are not installed" },
    { HASPERR_PROTOCOLS_UNAVAILABLE,        "IPX, NetBIOS and TCP/IP are enabled but not installed" },
    { HASPERR_SUSPICIOUS_LOGOUT,            "Suspicious LOGOUT" },
    { HASPERR_NHCF_BAD_TOKEN,               "Invalid keyword or value in configuration file" },
    { HASPERR_MISSING_IP_ADDR,              "TCP or UDP were not employed due to missing IP address" },
    { HASPERR_CANT_FREE_MEM,                "HASP API cannot free memory" }
};

static int init = 0;

/**
 * Get error test description by its code
 * @param code - error code
 * @return - error message
 */
char *getHaspErrorMessage(int code) {
    
    if (!init) {
        initKeyValueList(errorMessages, sizeof(errorMessages)/sizeof(KeyValue));
        init = 1;
    }
    return getValueByKey(errorMessages, sizeof(errorMessages)/sizeof(KeyValue), code);
}

/**
 * Print corresponded error message
 * @param code
 */
void printHaspErrorMessage(int code) {
    
    char *msg = getHaspErrorMessage(code);
    if (code) {
        printf("CRITICAL - Error: (%d, 0x%x) %s\n", code, code, msg);
    } else {
        printf("OK - %s\n", msg);        
    }
}