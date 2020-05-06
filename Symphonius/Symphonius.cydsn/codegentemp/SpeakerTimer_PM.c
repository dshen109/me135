/*******************************************************************************
* File Name: SpeakerTimer_PM.c
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

#include "SpeakerTimer.h"

static SpeakerTimer_backupStruct SpeakerTimer_backup;


/*******************************************************************************
* Function Name: SpeakerTimer_SaveConfig
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
*  SpeakerTimer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void SpeakerTimer_SaveConfig(void) 
{
    #if (!SpeakerTimer_UsingFixedFunction)
        SpeakerTimer_backup.TimerUdb = SpeakerTimer_ReadCounter();
        SpeakerTimer_backup.InterruptMaskValue = SpeakerTimer_STATUS_MASK;
        #if (SpeakerTimer_UsingHWCaptureCounter)
            SpeakerTimer_backup.TimerCaptureCounter = SpeakerTimer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!SpeakerTimer_UDB_CONTROL_REG_REMOVED)
            SpeakerTimer_backup.TimerControlRegister = SpeakerTimer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: SpeakerTimer_RestoreConfig
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
*  SpeakerTimer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void SpeakerTimer_RestoreConfig(void) 
{   
    #if (!SpeakerTimer_UsingFixedFunction)

        SpeakerTimer_WriteCounter(SpeakerTimer_backup.TimerUdb);
        SpeakerTimer_STATUS_MASK =SpeakerTimer_backup.InterruptMaskValue;
        #if (SpeakerTimer_UsingHWCaptureCounter)
            SpeakerTimer_SetCaptureCount(SpeakerTimer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!SpeakerTimer_UDB_CONTROL_REG_REMOVED)
            SpeakerTimer_WriteControlRegister(SpeakerTimer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: SpeakerTimer_Sleep
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
*  SpeakerTimer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void SpeakerTimer_Sleep(void) 
{
    #if(!SpeakerTimer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(SpeakerTimer_CTRL_ENABLE == (SpeakerTimer_CONTROL & SpeakerTimer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            SpeakerTimer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            SpeakerTimer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    SpeakerTimer_Stop();
    SpeakerTimer_SaveConfig();
}


/*******************************************************************************
* Function Name: SpeakerTimer_Wakeup
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
*  SpeakerTimer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void SpeakerTimer_Wakeup(void) 
{
    SpeakerTimer_RestoreConfig();
    #if(!SpeakerTimer_UDB_CONTROL_REG_REMOVED)
        if(SpeakerTimer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                SpeakerTimer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
