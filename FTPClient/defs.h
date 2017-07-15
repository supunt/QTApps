#ifndef DEFS_H
#define DEFS_H

#pragma once
#define MAIN_SCAN_INTERVAL_M_SEC 5000
#define RETRY_SCAN_INTERVAL_M_SEC 60000

#define FTP_DEF_THREAD_COUNT 4
#define DEFAULT_SCAN_TIMER_INTERVAL 30

//-----------------------------------------------------------------------------------------------------------------------------------------
enum SOURCE
{
    DIR_SC,
    FTP,
    NTWK,
    MAINWND,
    SYNCMAN,
    LOGGER
};
//-----------------------------------------------------------------------------------------------------------------------------------------
enum NOTIF_SUBTYPE
{
    NT_ERROR_FILE_READ,
    NT_ERROR_FTP_CONNECT,
    NT_ERROR_FTP_LOGIN,
    NT_ERROR_GENERAL
};
//-----------------------------------------------------------------------------------------------------------------------------------------
enum TWE{
    TEXT,
    WARNING,
    ERROR,
    SUCCESS
};
//-----------------------------------------------------------------------------------------------------------------------------------------
enum TWE_TYPE
{
    E_STATUS_BAR,
    E_POPUP,
    E_STATUS_POPUP
};
//-----------------------------------------------------------------------------------------------------------------------------------------

#endif // DEFS_H
