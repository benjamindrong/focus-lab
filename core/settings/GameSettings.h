//
// Created by Benjamin Drong on 5/13/26.
//

#ifndef FOCUSLAB_GAMESETTINGS_H
#define FOCUSLAB_GAMESETTINGS_H

#endif //FOCUSLAB_GAMESETTINGS_H

#pragma once

struct GameSettings {

    // Reaction game
    float reactionMinDelay = 0.5f;
    float reactionMaxDelay = 2.0f;
    int reactionRounds = 5;

    // Memory game
    int memoryRounds = 5;
    float memoryShowDuration = 1.5f;
    int memoryMinSequence = 3;
    int memoryMaxSequence = 5;
};

