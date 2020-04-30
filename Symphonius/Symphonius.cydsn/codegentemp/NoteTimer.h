/*******************************************************************************
* File Name: NoteTimer.h
* Version 2.80
*
*  Description:
*     Contains the function prototypes and constants available to the timer
*     user module.
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_TIMER_NoteTimer_H)
#define CY_TIMER_NoteTimer_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 NoteTimer_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_80 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define NoteTimer_Resolution                 16u
#define NoteTimer_UsingFixedFunction         1u
#define NoteTimer_UsingHWCaptureCounter      0u
#define NoteTimer_SoftwareCaptureMode        0u
#define NoteTimer_SoftwareTriggerMode        0u
#define NoteTimer_UsingHWEnable              0u
#define NoteTimer_EnableTriggerMode          0u
#define NoteTimer_InterruptOnCaptureCount    0u
#define NoteTimer_RunModeUsed                0u
#define NoteTimer_ControlRegRemoved          0u

#if defined(NoteTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    #define NoteTimer_UDB_CONTROL_REG_REMOVED            (0u)
#elif  (NoteTimer_UsingFixedFunction)
    #define NoteTimer_UDB_CONTROL_REG_REMOVED            (0u)
#else 
    #define NoteTimer_UDB_CONTROL_REG_REMOVED            (1u)
#endif /* End NoteTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG */


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!NoteTimer_UsingFixedFunction)

        uint16 TimerUdb;
        uint8 InterruptMaskValue;
        #if (NoteTimer_UsingHWCaptureCounter)
            uint8 TimerCaptureCounter;
        #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */

        #if (!NoteTimer_UDB_CONTROL_REG_REMOVED)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */

}NoteTimer_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    NoteTimer_Start(void) ;
void    NoteTimer_Stop(void) ;

void    NoteTimer_SetInterruptMode(uint8 interruptMode) ;
uint8   NoteTimer_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define NoteTimer_GetInterruptSource() NoteTimer_ReadStatusRegister()

#if(!NoteTimer_UDB_CONTROL_REG_REMOVED)
    uint8   NoteTimer_ReadControlRegister(void) ;
    void    NoteTimer_WriteControlRegister(uint8 control) ;
#endif /* (!NoteTimer_UDB_CONTROL_REG_REMOVED) */

uint16  NoteTimer_ReadPeriod(void) ;
void    NoteTimer_WritePeriod(uint16 period) ;
uint16  NoteTimer_ReadCounter(void) ;
void    NoteTimer_WriteCounter(uint16 counter) ;
uint16  NoteTimer_ReadCapture(void) ;
void    NoteTimer_SoftwareCapture(void) ;

#if(!NoteTimer_UsingFixedFunction) /* UDB Prototypes */
    #if (NoteTimer_SoftwareCaptureMode)
        void    NoteTimer_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!NoteTimer_UsingFixedFunction) */

    #if (NoteTimer_SoftwareTriggerMode)
        void    NoteTimer_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (NoteTimer_SoftwareTriggerMode) */

    #if (NoteTimer_EnableTriggerMode)
        void    NoteTimer_EnableTrigger(void) ;
        void    NoteTimer_DisableTrigger(void) ;
    #endif /* (NoteTimer_EnableTriggerMode) */


    #if(NoteTimer_InterruptOnCaptureCount)
        void    NoteTimer_SetInterruptCount(uint8 interruptCount) ;
    #endif /* (NoteTimer_InterruptOnCaptureCount) */

    #if (NoteTimer_UsingHWCaptureCounter)
        void    NoteTimer_SetCaptureCount(uint8 captureCount) ;
        uint8   NoteTimer_ReadCaptureCount(void) ;
    #endif /* (NoteTimer_UsingHWCaptureCounter) */

    void NoteTimer_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void NoteTimer_Init(void)          ;
void NoteTimer_Enable(void)        ;
void NoteTimer_SaveConfig(void)    ;
void NoteTimer_RestoreConfig(void) ;
void NoteTimer_Sleep(void)         ;
void NoteTimer_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define NoteTimer__B_TIMER__CM_NONE 0
#define NoteTimer__B_TIMER__CM_RISINGEDGE 1
#define NoteTimer__B_TIMER__CM_FALLINGEDGE 2
#define NoteTimer__B_TIMER__CM_EITHEREDGE 3
#define NoteTimer__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define NoteTimer__B_TIMER__TM_NONE 0x00u
#define NoteTimer__B_TIMER__TM_RISINGEDGE 0x04u
#define NoteTimer__B_TIMER__TM_FALLINGEDGE 0x08u
#define NoteTimer__B_TIMER__TM_EITHEREDGE 0x0Cu
#define NoteTimer__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define NoteTimer_INIT_PERIOD             9u
#define NoteTimer_INIT_CAPTURE_MODE       ((uint8)((uint8)1u << NoteTimer_CTRL_CAP_MODE_SHIFT))
#define NoteTimer_INIT_TRIGGER_MODE       ((uint8)((uint8)0u << NoteTimer_CTRL_TRIG_MODE_SHIFT))
#if (NoteTimer_UsingFixedFunction)
    #define NoteTimer_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << NoteTimer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << NoteTimer_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define NoteTimer_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << NoteTimer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << NoteTimer_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << NoteTimer_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (NoteTimer_UsingFixedFunction) */
#define NoteTimer_INIT_CAPTURE_COUNT      (2u)
#define NoteTimer_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << NoteTimer_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (NoteTimer_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define NoteTimer_STATUS         (*(reg8 *) NoteTimer_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define NoteTimer_STATUS_MASK    (*(reg8 *) NoteTimer_TimerHW__SR0 )
    #define NoteTimer_CONTROL        (*(reg8 *) NoteTimer_TimerHW__CFG0)
    #define NoteTimer_CONTROL2       (*(reg8 *) NoteTimer_TimerHW__CFG1)
    #define NoteTimer_CONTROL2_PTR   ( (reg8 *) NoteTimer_TimerHW__CFG1)
    #define NoteTimer_RT1            (*(reg8 *) NoteTimer_TimerHW__RT1)
    #define NoteTimer_RT1_PTR        ( (reg8 *) NoteTimer_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define NoteTimer_CONTROL3       (*(reg8 *) NoteTimer_TimerHW__CFG2)
        #define NoteTimer_CONTROL3_PTR   ( (reg8 *) NoteTimer_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define NoteTimer_GLOBAL_ENABLE  (*(reg8 *) NoteTimer_TimerHW__PM_ACT_CFG)
    #define NoteTimer_GLOBAL_STBY_ENABLE  (*(reg8 *) NoteTimer_TimerHW__PM_STBY_CFG)

    #define NoteTimer_CAPTURE_LSB         (* (reg16 *) NoteTimer_TimerHW__CAP0 )
    #define NoteTimer_CAPTURE_LSB_PTR       ((reg16 *) NoteTimer_TimerHW__CAP0 )
    #define NoteTimer_PERIOD_LSB          (* (reg16 *) NoteTimer_TimerHW__PER0 )
    #define NoteTimer_PERIOD_LSB_PTR        ((reg16 *) NoteTimer_TimerHW__PER0 )
    #define NoteTimer_COUNTER_LSB         (* (reg16 *) NoteTimer_TimerHW__CNT_CMP0 )
    #define NoteTimer_COUNTER_LSB_PTR       ((reg16 *) NoteTimer_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define NoteTimer_BLOCK_EN_MASK                     NoteTimer_TimerHW__PM_ACT_MSK
    #define NoteTimer_BLOCK_STBY_EN_MASK                NoteTimer_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define NoteTimer_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define NoteTimer_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define NoteTimer_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define NoteTimer_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define NoteTimer_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define NoteTimer_CTRL_ENABLE                        ((uint8)((uint8)0x01u << NoteTimer_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define NoteTimer_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define NoteTimer_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << NoteTimer_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define NoteTimer_CTRL_MODE_SHIFT                 0x01u
        #define NoteTimer_CTRL_MODE_MASK                 ((uint8)((uint8)0x07u << NoteTimer_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define NoteTimer_CTRL_RCOD_SHIFT        0x02u
        #define NoteTimer_CTRL_ENBL_SHIFT        0x00u
        #define NoteTimer_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define NoteTimer_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << NoteTimer_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define NoteTimer_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << NoteTimer_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define NoteTimer_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << NoteTimer_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define NoteTimer_CTRL_RCOD       ((uint8)((uint8)0x03u << NoteTimer_CTRL_RCOD_SHIFT))
        #define NoteTimer_CTRL_ENBL       ((uint8)((uint8)0x80u << NoteTimer_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define NoteTimer_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define NoteTimer_RT1_MASK                        ((uint8)((uint8)0x03u << NoteTimer_RT1_SHIFT))
    #define NoteTimer_SYNC                            ((uint8)((uint8)0x03u << NoteTimer_RT1_SHIFT))
    #define NoteTimer_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define NoteTimer_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << NoteTimer_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define NoteTimer_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << NoteTimer_SYNCDSI_SHIFT))

    #define NoteTimer_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << NoteTimer_CTRL_MODE_SHIFT))
    #define NoteTimer_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << NoteTimer_CTRL_MODE_SHIFT))
    #define NoteTimer_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << NoteTimer_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define NoteTimer_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define NoteTimer_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define NoteTimer_STATUS_TC_INT_MASK_SHIFT        (NoteTimer_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define NoteTimer_STATUS_CAPTURE_INT_MASK_SHIFT   (NoteTimer_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define NoteTimer_STATUS_TC                       ((uint8)((uint8)0x01u << NoteTimer_STATUS_TC_SHIFT))
    #define NoteTimer_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << NoteTimer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define NoteTimer_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << NoteTimer_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define NoteTimer_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << NoteTimer_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define NoteTimer_STATUS              (* (reg8 *) NoteTimer_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define NoteTimer_STATUS_MASK         (* (reg8 *) NoteTimer_TimerUDB_rstSts_stsreg__MASK_REG)
    #define NoteTimer_STATUS_AUX_CTRL     (* (reg8 *) NoteTimer_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define NoteTimer_CONTROL             (* (reg8 *) NoteTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(NoteTimer_Resolution <= 8u) /* 8-bit Timer */
        #define NoteTimer_CAPTURE_LSB         (* (reg8 *) NoteTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define NoteTimer_CAPTURE_LSB_PTR       ((reg8 *) NoteTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define NoteTimer_PERIOD_LSB          (* (reg8 *) NoteTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define NoteTimer_PERIOD_LSB_PTR        ((reg8 *) NoteTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define NoteTimer_COUNTER_LSB         (* (reg8 *) NoteTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
        #define NoteTimer_COUNTER_LSB_PTR       ((reg8 *) NoteTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
    #elif(NoteTimer_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define NoteTimer_CAPTURE_LSB         (* (reg16 *) NoteTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define NoteTimer_CAPTURE_LSB_PTR       ((reg16 *) NoteTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define NoteTimer_PERIOD_LSB          (* (reg16 *) NoteTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define NoteTimer_PERIOD_LSB_PTR        ((reg16 *) NoteTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define NoteTimer_COUNTER_LSB         (* (reg16 *) NoteTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
            #define NoteTimer_COUNTER_LSB_PTR       ((reg16 *) NoteTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define NoteTimer_CAPTURE_LSB         (* (reg16 *) NoteTimer_TimerUDB_sT16_timerdp_u0__16BIT_F0_REG )
            #define NoteTimer_CAPTURE_LSB_PTR       ((reg16 *) NoteTimer_TimerUDB_sT16_timerdp_u0__16BIT_F0_REG )
            #define NoteTimer_PERIOD_LSB          (* (reg16 *) NoteTimer_TimerUDB_sT16_timerdp_u0__16BIT_D0_REG )
            #define NoteTimer_PERIOD_LSB_PTR        ((reg16 *) NoteTimer_TimerUDB_sT16_timerdp_u0__16BIT_D0_REG )
            #define NoteTimer_COUNTER_LSB         (* (reg16 *) NoteTimer_TimerUDB_sT16_timerdp_u0__16BIT_A0_REG )
            #define NoteTimer_COUNTER_LSB_PTR       ((reg16 *) NoteTimer_TimerUDB_sT16_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(NoteTimer_Resolution <= 24u)/* 24-bit Timer */
        #define NoteTimer_CAPTURE_LSB         (* (reg32 *) NoteTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define NoteTimer_CAPTURE_LSB_PTR       ((reg32 *) NoteTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define NoteTimer_PERIOD_LSB          (* (reg32 *) NoteTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define NoteTimer_PERIOD_LSB_PTR        ((reg32 *) NoteTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define NoteTimer_COUNTER_LSB         (* (reg32 *) NoteTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
        #define NoteTimer_COUNTER_LSB_PTR       ((reg32 *) NoteTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define NoteTimer_CAPTURE_LSB         (* (reg32 *) NoteTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define NoteTimer_CAPTURE_LSB_PTR       ((reg32 *) NoteTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define NoteTimer_PERIOD_LSB          (* (reg32 *) NoteTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define NoteTimer_PERIOD_LSB_PTR        ((reg32 *) NoteTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define NoteTimer_COUNTER_LSB         (* (reg32 *) NoteTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
            #define NoteTimer_COUNTER_LSB_PTR       ((reg32 *) NoteTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define NoteTimer_CAPTURE_LSB         (* (reg32 *) NoteTimer_TimerUDB_sT16_timerdp_u0__32BIT_F0_REG )
            #define NoteTimer_CAPTURE_LSB_PTR       ((reg32 *) NoteTimer_TimerUDB_sT16_timerdp_u0__32BIT_F0_REG )
            #define NoteTimer_PERIOD_LSB          (* (reg32 *) NoteTimer_TimerUDB_sT16_timerdp_u0__32BIT_D0_REG )
            #define NoteTimer_PERIOD_LSB_PTR        ((reg32 *) NoteTimer_TimerUDB_sT16_timerdp_u0__32BIT_D0_REG )
            #define NoteTimer_COUNTER_LSB         (* (reg32 *) NoteTimer_TimerUDB_sT16_timerdp_u0__32BIT_A0_REG )
            #define NoteTimer_COUNTER_LSB_PTR       ((reg32 *) NoteTimer_TimerUDB_sT16_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #define NoteTimer_COUNTER_LSB_PTR_8BIT       ((reg8 *) NoteTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
    
    #if (NoteTimer_UsingHWCaptureCounter)
        #define NoteTimer_CAP_COUNT              (*(reg8 *) NoteTimer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define NoteTimer_CAP_COUNT_PTR          ( (reg8 *) NoteTimer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define NoteTimer_CAPTURE_COUNT_CTRL     (*(reg8 *) NoteTimer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define NoteTimer_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) NoteTimer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (NoteTimer_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define NoteTimer_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define NoteTimer_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define NoteTimer_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define NoteTimer_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define NoteTimer_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define NoteTimer_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << NoteTimer_CTRL_INTCNT_SHIFT))
    #define NoteTimer_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << NoteTimer_CTRL_TRIG_MODE_SHIFT))
    #define NoteTimer_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << NoteTimer_CTRL_TRIG_EN_SHIFT))
    #define NoteTimer_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << NoteTimer_CTRL_CAP_MODE_SHIFT))
    #define NoteTimer_CTRL_ENABLE                    ((uint8)((uint8)0x01u << NoteTimer_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define NoteTimer_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define NoteTimer_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define NoteTimer_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define NoteTimer_STATUS_TC_INT_MASK_SHIFT       NoteTimer_STATUS_TC_SHIFT
    #define NoteTimer_STATUS_CAPTURE_INT_MASK_SHIFT  NoteTimer_STATUS_CAPTURE_SHIFT
    #define NoteTimer_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define NoteTimer_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define NoteTimer_STATUS_FIFOFULL_INT_MASK_SHIFT NoteTimer_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define NoteTimer_STATUS_TC                      ((uint8)((uint8)0x01u << NoteTimer_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define NoteTimer_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << NoteTimer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define NoteTimer_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << NoteTimer_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define NoteTimer_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << NoteTimer_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define NoteTimer_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << NoteTimer_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define NoteTimer_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << NoteTimer_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define NoteTimer_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << NoteTimer_STATUS_FIFOFULL_SHIFT))

    #define NoteTimer_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define NoteTimer_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define NoteTimer_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define NoteTimer_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define NoteTimer_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define NoteTimer_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_TIMER_NoteTimer_H */


/* [] END OF FILE */
