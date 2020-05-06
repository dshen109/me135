/*******************************************************************************
* File Name: SpeakerCounter.c  
* Version 3.0
*
*  Description:
*     The Counter component consists of a 8, 16, 24 or 32-bit counter with
*     a selectable period between 2 and 2^Width - 1.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "SpeakerCounter.h"

uint8 SpeakerCounter_initVar = 0u;


/*******************************************************************************
* Function Name: SpeakerCounter_Init
********************************************************************************
* Summary:
*     Initialize to the schematic state
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void SpeakerCounter_Init(void) 
{
        #if (!SpeakerCounter_UsingFixedFunction && !SpeakerCounter_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!SpeakerCounter_UsingFixedFunction && !SpeakerCounter_ControlRegRemoved) */
        
        #if(!SpeakerCounter_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 SpeakerCounter_interruptState;
        #endif /* (!SpeakerCounter_UsingFixedFunction) */
        
        #if (SpeakerCounter_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            SpeakerCounter_CONTROL &= SpeakerCounter_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                SpeakerCounter_CONTROL2 &= ((uint8)(~SpeakerCounter_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                SpeakerCounter_CONTROL3 &= ((uint8)(~SpeakerCounter_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (SpeakerCounter_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                SpeakerCounter_CONTROL |= SpeakerCounter_ONESHOT;
            #endif /* (SpeakerCounter_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            SpeakerCounter_CONTROL2 |= SpeakerCounter_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            SpeakerCounter_RT1 &= ((uint8)(~SpeakerCounter_RT1_MASK));
            SpeakerCounter_RT1 |= SpeakerCounter_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            SpeakerCounter_RT1 &= ((uint8)(~SpeakerCounter_SYNCDSI_MASK));
            SpeakerCounter_RT1 |= SpeakerCounter_SYNCDSI_EN;

        #else
            #if(!SpeakerCounter_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = SpeakerCounter_CONTROL & ((uint8)(~SpeakerCounter_CTRL_CMPMODE_MASK));
            SpeakerCounter_CONTROL = ctrl | SpeakerCounter_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = SpeakerCounter_CONTROL & ((uint8)(~SpeakerCounter_CTRL_CAPMODE_MASK));
            
            #if( 0 != SpeakerCounter_CAPTURE_MODE_CONF)
                SpeakerCounter_CONTROL = ctrl | SpeakerCounter_DEFAULT_CAPTURE_MODE;
            #else
                SpeakerCounter_CONTROL = ctrl;
            #endif /* 0 != SpeakerCounter_CAPTURE_MODE */ 
            
            #endif /* (!SpeakerCounter_ControlRegRemoved) */
        #endif /* (SpeakerCounter_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!SpeakerCounter_UsingFixedFunction)
            SpeakerCounter_ClearFIFO();
        #endif /* (!SpeakerCounter_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        SpeakerCounter_WritePeriod(SpeakerCounter_INIT_PERIOD_VALUE);
        #if (!(SpeakerCounter_UsingFixedFunction && (CY_PSOC5A)))
            SpeakerCounter_WriteCounter(SpeakerCounter_INIT_COUNTER_VALUE);
        #endif /* (!(SpeakerCounter_UsingFixedFunction && (CY_PSOC5A))) */
        SpeakerCounter_SetInterruptMode(SpeakerCounter_INIT_INTERRUPTS_MASK);
        
        #if (!SpeakerCounter_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)SpeakerCounter_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            SpeakerCounter_WriteCompare(SpeakerCounter_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            SpeakerCounter_interruptState = CyEnterCriticalSection();
            
            SpeakerCounter_STATUS_AUX_CTRL |= SpeakerCounter_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(SpeakerCounter_interruptState);
            
        #endif /* (!SpeakerCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: SpeakerCounter_Enable
********************************************************************************
* Summary:
*     Enable the Counter
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: 
*   If the Enable mode is set to Hardware only then this function has no effect 
*   on the operation of the counter.
*
*******************************************************************************/
void SpeakerCounter_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (SpeakerCounter_UsingFixedFunction)
        SpeakerCounter_GLOBAL_ENABLE |= SpeakerCounter_BLOCK_EN_MASK;
        SpeakerCounter_GLOBAL_STBY_ENABLE |= SpeakerCounter_BLOCK_STBY_EN_MASK;
    #endif /* (SpeakerCounter_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!SpeakerCounter_ControlRegRemoved || SpeakerCounter_UsingFixedFunction)
        SpeakerCounter_CONTROL |= SpeakerCounter_CTRL_ENABLE;                
    #endif /* (!SpeakerCounter_ControlRegRemoved || SpeakerCounter_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: SpeakerCounter_Start
********************************************************************************
* Summary:
*  Enables the counter for operation 
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Global variables:
*  SpeakerCounter_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void SpeakerCounter_Start(void) 
{
    if(SpeakerCounter_initVar == 0u)
    {
        SpeakerCounter_Init();
        
        SpeakerCounter_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    SpeakerCounter_Enable();        
}


/*******************************************************************************
* Function Name: SpeakerCounter_Stop
********************************************************************************
* Summary:
* Halts the counter, but does not change any modes or disable interrupts.
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the counter.
*
*******************************************************************************/
void SpeakerCounter_Stop(void) 
{
    /* Disable Counter */
    #if(!SpeakerCounter_ControlRegRemoved || SpeakerCounter_UsingFixedFunction)
        SpeakerCounter_CONTROL &= ((uint8)(~SpeakerCounter_CTRL_ENABLE));        
    #endif /* (!SpeakerCounter_ControlRegRemoved || SpeakerCounter_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (SpeakerCounter_UsingFixedFunction)
        SpeakerCounter_GLOBAL_ENABLE &= ((uint8)(~SpeakerCounter_BLOCK_EN_MASK));
        SpeakerCounter_GLOBAL_STBY_ENABLE &= ((uint8)(~SpeakerCounter_BLOCK_STBY_EN_MASK));
    #endif /* (SpeakerCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: SpeakerCounter_SetInterruptMode
********************************************************************************
* Summary:
* Configures which interrupt sources are enabled to generate the final interrupt
*
* Parameters:  
*  InterruptsMask: This parameter is an or'd collection of the status bits
*                   which will be allowed to generate the counters interrupt.   
*
* Return: 
*  void
*
*******************************************************************************/
void SpeakerCounter_SetInterruptMode(uint8 interruptsMask) 
{
    SpeakerCounter_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: SpeakerCounter_ReadStatusRegister
********************************************************************************
* Summary:
*   Reads the status register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the status register
*
* Side Effects:
*   Status register bits may be clear on read. 
*
*******************************************************************************/
uint8   SpeakerCounter_ReadStatusRegister(void) 
{
    return SpeakerCounter_STATUS;
}


#if(!SpeakerCounter_ControlRegRemoved)
/*******************************************************************************
* Function Name: SpeakerCounter_ReadControlRegister
********************************************************************************
* Summary:
*   Reads the control register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
uint8   SpeakerCounter_ReadControlRegister(void) 
{
    return SpeakerCounter_CONTROL;
}


/*******************************************************************************
* Function Name: SpeakerCounter_WriteControlRegister
********************************************************************************
* Summary:
*   Sets the bit-field of the control register.  This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
void    SpeakerCounter_WriteControlRegister(uint8 control) 
{
    SpeakerCounter_CONTROL = control;
}

#endif  /* (!SpeakerCounter_ControlRegRemoved) */


#if (!(SpeakerCounter_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: SpeakerCounter_WriteCounter
********************************************************************************
* Summary:
*   This funtion is used to set the counter to a specific value
*
* Parameters:  
*  counter:  New counter value. 
*
* Return: 
*  void 
*
*******************************************************************************/
void SpeakerCounter_WriteCounter(uint16 counter) \
                                   
{
    #if(SpeakerCounter_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (SpeakerCounter_GLOBAL_ENABLE & SpeakerCounter_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        SpeakerCounter_GLOBAL_ENABLE |= SpeakerCounter_BLOCK_EN_MASK;
        CY_SET_REG16(SpeakerCounter_COUNTER_LSB_PTR, (uint16)counter);
        SpeakerCounter_GLOBAL_ENABLE &= ((uint8)(~SpeakerCounter_BLOCK_EN_MASK));
    #else
        CY_SET_REG16(SpeakerCounter_COUNTER_LSB_PTR, counter);
    #endif /* (SpeakerCounter_UsingFixedFunction) */
}
#endif /* (!(SpeakerCounter_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: SpeakerCounter_ReadCounter
********************************************************************************
* Summary:
* Returns the current value of the counter.  It doesn't matter
* if the counter is enabled or running.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint16) The present value of the counter.
*
*******************************************************************************/
uint16 SpeakerCounter_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(SpeakerCounter_UsingFixedFunction)
		(void)CY_GET_REG16(SpeakerCounter_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(SpeakerCounter_COUNTER_LSB_PTR_8BIT);
	#endif/* (SpeakerCounter_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(SpeakerCounter_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(SpeakerCounter_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG16(SpeakerCounter_STATICCOUNT_LSB_PTR));
    #endif /* (SpeakerCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: SpeakerCounter_ReadCapture
********************************************************************************
* Summary:
*   This function returns the last value captured.
*
* Parameters:  
*  void
*
* Return: 
*  (uint16) Present Capture value.
*
*******************************************************************************/
uint16 SpeakerCounter_ReadCapture(void) 
{
    #if(SpeakerCounter_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(SpeakerCounter_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG16(SpeakerCounter_STATICCOUNT_LSB_PTR));
    #endif /* (SpeakerCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: SpeakerCounter_WritePeriod
********************************************************************************
* Summary:
* Changes the period of the counter.  The new period 
* will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period: (uint16) A value of 0 will result in
*         the counter remaining at zero.  
*
* Return: 
*  void
*
*******************************************************************************/
void SpeakerCounter_WritePeriod(uint16 period) 
{
    #if(SpeakerCounter_UsingFixedFunction)
        CY_SET_REG16(SpeakerCounter_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG16(SpeakerCounter_PERIOD_LSB_PTR, period);
    #endif /* (SpeakerCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: SpeakerCounter_ReadPeriod
********************************************************************************
* Summary:
* Reads the current period value without affecting counter operation.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint16) Present period value.
*
*******************************************************************************/
uint16 SpeakerCounter_ReadPeriod(void) 
{
    #if(SpeakerCounter_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(SpeakerCounter_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG16(SpeakerCounter_PERIOD_LSB_PTR));
    #endif /* (SpeakerCounter_UsingFixedFunction) */
}


#if (!SpeakerCounter_UsingFixedFunction)
/*******************************************************************************
* Function Name: SpeakerCounter_WriteCompare
********************************************************************************
* Summary:
* Changes the compare value.  The compare output will 
* reflect the new value on the next UDB clock.  The compare output will be 
* driven high when the present counter value compares true based on the 
* configured compare mode setting. 
*
* Parameters:  
*  Compare:  New compare value. 
*
* Return: 
*  void
*
*******************************************************************************/
void SpeakerCounter_WriteCompare(uint16 compare) \
                                   
{
    #if(SpeakerCounter_UsingFixedFunction)
        CY_SET_REG16(SpeakerCounter_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG16(SpeakerCounter_COMPARE_LSB_PTR, compare);
    #endif /* (SpeakerCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: SpeakerCounter_ReadCompare
********************************************************************************
* Summary:
* Returns the compare value.
*
* Parameters:  
*  void:
*
* Return: 
*  (uint16) Present compare value.
*
*******************************************************************************/
uint16 SpeakerCounter_ReadCompare(void) 
{
    return (CY_GET_REG16(SpeakerCounter_COMPARE_LSB_PTR));
}


#if (SpeakerCounter_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: SpeakerCounter_SetCompareMode
********************************************************************************
* Summary:
*  Sets the software controlled Compare Mode.
*
* Parameters:
*  compareMode:  Compare Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void SpeakerCounter_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    SpeakerCounter_CONTROL &= ((uint8)(~SpeakerCounter_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    SpeakerCounter_CONTROL |= compareMode;
}
#endif  /* (SpeakerCounter_COMPARE_MODE_SOFTWARE) */


#if (SpeakerCounter_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: SpeakerCounter_SetCaptureMode
********************************************************************************
* Summary:
*  Sets the software controlled Capture Mode.
*
* Parameters:
*  captureMode:  Capture Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void SpeakerCounter_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    SpeakerCounter_CONTROL &= ((uint8)(~SpeakerCounter_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    SpeakerCounter_CONTROL |= ((uint8)((uint8)captureMode << SpeakerCounter_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (SpeakerCounter_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: SpeakerCounter_ClearFIFO
********************************************************************************
* Summary:
*   This function clears all capture data from the capture FIFO
*
* Parameters:  
*  void:
*
* Return: 
*  None
*
*******************************************************************************/
void SpeakerCounter_ClearFIFO(void) 
{

    while(0u != (SpeakerCounter_ReadStatusRegister() & SpeakerCounter_STATUS_FIFONEMP))
    {
        (void)SpeakerCounter_ReadCapture();
    }

}
#endif  /* (!SpeakerCounter_UsingFixedFunction) */


/* [] END OF FILE */

