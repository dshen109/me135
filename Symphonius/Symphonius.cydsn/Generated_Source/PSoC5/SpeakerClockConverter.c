/*******************************************************************************
* File Name: SpeakerClockConverter.c  
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

#include "SpeakerClockConverter.h"

uint8 SpeakerClockConverter_initVar = 0u;


/*******************************************************************************
* Function Name: SpeakerClockConverter_Init
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
void SpeakerClockConverter_Init(void) 
{
        #if (!SpeakerClockConverter_UsingFixedFunction && !SpeakerClockConverter_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!SpeakerClockConverter_UsingFixedFunction && !SpeakerClockConverter_ControlRegRemoved) */
        
        #if(!SpeakerClockConverter_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 SpeakerClockConverter_interruptState;
        #endif /* (!SpeakerClockConverter_UsingFixedFunction) */
        
        #if (SpeakerClockConverter_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            SpeakerClockConverter_CONTROL &= SpeakerClockConverter_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                SpeakerClockConverter_CONTROL2 &= ((uint8)(~SpeakerClockConverter_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                SpeakerClockConverter_CONTROL3 &= ((uint8)(~SpeakerClockConverter_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (SpeakerClockConverter_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                SpeakerClockConverter_CONTROL |= SpeakerClockConverter_ONESHOT;
            #endif /* (SpeakerClockConverter_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            SpeakerClockConverter_CONTROL2 |= SpeakerClockConverter_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            SpeakerClockConverter_RT1 &= ((uint8)(~SpeakerClockConverter_RT1_MASK));
            SpeakerClockConverter_RT1 |= SpeakerClockConverter_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            SpeakerClockConverter_RT1 &= ((uint8)(~SpeakerClockConverter_SYNCDSI_MASK));
            SpeakerClockConverter_RT1 |= SpeakerClockConverter_SYNCDSI_EN;

        #else
            #if(!SpeakerClockConverter_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = SpeakerClockConverter_CONTROL & ((uint8)(~SpeakerClockConverter_CTRL_CMPMODE_MASK));
            SpeakerClockConverter_CONTROL = ctrl | SpeakerClockConverter_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = SpeakerClockConverter_CONTROL & ((uint8)(~SpeakerClockConverter_CTRL_CAPMODE_MASK));
            
            #if( 0 != SpeakerClockConverter_CAPTURE_MODE_CONF)
                SpeakerClockConverter_CONTROL = ctrl | SpeakerClockConverter_DEFAULT_CAPTURE_MODE;
            #else
                SpeakerClockConverter_CONTROL = ctrl;
            #endif /* 0 != SpeakerClockConverter_CAPTURE_MODE */ 
            
            #endif /* (!SpeakerClockConverter_ControlRegRemoved) */
        #endif /* (SpeakerClockConverter_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!SpeakerClockConverter_UsingFixedFunction)
            SpeakerClockConverter_ClearFIFO();
        #endif /* (!SpeakerClockConverter_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        SpeakerClockConverter_WritePeriod(SpeakerClockConverter_INIT_PERIOD_VALUE);
        #if (!(SpeakerClockConverter_UsingFixedFunction && (CY_PSOC5A)))
            SpeakerClockConverter_WriteCounter(SpeakerClockConverter_INIT_COUNTER_VALUE);
        #endif /* (!(SpeakerClockConverter_UsingFixedFunction && (CY_PSOC5A))) */
        SpeakerClockConverter_SetInterruptMode(SpeakerClockConverter_INIT_INTERRUPTS_MASK);
        
        #if (!SpeakerClockConverter_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)SpeakerClockConverter_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            SpeakerClockConverter_WriteCompare(SpeakerClockConverter_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            SpeakerClockConverter_interruptState = CyEnterCriticalSection();
            
            SpeakerClockConverter_STATUS_AUX_CTRL |= SpeakerClockConverter_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(SpeakerClockConverter_interruptState);
            
        #endif /* (!SpeakerClockConverter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: SpeakerClockConverter_Enable
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
void SpeakerClockConverter_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (SpeakerClockConverter_UsingFixedFunction)
        SpeakerClockConverter_GLOBAL_ENABLE |= SpeakerClockConverter_BLOCK_EN_MASK;
        SpeakerClockConverter_GLOBAL_STBY_ENABLE |= SpeakerClockConverter_BLOCK_STBY_EN_MASK;
    #endif /* (SpeakerClockConverter_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!SpeakerClockConverter_ControlRegRemoved || SpeakerClockConverter_UsingFixedFunction)
        SpeakerClockConverter_CONTROL |= SpeakerClockConverter_CTRL_ENABLE;                
    #endif /* (!SpeakerClockConverter_ControlRegRemoved || SpeakerClockConverter_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: SpeakerClockConverter_Start
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
*  SpeakerClockConverter_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void SpeakerClockConverter_Start(void) 
{
    if(SpeakerClockConverter_initVar == 0u)
    {
        SpeakerClockConverter_Init();
        
        SpeakerClockConverter_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    SpeakerClockConverter_Enable();        
}


/*******************************************************************************
* Function Name: SpeakerClockConverter_Stop
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
void SpeakerClockConverter_Stop(void) 
{
    /* Disable Counter */
    #if(!SpeakerClockConverter_ControlRegRemoved || SpeakerClockConverter_UsingFixedFunction)
        SpeakerClockConverter_CONTROL &= ((uint8)(~SpeakerClockConverter_CTRL_ENABLE));        
    #endif /* (!SpeakerClockConverter_ControlRegRemoved || SpeakerClockConverter_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (SpeakerClockConverter_UsingFixedFunction)
        SpeakerClockConverter_GLOBAL_ENABLE &= ((uint8)(~SpeakerClockConverter_BLOCK_EN_MASK));
        SpeakerClockConverter_GLOBAL_STBY_ENABLE &= ((uint8)(~SpeakerClockConverter_BLOCK_STBY_EN_MASK));
    #endif /* (SpeakerClockConverter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: SpeakerClockConverter_SetInterruptMode
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
void SpeakerClockConverter_SetInterruptMode(uint8 interruptsMask) 
{
    SpeakerClockConverter_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: SpeakerClockConverter_ReadStatusRegister
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
uint8   SpeakerClockConverter_ReadStatusRegister(void) 
{
    return SpeakerClockConverter_STATUS;
}


#if(!SpeakerClockConverter_ControlRegRemoved)
/*******************************************************************************
* Function Name: SpeakerClockConverter_ReadControlRegister
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
uint8   SpeakerClockConverter_ReadControlRegister(void) 
{
    return SpeakerClockConverter_CONTROL;
}


/*******************************************************************************
* Function Name: SpeakerClockConverter_WriteControlRegister
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
void    SpeakerClockConverter_WriteControlRegister(uint8 control) 
{
    SpeakerClockConverter_CONTROL = control;
}

#endif  /* (!SpeakerClockConverter_ControlRegRemoved) */


#if (!(SpeakerClockConverter_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: SpeakerClockConverter_WriteCounter
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
void SpeakerClockConverter_WriteCounter(uint16 counter) \
                                   
{
    #if(SpeakerClockConverter_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (SpeakerClockConverter_GLOBAL_ENABLE & SpeakerClockConverter_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        SpeakerClockConverter_GLOBAL_ENABLE |= SpeakerClockConverter_BLOCK_EN_MASK;
        CY_SET_REG16(SpeakerClockConverter_COUNTER_LSB_PTR, (uint16)counter);
        SpeakerClockConverter_GLOBAL_ENABLE &= ((uint8)(~SpeakerClockConverter_BLOCK_EN_MASK));
    #else
        CY_SET_REG16(SpeakerClockConverter_COUNTER_LSB_PTR, counter);
    #endif /* (SpeakerClockConverter_UsingFixedFunction) */
}
#endif /* (!(SpeakerClockConverter_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: SpeakerClockConverter_ReadCounter
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
uint16 SpeakerClockConverter_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(SpeakerClockConverter_UsingFixedFunction)
		(void)CY_GET_REG16(SpeakerClockConverter_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(SpeakerClockConverter_COUNTER_LSB_PTR_8BIT);
	#endif/* (SpeakerClockConverter_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(SpeakerClockConverter_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(SpeakerClockConverter_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG16(SpeakerClockConverter_STATICCOUNT_LSB_PTR));
    #endif /* (SpeakerClockConverter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: SpeakerClockConverter_ReadCapture
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
uint16 SpeakerClockConverter_ReadCapture(void) 
{
    #if(SpeakerClockConverter_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(SpeakerClockConverter_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG16(SpeakerClockConverter_STATICCOUNT_LSB_PTR));
    #endif /* (SpeakerClockConverter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: SpeakerClockConverter_WritePeriod
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
void SpeakerClockConverter_WritePeriod(uint16 period) 
{
    #if(SpeakerClockConverter_UsingFixedFunction)
        CY_SET_REG16(SpeakerClockConverter_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG16(SpeakerClockConverter_PERIOD_LSB_PTR, period);
    #endif /* (SpeakerClockConverter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: SpeakerClockConverter_ReadPeriod
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
uint16 SpeakerClockConverter_ReadPeriod(void) 
{
    #if(SpeakerClockConverter_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(SpeakerClockConverter_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG16(SpeakerClockConverter_PERIOD_LSB_PTR));
    #endif /* (SpeakerClockConverter_UsingFixedFunction) */
}


#if (!SpeakerClockConverter_UsingFixedFunction)
/*******************************************************************************
* Function Name: SpeakerClockConverter_WriteCompare
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
void SpeakerClockConverter_WriteCompare(uint16 compare) \
                                   
{
    #if(SpeakerClockConverter_UsingFixedFunction)
        CY_SET_REG16(SpeakerClockConverter_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG16(SpeakerClockConverter_COMPARE_LSB_PTR, compare);
    #endif /* (SpeakerClockConverter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: SpeakerClockConverter_ReadCompare
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
uint16 SpeakerClockConverter_ReadCompare(void) 
{
    return (CY_GET_REG16(SpeakerClockConverter_COMPARE_LSB_PTR));
}


#if (SpeakerClockConverter_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: SpeakerClockConverter_SetCompareMode
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
void SpeakerClockConverter_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    SpeakerClockConverter_CONTROL &= ((uint8)(~SpeakerClockConverter_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    SpeakerClockConverter_CONTROL |= compareMode;
}
#endif  /* (SpeakerClockConverter_COMPARE_MODE_SOFTWARE) */


#if (SpeakerClockConverter_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: SpeakerClockConverter_SetCaptureMode
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
void SpeakerClockConverter_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    SpeakerClockConverter_CONTROL &= ((uint8)(~SpeakerClockConverter_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    SpeakerClockConverter_CONTROL |= ((uint8)((uint8)captureMode << SpeakerClockConverter_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (SpeakerClockConverter_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: SpeakerClockConverter_ClearFIFO
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
void SpeakerClockConverter_ClearFIFO(void) 
{

    while(0u != (SpeakerClockConverter_ReadStatusRegister() & SpeakerClockConverter_STATUS_FIFONEMP))
    {
        (void)SpeakerClockConverter_ReadCapture();
    }

}
#endif  /* (!SpeakerClockConverter_UsingFixedFunction) */


/* [] END OF FILE */

