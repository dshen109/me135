/*******************************************************************************
* File Name: LEDPWM.h
* Version 3.30
*
* Description:
*  Contains the prototypes and constants for the functions available to the
*  PWM user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PWM_LEDPWM_H)
#define CY_PWM_LEDPWM_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 LEDPWM_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define LEDPWM_Resolution                     (8u)
#define LEDPWM_UsingFixedFunction             (0u)
#define LEDPWM_DeadBandMode                   (0u)
#define LEDPWM_KillModeMinTime                (0u)
#define LEDPWM_KillMode                       (0u)
#define LEDPWM_PWMMode                        (1u)
#define LEDPWM_PWMModeIsCenterAligned         (0u)
#define LEDPWM_DeadBandUsed                   (0u)
#define LEDPWM_DeadBand2_4                    (0u)

#if !defined(LEDPWM_PWMUDB_genblk8_stsreg__REMOVED)
    #define LEDPWM_UseStatus                  (1u)
#else
    #define LEDPWM_UseStatus                  (0u)
#endif /* !defined(LEDPWM_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(LEDPWM_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define LEDPWM_UseControl                 (1u)
#else
    #define LEDPWM_UseControl                 (0u)
#endif /* !defined(LEDPWM_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define LEDPWM_UseOneCompareMode              (0u)
#define LEDPWM_MinimumKillTime                (1u)
#define LEDPWM_EnableMode                     (0u)

#define LEDPWM_CompareMode1SW                 (0u)
#define LEDPWM_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define LEDPWM__B_PWM__DISABLED 0
#define LEDPWM__B_PWM__ASYNCHRONOUS 1
#define LEDPWM__B_PWM__SINGLECYCLE 2
#define LEDPWM__B_PWM__LATCHED 3
#define LEDPWM__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define LEDPWM__B_PWM__DBMDISABLED 0
#define LEDPWM__B_PWM__DBM_2_4_CLOCKS 1
#define LEDPWM__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define LEDPWM__B_PWM__ONE_OUTPUT 0
#define LEDPWM__B_PWM__TWO_OUTPUTS 1
#define LEDPWM__B_PWM__DUAL_EDGE 2
#define LEDPWM__B_PWM__CENTER_ALIGN 3
#define LEDPWM__B_PWM__DITHER 5
#define LEDPWM__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define LEDPWM__B_PWM__LESS_THAN 1
#define LEDPWM__B_PWM__LESS_THAN_OR_EQUAL 2
#define LEDPWM__B_PWM__GREATER_THAN 3
#define LEDPWM__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define LEDPWM__B_PWM__EQUAL 0
#define LEDPWM__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!LEDPWM_UsingFixedFunction)
        uint8 PWMUdb;               /* PWM Current Counter value  */
        #if(!LEDPWM_PWMModeIsCenterAligned)
            uint8 PWMPeriod;
        #endif /* (!LEDPWM_PWMModeIsCenterAligned) */
        #if (LEDPWM_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (LEDPWM_UseStatus) */

        /* Backup for Deadband parameters */
        #if(LEDPWM_DeadBandMode == LEDPWM__B_PWM__DBM_256_CLOCKS || \
            LEDPWM_DeadBandMode == LEDPWM__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(LEDPWM_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (LEDPWM_KillModeMinTime) */

        /* Backup control register */
        #if(LEDPWM_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (LEDPWM_UseControl) */

    #endif /* (!LEDPWM_UsingFixedFunction) */

}LEDPWM_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    LEDPWM_Start(void) ;
void    LEDPWM_Stop(void) ;

#if (LEDPWM_UseStatus || LEDPWM_UsingFixedFunction)
    void  LEDPWM_SetInterruptMode(uint8 interruptMode) ;
    uint8 LEDPWM_ReadStatusRegister(void) ;
#endif /* (LEDPWM_UseStatus || LEDPWM_UsingFixedFunction) */

#define LEDPWM_GetInterruptSource() LEDPWM_ReadStatusRegister()

#if (LEDPWM_UseControl)
    uint8 LEDPWM_ReadControlRegister(void) ;
    void  LEDPWM_WriteControlRegister(uint8 control)
          ;
#endif /* (LEDPWM_UseControl) */

#if (LEDPWM_UseOneCompareMode)
   #if (LEDPWM_CompareMode1SW)
       void    LEDPWM_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (LEDPWM_CompareMode1SW) */
#else
    #if (LEDPWM_CompareMode1SW)
        void    LEDPWM_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (LEDPWM_CompareMode1SW) */
    #if (LEDPWM_CompareMode2SW)
        void    LEDPWM_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (LEDPWM_CompareMode2SW) */
#endif /* (LEDPWM_UseOneCompareMode) */

#if (!LEDPWM_UsingFixedFunction)
    uint8   LEDPWM_ReadCounter(void) ;
    uint8 LEDPWM_ReadCapture(void) ;

    #if (LEDPWM_UseStatus)
            void LEDPWM_ClearFIFO(void) ;
    #endif /* (LEDPWM_UseStatus) */

    void    LEDPWM_WriteCounter(uint8 counter)
            ;
#endif /* (!LEDPWM_UsingFixedFunction) */

void    LEDPWM_WritePeriod(uint8 period)
        ;
uint8 LEDPWM_ReadPeriod(void) ;

#if (LEDPWM_UseOneCompareMode)
    void    LEDPWM_WriteCompare(uint8 compare)
            ;
    uint8 LEDPWM_ReadCompare(void) ;
#else
    void    LEDPWM_WriteCompare1(uint8 compare)
            ;
    uint8 LEDPWM_ReadCompare1(void) ;
    void    LEDPWM_WriteCompare2(uint8 compare)
            ;
    uint8 LEDPWM_ReadCompare2(void) ;
#endif /* (LEDPWM_UseOneCompareMode) */


#if (LEDPWM_DeadBandUsed)
    void    LEDPWM_WriteDeadTime(uint8 deadtime) ;
    uint8   LEDPWM_ReadDeadTime(void) ;
#endif /* (LEDPWM_DeadBandUsed) */

#if ( LEDPWM_KillModeMinTime)
    void LEDPWM_WriteKillTime(uint8 killtime) ;
    uint8 LEDPWM_ReadKillTime(void) ;
#endif /* ( LEDPWM_KillModeMinTime) */

void LEDPWM_Init(void) ;
void LEDPWM_Enable(void) ;
void LEDPWM_Sleep(void) ;
void LEDPWM_Wakeup(void) ;
void LEDPWM_SaveConfig(void) ;
void LEDPWM_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define LEDPWM_INIT_PERIOD_VALUE          (255u)
#define LEDPWM_INIT_COMPARE_VALUE1        (127u)
#define LEDPWM_INIT_COMPARE_VALUE2        (63u)
#define LEDPWM_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    LEDPWM_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    LEDPWM_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    LEDPWM_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    LEDPWM_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define LEDPWM_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  LEDPWM_CTRL_CMPMODE2_SHIFT)
#define LEDPWM_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  LEDPWM_CTRL_CMPMODE1_SHIFT)
#define LEDPWM_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (LEDPWM_UsingFixedFunction)
   #define LEDPWM_PERIOD_LSB              (*(reg16 *) LEDPWM_PWMHW__PER0)
   #define LEDPWM_PERIOD_LSB_PTR          ( (reg16 *) LEDPWM_PWMHW__PER0)
   #define LEDPWM_COMPARE1_LSB            (*(reg16 *) LEDPWM_PWMHW__CNT_CMP0)
   #define LEDPWM_COMPARE1_LSB_PTR        ( (reg16 *) LEDPWM_PWMHW__CNT_CMP0)
   #define LEDPWM_COMPARE2_LSB            (0x00u)
   #define LEDPWM_COMPARE2_LSB_PTR        (0x00u)
   #define LEDPWM_COUNTER_LSB             (*(reg16 *) LEDPWM_PWMHW__CNT_CMP0)
   #define LEDPWM_COUNTER_LSB_PTR         ( (reg16 *) LEDPWM_PWMHW__CNT_CMP0)
   #define LEDPWM_CAPTURE_LSB             (*(reg16 *) LEDPWM_PWMHW__CAP0)
   #define LEDPWM_CAPTURE_LSB_PTR         ( (reg16 *) LEDPWM_PWMHW__CAP0)
   #define LEDPWM_RT1                     (*(reg8 *)  LEDPWM_PWMHW__RT1)
   #define LEDPWM_RT1_PTR                 ( (reg8 *)  LEDPWM_PWMHW__RT1)

#else
   #if (LEDPWM_Resolution == 8u) /* 8bit - PWM */

       #if(LEDPWM_PWMModeIsCenterAligned)
           #define LEDPWM_PERIOD_LSB      (*(reg8 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
           #define LEDPWM_PERIOD_LSB_PTR  ((reg8 *)   LEDPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #else
           #define LEDPWM_PERIOD_LSB      (*(reg8 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__F0_REG)
           #define LEDPWM_PERIOD_LSB_PTR  ((reg8 *)   LEDPWM_PWMUDB_sP8_pwmdp_u0__F0_REG)
       #endif /* (LEDPWM_PWMModeIsCenterAligned) */

       #define LEDPWM_COMPARE1_LSB        (*(reg8 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define LEDPWM_COMPARE1_LSB_PTR    ((reg8 *)   LEDPWM_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define LEDPWM_COMPARE2_LSB        (*(reg8 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define LEDPWM_COMPARE2_LSB_PTR    ((reg8 *)   LEDPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define LEDPWM_COUNTERCAP_LSB      (*(reg8 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define LEDPWM_COUNTERCAP_LSB_PTR  ((reg8 *)   LEDPWM_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define LEDPWM_COUNTER_LSB         (*(reg8 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define LEDPWM_COUNTER_LSB_PTR     ((reg8 *)   LEDPWM_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define LEDPWM_CAPTURE_LSB         (*(reg8 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__F1_REG)
       #define LEDPWM_CAPTURE_LSB_PTR     ((reg8 *)   LEDPWM_PWMUDB_sP8_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(LEDPWM_PWMModeIsCenterAligned)
               #define LEDPWM_PERIOD_LSB      (*(reg16 *) LEDPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
               #define LEDPWM_PERIOD_LSB_PTR  ((reg16 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #else
               #define LEDPWM_PERIOD_LSB      (*(reg16 *) LEDPWM_PWMUDB_sP8_pwmdp_u0__F0_REG)
               #define LEDPWM_PERIOD_LSB_PTR  ((reg16 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__F0_REG)
            #endif /* (LEDPWM_PWMModeIsCenterAligned) */

            #define LEDPWM_COMPARE1_LSB       (*(reg16 *) LEDPWM_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define LEDPWM_COMPARE1_LSB_PTR   ((reg16 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define LEDPWM_COMPARE2_LSB       (*(reg16 *) LEDPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define LEDPWM_COMPARE2_LSB_PTR   ((reg16 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define LEDPWM_COUNTERCAP_LSB     (*(reg16 *) LEDPWM_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define LEDPWM_COUNTERCAP_LSB_PTR ((reg16 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define LEDPWM_COUNTER_LSB        (*(reg16 *) LEDPWM_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define LEDPWM_COUNTER_LSB_PTR    ((reg16 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define LEDPWM_CAPTURE_LSB        (*(reg16 *) LEDPWM_PWMUDB_sP8_pwmdp_u0__F1_REG)
            #define LEDPWM_CAPTURE_LSB_PTR    ((reg16 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__F1_REG)
        #else
            #if(LEDPWM_PWMModeIsCenterAligned)
               #define LEDPWM_PERIOD_LSB      (*(reg16 *) LEDPWM_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
               #define LEDPWM_PERIOD_LSB_PTR  ((reg16 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #else
               #define LEDPWM_PERIOD_LSB      (*(reg16 *) LEDPWM_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
               #define LEDPWM_PERIOD_LSB_PTR  ((reg16 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
            #endif /* (LEDPWM_PWMModeIsCenterAligned) */

            #define LEDPWM_COMPARE1_LSB       (*(reg16 *) LEDPWM_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define LEDPWM_COMPARE1_LSB_PTR   ((reg16 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define LEDPWM_COMPARE2_LSB       (*(reg16 *) LEDPWM_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define LEDPWM_COMPARE2_LSB_PTR   ((reg16 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define LEDPWM_COUNTERCAP_LSB     (*(reg16 *) LEDPWM_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define LEDPWM_COUNTERCAP_LSB_PTR ((reg16 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define LEDPWM_COUNTER_LSB        (*(reg16 *) LEDPWM_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define LEDPWM_COUNTER_LSB_PTR    ((reg16 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define LEDPWM_CAPTURE_LSB        (*(reg16 *) LEDPWM_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
            #define LEDPWM_CAPTURE_LSB_PTR    ((reg16 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define LEDPWM_AUX_CONTROLDP1          (*(reg8 *)  LEDPWM_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)
       #define LEDPWM_AUX_CONTROLDP1_PTR      ((reg8 *)   LEDPWM_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (LEDPWM_Resolution == 8) */

   #define LEDPWM_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__A1_REG)
   #define LEDPWM_AUX_CONTROLDP0          (*(reg8 *)  LEDPWM_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)
   #define LEDPWM_AUX_CONTROLDP0_PTR      ((reg8 *)   LEDPWM_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (LEDPWM_UsingFixedFunction) */

#if(LEDPWM_KillModeMinTime )
    #define LEDPWM_KILLMODEMINTIME        (*(reg8 *)  LEDPWM_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define LEDPWM_KILLMODEMINTIME_PTR    ((reg8 *)   LEDPWM_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (LEDPWM_KillModeMinTime ) */

#if(LEDPWM_DeadBandMode == LEDPWM__B_PWM__DBM_256_CLOCKS)
    #define LEDPWM_DEADBAND_COUNT         (*(reg8 *)  LEDPWM_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define LEDPWM_DEADBAND_COUNT_PTR     ((reg8 *)   LEDPWM_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define LEDPWM_DEADBAND_LSB_PTR       ((reg8 *)   LEDPWM_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define LEDPWM_DEADBAND_LSB           (*(reg8 *)  LEDPWM_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(LEDPWM_DeadBandMode == LEDPWM__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (LEDPWM_UsingFixedFunction)
        #define LEDPWM_DEADBAND_COUNT         (*(reg8 *)  LEDPWM_PWMHW__CFG0)
        #define LEDPWM_DEADBAND_COUNT_PTR     ((reg8 *)   LEDPWM_PWMHW__CFG0)
        #define LEDPWM_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << LEDPWM_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define LEDPWM_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define LEDPWM_DEADBAND_COUNT         (*(reg8 *)  LEDPWM_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define LEDPWM_DEADBAND_COUNT_PTR     ((reg8 *)   LEDPWM_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define LEDPWM_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << LEDPWM_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define LEDPWM_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (LEDPWM_UsingFixedFunction) */
#endif /* (LEDPWM_DeadBandMode == LEDPWM__B_PWM__DBM_256_CLOCKS) */



#if (LEDPWM_UsingFixedFunction)
    #define LEDPWM_STATUS                 (*(reg8 *) LEDPWM_PWMHW__SR0)
    #define LEDPWM_STATUS_PTR             ((reg8 *) LEDPWM_PWMHW__SR0)
    #define LEDPWM_STATUS_MASK            (*(reg8 *) LEDPWM_PWMHW__SR0)
    #define LEDPWM_STATUS_MASK_PTR        ((reg8 *) LEDPWM_PWMHW__SR0)
    #define LEDPWM_CONTROL                (*(reg8 *) LEDPWM_PWMHW__CFG0)
    #define LEDPWM_CONTROL_PTR            ((reg8 *) LEDPWM_PWMHW__CFG0)
    #define LEDPWM_CONTROL2               (*(reg8 *) LEDPWM_PWMHW__CFG1)
    #define LEDPWM_CONTROL3               (*(reg8 *) LEDPWM_PWMHW__CFG2)
    #define LEDPWM_GLOBAL_ENABLE          (*(reg8 *) LEDPWM_PWMHW__PM_ACT_CFG)
    #define LEDPWM_GLOBAL_ENABLE_PTR      ( (reg8 *) LEDPWM_PWMHW__PM_ACT_CFG)
    #define LEDPWM_GLOBAL_STBY_ENABLE     (*(reg8 *) LEDPWM_PWMHW__PM_STBY_CFG)
    #define LEDPWM_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) LEDPWM_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define LEDPWM_BLOCK_EN_MASK          (LEDPWM_PWMHW__PM_ACT_MSK)
    #define LEDPWM_BLOCK_STBY_EN_MASK     (LEDPWM_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define LEDPWM_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define LEDPWM_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define LEDPWM_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define LEDPWM_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define LEDPWM_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define LEDPWM_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define LEDPWM_CTRL_ENABLE            (uint8)((uint8)0x01u << LEDPWM_CTRL_ENABLE_SHIFT)
    #define LEDPWM_CTRL_RESET             (uint8)((uint8)0x01u << LEDPWM_CTRL_RESET_SHIFT)
    #define LEDPWM_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << LEDPWM_CTRL_CMPMODE2_SHIFT)
    #define LEDPWM_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << LEDPWM_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define LEDPWM_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define LEDPWM_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << LEDPWM_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define LEDPWM_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define LEDPWM_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define LEDPWM_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define LEDPWM_STATUS_TC_INT_EN_MASK_SHIFT            (LEDPWM_STATUS_TC_SHIFT - 4u)
    #define LEDPWM_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define LEDPWM_STATUS_CMP1_INT_EN_MASK_SHIFT          (LEDPWM_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define LEDPWM_STATUS_TC              (uint8)((uint8)0x01u << LEDPWM_STATUS_TC_SHIFT)
    #define LEDPWM_STATUS_CMP1            (uint8)((uint8)0x01u << LEDPWM_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define LEDPWM_STATUS_TC_INT_EN_MASK              (uint8)((uint8)LEDPWM_STATUS_TC >> 4u)
    #define LEDPWM_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)LEDPWM_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define LEDPWM_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define LEDPWM_RT1_MASK              (uint8)((uint8)0x03u << LEDPWM_RT1_SHIFT)
    #define LEDPWM_SYNC                  (uint8)((uint8)0x03u << LEDPWM_RT1_SHIFT)
    #define LEDPWM_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define LEDPWM_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << LEDPWM_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define LEDPWM_SYNCDSI_EN            (uint8)((uint8)0x0Fu << LEDPWM_SYNCDSI_SHIFT)


#else
    #define LEDPWM_STATUS                (*(reg8 *)   LEDPWM_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define LEDPWM_STATUS_PTR            ((reg8 *)    LEDPWM_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define LEDPWM_STATUS_MASK           (*(reg8 *)   LEDPWM_PWMUDB_genblk8_stsreg__MASK_REG)
    #define LEDPWM_STATUS_MASK_PTR       ((reg8 *)    LEDPWM_PWMUDB_genblk8_stsreg__MASK_REG)
    #define LEDPWM_STATUS_AUX_CTRL       (*(reg8 *)   LEDPWM_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define LEDPWM_CONTROL               (*(reg8 *)   LEDPWM_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define LEDPWM_CONTROL_PTR           ((reg8 *)    LEDPWM_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define LEDPWM_CTRL_ENABLE_SHIFT      (0x07u)
    #define LEDPWM_CTRL_RESET_SHIFT       (0x06u)
    #define LEDPWM_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define LEDPWM_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define LEDPWM_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define LEDPWM_CTRL_ENABLE            (uint8)((uint8)0x01u << LEDPWM_CTRL_ENABLE_SHIFT)
    #define LEDPWM_CTRL_RESET             (uint8)((uint8)0x01u << LEDPWM_CTRL_RESET_SHIFT)
    #define LEDPWM_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << LEDPWM_CTRL_CMPMODE2_SHIFT)
    #define LEDPWM_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << LEDPWM_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define LEDPWM_STATUS_KILL_SHIFT          (0x05u)
    #define LEDPWM_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define LEDPWM_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define LEDPWM_STATUS_TC_SHIFT            (0x02u)
    #define LEDPWM_STATUS_CMP2_SHIFT          (0x01u)
    #define LEDPWM_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define LEDPWM_STATUS_KILL_INT_EN_MASK_SHIFT          (LEDPWM_STATUS_KILL_SHIFT)
    #define LEDPWM_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (LEDPWM_STATUS_FIFONEMPTY_SHIFT)
    #define LEDPWM_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (LEDPWM_STATUS_FIFOFULL_SHIFT)
    #define LEDPWM_STATUS_TC_INT_EN_MASK_SHIFT            (LEDPWM_STATUS_TC_SHIFT)
    #define LEDPWM_STATUS_CMP2_INT_EN_MASK_SHIFT          (LEDPWM_STATUS_CMP2_SHIFT)
    #define LEDPWM_STATUS_CMP1_INT_EN_MASK_SHIFT          (LEDPWM_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define LEDPWM_STATUS_KILL            (uint8)((uint8)0x00u << LEDPWM_STATUS_KILL_SHIFT )
    #define LEDPWM_STATUS_FIFOFULL        (uint8)((uint8)0x01u << LEDPWM_STATUS_FIFOFULL_SHIFT)
    #define LEDPWM_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << LEDPWM_STATUS_FIFONEMPTY_SHIFT)
    #define LEDPWM_STATUS_TC              (uint8)((uint8)0x01u << LEDPWM_STATUS_TC_SHIFT)
    #define LEDPWM_STATUS_CMP2            (uint8)((uint8)0x01u << LEDPWM_STATUS_CMP2_SHIFT)
    #define LEDPWM_STATUS_CMP1            (uint8)((uint8)0x01u << LEDPWM_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define LEDPWM_STATUS_KILL_INT_EN_MASK            (LEDPWM_STATUS_KILL)
    #define LEDPWM_STATUS_FIFOFULL_INT_EN_MASK        (LEDPWM_STATUS_FIFOFULL)
    #define LEDPWM_STATUS_FIFONEMPTY_INT_EN_MASK      (LEDPWM_STATUS_FIFONEMPTY)
    #define LEDPWM_STATUS_TC_INT_EN_MASK              (LEDPWM_STATUS_TC)
    #define LEDPWM_STATUS_CMP2_INT_EN_MASK            (LEDPWM_STATUS_CMP2)
    #define LEDPWM_STATUS_CMP1_INT_EN_MASK            (LEDPWM_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define LEDPWM_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define LEDPWM_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define LEDPWM_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define LEDPWM_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define LEDPWM_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* LEDPWM_UsingFixedFunction */

#endif  /* CY_PWM_LEDPWM_H */


/* [] END OF FILE */
