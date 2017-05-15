//
//  Timer.hpp
//  VinylARt
//
//  Created by Tomas on 18/04/2017.
//
//

#ifndef Timer_hpp
#define Timer_hpp

#include "ofMain.h"
#include <stdio.h>

class Timer {
public:
    //function declarations
    void update(int type);
    bool isFinished();
    //variable declaratins
private:
    int timerStartTime;
    int timerEndTime;
    bool bTimerFinished;
};

#endif /* Timer_hpp */
