/*******************************************************************************
* File Name: LEDDuration_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "LEDDuration.h"

static LEDDuration_backupStruct LEDDuration_backup;


/*******************************************************************************
* Function Name: LEDDuration_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  LEDDuration_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void LEDDuration_SaveConfig(void) 
{
    #if (!LEDDuration_UsingFixedFunction)
        LEDDuration_backup.TimerUdb = LEDDuration_ReadCounter();
        LEDDuration_backup.InterruptMaskValue = LEDDuration_STATUS_MASK;
        #if (LEDDuration_UsingHWCaptureCounter)
            LEDDuration_backup.TimerCaptureCounter = LEDDuration_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!LEDDuration_UDB_CONTROL_REG_REMOVED)
            LEDDuration_backup.TimerControlRegister = LEDDuration_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: LEDDuration_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  LEDDuration_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void LEDDuration_RestoreConfig(void) 
{   
    #if (!LEDDuration_UsingFixedFunction)

        LEDDuration_WriteCounter(LEDDuration_backup.TimerUdb);
        LEDDuration_STATUS_MASK =LEDDuration_backup.InterruptMaskValue;
        #if (LEDDuration_UsingHWCaptureCounter)
            LEDDuration_SetCaptureCount(LEDDuration_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!LEDDuration_UDB_CONTROL_REG_REMOVED)
            LEDDuration_WriteControlRegister(LEDDuration_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: LEDDuration_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  LEDDuration_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void LEDDuration_Sleep(void) 
{
    #if(!LEDDuration_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(LEDDuration_CTRL_ENABLE == (LEDDuration_CONTROL & LEDDuration_CTRL_ENABLE))
        {
            /* Timer is enabled */
            LEDDuration_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            LEDDuration_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    LEDDuration_Stop();
    LEDDuration_SaveConfig();
}


/*******************************************************************************
* Function Name: LEDDuration_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  LEDDuration_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void LEDDuration_Wakeup(void) 
{
    LEDDuration_RestoreConfig();
    #if(!LEDDuration_UDB_CONTROL_REG_REMOVED)
        if(LEDDuration_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                LEDDuration_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
