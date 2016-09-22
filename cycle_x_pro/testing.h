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
//Test Parameter - Comment out to ignore test cases
#define TEST_CODE

#ifdef TEST_CODE
#define PIN3 3
#define PIN4 4
#define PIN5 5

void feedbackLED() {
    switch (g_byMode) {
        case MODE_IDLE:
            digitalWrite(PIN3, HIGH);
            digitalWrite(PIN4, LOW);
            digitalWrite(PIN5, LOW);
            break;
        case MODE_ERPS:
            digitalWrite(PIN3, LOW);
            digitalWrite(PIN4, HIGH);
            digitalWrite(PIN5, LOW);
            break;
        case MODE_SOLO:
            digitalWrite(PIN3, LOW);
            digitalWrite(PIN4, LOW);
            digitalWrite(PIN5, HIGH);
            break;
        default:
            digitalWrite(PIN3, LOW);
            digitalWrite(PIN4, LOW);
            digitalWrite(PIN5, LOW);
            
    }
}
#endif

#endif /* testing_h */
