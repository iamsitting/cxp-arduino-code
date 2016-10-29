//
//  testing.h
//  cycle-x-pro
//
//  Created by Carlos Salamanca on 9/22/16.
//  Copyright © 2016 team14. All rights reserved.
//

#ifndef testing_h
#define testing_h

#include "globals.h"

#ifdef TEST_CODE

void feedbackLED() {
    switch (g_byMode) {
        case MODE_IDLE:
            digitalWrite(PIN53, HIGH);
            digitalWrite(PIN51, LOW);
            digitalWrite(PIN49, LOW);
            break;
        case MODE_ERPS:
            digitalWrite(PIN53, LOW);
            digitalWrite(PIN51, HIGH);
            digitalWrite(PIN49, LOW);
            break;
        case MODE_SOLO:
            digitalWrite(PIN53, LOW);
            digitalWrite(PIN51, LOW);
            digitalWrite(PIN49, HIGH);
            break;
        default:
            digitalWrite(PIN53, LOW);
            digitalWrite(PIN51, LOW);
            digitalWrite(PIN49, LOW);
            
    }
}

#endif

#endif /* testing_h */
