//
// Created by Benjamin Drong on 5/10/26.
//

#pragma once
#include <vector>

struct Metrics {
    std::vector<float> reactionTimes;

    int missedInputs = 0;
    int falsePresses = 0;

    float sessionTime = 0.f;
};
