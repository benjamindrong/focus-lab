//
// Created by Benjamin Drong on 5/10/26.
//

#ifndef FOCUSLAB_GAMESTATE_H
#define FOCUSLAB_GAMESTATE_H



struct GameState {
    enum class Screen {
        Menu,
        Playing,
        Results
    };

    Screen currentScreen;

    // Example:
    bool showTarget;
    int score;
};



#endif //FOCUSLAB_GAMESTATE_H
