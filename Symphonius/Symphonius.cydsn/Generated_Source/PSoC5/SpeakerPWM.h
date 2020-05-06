/*******************************************************************************
* File Name: SpeakerPWM.h
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

#if !defined(CY_PWM_SpeakerPWM_H)
#define CY_PWM_SpeakerPWM_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 SpeakerPWM_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define SpeakerPWM_Resolution                     (8u)
#define SpeakerPWM_UsingFixedFunction             (0u)
#define SpeakerPWM_DeadBandMode                   (0u)
#define SpeakerPWM_KillModeMinTime                (0u)
#define SpeakerPWM_KillMode                       (0u)
#define SpeakerPWM_PWMMode                        (1u)
#define SpeakerPWM_PWMModeIsCenterAligned         (0u)
#define SpeakerPWM_DeadBandUsed                   (0u)
#define SpeakerPWM_DeadBand2_4                    (0u)

#if !defined(SpeakerPWM_PWMUDB_genblk8_stsreg__REMOVED)
    #define SpeakerPWM_UseStatus                  (1u)
#else
    #define SpeakerPWM_UseStatus                  (0u)
#endif /* !defined(SpeakerPWM_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(SpeakerPWM_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define SpeakerPWM_UseControl                 (1u)
#else
    #define SpeakerPWM_UseControl                 (0u)
#endif /* !defined(SpeakerPWM_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define SpeakerPWM_UseOneCompareMode              (0u)
#define SpeakerPWM_MinimumKillTime                (1u)
#define SpeakerPWM_EnableMode                     (0u)

#define SpeakerPWM_CompareMode1SW                 (0u)
#define SpeakerPWM_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define SpeakerPWM__B_PWM__DISABLED 0
#define SpeakerPWM__B_PWM__ASYNCHRONOUS 1
#define SpeakerPWM__B_PWM__SINGLECYCLE 2
#define SpeakerPWM__B_PWM__LATCHED 3
#define SpeakerPWM__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define SpeakerPWM__B_PWM__DBMDISABLED 0
#define SpeakerPWM__B_PWM__DBM_2_4_CLOCKS 1
#define SpeakerPWM__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define SpeakerPWM__B_PWM__ONE_OUTPUT 0
#define SpeakerPWM__B_PWM__TWO_OUTPUTS 1
#define SpeakerPWM__B_PWM__DUAL_EDGE 2
#define SpeakerPWM__B_PWM__CENTER_ALIGN 3
#define SpeakerPWM__B_PWM__DITHER 5
#define SpeakerPWM__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define SpeakerPWM__B_PWM__LESS_THAN 1
#define SpeakerPWM__B_PWM__LESS_THAN_OR_EQUAL 2
#define SpeakerPWM__B_PWM__GREATER_THAN 3
#define SpeakerPWM__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define SpeakerPWM__B_PWM__EQUAL 0
#define SpeakerPWM__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!SpeakerPWM_UsingFixedFunction)
        uint8 PWMUdb;               /* PWM Current Counter value  */
        #if(!SpeakerPWM_PWMModeIsCenterAligned)
            uint8 PWMPeriod;
        #endif /* (!SpeakerPWM_PWMModeIsCenterAligned) */
        #if (SpeakerPWM_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (SpeakerPWM_UseStatus) */

        /* Backup for Deadband parameters */
        #if(SpeakerPWM_DeadBandMode == SpeakerPWM__B_PWM__DBM_256_CLOCKS || \
            SpeakerPWM_DeadBandMode == SpeakerPWM__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(SpeakerPWM_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (SpeakerPWM_KillModeMinTime) */

        /* Backup control register */
        #if(SpeakerPWM_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (SpeakerPWM_UseControl) */

    #endif /* (!SpeakerPWM_UsingFixedFunction) */

}SpeakerPWM_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    SpeakerPWM_Start(void) ;
void    SpeakerPWM_Stop(void) ;

#if (SpeakerPWM_UseStatus || SpeakerPWM_UsingFixedFunction)
    void  SpeakerPWM_SetInterruptMode(uint8 interruptMode) ;
    uint8 SpeakerPWM_ReadStatusRegister(void) ;
#endif /* (SpeakerPWM_UseStatus || SpeakerPWM_UsingFixedFunction) */

#define SpeakerPWM_GetInterruptSource() SpeakerPWM_ReadStatusRegister()

#if (SpeakerPWM_UseControl)
    uint8 SpeakerPWM_ReadControlRegister(void) ;
    void  SpeakerPWM_WriteControlRegister(uint8 control)
          ;
#endif /* (SpeakerPWM_UseControl) */

#if (SpeakerPWM_UseOneCompareMode)
   #if (SpeakerPWM_CompareMode1SW)
       void    SpeakerPWM_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (SpeakerPWM_CompareMode1SW) */
#else
    #if (SpeakerPWM_CompareMode1SW)
        void    SpeakerPWM_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (SpeakerPWM_CompareMode1SW) */
    #if (SpeakerPWM_CompareMode2SW)
        void    SpeakerPWM_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (SpeakerPWM_CompareMode2SW) */
#endif /* (SpeakerPWM_UseOneCompareMode) */

#if (!SpeakerPWM_UsingFixedFunction)
    uint8   SpeakerPWM_ReadCounter(void) ;
    uint8 SpeakerPWM_ReadCapture(void) ;

    #if (SpeakerPWM_UseStatus)
            void SpeakerPWM_ClearFIFO(void) ;
    #endif /* (SpeakerPWM_UseStatus) */

    void    SpeakerPWM_WriteCounter(uint8 counter)
            ;
#endif /* (!SpeakerPWM_UsingFixedFunction) */

void    SpeakerPWM_WritePeriod(uint8 period)
        ;
uint8 SpeakerPWM_ReadPeriod(void) ;

#if (SpeakerPWM_UseOneCompareMode)
    void    SpeakerPWM_WriteCompare(uint8 compare)
            ;
    uint8 SpeakerPWM_ReadCompare(void) ;
#else
    void    SpeakerPWM_WriteCompare1(uint8 compare)
            ;
    uint8 SpeakerPWM_ReadCompare1(void) ;
    void    SpeakerPWM_WriteCompare2(uint8 compare)
            ;
    uint8 SpeakerPWM_ReadCompare2(void) ;
#endif /* (SpeakerPWM_UseOneCompareMode) */


#if (SpeakerPWM_DeadBandUsed)
    void    SpeakerPWM_WriteDeadTime(uint8 deadtime) ;
    uint8   SpeakerPWM_ReadDeadTime(void) ;
#endif /* (SpeakerPWM_DeadBandUsed) */

#if ( SpeakerPWM_KillModeMinTime)
    void SpeakerPWM_WriteKillTime(uint8 killtime) ;
    uint8 SpeakerPWM_ReadKillTime(void) ;
#endif /* ( SpeakerPWM_KillModeMinTime) */

void SpeakerPWM_Init(void) ;
void SpeakerPWM_Enable(void) ;
void SpeakerPWM_Sleep(void) ;
void SpeakerPWM_Wakeup(void) ;
void SpeakerPWM_SaveConfig(void) ;
void SpeakerPWM_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define SpeakerPWM_INIT_PERIOD_VALUE          (255u)
#define SpeakerPWM_INIT_COMPARE_VALUE1        (128u)
#define SpeakerPWM_INIT_COMPARE_VALUE2        (64u)
#define SpeakerPWM_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    SpeakerPWM_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    SpeakerPWM_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    SpeakerPWM_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    SpeakerPWM_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define SpeakerPWM_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  SpeakerPWM_CTRL_CMPMODE2_SHIFT)
#define SpeakerPWM_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  SpeakerPWM_CTRL_CMPMODE1_SHIFT)
#define SpeakerPWM_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (SpeakerPWM_UsingFixedFunction)
   #define SpeakerPWM_PERIOD_LSB              (*(reg16 *) SpeakerPWM_PWMHW__PER0)
   #define SpeakerPWM_PERIOD_LSB_PTR          ( (reg16 *) SpeakerPWM_PWMHW__PER0)
   #define SpeakerPWM_COMPARE1_LSB            (*(reg16 *) SpeakerPWM_PWMHW__CNT_CMP0)
   #define SpeakerPWM_COMPARE1_LSB_PTR        ( (reg16 *) SpeakerPWM_PWMHW__CNT_CMP0)
   #define SpeakerPWM_COMPARE2_LSB            (0x00u)
   #define SpeakerPWM_COMPARE2_LSB_PTR        (0x00u)
   #define SpeakerPWM_COUNTER_LSB             (*(reg16 *) SpeakerPWM_PWMHW__CNT_CMP0)
   #define SpeakerPWM_COUNTER_LSB_PTR         ( (reg16 *) SpeakerPWM_PWMHW__CNT_CMP0)
   #define SpeakerPWM_CAPTURE_LSB             (*(reg16 *) SpeakerPWM_PWMHW__CAP0)
   #define SpeakerPWM_CAPTURE_LSB_PTR         ( (reg16 *) SpeakerPWM_PWMHW__CAP0)
   #define SpeakerPWM_RT1                     (*(reg8 *)  SpeakerPWM_PWMHW__RT1)
   #define SpeakerPWM_RT1_PTR                 ( (reg8 *)  SpeakerPWM_PWMHW__RT1)

#else
   #if (SpeakerPWM_Resolution == 8u) /* 8bit - PWM */

       #if(SpeakerPWM_PWMModeIsCenterAligned)
           #define SpeakerPWM_PERIOD_LSB      (*(reg8 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
           #define SpeakerPWM_PERIOD_LSB_PTR  ((reg8 *)   SpeakerPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #else
           #define SpeakerPWM_PERIOD_LSB      (*(reg8 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__F0_REG)
           #define SpeakerPWM_PERIOD_LSB_PTR  ((reg8 *)   SpeakerPWM_PWMUDB_sP8_pwmdp_u0__F0_REG)
       #endif /* (SpeakerPWM_PWMModeIsCenterAligned) */

       #define SpeakerPWM_COMPARE1_LSB        (*(reg8 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define SpeakerPWM_COMPARE1_LSB_PTR    ((reg8 *)   SpeakerPWM_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define SpeakerPWM_COMPARE2_LSB        (*(reg8 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define SpeakerPWM_COMPARE2_LSB_PTR    ((reg8 *)   SpeakerPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define SpeakerPWM_COUNTERCAP_LSB      (*(reg8 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define SpeakerPWM_COUNTERCAP_LSB_PTR  ((reg8 *)   SpeakerPWM_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define SpeakerPWM_COUNTER_LSB         (*(reg8 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define SpeakerPWM_COUNTER_LSB_PTR     ((reg8 *)   SpeakerPWM_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define SpeakerPWM_CAPTURE_LSB         (*(reg8 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__F1_REG)
       #define SpeakerPWM_CAPTURE_LSB_PTR     ((reg8 *)   SpeakerPWM_PWMUDB_sP8_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(SpeakerPWM_PWMModeIsCenterAligned)
               #define SpeakerPWM_PERIOD_LSB      (*(reg16 *) SpeakerPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
               #define SpeakerPWM_PERIOD_LSB_PTR  ((reg16 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #else
               #define SpeakerPWM_PERIOD_LSB      (*(reg16 *) SpeakerPWM_PWMUDB_sP8_pwmdp_u0__F0_REG)
               #define SpeakerPWM_PERIOD_LSB_PTR  ((reg16 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__F0_REG)
            #endif /* (SpeakerPWM_PWMModeIsCenterAligned) */

            #define SpeakerPWM_COMPARE1_LSB       (*(reg16 *) SpeakerPWM_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define SpeakerPWM_COMPARE1_LSB_PTR   ((reg16 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define SpeakerPWM_COMPARE2_LSB       (*(reg16 *) SpeakerPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define SpeakerPWM_COMPARE2_LSB_PTR   ((reg16 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define SpeakerPWM_COUNTERCAP_LSB     (*(reg16 *) SpeakerPWM_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define SpeakerPWM_COUNTERCAP_LSB_PTR ((reg16 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define SpeakerPWM_COUNTER_LSB        (*(reg16 *) SpeakerPWM_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define SpeakerPWM_COUNTER_LSB_PTR    ((reg16 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define SpeakerPWM_CAPTURE_LSB        (*(reg16 *) SpeakerPWM_PWMUDB_sP8_pwmdp_u0__F1_REG)
            #define SpeakerPWM_CAPTURE_LSB_PTR    ((reg16 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__F1_REG)
        #else
            #if(SpeakerPWM_PWMModeIsCenterAligned)
               #define SpeakerPWM_PERIOD_LSB      (*(reg16 *) SpeakerPWM_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
               #define SpeakerPWM_PERIOD_LSB_PTR  ((reg16 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #else
               #define SpeakerPWM_PERIOD_LSB      (*(reg16 *) SpeakerPWM_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
               #define SpeakerPWM_PERIOD_LSB_PTR  ((reg16 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
            #endif /* (SpeakerPWM_PWMModeIsCenterAligned) */

            #define SpeakerPWM_COMPARE1_LSB       (*(reg16 *) SpeakerPWM_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define SpeakerPWM_COMPARE1_LSB_PTR   ((reg16 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define SpeakerPWM_COMPARE2_LSB       (*(reg16 *) SpeakerPWM_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define SpeakerPWM_COMPARE2_LSB_PTR   ((reg16 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define SpeakerPWM_COUNTERCAP_LSB     (*(reg16 *) SpeakerPWM_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define SpeakerPWM_COUNTERCAP_LSB_PTR ((reg16 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define SpeakerPWM_COUNTER_LSB        (*(reg16 *) SpeakerPWM_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define SpeakerPWM_COUNTER_LSB_PTR    ((reg16 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define SpeakerPWM_CAPTURE_LSB        (*(reg16 *) SpeakerPWM_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
            #define SpeakerPWM_CAPTURE_LSB_PTR    ((reg16 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define SpeakerPWM_AUX_CONTROLDP1          (*(reg8 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)
       #define SpeakerPWM_AUX_CONTROLDP1_PTR      ((reg8 *)   SpeakerPWM_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (SpeakerPWM_Resolution == 8) */

   #define SpeakerPWM_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__A1_REG)
   #define SpeakerPWM_AUX_CONTROLDP0          (*(reg8 *)  SpeakerPWM_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)
   #define SpeakerPWM_AUX_CONTROLDP0_PTR      ((reg8 *)   SpeakerPWM_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (SpeakerPWM_UsingFixedFunction) */

#if(SpeakerPWM_KillModeMinTime )
    #define SpeakerPWM_KILLMODEMINTIME        (*(reg8 *)  SpeakerPWM_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define SpeakerPWM_KILLMODEMINTIME_PTR    ((reg8 *)   SpeakerPWM_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (SpeakerPWM_KillModeMinTime ) */

#if(SpeakerPWM_DeadBandMode == SpeakerPWM__B_PWM__DBM_256_CLOCKS)
    #define SpeakerPWM_DEADBAND_COUNT         (*(reg8 *)  SpeakerPWM_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define SpeakerPWM_DEADBAND_COUNT_PTR     ((reg8 *)   SpeakerPWM_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define SpeakerPWM_DEADBAND_LSB_PTR       ((reg8 *)   SpeakerPWM_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define SpeakerPWM_DEADBAND_LSB           (*(reg8 *)  SpeakerPWM_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(SpeakerPWM_DeadBandMode == SpeakerPWM__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (SpeakerPWM_UsingFixedFunction)
        #define SpeakerPWM_DEADBAND_COUNT         (*(reg8 *)  SpeakerPWM_PWMHW__CFG0)
        #define SpeakerPWM_DEADBAND_COUNT_PTR     ((reg8 *)   SpeakerPWM_PWMHW__CFG0)
        #define SpeakerPWM_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << SpeakerPWM_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define SpeakerPWM_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define SpeakerPWM_DEADBAND_COUNT         (*(reg8 *)  SpeakerPWM_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define SpeakerPWM_DEADBAND_COUNT_PTR     ((reg8 *)   SpeakerPWM_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define SpeakerPWM_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << SpeakerPWM_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define SpeakerPWM_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (SpeakerPWM_UsingFixedFunction) */
#endif /* (SpeakerPWM_DeadBandMode == SpeakerPWM__B_PWM__DBM_256_CLOCKS) */



#if (SpeakerPWM_UsingFixedFunction)
    #define SpeakerPWM_STATUS                 (*(reg8 *) SpeakerPWM_PWMHW__SR0)
    #define SpeakerPWM_STATUS_PTR             ((reg8 *) SpeakerPWM_PWMHW__SR0)
    #define SpeakerPWM_STATUS_MASK            (*(reg8 *) SpeakerPWM_PWMHW__SR0)
    #define SpeakerPWM_STATUS_MASK_PTR        ((reg8 *) SpeakerPWM_PWMHW__SR0)
    #define SpeakerPWM_CONTROL                (*(reg8 *) SpeakerPWM_PWMHW__CFG0)
    #define SpeakerPWM_CONTROL_PTR            ((reg8 *) SpeakerPWM_PWMHW__CFG0)
    #define SpeakerPWM_CONTROL2               (*(reg8 *) SpeakerPWM_PWMHW__CFG1)
    #define SpeakerPWM_CONTROL3               (*(reg8 *) SpeakerPWM_PWMHW__CFG2)
    #define SpeakerPWM_GLOBAL_ENABLE          (*(reg8 *) SpeakerPWM_PWMHW__PM_ACT_CFG)
    #define SpeakerPWM_GLOBAL_ENABLE_PTR      ( (reg8 *) SpeakerPWM_PWMHW__PM_ACT_CFG)
    #define SpeakerPWM_GLOBAL_STBY_ENABLE     (*(reg8 *) SpeakerPWM_PWMHW__PM_STBY_CFG)
    #define SpeakerPWM_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) SpeakerPWM_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define SpeakerPWM_BLOCK_EN_MASK          (SpeakerPWM_PWMHW__PM_ACT_MSK)
    #define SpeakerPWM_BLOCK_STBY_EN_MASK     (SpeakerPWM_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define SpeakerPWM_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define SpeakerPWM_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define SpeakerPWM_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define SpeakerPWM_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define SpeakerPWM_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define SpeakerPWM_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define SpeakerPWM_CTRL_ENABLE            (uint8)((uint8)0x01u << SpeakerPWM_CTRL_ENABLE_SHIFT)
    #define SpeakerPWM_CTRL_RESET             (uint8)((uint8)0x01u << SpeakerPWM_CTRL_RESET_SHIFT)
    #define SpeakerPWM_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << SpeakerPWM_CTRL_CMPMODE2_SHIFT)
    #define SpeakerPWM_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << SpeakerPWM_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define SpeakerPWM_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define SpeakerPWM_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << SpeakerPWM_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define SpeakerPWM_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define SpeakerPWM_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define SpeakerPWM_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define SpeakerPWM_STATUS_TC_INT_EN_MASK_SHIFT            (SpeakerPWM_STATUS_TC_SHIFT - 4u)
    #define SpeakerPWM_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define SpeakerPWM_STATUS_CMP1_INT_EN_MASK_SHIFT          (SpeakerPWM_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define SpeakerPWM_STATUS_TC              (uint8)((uint8)0x01u << SpeakerPWM_STATUS_TC_SHIFT)
    #define SpeakerPWM_STATUS_CMP1            (uint8)((uint8)0x01u << SpeakerPWM_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define SpeakerPWM_STATUS_TC_INT_EN_MASK              (uint8)((uint8)SpeakerPWM_STATUS_TC >> 4u)
    #define SpeakerPWM_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)SpeakerPWM_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define SpeakerPWM_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define SpeakerPWM_RT1_MASK              (uint8)((uint8)0x03u << SpeakerPWM_RT1_SHIFT)
    #define SpeakerPWM_SYNC                  (uint8)((uint8)0x03u << SpeakerPWM_RT1_SHIFT)
    #define SpeakerPWM_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define SpeakerPWM_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << SpeakerPWM_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define SpeakerPWM_SYNCDSI_EN            (uint8)((uint8)0x0Fu << SpeakerPWM_SYNCDSI_SHIFT)


#else
    #define SpeakerPWM_STATUS                (*(reg8 *)   SpeakerPWM_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define SpeakerPWM_STATUS_PTR            ((reg8 *)    SpeakerPWM_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define SpeakerPWM_STATUS_MASK           (*(reg8 *)   SpeakerPWM_PWMUDB_genblk8_stsreg__MASK_REG)
    #define SpeakerPWM_STATUS_MASK_PTR       ((reg8 *)    SpeakerPWM_PWMUDB_genblk8_stsreg__MASK_REG)
    #define SpeakerPWM_STATUS_AUX_CTRL       (*(reg8 *)   SpeakerPWM_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define SpeakerPWM_CONTROL               (*(reg8 *)   SpeakerPWM_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define SpeakerPWM_CONTROL_PTR           ((reg8 *)    SpeakerPWM_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define SpeakerPWM_CTRL_ENABLE_SHIFT      (0x07u)
    #define SpeakerPWM_CTRL_RESET_SHIFT       (0x06u)
    #define SpeakerPWM_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define SpeakerPWM_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define SpeakerPWM_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define SpeakerPWM_CTRL_ENABLE            (uint8)((uint8)0x01u << SpeakerPWM_CTRL_ENABLE_SHIFT)
    #define SpeakerPWM_CTRL_RESET             (uint8)((uint8)0x01u << SpeakerPWM_CTRL_RESET_SHIFT)
    #define SpeakerPWM_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << SpeakerPWM_CTRL_CMPMODE2_SHIFT)
    #define SpeakerPWM_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << SpeakerPWM_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define SpeakerPWM_STATUS_KILL_SHIFT          (0x05u)
    #define SpeakerPWM_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define SpeakerPWM_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define SpeakerPWM_STATUS_TC_SHIFT            (0x02u)
    #define SpeakerPWM_STATUS_CMP2_SHIFT          (0x01u)
    #define SpeakerPWM_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define SpeakerPWM_STATUS_KILL_INT_EN_MASK_SHIFT          (SpeakerPWM_STATUS_KILL_SHIFT)
    #define SpeakerPWM_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (SpeakerPWM_STATUS_FIFONEMPTY_SHIFT)
    #define SpeakerPWM_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (SpeakerPWM_STATUS_FIFOFULL_SHIFT)
    #define SpeakerPWM_STATUS_TC_INT_EN_MASK_SHIFT            (SpeakerPWM_STATUS_TC_SHIFT)
    #define SpeakerPWM_STATUS_CMP2_INT_EN_MASK_SHIFT          (SpeakerPWM_STATUS_CMP2_SHIFT)
    #define SpeakerPWM_STATUS_CMP1_INT_EN_MASK_SHIFT          (SpeakerPWM_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define SpeakerPWM_STATUS_KILL            (uint8)((uint8)0x00u << SpeakerPWM_STATUS_KILL_SHIFT )
    #define SpeakerPWM_STATUS_FIFOFULL        (uint8)((uint8)0x01u << SpeakerPWM_STATUS_FIFOFULL_SHIFT)
    #define SpeakerPWM_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << SpeakerPWM_STATUS_FIFONEMPTY_SHIFT)
    #define SpeakerPWM_STATUS_TC              (uint8)((uint8)0x01u << SpeakerPWM_STATUS_TC_SHIFT)
    #define SpeakerPWM_STATUS_CMP2            (uint8)((uint8)0x01u << SpeakerPWM_STATUS_CMP2_SHIFT)
    #define SpeakerPWM_STATUS_CMP1            (uint8)((uint8)0x01u << SpeakerPWM_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define SpeakerPWM_STATUS_KILL_INT_EN_MASK            (SpeakerPWM_STATUS_KILL)
    #define SpeakerPWM_STATUS_FIFOFULL_INT_EN_MASK        (SpeakerPWM_STATUS_FIFOFULL)
    #define SpeakerPWM_STATUS_FIFONEMPTY_INT_EN_MASK      (SpeakerPWM_STATUS_FIFONEMPTY)
    #define SpeakerPWM_STATUS_TC_INT_EN_MASK              (SpeakerPWM_STATUS_TC)
    #define SpeakerPWM_STATUS_CMP2_INT_EN_MASK            (SpeakerPWM_STATUS_CMP2)
    #define SpeakerPWM_STATUS_CMP1_INT_EN_MASK            (SpeakerPWM_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define SpeakerPWM_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define SpeakerPWM_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define SpeakerPWM_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define SpeakerPWM_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define SpeakerPWM_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* SpeakerPWM_UsingFixedFunction */

#endif  /* CY_PWM_SpeakerPWM_H */


/* [] END OF FILE */
