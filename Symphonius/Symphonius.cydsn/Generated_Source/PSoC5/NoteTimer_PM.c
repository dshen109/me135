/*******************************************************************************
* File Name: NoteTimer_PM.c
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

#include "NoteTimer.h"

static NoteTimer_backupStruct NoteTimer_backup;


/*******************************************************************************
* Function Name: NoteTimer_SaveConfig
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
*  NoteTimer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void NoteTimer_SaveConfig(void) 
{
    #if (!NoteTimer_UsingFixedFunction)
        NoteTimer_backup.TimerUdb = NoteTimer_ReadCounter();
        NoteTimer_backup.InterruptMaskValue = NoteTimer_STATUS_MASK;
        #if (NoteTimer_UsingHWCaptureCounter)
            NoteTimer_backup.TimerCaptureCounter = NoteTimer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!NoteTimer_UDB_CONTROL_REG_REMOVED)
            NoteTimer_backup.TimerControlRegister = NoteTimer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: NoteTimer_RestoreConfig
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
*  NoteTimer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void NoteTimer_RestoreConfig(void) 
{   
    #if (!NoteTimer_UsingFixedFunction)

        NoteTimer_WriteCounter(NoteTimer_backup.TimerUdb);
        NoteTimer_STATUS_MASK =NoteTimer_backup.InterruptMaskValue;
        #if (NoteTimer_UsingHWCaptureCounter)
            NoteTimer_SetCaptureCount(NoteTimer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!NoteTimer_UDB_CONTROL_REG_REMOVED)
            NoteTimer_WriteControlRegister(NoteTimer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: NoteTimer_Sleep
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
*  NoteTimer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void NoteTimer_Sleep(void) 
{
    #if(!NoteTimer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(NoteTimer_CTRL_ENABLE == (NoteTimer_CONTROL & NoteTimer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            NoteTimer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            NoteTimer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    NoteTimer_Stop();
    NoteTimer_SaveConfig();
}


/*******************************************************************************
* Function Name: NoteTimer_Wakeup
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
*  NoteTimer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void NoteTimer_Wakeup(void) 
{
    NoteTimer_RestoreConfig();
    #if(!NoteTimer_UDB_CONTROL_REG_REMOVED)
        if(NoteTimer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                NoteTimer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
