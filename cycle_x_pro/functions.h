//
//  functions.h
//  cycle-x-pro
//
//  Created by Carlos Salamanca on 9/22/16.
//  Copyright © 2016 team14. All rights reserved.
//

#ifndef functions_h
#define functions_h

#include "globals.h"
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
    
    
    /** main functions **/
    void BluetoothReceive(void);
    void BluetoothSend(void);
    void BluetoothBuildMessage(uint8_t protocol);
    
    /** Metric Functions **/
    
    //This is where we'll define the functions to
    void getTime(void);
    void udpateBatteryLevel(void);
    void getRaceData(void);
    void updateData(void);
    void readHandshake(void);
    void sendHandshake(void);
    void updateData2(void);

    
    /** Packet Functions **/
    void BluetoothDeconstructMessage(void);
    
#ifdef __cplusplus
}
#endif //__cplusplus


#endif /* functions_h */
