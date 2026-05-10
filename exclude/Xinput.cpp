/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "Xinput.h"

PoseControlEvent(){
    DWORD dwResult;    
    DWORD stateChangeDetect;    

    XINPUT_KEYSTROKE keys;
    _XINPUT_VIBRATION vibrate;
    
    int lastLThumbX = 0; 
    int lastLThumbY = 0; 

    int LThumbXOffset = 0; 
    int LThumbYOffset = 0; 
    for (DWORD i=0; i< XUSER_MAX_COUNT; i++ )
    {
        XINPUT_STATE state;
        ZeroMemory( &state, sizeof(XINPUT_STATE) );

        // Simply get the state of the controller from XInput.
        dwResult = XInputGetState( i, &state );
        
        if( dwResult == ERROR_SUCCESS )
        {
            // Controller is connected
            printf("Control is connected\n");
            // We don't have gamepad here, so can not test it out
            stateChangeDetect != state.dwPacketNumber;

// IF Ljoystick move, move the vampire                        

            LThumbXOffset = state.Gamepad.sThumbLX - lastLThumbX;
            LThumbYOffset = state.Gamepad.sThumbLY - lastLThumbY;

            dancing_vampire_core = glm::translate(dancing_vampire_core, glm::vec3((float)LThumbXOffset * DelayedRatio, 0.0f, (float)LThumbYOffset * DelayedRatio));

            lastLThumbX = state.Gamepad.sThumbLX;
            lastLThumbY = state.Gamepad.sThumbLY;

// IF button hit, motor vibrate

            if(XInputGetKeystroke(i, 0, &keys) == ERROR_SUCCESS){
                // switch this flags here for controlling the characters;
                if(keys.Flags == XINPUT_KEYSTROKE_KEYDOWN){
                    vibrate.wLeftMotorSpeed = 1000; 
                    vibrate.wRightMotorSpeed = 1000; 
                }
            };

            XInputSetState(i, &vibrate);
        }
        else
        {
            // Controller is not connected
            printf("There is no available connected controller\n")
        }
    }

}
