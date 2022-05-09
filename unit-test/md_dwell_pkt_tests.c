
/*
 * Includes
 */

#include "md_dwell_pkt.h"
#include "md_msg.h"
#include "md_msgdefs.h"
#include "md_events.h"
#include "md_version.h"
#include "md_test_utils.h"
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdlib.h>

/* UT includes */
#include "uttest.h"
#include "utassert.h"
#include "utstubs.h"

/* md_dwell_pkt_tests globals */
uint8 call_count_CFE_EVS_SendEvent;

/*
 * Function Definitions
 */

void MD_DwellLoop_Test_PacketAlreadyFull(void)
{
    MD_AppData.MD_DwellTables[0].AddrCount                       = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].AddrCount = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].AddrCount = 1;

    MD_AppData.MD_DwellTables[0].Enabled                       = MD_DWELL_STREAM_ENABLED;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Enabled = MD_DWELL_STREAM_ENABLED;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Enabled = MD_DWELL_STREAM_ENABLED;

    MD_AppData.MD_DwellTables[0].Rate                       = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Rate = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Rate = 1;

    MD_AppData.MD_DwellTables[0].CurrentEntry                       = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].CurrentEntry = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].CurrentEntry = 1;

    MD_AppData.MD_DwellTables[0].Entry[0].Delay                       = 2;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Entry[0].Delay = 2;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Entry[0].Delay = 2;

    /* Execute the function being tested */
    MD_DwellLoop();

    /* Verify results */
    UtAssert_True(MD_AppData.MD_DwellTables[0].CurrentEntry == 0, "MD_AppData.MD_DwellTables[0].CurrentEntry == 0");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].CurrentEntry == 0,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].CurrentEntry == 0");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].CurrentEntry == 0,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].CurrentEntry == 0");

    UtAssert_True(MD_AppData.MD_DwellTables[0].PktOffset == 0, "MD_AppData.MD_DwellTables[0].PktOffset == 0");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].PktOffset == 0,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].PktOffset == 0");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].PktOffset == 0,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].PktOffset == 0");

    UtAssert_True(MD_AppData.MD_DwellTables[0].Countdown == 2, "MD_AppData.MD_DwellTables[0].Countdown == 2");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Countdown == 2,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Countdown == 2");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Countdown == 2,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Countdown == 2");

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    UtAssert_True(call_count_CFE_EVS_SendEvent == 0, "CFE_EVS_SendEvent was called %u time(s), expected 0",
                  call_count_CFE_EVS_SendEvent);

} /* end MD_DwellLoop_Test_PacketAlreadyFull */

void MD_DwellLoop_Test_SendDwellPacket(void)
{
    MD_AppData.MD_DwellTables[0].AddrCount                       = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].AddrCount = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].AddrCount = 1;

    MD_AppData.MD_DwellTables[0].Enabled                       = MD_DWELL_STREAM_ENABLED;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Enabled = MD_DWELL_STREAM_ENABLED;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Enabled = MD_DWELL_STREAM_ENABLED;

    MD_AppData.MD_DwellTables[0].Rate                       = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Rate = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Rate = 1;

    MD_AppData.MD_DwellTables[0].CurrentEntry                       = 0;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].CurrentEntry = 0;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].CurrentEntry = 0;

    MD_AppData.MD_DwellTables[0].Entry[0].Delay                       = 2;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Entry[0].Delay = 2;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Entry[0].Delay = 2;

    MD_AppData.MD_DwellTables[0].Countdown                       = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Countdown = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Countdown = 1;

    MD_AppData.MD_DwellTables[0].Entry[0].Length                       = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Entry[0].Length = 2;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Entry[0].Length = 4;

    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Entry[0].ResolvedAddress = 2;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Entry[0].ResolvedAddress = 4;

    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_MemRead8), 1, CFE_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_MemRead16), 1, CFE_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_MemRead32), 1, CFE_SUCCESS);

    /* Execute the function being tested */
    MD_DwellLoop();

    /* Verify results */
    UtAssert_True(MD_AppData.MD_DwellTables[0].CurrentEntry == 0, "MD_AppData.MD_DwellTables[0].CurrentEntry == 0");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].CurrentEntry == 0,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].CurrentEntry == 0");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].CurrentEntry == 0,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].CurrentEntry == 0");

    UtAssert_True(MD_AppData.MD_DwellTables[0].PktOffset == 0, "MD_AppData.MD_DwellTables[0].PktOffset == 0");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].PktOffset == 0,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].PktOffset == 0");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].PktOffset == 0,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].PktOffset == 0");

    UtAssert_True(MD_AppData.MD_DwellTables[0].Countdown == 2, "MD_AppData.MD_DwellTables[0].Countdown == 2");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Countdown == 2,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Countdown == 2");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Countdown == 2,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Countdown == 2");

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    UtAssert_True(call_count_CFE_EVS_SendEvent == 0, "CFE_EVS_SendEvent was called %u time(s), expected 0",
                  call_count_CFE_EVS_SendEvent);

} /* end MD_DwellLoop_Test_SendDwellPacket */

void MD_DwellLoop_Test_MoreAddressesToRead(void)
{
    MD_AppData.MD_DwellTables[0].AddrCount                       = 2;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].AddrCount = 2;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].AddrCount = 2;

    MD_AppData.MD_DwellTables[0].Enabled                       = MD_DWELL_STREAM_ENABLED;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Enabled = MD_DWELL_STREAM_ENABLED;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Enabled = MD_DWELL_STREAM_ENABLED;

    MD_AppData.MD_DwellTables[0].Rate                       = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Rate = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Rate = 1;

    MD_AppData.MD_DwellTables[0].CurrentEntry                       = 0;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].CurrentEntry = 0;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].CurrentEntry = 0;

    MD_AppData.MD_DwellTables[0].Entry[0].Delay                       = 2;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Entry[0].Delay = 2;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Entry[0].Delay = 2;

    MD_AppData.MD_DwellTables[0].Countdown                       = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Countdown = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Countdown = 1;

    MD_AppData.MD_DwellTables[0].Entry[0].Length                       = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Entry[0].Length = 2;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Entry[0].Length = 4;

    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Entry[0].ResolvedAddress = 2;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Entry[0].ResolvedAddress = 4;

    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_MemRead8), 1, CFE_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_MemRead16), 1, CFE_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_MemRead32), 1, CFE_SUCCESS);

    /* Execute the function being tested */
    MD_DwellLoop();

    /* Verify results */
    UtAssert_True(MD_AppData.MD_DwellTables[0].Countdown == 2, "MD_AppData.MD_DwellTables[0].Countdown == 2");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Countdown == 2,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Countdown == 2");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Countdown == 2,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Countdown == 2");

    UtAssert_True(MD_AppData.MD_DwellTables[0].CurrentEntry == 1, "MD_AppData.MD_DwellTables[0].CurrentEntry == 1");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].CurrentEntry == 1,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].CurrentEntry == 1");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].CurrentEntry == 1,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].CurrentEntry == 1");

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    UtAssert_True(call_count_CFE_EVS_SendEvent == 0, "CFE_EVS_SendEvent was called %u time(s), expected 0",
                  call_count_CFE_EVS_SendEvent);

} /* end MD_DwellLoop_Test_MoreAddressesToRead */

void MD_DwellLoop_Test_ZeroRate(void)
{
    MD_AppData.MD_DwellTables[0].AddrCount                       = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].AddrCount = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].AddrCount = 1;

    MD_AppData.MD_DwellTables[0].Enabled                       = MD_DWELL_STREAM_ENABLED;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Enabled = MD_DWELL_STREAM_ENABLED;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Enabled = MD_DWELL_STREAM_ENABLED;

    MD_AppData.MD_DwellTables[0].Rate                       = 0;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Rate = 0;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Rate = 0;

    MD_AppData.MD_DwellTables[0].CurrentEntry                       = 0;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].CurrentEntry = 0;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].CurrentEntry = 0;

    MD_AppData.MD_DwellTables[0].Entry[0].Delay                       = 2;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Entry[0].Delay = 2;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Entry[0].Delay = 2;

    MD_AppData.MD_DwellTables[0].Countdown                       = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Countdown = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Countdown = 1;

    MD_AppData.MD_DwellTables[0].Entry[0].Length                       = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Entry[0].Length = 2;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Entry[0].Length = 4;

    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Entry[0].ResolvedAddress = 2;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Entry[0].ResolvedAddress = 4;

    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_MemRead8), 1, CFE_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_MemRead16), 1, CFE_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_MemRead32), 1, CFE_SUCCESS);

    /* Execute the function being tested */
    MD_DwellLoop();

    /* Verify results */
    UtAssert_True(MD_AppData.MD_DwellTables[0].CurrentEntry == 0, "MD_AppData.MD_DwellTables[0].CurrentEntry == 0");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].CurrentEntry == 0,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].CurrentEntry == 0");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].CurrentEntry == 0,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].CurrentEntry == 0");

    UtAssert_True(MD_AppData.MD_DwellTables[0].PktOffset == 0, "MD_AppData.MD_DwellTables[0].PktOffset == 0");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].PktOffset == 0,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].PktOffset == 0");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].PktOffset == 0,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].PktOffset == 0");

    UtAssert_True(MD_AppData.MD_DwellTables[0].Countdown == 1, "MD_AppData.MD_DwellTables[0].Countdown == 1");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Countdown == 1,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Countdown == 1");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Countdown == 1,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Countdown == 1");

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    UtAssert_True(call_count_CFE_EVS_SendEvent == 0, "CFE_EVS_SendEvent was called %u time(s), expected 0",
                  call_count_CFE_EVS_SendEvent);

} /* end MD_DwellLoop_Test_ZeroRate */

void MD_DwellLoop_Test_DataError(void)
{
    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Dwell Table failed to read entry %%d in table %%d ");

    CFE_EVS_SendEvent_context_t context_CFE_EVS_SendEvent;
    UT_SetHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_Utils_stub_reporter_hook, &context_CFE_EVS_SendEvent);

    MD_AppData.MD_DwellTables[0].AddrCount                       = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].AddrCount = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].AddrCount = 1;

    MD_AppData.MD_DwellTables[0].Enabled                       = MD_DWELL_STREAM_ENABLED;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Enabled = MD_DWELL_STREAM_ENABLED;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Enabled = MD_DWELL_STREAM_ENABLED;

    MD_AppData.MD_DwellTables[0].Rate                       = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Rate = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Rate = 1;

    MD_AppData.MD_DwellTables[0].CurrentEntry                       = 0;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].CurrentEntry = 0;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].CurrentEntry = 0;

    MD_AppData.MD_DwellTables[0].Entry[0].Delay                       = 2;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Entry[0].Delay = 2;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Entry[0].Delay = 2;

    MD_AppData.MD_DwellTables[0].Countdown                       = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Countdown = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Countdown = 1;

    MD_AppData.MD_DwellTables[0].Entry[0].Length                       = 1;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Entry[0].Length = 2;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Entry[0].Length = 4;

    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Entry[0].ResolvedAddress = 2;
    MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Entry[0].ResolvedAddress = 4;

    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_MemRead8), 1, -1);
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_MemRead16), 1, -1);
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_MemRead32), 1, -1);

    /* Execute the function being tested */
    MD_DwellLoop();

    /* Verify results */
    UtAssert_True(MD_AppData.MD_DwellTables[0].CurrentEntry == 0, "MD_AppData.MD_DwellTables[0].CurrentEntry == 0");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].CurrentEntry == 0,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].CurrentEntry == 0");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].CurrentEntry == 0,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].CurrentEntry == 0");

    UtAssert_True(MD_AppData.MD_DwellTables[0].PktOffset == 0, "MD_AppData.MD_DwellTables[0].PktOffset == 0");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].PktOffset == 0,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].PktOffset == 0");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].PktOffset == 0,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].PktOffset == 0");

    UtAssert_True(MD_AppData.MD_DwellTables[0].Countdown == 2, "MD_AppData.MD_DwellTables[0].Countdown == 2");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Countdown == 2,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES / 2].Countdown == 2");
    UtAssert_True(MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Countdown == 2,
                  "MD_AppData.MD_DwellTables[MD_NUM_DWELL_TABLES - 1].Countdown == 2");

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventID, MD_DWELL_LOOP_GET_DWELL_DATA_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent.EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent.Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent.Spec);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    UtAssert_True(call_count_CFE_EVS_SendEvent == 1, "CFE_EVS_SendEvent was called %u time(s), expected 1",
                  call_count_CFE_EVS_SendEvent);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    UtAssert_True(call_count_CFE_EVS_SendEvent == 1, "CFE_EVS_SendEvent was called %u time(s), expected 1",
                  call_count_CFE_EVS_SendEvent);

} /* end MD_DwellLoop_Test_DataError */

void MD_GetDwellData_Test_MemRead8Error(void)
{
    int32  Result;
    uint16 TblIndex   = 1;
    uint16 EntryIndex = 1;

    MD_AppData.MD_DwellTables[TblIndex].Entry[EntryIndex].Length = 1;
    MD_AppData.MD_DwellTables[TblIndex].PktOffset                = 0;

    /* Cause Status to be set to -1 */
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_MemRead8), 1, -1);

    /* Execute the function being tested */
    Result = MD_GetDwellData(TblIndex, EntryIndex);

    /* Verify results */
    UtAssert_True(MD_AppData.MD_DwellTables[TblIndex].PktOffset == 1,
                  "MD_AppData.MD_DwellTables[TblIndex].PktOffset == 1");

    UtAssert_True(Result == -1, "Result == -1");

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    UtAssert_True(call_count_CFE_EVS_SendEvent == 0, "CFE_EVS_SendEvent was called %u time(s), expected 0",
                  call_count_CFE_EVS_SendEvent);

} /* end MD_GetDwellData_Test_MemRead8Error */

void MD_GetDwellData_Test_MemRead16Error(void)
{
    int32  Result;
    uint16 TblIndex   = 1;
    uint16 EntryIndex = 1;

    MD_AppData.MD_DwellTables[TblIndex].Entry[EntryIndex].Length = 2;
    MD_AppData.MD_DwellTables[TblIndex].PktOffset                = 0;

    /* Cause Status to be set to -1 */
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_MemRead16), 1, -1);

    /* Execute the function being tested */
    Result = MD_GetDwellData(TblIndex, EntryIndex);

    /* Verify results */
    UtAssert_True(MD_AppData.MD_DwellTables[TblIndex].PktOffset == 2,
                  "MD_AppData.MD_DwellTables[TblIndex].PktOffset == 2");

    UtAssert_True(Result == -1, "Result == -1");

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    UtAssert_True(call_count_CFE_EVS_SendEvent == 0, "CFE_EVS_SendEvent was called %u time(s), expected 0",
                  call_count_CFE_EVS_SendEvent);

} /* end MD_GetDwellData_Test_MemRead16Error */

void MD_GetDwellData_Test_MemRead32Error(void)
{
    int32  Result;
    uint16 TblIndex   = 1;
    uint16 EntryIndex = 1;

    MD_AppData.MD_DwellTables[TblIndex].Entry[EntryIndex].Length = 4;
    MD_AppData.MD_DwellTables[TblIndex].PktOffset                = 0;

    /* Cause Status to be set to -1 */
    UT_SetDeferredRetcode(UT_KEY(CFE_PSP_MemRead32), 1, -1);

    /* Execute the function being tested */
    Result = MD_GetDwellData(TblIndex, EntryIndex);

    /* Verify results */
    UtAssert_True(MD_AppData.MD_DwellTables[TblIndex].PktOffset == 4,
                  "MD_AppData.MD_DwellTables[TblIndex].PktOffset == 4");

    UtAssert_True(Result == -1, "Result == -1");

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    UtAssert_True(call_count_CFE_EVS_SendEvent == 0, "CFE_EVS_SendEvent was called %u time(s), expected 0",
                  call_count_CFE_EVS_SendEvent);

} /* end MD_GetDwellData_Test_MemRead32Error */

void MD_GetDwellData_Test_InvalidDwellLength(void)
{
    int32  Result;
    uint16 TblIndex   = 1;
    uint16 EntryIndex = 1;

    MD_AppData.MD_DwellTables[TblIndex].Entry[EntryIndex].Length = 5;
    MD_AppData.MD_DwellTables[TblIndex].PktOffset                = 0;

    /* Execute the function being tested */
    Result = MD_GetDwellData(TblIndex, EntryIndex);

    /* Verify results */
    UtAssert_True(MD_AppData.MD_DwellTables[TblIndex].PktOffset == 5,
                  "MD_AppData.MD_DwellTables[TblIndex].PktOffset == 5");

    UtAssert_True(Result == -1, "Result == -1");

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    UtAssert_True(call_count_CFE_EVS_SendEvent == 0, "CFE_EVS_SendEvent was called %u time(s), expected 0",
                  call_count_CFE_EVS_SendEvent);

} /* end MD_GetDwellData_Test_InvalidDwellLength */

void MD_GetDwellData_Test_Success(void)
{
    int32  Result;
    uint16 TblIndex   = 1;
    uint16 EntryIndex = 1;

    MD_AppData.MD_DwellTables[TblIndex].Entry[EntryIndex].Length = 1;
    MD_AppData.MD_DwellTables[TblIndex].PktOffset                = 0;

    /* Execute the function being tested */
    Result = MD_GetDwellData(TblIndex, EntryIndex);

    /* Verify results */
    UtAssert_True(MD_AppData.MD_DwellTables[TblIndex].PktOffset == 1,
                  "MD_AppData.MD_DwellTables[TblIndex].PktOffset == 1");

    UtAssert_True(Result == CFE_SUCCESS, "Result == CFE_SUCCESS");

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    UtAssert_True(call_count_CFE_EVS_SendEvent == 0, "CFE_EVS_SendEvent was called %u time(s), expected 0",
                  call_count_CFE_EVS_SendEvent);

} /* end MD_GetDwellData_Test_Success */

void MD_SendDwellPkt_Test(void)
{
    uint16 TableIndex                               = 1;
    MD_AppData.MD_DwellTables[TableIndex].AddrCount = 3;
    MD_AppData.MD_DwellTables[TableIndex].Rate      = 4;
    MD_AppData.MD_DwellTables[TableIndex].DataSize  = 5;

#if MD_SIGNATURE_OPTION == 1
    strncpy(MD_AppData.MD_DwellTables[TableIndex].Signature, "signature", MD_SIGNATURE_FIELD_LENGTH - 1);
#endif

    /* Execute the function being tested */
    MD_SendDwellPkt(TableIndex);

    /* Verify results */
    UtAssert_True(MD_AppData.MD_DwellPkt[TableIndex].TableId == 2, "MD_AppData.MD_DwellPkt[TableIndex].TableId == 2");
    UtAssert_True(MD_AppData.MD_DwellPkt[TableIndex].AddrCount == 3,
                  "MD_AppData.MD_DwellPkt[TableIndex].AddrCount == 3");
    UtAssert_True(MD_AppData.MD_DwellPkt[TableIndex].Rate == 4, "MD_AppData.MD_DwellPkt[TableIndex].Rate == 4");

#if MD_SIGNATURE_OPTION == 1
    UtAssert_True(
        strncmp(MD_AppData.MD_DwellTables[TableIndex].Signature, "signature", MD_SIGNATURE_FIELD_LENGTH - 1) == 0,
        "MD_AppData.MD_DwellTables[TableIndex].Signature, 'signature', MD_SIGNATURE_FIELD_LENGTH - 1) == 0");

    UtAssert_True(MD_AppData.MD_DwellTables[TableIndex].Signature[MD_SIGNATURE_FIELD_LENGTH - 1] == '\0',
                  "MD_AppData.MD_DwellTables[TableIndex].Signature[MD_SIGNATURE_FIELD_LENGTH - 1] == ''");
#endif

    UtAssert_True(MD_AppData.MD_DwellTables[TableIndex].DataSize == 5,
                  "MD_AppData.MD_DwellTables[TableIndex].DataSize == 5");

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    UtAssert_True(call_count_CFE_EVS_SendEvent == 0, "CFE_EVS_SendEvent was called %u time(s), expected 0",
                  call_count_CFE_EVS_SendEvent);

} /* end MD_SendDwellPkt_Test */

void MD_NoDwellRate_Test(void)
{
    uint16 TableIndex                               = 1;
    MD_AppData.MD_DwellTables[TableIndex].AddrCount = 3;
    MD_AppData.MD_DwellTables[TableIndex].Rate      = 0;
    MD_AppData.MD_DwellTables[TableIndex].DataSize  = 5;

#if MD_SIGNATURE_OPTION == 1
    strncpy(MD_AppData.MD_DwellTables[TableIndex].Signature, "signature", MD_SIGNATURE_FIELD_LENGTH - 1);
#endif

    /* Execute the function being tested */
    MD_SendDwellPkt(TableIndex);

    /* Verify results */
    UtAssert_True(MD_AppData.MD_DwellPkt[TableIndex].TableId == 2, "MD_AppData.MD_DwellPkt[TableIndex].TableId == 2");
    UtAssert_True(MD_AppData.MD_DwellPkt[TableIndex].AddrCount == 3,
                  "MD_AppData.MD_DwellPkt[TableIndex].AddrCount == 3");
    UtAssert_True(MD_AppData.MD_DwellPkt[TableIndex].Rate == 0, "MD_AppData.MD_DwellPkt[TableIndex].Rate == 0");

#if MD_SIGNATURE_OPTION == 1
    UtAssert_True(
        strncmp(MD_AppData.MD_DwellTables[TableIndex].Signature, "signature", MD_SIGNATURE_FIELD_LENGTH - 1) == 0,
        "MD_AppData.MD_DwellTables[TableIndex].Signature, 'signature', MD_SIGNATURE_FIELD_LENGTH - 1) == 0");

    UtAssert_True(MD_AppData.MD_DwellTables[TableIndex].Signature[MD_SIGNATURE_FIELD_LENGTH - 1] == '\0',
                  "MD_AppData.MD_DwellTables[TableIndex].Signature[MD_SIGNATURE_FIELD_LENGTH - 1] == ''");
#endif

    UtAssert_True(MD_AppData.MD_DwellTables[TableIndex].DataSize == 5,
                  "MD_AppData.MD_DwellTables[TableIndex].DataSize == 5");

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    UtAssert_True(call_count_CFE_EVS_SendEvent == 0, "CFE_EVS_SendEvent was called %u time(s), expected 0",
                  call_count_CFE_EVS_SendEvent);

} /* end MD_NoDwellRate_Test */

void MD_StartDwellStream_Test(void)
{
    uint16 TableIndex = 1;

    /* Execute the function being tested */
    MD_StartDwellStream(TableIndex);

    /* Verify results */
    UtAssert_True(MD_AppData.MD_DwellTables[TableIndex].Countdown == 1,
                  "MD_AppData.MD_DwellTables[TableIndex].Countdown == 1");
    UtAssert_True(MD_AppData.MD_DwellTables[TableIndex].CurrentEntry == 0,
                  "MD_AppData.MD_DwellTables[TableIndex].CurrentEntry == 0");
    UtAssert_True(MD_AppData.MD_DwellTables[TableIndex].PktOffset == 0,
                  "MD_AppData.MD_DwellTables[TableIndex].PktOffset == 0");

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    UtAssert_True(call_count_CFE_EVS_SendEvent == 0, "CFE_EVS_SendEvent was called %u time(s), expected 0",
                  call_count_CFE_EVS_SendEvent);

} /* end MD_StartDwellStream_Test */

void UtTest_Setup(void)
{
    UtTest_Add(MD_DwellLoop_Test_PacketAlreadyFull, MD_Test_Setup, MD_Test_TearDown,
               "MD_DwellLoop_Test_PacketAlreadyFull");
    UtTest_Add(MD_DwellLoop_Test_SendDwellPacket, MD_Test_Setup, MD_Test_TearDown, "MD_DwellLoop_Test_SendDwellPacket");
    UtTest_Add(MD_DwellLoop_Test_MoreAddressesToRead, MD_Test_Setup, MD_Test_TearDown,
               "MD_DwellLoop_Test_MoreAddressesToRead");
    UtTest_Add(MD_DwellLoop_Test_ZeroRate, MD_Test_Setup, MD_Test_TearDown, "MD_DwellLoop_Test_ZeroRate");
    UtTest_Add(MD_DwellLoop_Test_DataError, MD_Test_Setup, MD_Test_TearDown, "MD_DwellLoop_Test_DataError");

    UtTest_Add(MD_GetDwellData_Test_MemRead8Error, MD_Test_Setup, MD_Test_TearDown,
               "MD_GetDwellData_Test_MemRead8Error");
    UtTest_Add(MD_GetDwellData_Test_MemRead16Error, MD_Test_Setup, MD_Test_TearDown,
               "MD_GetDwellData_Test_MemRead16Error");
    UtTest_Add(MD_GetDwellData_Test_MemRead32Error, MD_Test_Setup, MD_Test_TearDown,
               "MD_GetDwellData_Test_MemRead32Error");

    UtTest_Add(MD_GetDwellData_Test_InvalidDwellLength, MD_Test_Setup, MD_Test_TearDown,
               "MD_GetDwellData_Test_InvalidDwellLength");
    UtTest_Add(MD_GetDwellData_Test_Success, MD_Test_Setup, MD_Test_TearDown, "MD_GetDwellData_Test_Success");

    UtTest_Add(MD_SendDwellPkt_Test, MD_Test_Setup, MD_Test_TearDown, "MD_SendDwellPkt_Test");
    UtTest_Add(MD_NoDwellRate_Test, MD_Test_Setup, MD_Test_TearDown, "MD_NoDwellRate_Test");
    UtTest_Add(MD_StartDwellStream_Test, MD_Test_Setup, MD_Test_TearDown, "MD_StartDwellStream_Test");

} /* end UtTest_Setup */

/************************/
/*  End of File Comment */
/************************/