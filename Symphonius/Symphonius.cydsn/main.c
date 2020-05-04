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
#include "queue.h"

uint8 ByteCount = 0;
uint8 firstbyte = 0;
// Flag that's set once we process an incoming message with a command.
uint8 CommandReady = 0;

uint8 ReceivedBuffer[66];
uint8 TransmitBuffer[3];

// Tempo in beats per minute.
uint8 tempo = 60;

// Tracks if currentNote is ready to be sent to be played.
uint8 noteReady = 0;
// Frequency to be played.
uint16 currentNote = 0;

// Clock rate of tempo timer [Hz].
// TODO: Figure out if this can be read from TopDesign?
uint16 clockRate = 100;

#define NUMNOTES 9
// Sequential number for notes, starting from E4 and ending on F5
static uint8 noteNums[NUMNOTES] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
// Sequential frequencies [Hz] matching the array above.
static uint16 noteFreqs[NUMNOTES] = {330, 349, 392, 440, 494, 523, 587, 659, 698};


// Help convert uint16 note frequency to Byte array.
union uint16ToByte {
    uint8 data[2];
    uint16 number;
} ConvertToByte;

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
uint16 tempoToCycles(uint16 clockrate, uint8 bpm) {
    clockrate = (float) clockrate;
    bpm = (float) bpm;
    return clockrate * 60.0 / bpm;    
}

/** Interrupt when we get a byte over serial. */
CY_ISR(ByteReceived) { 
    // LEDDrive_Write(1);
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
    ByteCounter_ReadStatusRegister();
    // LEDDrive_Write(0);
    
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
    // LEDDrive_Write(0);
}

/** Set the LED to turn on for *time* milliseconds 
 *  at *pwmMag* intensity*/
void setLED(int pwmMag, int time) {
    LEDDuration_WritePeriod(time);
    LEDPWM_WriteCompare1(pwmMag);
    LEDDuration_WriteCounter(0);
}

CY_ISR(LEDOffInterrupt) {
    LEDPWM_WriteCompare1(0);
    LEDDuration_ReadStatusRegister();
}

/** Interrupt once the timer loops and we need to write a note. */
CY_ISR(NoteInterrupt) {
    // If no notes to send, exit.
    if (!noteReady) {
        // Tun the LED on low for 0.2 seconds.
        setLED(10, 100);
        NoteTimer_ReadStatusRegister();
        return;
    }
    // Do some note writing here.
    ConvertToByte.number = currentNote;
    // 1 for length and 2 for the note.    
    TransmitBuffer[0] = 3;
    TransmitBuffer[1] = ConvertToByte.data[0];
    TransmitBuffer[2] = ConvertToByte.data[1];
    LabVIEW_UART_PutArray(TransmitBuffer, 2);
    
    // Turn the LED on for 0.5 second.
    setLED(255, 500);
    // Note has been consumed.
    noteReady = 0;
    NoteTimer_ReadStatusRegister();
}
/** Convert a note number to a frequency. */
uint16 noteNumToFreq(int number) {
    for (int i = 0; i < NUMNOTES; i++) {
        if (noteNums[i] == number) {
            return noteFreqs[i];
        }
    }
    return 0;
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
    // Set the initial tempo.
    NoteTimer_WritePeriod(tempoToCycles(clockRate, tempo));
    
    ByteReceived_Start();
    ByteReceived_StartEx(ByteReceived);
    ByteReceived_Enable();
    
    CommandReceived_Start();
    CommandReceived_StartEx(CommandReceived);
    CommandReceived_Enable();
    
    LEDDuration_Start();
    
    LEDOffInterrupt_Start();
    LEDOffInterrupt_StartEx(LEDOffInterrupt);
    LEDPWM_Start();
    LEDPWM_WriteCompare1(0);
    
    // Queue that store the notes to be played.
    struct Queue* noteQueue = createQueue((uint8) 100);
    
    // Enqueue some notes as a test.
    enqueue(noteQueue, 1);
    enqueue(noteQueue, 2);
    enqueue(noteQueue, 3);
    enqueue(noteQueue, 4);
    enqueue(noteQueue, 1);
    enqueue(noteQueue, 2);
    enqueue(noteQueue, 3);
    enqueue(noteQueue, 4);
    
    for(;;)
    {
        switch (CommandReady) {
            // Tempo payload:
            case 1:
                // Reset the tempo by changing the period of the NoteTimer
                tempo = Command_Packet.buffer[0];
                NoteTimer_WritePeriod(tempoToCycles(clockRate, tempo));
                CommandReady = 0;
                break;
            // Note Payload
            case 2:
                // Enqueue all the notes into the buffer.
                for (int i = 0; i < Command_Packet.packet_size; i++) {
                    enqueue(noteQueue, Command_Packet.buffer[i]);
                }
                CommandReady = 0;
                break;
            default:
                break;
        }
        if (!isEmpty(noteQueue) && !noteReady) {
            currentNote = noteNumToFreq(dequeue(noteQueue));
            noteReady = 1;
        }
    }
}

/* [] END OF FILE */
