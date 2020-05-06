/*******************************************************************************
* File Name: SpeakerCounter.h  
* Version 3.0
*
*  Description:
*   Contains the function prototypes and constants available to the counter
*   user module.
*
*   Note:
*    None
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
    
#if !defined(CY_COUNTER_SpeakerCounter_H)
#define CY_COUNTER_SpeakerCounter_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 SpeakerCounter_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Counter_v3_0 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Error message for removed SpeakerCounter_CounterUDB_sCTRLReg_ctrlreg through optimization */
#ifdef SpeakerCounter_CounterUDB_sCTRLReg_ctrlreg__REMOVED
    #error Counter_v3_0 detected with a constant 0 for the enable, a \
                                constant 0 for the count or constant 1 for \
                                the reset. This will prevent the component from\
                                operating.
#endif /* SpeakerCounter_CounterUDB_sCTRLReg_ctrlreg__REMOVED */


/**************************************
*           Parameter Defaults        
**************************************/

#define SpeakerCounter_Resolution            16u
#define SpeakerCounter_UsingFixedFunction    1u
#define SpeakerCounter_ControlRegRemoved     0u
#define SpeakerCounter_COMPARE_MODE_SOFTWARE 0u
#define SpeakerCounter_CAPTURE_MODE_SOFTWARE 0u
#define SpeakerCounter_RunModeUsed           0u


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Mode API Support
 * Backup structure for Sleep Wake up operations
 *************************************************************************/
typedef struct
{
    uint8 CounterEnableState; 
    uint16 CounterUdb;         /* Current Counter Value */

    #if (!SpeakerCounter_ControlRegRemoved)
        uint8 CounterControlRegister;               /* Counter Control Register */
    #endif /* (!SpeakerCounter_ControlRegRemoved) */

}SpeakerCounter_backupStruct;


/**************************************
 *  Function Prototypes
 *************************************/
void    SpeakerCounter_Start(void) ;
void    SpeakerCounter_Stop(void) ;
void    SpeakerCounter_SetInterruptMode(uint8 interruptsMask) ;
uint8   SpeakerCounter_ReadStatusRegister(void) ;
#define SpeakerCounter_GetInterruptSource() SpeakerCounter_ReadStatusRegister()
#if(!SpeakerCounter_ControlRegRemoved)
    uint8   SpeakerCounter_ReadControlRegister(void) ;
    void    SpeakerCounter_WriteControlRegister(uint8 control) \
        ;
#endif /* (!SpeakerCounter_ControlRegRemoved) */
#if (!(SpeakerCounter_UsingFixedFunction && (CY_PSOC5A)))
    void    SpeakerCounter_WriteCounter(uint16 counter) \
            ; 
#endif /* (!(SpeakerCounter_UsingFixedFunction && (CY_PSOC5A))) */
uint16  SpeakerCounter_ReadCounter(void) ;
uint16  SpeakerCounter_ReadCapture(void) ;
void    SpeakerCounter_WritePeriod(uint16 period) \
    ;
uint16  SpeakerCounter_ReadPeriod( void ) ;
#if (!SpeakerCounter_UsingFixedFunction)
    void    SpeakerCounter_WriteCompare(uint16 compare) \
        ;
    uint16  SpeakerCounter_ReadCompare( void ) \
        ;
#endif /* (!SpeakerCounter_UsingFixedFunction) */

#if (SpeakerCounter_COMPARE_MODE_SOFTWARE)
    void    SpeakerCounter_SetCompareMode(uint8 compareMode) ;
#endif /* (SpeakerCounter_COMPARE_MODE_SOFTWARE) */
#if (SpeakerCounter_CAPTURE_MODE_SOFTWARE)
    void    SpeakerCounter_SetCaptureMode(uint8 captureMode) ;
#endif /* (SpeakerCounter_CAPTURE_MODE_SOFTWARE) */
void SpeakerCounter_ClearFIFO(void)     ;
void SpeakerCounter_Init(void)          ;
void SpeakerCounter_Enable(void)        ;
void SpeakerCounter_SaveConfig(void)    ;
void SpeakerCounter_RestoreConfig(void) ;
void SpeakerCounter_Sleep(void)         ;
void SpeakerCounter_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Counter__CompareModes, Used in Compare Mode retained for backward compatibility of tests*/
#define SpeakerCounter__B_COUNTER__LESS_THAN 1
#define SpeakerCounter__B_COUNTER__LESS_THAN_OR_EQUAL 2
#define SpeakerCounter__B_COUNTER__EQUAL 0
#define SpeakerCounter__B_COUNTER__GREATER_THAN 3
#define SpeakerCounter__B_COUNTER__GREATER_THAN_OR_EQUAL 4
#define SpeakerCounter__B_COUNTER__SOFTWARE 5

/* Enumerated Type Counter_CompareModes */
#define SpeakerCounter_CMP_MODE_LT 1u
#define SpeakerCounter_CMP_MODE_LTE 2u
#define SpeakerCounter_CMP_MODE_EQ 0u
#define SpeakerCounter_CMP_MODE_GT 3u
#define SpeakerCounter_CMP_MODE_GTE 4u
#define SpeakerCounter_CMP_MODE_SOFTWARE_CONTROLLED 5u

/* Enumerated Type B_Counter__CaptureModes, Used in Capture Mode retained for backward compatibility of tests*/
#define SpeakerCounter__B_COUNTER__NONE 0
#define SpeakerCounter__B_COUNTER__RISING_EDGE 1
#define SpeakerCounter__B_COUNTER__FALLING_EDGE 2
#define SpeakerCounter__B_COUNTER__EITHER_EDGE 3
#define SpeakerCounter__B_COUNTER__SOFTWARE_CONTROL 4

/* Enumerated Type Counter_CompareModes */
#define SpeakerCounter_CAP_MODE_NONE 0u
#define SpeakerCounter_CAP_MODE_RISE 1u
#define SpeakerCounter_CAP_MODE_FALL 2u
#define SpeakerCounter_CAP_MODE_BOTH 3u
#define SpeakerCounter_CAP_MODE_SOFTWARE_CONTROLLED 4u


/***************************************
 *  Initialization Values
 **************************************/
#define SpeakerCounter_CAPTURE_MODE_CONF       0u
#define SpeakerCounter_INIT_PERIOD_VALUE       1u
#define SpeakerCounter_INIT_COUNTER_VALUE      0u
#if (SpeakerCounter_UsingFixedFunction)
#define SpeakerCounter_INIT_INTERRUPTS_MASK    ((uint8)((uint8)0u << SpeakerCounter_STATUS_ZERO_INT_EN_MASK_SHIFT))
#else
#define SpeakerCounter_INIT_COMPARE_VALUE      1u
#define SpeakerCounter_INIT_INTERRUPTS_MASK ((uint8)((uint8)0u << SpeakerCounter_STATUS_ZERO_INT_EN_MASK_SHIFT) | \
        ((uint8)((uint8)0u << SpeakerCounter_STATUS_CAPTURE_INT_EN_MASK_SHIFT)) | \
        ((uint8)((uint8)0u << SpeakerCounter_STATUS_CMP_INT_EN_MASK_SHIFT)) | \
        ((uint8)((uint8)0u << SpeakerCounter_STATUS_OVERFLOW_INT_EN_MASK_SHIFT)) | \
        ((uint8)((uint8)0u << SpeakerCounter_STATUS_UNDERFLOW_INT_EN_MASK_SHIFT)))
#define SpeakerCounter_DEFAULT_COMPARE_MODE    1u

#if( 0 != SpeakerCounter_CAPTURE_MODE_CONF)
    #define SpeakerCounter_DEFAULT_CAPTURE_MODE    ((uint8)((uint8)0u << SpeakerCounter_CTRL_CAPMODE0_SHIFT))
#else    
    #define SpeakerCounter_DEFAULT_CAPTURE_MODE    (0u )
#endif /* ( 0 != SpeakerCounter_CAPTURE_MODE_CONF) */

#endif /* (SpeakerCounter_UsingFixedFunction) */


/**************************************
 *  Registers
 *************************************/
#if (SpeakerCounter_UsingFixedFunction)
    #define SpeakerCounter_STATICCOUNT_LSB     (*(reg16 *) SpeakerCounter_CounterHW__CAP0 )
    #define SpeakerCounter_STATICCOUNT_LSB_PTR ( (reg16 *) SpeakerCounter_CounterHW__CAP0 )
    #define SpeakerCounter_PERIOD_LSB          (*(reg16 *) SpeakerCounter_CounterHW__PER0 )
    #define SpeakerCounter_PERIOD_LSB_PTR      ( (reg16 *) SpeakerCounter_CounterHW__PER0 )
    /* MODE must be set to 1 to set the compare value */
    #define SpeakerCounter_COMPARE_LSB         (*(reg16 *) SpeakerCounter_CounterHW__CNT_CMP0 )
    #define SpeakerCounter_COMPARE_LSB_PTR     ( (reg16 *) SpeakerCounter_CounterHW__CNT_CMP0 )
    /* MODE must be set to 0 to get the count */
    #define SpeakerCounter_COUNTER_LSB         (*(reg16 *) SpeakerCounter_CounterHW__CNT_CMP0 )
    #define SpeakerCounter_COUNTER_LSB_PTR     ( (reg16 *) SpeakerCounter_CounterHW__CNT_CMP0 )
    #define SpeakerCounter_RT1                 (*(reg8 *) SpeakerCounter_CounterHW__RT1)
    #define SpeakerCounter_RT1_PTR             ( (reg8 *) SpeakerCounter_CounterHW__RT1)
#else
    
    #if (SpeakerCounter_Resolution <= 8u) /* 8-bit Counter */
    
        #define SpeakerCounter_STATICCOUNT_LSB     (*(reg8 *) \
            SpeakerCounter_CounterUDB_sC16_counterdp_u0__F0_REG )
        #define SpeakerCounter_STATICCOUNT_LSB_PTR ( (reg8 *) \
            SpeakerCounter_CounterUDB_sC16_counterdp_u0__F0_REG )
        #define SpeakerCounter_PERIOD_LSB          (*(reg8 *) \
            SpeakerCounter_CounterUDB_sC16_counterdp_u0__D0_REG )
        #define SpeakerCounter_PERIOD_LSB_PTR      ( (reg8 *) \
            SpeakerCounter_CounterUDB_sC16_counterdp_u0__D0_REG )
        #define SpeakerCounter_COMPARE_LSB         (*(reg8 *) \
            SpeakerCounter_CounterUDB_sC16_counterdp_u0__D1_REG )
        #define SpeakerCounter_COMPARE_LSB_PTR     ( (reg8 *) \
            SpeakerCounter_CounterUDB_sC16_counterdp_u0__D1_REG )
        #define SpeakerCounter_COUNTER_LSB         (*(reg8 *) \
            SpeakerCounter_CounterUDB_sC16_counterdp_u0__A0_REG )  
        #define SpeakerCounter_COUNTER_LSB_PTR     ( (reg8 *)\
            SpeakerCounter_CounterUDB_sC16_counterdp_u0__A0_REG )
    
    #elif(SpeakerCounter_Resolution <= 16u) /* 16-bit Counter */
        #if(CY_PSOC3) /* 8-bit address space */ 
            #define SpeakerCounter_STATICCOUNT_LSB     (*(reg16 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__F0_REG )
            #define SpeakerCounter_STATICCOUNT_LSB_PTR ( (reg16 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__F0_REG )
            #define SpeakerCounter_PERIOD_LSB          (*(reg16 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__D0_REG )
            #define SpeakerCounter_PERIOD_LSB_PTR      ( (reg16 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__D0_REG )
            #define SpeakerCounter_COMPARE_LSB         (*(reg16 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__D1_REG )
            #define SpeakerCounter_COMPARE_LSB_PTR     ( (reg16 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__D1_REG )
            #define SpeakerCounter_COUNTER_LSB         (*(reg16 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__A0_REG )  
            #define SpeakerCounter_COUNTER_LSB_PTR     ( (reg16 *)\
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define SpeakerCounter_STATICCOUNT_LSB     (*(reg16 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__16BIT_F0_REG )
            #define SpeakerCounter_STATICCOUNT_LSB_PTR ( (reg16 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__16BIT_F0_REG )
            #define SpeakerCounter_PERIOD_LSB          (*(reg16 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__16BIT_D0_REG )
            #define SpeakerCounter_PERIOD_LSB_PTR      ( (reg16 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__16BIT_D0_REG )
            #define SpeakerCounter_COMPARE_LSB         (*(reg16 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__16BIT_D1_REG )
            #define SpeakerCounter_COMPARE_LSB_PTR     ( (reg16 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__16BIT_D1_REG )
            #define SpeakerCounter_COUNTER_LSB         (*(reg16 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__16BIT_A0_REG )  
            #define SpeakerCounter_COUNTER_LSB_PTR     ( (reg16 *)\
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */   
    #elif(SpeakerCounter_Resolution <= 24u) /* 24-bit Counter */
        
        #define SpeakerCounter_STATICCOUNT_LSB     (*(reg32 *) \
            SpeakerCounter_CounterUDB_sC16_counterdp_u0__F0_REG )
        #define SpeakerCounter_STATICCOUNT_LSB_PTR ( (reg32 *) \
            SpeakerCounter_CounterUDB_sC16_counterdp_u0__F0_REG )
        #define SpeakerCounter_PERIOD_LSB          (*(reg32 *) \
            SpeakerCounter_CounterUDB_sC16_counterdp_u0__D0_REG )
        #define SpeakerCounter_PERIOD_LSB_PTR      ( (reg32 *) \
            SpeakerCounter_CounterUDB_sC16_counterdp_u0__D0_REG )
        #define SpeakerCounter_COMPARE_LSB         (*(reg32 *) \
            SpeakerCounter_CounterUDB_sC16_counterdp_u0__D1_REG )
        #define SpeakerCounter_COMPARE_LSB_PTR     ( (reg32 *) \
            SpeakerCounter_CounterUDB_sC16_counterdp_u0__D1_REG )
        #define SpeakerCounter_COUNTER_LSB         (*(reg32 *) \
            SpeakerCounter_CounterUDB_sC16_counterdp_u0__A0_REG )  
        #define SpeakerCounter_COUNTER_LSB_PTR     ( (reg32 *)\
            SpeakerCounter_CounterUDB_sC16_counterdp_u0__A0_REG )
    
    #else /* 32-bit Counter */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define SpeakerCounter_STATICCOUNT_LSB     (*(reg32 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__F0_REG )
            #define SpeakerCounter_STATICCOUNT_LSB_PTR ( (reg32 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__F0_REG )
            #define SpeakerCounter_PERIOD_LSB          (*(reg32 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__D0_REG )
            #define SpeakerCounter_PERIOD_LSB_PTR      ( (reg32 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__D0_REG )
            #define SpeakerCounter_COMPARE_LSB         (*(reg32 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__D1_REG )
            #define SpeakerCounter_COMPARE_LSB_PTR     ( (reg32 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__D1_REG )
            #define SpeakerCounter_COUNTER_LSB         (*(reg32 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__A0_REG )  
            #define SpeakerCounter_COUNTER_LSB_PTR     ( (reg32 *)\
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define SpeakerCounter_STATICCOUNT_LSB     (*(reg32 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__32BIT_F0_REG )
            #define SpeakerCounter_STATICCOUNT_LSB_PTR ( (reg32 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__32BIT_F0_REG )
            #define SpeakerCounter_PERIOD_LSB          (*(reg32 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__32BIT_D0_REG )
            #define SpeakerCounter_PERIOD_LSB_PTR      ( (reg32 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__32BIT_D0_REG )
            #define SpeakerCounter_COMPARE_LSB         (*(reg32 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__32BIT_D1_REG )
            #define SpeakerCounter_COMPARE_LSB_PTR     ( (reg32 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__32BIT_D1_REG )
            #define SpeakerCounter_COUNTER_LSB         (*(reg32 *) \
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__32BIT_A0_REG )  
            #define SpeakerCounter_COUNTER_LSB_PTR     ( (reg32 *)\
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */   
    #endif

	#define SpeakerCounter_COUNTER_LSB_PTR_8BIT     ( (reg8 *)\
                SpeakerCounter_CounterUDB_sC16_counterdp_u0__A0_REG )
				
    #define SpeakerCounter_AUX_CONTROLDP0 \
        (*(reg8 *) SpeakerCounter_CounterUDB_sC16_counterdp_u0__DP_AUX_CTL_REG)
    
    #define SpeakerCounter_AUX_CONTROLDP0_PTR \
        ( (reg8 *) SpeakerCounter_CounterUDB_sC16_counterdp_u0__DP_AUX_CTL_REG)
    
    #if (SpeakerCounter_Resolution == 16 || SpeakerCounter_Resolution == 24 || SpeakerCounter_Resolution == 32)
       #define SpeakerCounter_AUX_CONTROLDP1 \
           (*(reg8 *) SpeakerCounter_CounterUDB_sC16_counterdp_u1__DP_AUX_CTL_REG)
       #define SpeakerCounter_AUX_CONTROLDP1_PTR \
           ( (reg8 *) SpeakerCounter_CounterUDB_sC16_counterdp_u1__DP_AUX_CTL_REG)
    #endif /* (SpeakerCounter_Resolution == 16 || SpeakerCounter_Resolution == 24 || SpeakerCounter_Resolution == 32) */
    
    #if (SpeakerCounter_Resolution == 24 || SpeakerCounter_Resolution == 32)
       #define SpeakerCounter_AUX_CONTROLDP2 \
           (*(reg8 *) SpeakerCounter_CounterUDB_sC16_counterdp_u2__DP_AUX_CTL_REG)
       #define SpeakerCounter_AUX_CONTROLDP2_PTR \
           ( (reg8 *) SpeakerCounter_CounterUDB_sC16_counterdp_u2__DP_AUX_CTL_REG)
    #endif /* (SpeakerCounter_Resolution == 24 || SpeakerCounter_Resolution == 32) */
    
    #if (SpeakerCounter_Resolution == 32)
       #define SpeakerCounter_AUX_CONTROLDP3 \
           (*(reg8 *) SpeakerCounter_CounterUDB_sC16_counterdp_u3__DP_AUX_CTL_REG)
       #define SpeakerCounter_AUX_CONTROLDP3_PTR \
           ( (reg8 *) SpeakerCounter_CounterUDB_sC16_counterdp_u3__DP_AUX_CTL_REG)
    #endif /* (SpeakerCounter_Resolution == 32) */

#endif  /* (SpeakerCounter_UsingFixedFunction) */

#if (SpeakerCounter_UsingFixedFunction)
    #define SpeakerCounter_STATUS         (*(reg8 *) SpeakerCounter_CounterHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define SpeakerCounter_STATUS_MASK             (*(reg8 *) SpeakerCounter_CounterHW__SR0 )
    #define SpeakerCounter_STATUS_MASK_PTR         ( (reg8 *) SpeakerCounter_CounterHW__SR0 )
    #define SpeakerCounter_CONTROL                 (*(reg8 *) SpeakerCounter_CounterHW__CFG0)
    #define SpeakerCounter_CONTROL_PTR             ( (reg8 *) SpeakerCounter_CounterHW__CFG0)
    #define SpeakerCounter_CONTROL2                (*(reg8 *) SpeakerCounter_CounterHW__CFG1)
    #define SpeakerCounter_CONTROL2_PTR            ( (reg8 *) SpeakerCounter_CounterHW__CFG1)
    #if (CY_PSOC3 || CY_PSOC5LP)
        #define SpeakerCounter_CONTROL3       (*(reg8 *) SpeakerCounter_CounterHW__CFG2)
        #define SpeakerCounter_CONTROL3_PTR   ( (reg8 *) SpeakerCounter_CounterHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define SpeakerCounter_GLOBAL_ENABLE           (*(reg8 *) SpeakerCounter_CounterHW__PM_ACT_CFG)
    #define SpeakerCounter_GLOBAL_ENABLE_PTR       ( (reg8 *) SpeakerCounter_CounterHW__PM_ACT_CFG)
    #define SpeakerCounter_GLOBAL_STBY_ENABLE      (*(reg8 *) SpeakerCounter_CounterHW__PM_STBY_CFG)
    #define SpeakerCounter_GLOBAL_STBY_ENABLE_PTR  ( (reg8 *) SpeakerCounter_CounterHW__PM_STBY_CFG)
    

    /********************************
    *    Constants
    ********************************/

    /* Fixed Function Block Chosen */
    #define SpeakerCounter_BLOCK_EN_MASK          SpeakerCounter_CounterHW__PM_ACT_MSK
    #define SpeakerCounter_BLOCK_STBY_EN_MASK     SpeakerCounter_CounterHW__PM_STBY_MSK 
    
    /* Control Register Bit Locations */    
    /* As defined in Register Map, part of TMRX_CFG0 register */
    #define SpeakerCounter_CTRL_ENABLE_SHIFT      0x00u
    #define SpeakerCounter_ONESHOT_SHIFT          0x02u
    /* Control Register Bit Masks */
    #define SpeakerCounter_CTRL_ENABLE            ((uint8)((uint8)0x01u << SpeakerCounter_CTRL_ENABLE_SHIFT))         
    #define SpeakerCounter_ONESHOT                ((uint8)((uint8)0x01u << SpeakerCounter_ONESHOT_SHIFT))

    /* Control2 Register Bit Masks */
    /* Set the mask for run mode */
    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        #define SpeakerCounter_CTRL_MODE_SHIFT        0x01u    
        #define SpeakerCounter_CTRL_MODE_MASK         ((uint8)((uint8)0x07u << SpeakerCounter_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Use CFG2 Mode bits to set run mode */
        #define SpeakerCounter_CTRL_MODE_SHIFT        0x00u    
        #define SpeakerCounter_CTRL_MODE_MASK         ((uint8)((uint8)0x03u << SpeakerCounter_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    /* Set the mask for interrupt (raw/status register) */
    #define SpeakerCounter_CTRL2_IRQ_SEL_SHIFT     0x00u
    #define SpeakerCounter_CTRL2_IRQ_SEL          ((uint8)((uint8)0x01u << SpeakerCounter_CTRL2_IRQ_SEL_SHIFT))     
    
    /* Status Register Bit Locations */
    #define SpeakerCounter_STATUS_ZERO_SHIFT      0x07u  /* As defined in Register Map, part of TMRX_SR0 register */ 

    /* Status Register Interrupt Enable Bit Locations */
    #define SpeakerCounter_STATUS_ZERO_INT_EN_MASK_SHIFT      (SpeakerCounter_STATUS_ZERO_SHIFT - 0x04u)

    /* Status Register Bit Masks */                           
    #define SpeakerCounter_STATUS_ZERO            ((uint8)((uint8)0x01u << SpeakerCounter_STATUS_ZERO_SHIFT))

    /* Status Register Interrupt Bit Masks*/
    #define SpeakerCounter_STATUS_ZERO_INT_EN_MASK       ((uint8)((uint8)SpeakerCounter_STATUS_ZERO >> 0x04u))
    
    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define SpeakerCounter_RT1_SHIFT            0x04u
    #define SpeakerCounter_RT1_MASK             ((uint8)((uint8)0x03u << SpeakerCounter_RT1_SHIFT))  /* Sync TC and CMP bit masks */
    #define SpeakerCounter_SYNC                 ((uint8)((uint8)0x03u << SpeakerCounter_RT1_SHIFT))
    #define SpeakerCounter_SYNCDSI_SHIFT        0x00u
    #define SpeakerCounter_SYNCDSI_MASK         ((uint8)((uint8)0x0Fu << SpeakerCounter_SYNCDSI_SHIFT)) /* Sync all DSI inputs */
    #define SpeakerCounter_SYNCDSI_EN           ((uint8)((uint8)0x0Fu << SpeakerCounter_SYNCDSI_SHIFT)) /* Sync all DSI inputs */
    
#else /* !SpeakerCounter_UsingFixedFunction */
    #define SpeakerCounter_STATUS               (* (reg8 *) SpeakerCounter_CounterUDB_sSTSReg_stsreg__STATUS_REG )
    #define SpeakerCounter_STATUS_PTR           (  (reg8 *) SpeakerCounter_CounterUDB_sSTSReg_stsreg__STATUS_REG )
    #define SpeakerCounter_STATUS_MASK          (* (reg8 *) SpeakerCounter_CounterUDB_sSTSReg_stsreg__MASK_REG )
    #define SpeakerCounter_STATUS_MASK_PTR      (  (reg8 *) SpeakerCounter_CounterUDB_sSTSReg_stsreg__MASK_REG )
    #define SpeakerCounter_STATUS_AUX_CTRL      (*(reg8 *) SpeakerCounter_CounterUDB_sSTSReg_stsreg__STATUS_AUX_CTL_REG)
    #define SpeakerCounter_STATUS_AUX_CTRL_PTR  ( (reg8 *) SpeakerCounter_CounterUDB_sSTSReg_stsreg__STATUS_AUX_CTL_REG)
    #define SpeakerCounter_CONTROL              (* (reg8 *) SpeakerCounter_CounterUDB_sCTRLReg_ctrlreg__CONTROL_REG )
    #define SpeakerCounter_CONTROL_PTR          (  (reg8 *) SpeakerCounter_CounterUDB_sCTRLReg_ctrlreg__CONTROL_REG )


    /********************************
    *    Constants
    ********************************/
    /* Control Register Bit Locations */
    #define SpeakerCounter_CTRL_CAPMODE0_SHIFT    0x03u       /* As defined by Verilog Implementation */
    #define SpeakerCounter_CTRL_RESET_SHIFT       0x06u       /* As defined by Verilog Implementation */
    #define SpeakerCounter_CTRL_ENABLE_SHIFT      0x07u       /* As defined by Verilog Implementation */
    /* Control Register Bit Masks */
    #define SpeakerCounter_CTRL_CMPMODE_MASK      0x07u 
    #define SpeakerCounter_CTRL_CAPMODE_MASK      0x03u  
    #define SpeakerCounter_CTRL_RESET             ((uint8)((uint8)0x01u << SpeakerCounter_CTRL_RESET_SHIFT))  
    #define SpeakerCounter_CTRL_ENABLE            ((uint8)((uint8)0x01u << SpeakerCounter_CTRL_ENABLE_SHIFT)) 

    /* Status Register Bit Locations */
    #define SpeakerCounter_STATUS_CMP_SHIFT       0x00u       /* As defined by Verilog Implementation */
    #define SpeakerCounter_STATUS_ZERO_SHIFT      0x01u       /* As defined by Verilog Implementation */
    #define SpeakerCounter_STATUS_OVERFLOW_SHIFT  0x02u       /* As defined by Verilog Implementation */
    #define SpeakerCounter_STATUS_UNDERFLOW_SHIFT 0x03u       /* As defined by Verilog Implementation */
    #define SpeakerCounter_STATUS_CAPTURE_SHIFT   0x04u       /* As defined by Verilog Implementation */
    #define SpeakerCounter_STATUS_FIFOFULL_SHIFT  0x05u       /* As defined by Verilog Implementation */
    #define SpeakerCounter_STATUS_FIFONEMP_SHIFT  0x06u       /* As defined by Verilog Implementation */
    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define SpeakerCounter_STATUS_CMP_INT_EN_MASK_SHIFT       SpeakerCounter_STATUS_CMP_SHIFT       
    #define SpeakerCounter_STATUS_ZERO_INT_EN_MASK_SHIFT      SpeakerCounter_STATUS_ZERO_SHIFT      
    #define SpeakerCounter_STATUS_OVERFLOW_INT_EN_MASK_SHIFT  SpeakerCounter_STATUS_OVERFLOW_SHIFT  
    #define SpeakerCounter_STATUS_UNDERFLOW_INT_EN_MASK_SHIFT SpeakerCounter_STATUS_UNDERFLOW_SHIFT 
    #define SpeakerCounter_STATUS_CAPTURE_INT_EN_MASK_SHIFT   SpeakerCounter_STATUS_CAPTURE_SHIFT   
    #define SpeakerCounter_STATUS_FIFOFULL_INT_EN_MASK_SHIFT  SpeakerCounter_STATUS_FIFOFULL_SHIFT  
    #define SpeakerCounter_STATUS_FIFONEMP_INT_EN_MASK_SHIFT  SpeakerCounter_STATUS_FIFONEMP_SHIFT  
    /* Status Register Bit Masks */                
    #define SpeakerCounter_STATUS_CMP             ((uint8)((uint8)0x01u << SpeakerCounter_STATUS_CMP_SHIFT))  
    #define SpeakerCounter_STATUS_ZERO            ((uint8)((uint8)0x01u << SpeakerCounter_STATUS_ZERO_SHIFT)) 
    #define SpeakerCounter_STATUS_OVERFLOW        ((uint8)((uint8)0x01u << SpeakerCounter_STATUS_OVERFLOW_SHIFT)) 
    #define SpeakerCounter_STATUS_UNDERFLOW       ((uint8)((uint8)0x01u << SpeakerCounter_STATUS_UNDERFLOW_SHIFT)) 
    #define SpeakerCounter_STATUS_CAPTURE         ((uint8)((uint8)0x01u << SpeakerCounter_STATUS_CAPTURE_SHIFT)) 
    #define SpeakerCounter_STATUS_FIFOFULL        ((uint8)((uint8)0x01u << SpeakerCounter_STATUS_FIFOFULL_SHIFT))
    #define SpeakerCounter_STATUS_FIFONEMP        ((uint8)((uint8)0x01u << SpeakerCounter_STATUS_FIFONEMP_SHIFT))
    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define SpeakerCounter_STATUS_CMP_INT_EN_MASK            SpeakerCounter_STATUS_CMP                    
    #define SpeakerCounter_STATUS_ZERO_INT_EN_MASK           SpeakerCounter_STATUS_ZERO            
    #define SpeakerCounter_STATUS_OVERFLOW_INT_EN_MASK       SpeakerCounter_STATUS_OVERFLOW        
    #define SpeakerCounter_STATUS_UNDERFLOW_INT_EN_MASK      SpeakerCounter_STATUS_UNDERFLOW       
    #define SpeakerCounter_STATUS_CAPTURE_INT_EN_MASK        SpeakerCounter_STATUS_CAPTURE         
    #define SpeakerCounter_STATUS_FIFOFULL_INT_EN_MASK       SpeakerCounter_STATUS_FIFOFULL        
    #define SpeakerCounter_STATUS_FIFONEMP_INT_EN_MASK       SpeakerCounter_STATUS_FIFONEMP         
    

    /* StatusI Interrupt Enable bit Location in the Auxilliary Control Register */
    #define SpeakerCounter_STATUS_ACTL_INT_EN     0x10u /* As defined for the ACTL Register */
    
    /* Datapath Auxillary Control Register definitions */
    #define SpeakerCounter_AUX_CTRL_FIFO0_CLR         0x01u   /* As defined by Register map */
    #define SpeakerCounter_AUX_CTRL_FIFO1_CLR         0x02u   /* As defined by Register map */
    #define SpeakerCounter_AUX_CTRL_FIFO0_LVL         0x04u   /* As defined by Register map */
    #define SpeakerCounter_AUX_CTRL_FIFO1_LVL         0x08u   /* As defined by Register map */
    #define SpeakerCounter_STATUS_ACTL_INT_EN_MASK    0x10u   /* As defined for the ACTL Register */
    
#endif /* SpeakerCounter_UsingFixedFunction */

#endif  /* CY_COUNTER_SpeakerCounter_H */


/* [] END OF FILE */

