/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-K04
 */

#include <xdc/std.h>

#include <ti/sysbios/family/c28/Hwi.h>
extern const ti_sysbios_family_c28_Hwi_Handle hwi0;

#include <ti/sysbios/family/c28/Timer.h>
extern const ti_sysbios_family_c28_Timer_Handle Timer1;

#include <ti/sysbios/family/c28/Timer.h>
extern const ti_sysbios_family_c28_Timer_Handle My_Timer0;

#include <ti/sysbios/knl/Swi.h>
extern const ti_sysbios_knl_Swi_Handle SwiSCIbRX;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle DeckTimers;

#include <ti/sysbios/knl/Semaphore.h>
extern const ti_sysbios_knl_Semaphore_Handle mySem;

#include <ti/sysbios/family/c28/Hwi.h>
extern const ti_sysbios_family_c28_Hwi_Handle hwi1;

#include <ti/sysbios/knl/Swi.h>
extern const ti_sysbios_knl_Swi_Handle SwiSCIcRX;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle Convert;

#include <ti/sysbios/knl/Semaphore.h>
extern const ti_sysbios_knl_Semaphore_Handle DSP;

extern int xdc_runtime_Startup__EXECFXN__C;

extern int xdc_runtime_Startup__RESETFXN__C;

extern int xdc_rov_runtime_Mon__checksum;

extern int xdc_rov_runtime_Mon__write;

