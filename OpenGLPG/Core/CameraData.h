#pragma once

#include "MovingFrame.h"
#include "Transform.h"

struct CameraData
{
    Transform myCameraTransform;
    float myFOV {0.f};
    float myAspectRatio {0.f};
    float myNear {0.f};
    float myFar {0.f};
    MovingFrame myWorldFrame;
};
