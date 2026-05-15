#include <catch2/catch_test_macros.hpp>
#include "../core/games/ReactionGame.h"
#include "../core/settings/GameSettings.h"

static GameSettings makeTestSettings() {
    GameSettings s;
    s.reactionMinDelay = 0.1f;
    s.reactionMaxDelay = 0.2f;
    s.reactionRounds = 5;
    return s;
}


TEST_CASE("ReactionGame starts unfinished", "[reaction]") {
    ReactionGame game(makeTestSettings(), 12345);
    game.start();

    REQUIRE_FALSE(game.isFinished());
}

TEST_CASE("False presses increment", "[reaction]") {
    ReactionGame game(makeTestSettings(), 12345);
    game.start();

    game.handleSpacePressed(); // before ready = false press
    game.handleSpacePressed();

    REQUIRE(game.getMetrics().reactionFalsePresses == 2);
}

TEST_CASE("Target eventually appears", "[reaction]") {
    ReactionGame game(makeTestSettings(), 12345);
    game.start();

    game.forceAdvanceToStimulus();

    REQUIRE(
        game.getPhase() ==
        ReactionGame::Phase::StimulusVisible
    );
}

TEST_CASE("Successful reaction records time", "[reaction]") {
    ReactionGame game(makeTestSettings(), 12345);
    game.start();

    game.forceAdvanceToStimulus();

    REQUIRE(
        game.getPhase() ==
        ReactionGame::Phase::StimulusVisible
    );

    game.update(0.2f);
    game.handleSpacePressed();

    REQUIRE_FALSE(game.getMetrics().reactionTimes.empty());
}

TEST_CASE("Game finishes after max rounds", "[reaction]") {
    ReactionGame game(makeTestSettings(), 12345);
    game.start();

    for (int i = 0; i < 5; i++) {
        game.forceAdvanceToStimulus();

        REQUIRE(
            game.getPhase() ==
            ReactionGame::Phase::StimulusVisible
        );

        game.handleSpacePressed();
    }

    REQUIRE(game.isFinished());
}

TEST_CASE(
    "ReactionGame uses configured round plan",
    "[reaction]"
) {
    GameSettings settings;

    settings.reactionRounds = 3;

    settings.reactionRoundPlan = {
        ReactionRoundType::Standard,
        ReactionRoundType::TargetStimulus,
        ReactionRoundType::Standard
    };

    ReactionGame game(settings);

    game.start();

    REQUIRE(
        game.getCurrentRoundType() ==
        ReactionRoundType::Standard
    );

    game.forceAdvanceToStimulus();

    REQUIRE(
        game.getPhase() ==
        ReactionGame::Phase::StimulusVisible
    );

    game.handleSpacePressed();

    REQUIRE(
        game.getCurrentRoundType() ==
        ReactionRoundType::TargetStimulus
    );
}

TEST_CASE(
    "ReactionGame falls back to standard round type",
    "[reaction]"
) {
    GameSettings settings;

    settings.reactionRounds = 3;

    settings.reactionRoundPlan = {
        ReactionRoundType::TargetStimulus
    };

    ReactionGame game(settings);

    game.start();

    game.forceAdvanceToStimulus();

    REQUIRE(
        game.getPhase() ==
        ReactionGame::Phase::StimulusVisible
    );

    game.handleSpacePressed();

    REQUIRE(
        game.getCurrentRoundType() ==
        ReactionRoundType::Standard
    );
}
