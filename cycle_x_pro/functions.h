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
    
    
    
    
    /** Metric Functions **/
    
    //This is where we'll define the functions to
    void getTime(void);
    void udpateBatteryLevel(void);
    void getRaceData(void);
    void updateData(void);

    
    /** Packet Functions **/
    void byteWrite(uint8_t protocol);
    void btParse();
    
#ifdef __cplusplus
}
#endif //__cplusplus


#endif /* functions_h */
