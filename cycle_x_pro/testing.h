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
#define PIN53 53
#define PIN51 51
#define PIN49 49
#define PIN47 47

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

uint8_t getPushButton() {
    return digitalRead(PIN47);
}
#endif

#endif /* testing_h */
