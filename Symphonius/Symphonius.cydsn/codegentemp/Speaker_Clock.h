/*******************************************************************************
* File Name: Speaker_Clock.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_Speaker_Clock_H)
#define CY_CLOCK_Speaker_Clock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void Speaker_Clock_Start(void) ;
void Speaker_Clock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void Speaker_Clock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void Speaker_Clock_StandbyPower(uint8 state) ;
void Speaker_Clock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 Speaker_Clock_GetDividerRegister(void) ;
void Speaker_Clock_SetModeRegister(uint8 modeBitMask) ;
void Speaker_Clock_ClearModeRegister(uint8 modeBitMask) ;
uint8 Speaker_Clock_GetModeRegister(void) ;
void Speaker_Clock_SetSourceRegister(uint8 clkSource) ;
uint8 Speaker_Clock_GetSourceRegister(void) ;
#if defined(Speaker_Clock__CFG3)
void Speaker_Clock_SetPhaseRegister(uint8 clkPhase) ;
uint8 Speaker_Clock_GetPhaseRegister(void) ;
#endif /* defined(Speaker_Clock__CFG3) */

#define Speaker_Clock_Enable()                       Speaker_Clock_Start()
#define Speaker_Clock_Disable()                      Speaker_Clock_Stop()
#define Speaker_Clock_SetDivider(clkDivider)         Speaker_Clock_SetDividerRegister(clkDivider, 1u)
#define Speaker_Clock_SetDividerValue(clkDivider)    Speaker_Clock_SetDividerRegister((clkDivider) - 1u, 1u)
#define Speaker_Clock_SetMode(clkMode)               Speaker_Clock_SetModeRegister(clkMode)
#define Speaker_Clock_SetSource(clkSource)           Speaker_Clock_SetSourceRegister(clkSource)
#if defined(Speaker_Clock__CFG3)
#define Speaker_Clock_SetPhase(clkPhase)             Speaker_Clock_SetPhaseRegister(clkPhase)
#define Speaker_Clock_SetPhaseValue(clkPhase)        Speaker_Clock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(Speaker_Clock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define Speaker_Clock_CLKEN              (* (reg8 *) Speaker_Clock__PM_ACT_CFG)
#define Speaker_Clock_CLKEN_PTR          ((reg8 *) Speaker_Clock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define Speaker_Clock_CLKSTBY            (* (reg8 *) Speaker_Clock__PM_STBY_CFG)
#define Speaker_Clock_CLKSTBY_PTR        ((reg8 *) Speaker_Clock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define Speaker_Clock_DIV_LSB            (* (reg8 *) Speaker_Clock__CFG0)
#define Speaker_Clock_DIV_LSB_PTR        ((reg8 *) Speaker_Clock__CFG0)
#define Speaker_Clock_DIV_PTR            ((reg16 *) Speaker_Clock__CFG0)

/* Clock MSB divider configuration register. */
#define Speaker_Clock_DIV_MSB            (* (reg8 *) Speaker_Clock__CFG1)
#define Speaker_Clock_DIV_MSB_PTR        ((reg8 *) Speaker_Clock__CFG1)

/* Mode and source configuration register */
#define Speaker_Clock_MOD_SRC            (* (reg8 *) Speaker_Clock__CFG2)
#define Speaker_Clock_MOD_SRC_PTR        ((reg8 *) Speaker_Clock__CFG2)

#if defined(Speaker_Clock__CFG3)
/* Analog clock phase configuration register */
#define Speaker_Clock_PHASE              (* (reg8 *) Speaker_Clock__CFG3)
#define Speaker_Clock_PHASE_PTR          ((reg8 *) Speaker_Clock__CFG3)
#endif /* defined(Speaker_Clock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define Speaker_Clock_CLKEN_MASK         Speaker_Clock__PM_ACT_MSK
#define Speaker_Clock_CLKSTBY_MASK       Speaker_Clock__PM_STBY_MSK

/* CFG2 field masks */
#define Speaker_Clock_SRC_SEL_MSK        Speaker_Clock__CFG2_SRC_SEL_MASK
#define Speaker_Clock_MODE_MASK          (~(Speaker_Clock_SRC_SEL_MSK))

#if defined(Speaker_Clock__CFG3)
/* CFG3 phase mask */
#define Speaker_Clock_PHASE_MASK         Speaker_Clock__CFG3_PHASE_DLY_MASK
#endif /* defined(Speaker_Clock__CFG3) */

#endif /* CY_CLOCK_Speaker_Clock_H */


/* [] END OF FILE */
