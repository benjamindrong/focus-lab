//
// Created by Benjamin Drong on 5/13/26.
//
#include <catch2/catch_test_macros.hpp>

#include "../core/games/MemoryGame.h"

TEST_CASE(
    "MemoryGame starts unfinished",
    "[memory]"
) {

    MemoryGame game;

    game.start();

    REQUIRE_FALSE(
        game.isFinished()
    );
}

TEST_CASE(
    "Correct sequence succeeds",
    "[memory]"
) {

    MemoryGame game;

    game.start();

    game.handleNumberInput(1);
    game.handleNumberInput(2);
    game.handleNumberInput(3);

    REQUIRE(
        game.isFinished()
    );

    REQUIRE(
        game.getMetrics().memoryCorrect == 3
    );
}

TEST_CASE(
    "Incorrect input fails game",
    "[memory]"
) {

    MemoryGame game;

    game.start();

    game.handleNumberInput(9);

    REQUIRE(
        game.isFinished()
    );

    REQUIRE(
        game.getMetrics().memoryIncorrect == 1
    );
}