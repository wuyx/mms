#include "RightWallFollow.h"

namespace wallFollow {

void RightWallFollow::solve(sim::MouseInterface* mouse){
    mouse->declareInterfaceType(sim::DISCRETE);
    while (true){
        rightWallFollowStep(mouse);
    }
}

void RightWallFollow::rightWallFollowStep(sim::MouseInterface* mouse){
    if (!mouse->wallRight()){
        mouse->turnRight();
    }
    while (mouse->wallFront()){
        mouse->turnLeft();
    }
    mouse->moveForward();
}

} // namespace wallFollow
