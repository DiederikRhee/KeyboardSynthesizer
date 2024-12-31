#include <stdio.h>
#include <cstdio>

#include "tusb.h"
#include "hardware/uart.h"


#include "KeyboardKey.h"

//#include "MidiOut.h"

static uint8_t MinVelocity = 30;
static uint8_t MaxVelocity = 127;
static uint32_t FastestKeyTimeMicros = 10000;   //MinVelocity
static uint32_t SlowestKeyTimeMicros = 40000;   //MaxVelocity
static float KeyTimeMultiplier = ((MaxVelocity - MinVelocity) / ((float)SlowestKeyTimeMicros - (float)FastestKeyTimeMicros));


KeyboardKey::KeyboardKey(unsigned int aKeyIndex)
{
    mKeyIndex = aKeyIndex;
    mKeyPitch = mKeyIndex + 36;
    mState = IDLE;
    
    int OctaveIndex = mKeyIndex % 12;
    if (OctaveIndex == 1 || OctaveIndex == 3 || OctaveIndex == 6 || OctaveIndex == 8 || OctaveIndex == 10)
    {
        mKeyType = BLACK;
    }
    else
    {
        mKeyType = WHITE;
    }
    
    mHalfStateTime = 0;
    mLastStateChangeTime = 0;
}

void KeyboardKey::HandleCommand(KeyCommand aCommand, absolute_time_t aTimeStamp)
{
    #ifdef DEBUG_USB
    if (aCommand == HALF_PRESSED)
    {
        printf("Key: %u half pressed\n", mKeyIndex);
    }
    else if (aCommand == FULL_PRESSED)
    {
        printf("Key: %u full pressed\n", mKeyIndex);
    }
    else if (aCommand == HALF_RELEASED)
    {
        printf("Key: %u half released\n", mKeyIndex);
    }
    else if (aCommand == FULL_RELEASED)
    {
        printf("Key: %u full released\n", mKeyIndex);
    }
    #endif

    switch (mState)
    {
        case IDLE:
        {
            if (aCommand == HALF_PRESSED)
            {
                SwitchStep(HALF_PRESSING, aTimeStamp);
            }
            break;
        }
        case HALF_PRESSING:
        {
            if (aCommand == HALF_RELEASED)
            {
                SwitchStep(IDLE, aTimeStamp);
            }
            else if (aCommand == FULL_PRESSED)
            {
                SwitchStep(FULL, aTimeStamp);
            }
            break;
        }
        case HALF_RELEASING:
        {
            if (aCommand == HALF_RELEASED)
            {
                SwitchStep(IDLE, aTimeStamp);
            }
            break;
        }
        case FULL:
        {
            if (aCommand == FULL_RELEASED)
            {
                SwitchStep(HALF_RELEASING, aTimeStamp);
            }
            break; 
        }
            
    
    
    default:
        break;
    }
}


void KeyboardKey::SwitchStep(KeyState aNewState, absolute_time_t aTimeStamp)
{
    if (aNewState == mState) { return; }

    mLastStateChangeTime = aTimeStamp;

    switch (aNewState)
    {
        case IDLE:
        {
            break;
        }

        case HALF_PRESSING:
        {
            mHalfStateTime = aTimeStamp;
            break;
        }

        case HALF_RELEASING:
        {
            #ifdef DEBUG_USB
            printf("Noteoff: key=%03d vel=%03d\n", mKeyPitch, Velocity);
            #else
            tud_midi_stream_write(0,   new uint8_t[3]{0x80, mKeyPitch, 0}, 3);
            uart_write_blocking(uart0, new uint8_t[3]{0x80, mKeyPitch, 0}, 3);
            #endif

            
            break;
        }

         case FULL:
         {
            int64_t DeltaTime = absolute_time_diff_us(mHalfStateTime, aTimeStamp);
            uint8_t Velocity = MinVelocity;
            if (DeltaTime > FastestKeyTimeMicros && DeltaTime < SlowestKeyTimeMicros)
            {
                Velocity = MaxVelocity - ((float)(DeltaTime - FastestKeyTimeMicros) * KeyTimeMultiplier);
            }
            else if (DeltaTime <= FastestKeyTimeMicros)
            {
                Velocity = MaxVelocity;
            }


            #ifdef DEBUG_USB
            printf("Noteon: key=%03d vel=%03d dt=%ld\n", mKeyPitch, Velocity, DeltaTime);
            #else
            tud_midi_stream_write(0,   new uint8_t[3]{0x90, mKeyPitch, Velocity}, 3);
            uart_write_blocking(uart0, new uint8_t[3]{0x90, mKeyPitch, Velocity}, 3);
            #endif

            break;
         }
    
        default:
            break;
    }

    mState = aNewState;

}