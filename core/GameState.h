//
// Created by Benjamin Drong on 5/10/26.
//

#pragma once

struct GameState {

    enum class Screen {
        Menu,
        Playing,
        Results
    };

    enum class ActiveGame {
        None,
        Reaction,
        Memory
    };

    Screen screen = Screen::Menu;

    ActiveGame activeGame = ActiveGame::None;

    bool showTarget = false;
};