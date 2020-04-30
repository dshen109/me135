/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <queue.c>


uint8 ByteCount = 0;
uint8 firstbyte = 0;
uint8 CommandReady = 0;
uint8 Count = 0;
uint8 Status;

uint8 ReceivedBuffer[66];
uint8 TransmitBuffer[1];
int16 LedCommand = 10000;

// Tempo in beats per minute.
uint8 tempo = 120;

// Track if we have notes in the queue.
uint noteReady = 0;

// Clock rate of tempo timer.
// TODO: Figure out if this can be read from TopDesign.
uint8 clockRate = 100;

struct command_protocol
{
    uint8 packet_size;
    uint8 command;
    uint8 buffer[64];
} Command_Packet;

struct command_protocol Transmit_Packet;

struct Queue* noteQueue;

/** Calculate the number of clock cycles between beats given a CLOCKRATE and 
 *  BPM */
int tempotoseconds(int clockrate, int bpm) {
    return clockrate * 60 / bpm;    
}

/** Interrupt when we get a byte over serial. */
CY_ISR(ByteReceived) { 
    LEDDrive_Write(1);
    // Get the next Byte in the buffer and store in ReceivedBuffer,
    // then increment ByteCount by 1
    ReceivedBuffer[ByteCount++] = (uint8) (LabVIEW_UART_GetByte()&0x00ff);
    // We're reading the first byte in the buffer since the firstbyte flag was
    // set
    if(firstbyte == 0)
    {
        // Set the counter to interrupt after all bytes have been read 
        // from the buffer.
        ByteCounter_WriteCompare(ReceivedBuffer[0]);
        // Unset firstbyte flag since we're reading subsequent bytes
        firstbyte = 1;
   
    }
    // Get the index of this byte. This line doesn't seem to do anything.
    // ByteCounter_ReadCounter();
    // Clear the interrupt by reading the status register.
    Status = ByteCounter_ReadStatusRegister();
    LEDDrive_Write(0);
    
}

/** Runs once all bytes have been read from the counter and is triggered by
 *  the timer. */
CY_ISR(CommandReceived) {
    uint8 i;
    
    if(CommandReady == 0)
    {
        Command_Packet.packet_size = ReceivedBuffer[0];
        // 0 if tempo payload and 1 if note payload
        Command_Packet.command = ReceivedBuffer[1];
    
        for(i=0;i<(Command_Packet.packet_size - 2);i++)
        {
            Command_Packet.buffer[i] =  ReceivedBuffer[i+2];
        }
        
        firstbyte = 0;
        CommandReady = 1;
        ByteCount = 0;
    }
    ByteCounter_ReadStatusRegister();
    LEDDrive_Write(0);
}

/** Interrupt once the timer loops and we need to write a note. */
CY_ISR(NoteInterrupt) {
    // If no notes to read, exit.
    if (~noteReady) {
        NoteTimer_ReadStatusRegister();
        return;
    }
    // Write opposite of whatever LED is at
    LEDDrive_Write(!LEDDrive_Read());
    // Do some note writing here.
    TransmitBuffer[0] = dequeue(noteQueue);
    LabVIEW_UART_PutArray(TransmitBuffer, 1);
    if (isEmpty(noteQueue)) {
        noteReady = 0;
    }
    
    // Clear the interrupt by reading the status register
    NoteTimer_ReadStatusRegister();
}

int main(void) {
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    /* Write 0 to the reset attr. */
    UARTReset_Write(0);
    ByteCountReset_Write(0);
    
    LabVIEW_UART_Start();
    
    ByteCounter_Start();
    ByteCounter_Enable();
    ByteCounter_SetInterruptMode(ByteCounter_STATUS_CMP_INT_EN_MASK);
    
    NoteTimer_Start();
    
    NoteInterrupt_Start();
    NoteInterrupt_StartEx(NoteInterrupt);
    
    ByteReceived_Start();
    ByteReceived_StartEx(ByteReceived);
    ByteReceived_Enable();
    
    CommandReceived_Start();
    CommandReceived_StartEx(CommandReceived);
    CommandReceived_Enable();

    struct Queue* noteQueue = createQueue(100);
    
    // Queue that store the notes to be played.
    for(;;)
    {
        switch (CommandReady) {
            // Tempo payload:
            case 1:
                // Reset the tempo by changing the period of the NoteTimer
                tempo = Command_Packet.buffer[0];
                NoteTimer_WritePeriod(tempotoseconds(clockRate, tempo));
                CommandReady = 0;
                break;
            // Note Payload
            case 2:
                // Enqueue all the notes into the buffer.
                for (int i = 0; i < Command_Packet.packet_size; i++) {
                    enqueue(noteQueue, Command_Packet.buffer[i]);
                }
                noteReady = 1;
                CommandReady = 0;
                break;
            default:
                break;
        }
    }
}

/* [] END OF FILE */
