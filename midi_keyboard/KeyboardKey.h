#ifndef KEYBOARDKEY_H
#define KEYBOARDKEY_H

#include "pico/time.h"


class KeyboardKey
{
    public:
        enum KeyCommand
        {
            HALF_RELEASED,
            HALF_PRESSED,
            FULL_RELEASED,
            FULL_PRESSED
        };


        enum KeyState
        {
            IDLE,
            HALF_PRESSING,
            HALF_RELEASING,
            FULL
        };

        enum KeyType
        {
            WHITE,
            BLACK
        };

    public:
        KeyboardKey(unsigned int aKeyIndex);

        void HandleCommand(KeyCommand aCommand, absolute_time_t aTimeStamp);

        absolute_time_t Get_LastStateChangeTime(void) { return mLastStateChangeTime; }

        KeyState Get_State(void) { return mState; }

    private:
        void SwitchStep(KeyState aNewState, absolute_time_t aTimeStamp);

        absolute_time_t mHalfStateTime;
        unsigned int mKeyIndex;
        uint8_t mKeyPitch;
        KeyState mState;
        KeyType mKeyType;
        absolute_time_t mLastStateChangeTime;
};

#endif