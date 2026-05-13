#include <catch2/catch_test_macros.hpp>

#include "../core/games/MemoryGame.h"

static void enterInputState(MemoryGame& game) {
    game.forceStateShow();
    game.update(2.0f); // forces transition to Input
}

TEST_CASE("MemoryGame starts unfinished", "[memory]") {

    MemoryGame game;
    game.start();

    REQUIRE_FALSE(game.isFinished());
}

TEST_CASE("Full correct sequence counts once", "[memory]") {

    MemoryGame game;
    game.start();

    enterInputState(game);

    auto seq = game.getSequence();

    for (int n : seq)
        game.handleNumberInput(n);

    REQUIRE(game.getMetrics().memoryCorrectSequences == 1);
    REQUIRE(game.getMetrics().memoryIncorrectSequences == 0);
}

TEST_CASE("Wrong input counts failure once", "[memory]") {

    MemoryGame game;
    game.start();

    enterInputState(game);

    game.handleNumberInput(999);

    REQUIRE(game.getMetrics().memoryIncorrectSequences == 1);
    REQUIRE(game.getMetrics().memoryCorrectSequences == 0);
}

TEST_CASE("Partial sequence failure ends round once", "[memory]") {

    MemoryGame game;
    game.start();

    enterInputState(game);

    auto seq = game.getSequence();

    game.handleNumberInput(seq[0]);
    game.handleNumberInput(999);

    REQUIRE(game.getMetrics().memoryIncorrectSequences == 1);
}

TEST_CASE("Does not double count a single failure", "[memory]") {

    MemoryGame game;
    game.start();

    enterInputState(game);

    game.handleNumberInput(999);
    game.handleNumberInput(999);
    game.handleNumberInput(999);

    REQUIRE(game.getMetrics().memoryIncorrectSequences == 1);
}

TEST_CASE("Completes full lifecycle", "[memory]") {

    MemoryGame game;
    game.start();

    enterInputState(game);

    for (int i = 0; i < 5; i++) {

        auto seq = game.getSequence();

        for (int n : seq)
            game.handleNumberInput(n);

        if (game.isFinished())
            break;

        enterInputState(game);
    }

    REQUIRE(game.isFinished());
}

TEST_CASE("Ignores input outside Input state", "[memory]") {

    MemoryGame game;
    game.start();

    game.forceStateShow();

    game.handleNumberInput(1);
    game.handleNumberInput(2);

    REQUIRE(game.getMetrics().memoryCorrectSequences == 0);
    REQUIRE(game.getMetrics().memoryIncorrectSequences == 0);
}