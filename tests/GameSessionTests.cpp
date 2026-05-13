//
// Created by Benjamin Drong on 5/13/26.
//
#include <catch2/catch_test_macros.hpp>

#include "../core/GameSession.h"

TEST_CASE(
    "Session starts at menu",
    "[session]"
) {

    GameSession session;

    REQUIRE(
        session.getState().screen ==
        GameState::Screen::Menu
    );
}

TEST_CASE(
    "Space starts game",
    "[session]"
) {

    GameSession session;

    session.handleSpacePressed();

    REQUIRE(
        session.getState().screen ==
        GameState::Screen::Playing
    );
}

TEST_CASE(
    "Session resets from results",
    "[session]"
) {

    GameSession session;

    session.handleSpacePressed();

    REQUIRE(
        session.getState().screen ==
        GameState::Screen::Playing
    );
}