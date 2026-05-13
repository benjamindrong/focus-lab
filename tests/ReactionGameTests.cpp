//
// Created by Benjamin Drong on 5/13/26.
//
#include <catch2/catch_test_macros.hpp>

#include "../core/games/ReactionGame.h"

TEST_CASE(
    "ReactionGame starts unfinished",
    "[reaction]"
) {

    ReactionGame game;

    game.start();

    REQUIRE_FALSE(
        game.isFinished()
    );
}

TEST_CASE(
    "False presses increment",
    "[reaction]"
) {

    ReactionGame game;

    game.start();

    game.handleInput(true);
    game.handleInput(true);

    REQUIRE(
        game.getMetrics().falsePresses == 2
    );
}

TEST_CASE(
    "Target eventually appears",
    "[reaction]"
) {

    ReactionGame game;

    game.start();

    float elapsed = 0.f;

    while (
        !game.isTargetVisible() &&
        elapsed < 10.f
    ) {

        game.update(0.1f);

        elapsed += 0.1f;
    }

    REQUIRE(
        game.isTargetVisible()
    );
}

TEST_CASE(
    "Successful reaction records time",
    "[reaction]"
) {

    ReactionGame game;

    game.start();

    while (!game.isTargetVisible()) {
        game.update(0.1f);
    }

    game.update(0.2f);

    game.handleInput(true);

    REQUIRE_FALSE(
        game.getMetrics()
            .reactionTimes.empty()
    );
}

TEST_CASE(
    "Game finishes after max rounds",
    "[reaction]"
) {

    ReactionGame game;

    game.start();

    for (int i = 0; i < 5; i++) {

        while (!game.isTargetVisible()) {
            game.update(0.1f);
        }

        game.handleInput(true);
    }

    REQUIRE(
        game.isFinished()
    );
}