//
// Created by Benjamin Drong on 5/10/26.
//

#pragma once

#include <vector>

struct Metrics {

    std::vector<float> reactionTimes;
    std::vector<float> memoryResponseTimes;

    float sessionTime = 0.f;

    int memoryCorrect = 0;
    int memoryIncorrect = 0;
    int memoryRounds = 0;
    int memoryCorrectSequences = 0;
    int memoryIncorrectSequences = 0;

    int falsePresses = 0;
    int missedInputs = 0;
    int reactionFalsePresses = 0;
    int reactionMissedTargets = 0;

};