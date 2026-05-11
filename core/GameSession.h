//
// Created by Benjamin Drong on 5/10/26.
//

#pragma once

#include "GameState.h"

#include "games/ReactionGame.h"
#include "games/MemoryGame.h"

#include "metrics/Metrics.h"

#include <memory>

class GameSession {
public:

    void startGame() {

        reactionGame = std::make_unique<ReactionGame>();

        reactionGame->start();

        state.activeGame = GameState::ActiveGame::Reaction;

        state.screen = GameState::Screen::Playing;
    }

    void update(float dt) {

        if (state.screen !=
            GameState::Screen::Playing)
            return;

        // REACTION GAME
        if (state.activeGame ==
            GameState::ActiveGame::Reaction &&
            reactionGame) {

            reactionGame->update(dt);

            state.showTarget =
                reactionGame
                    ->isTargetVisible();

            if (reactionGame
                    ->isFinished()) {

                metrics =
                    reactionGame
                        ->getMetrics();

                memoryGame =
                    std::make_unique<
                        MemoryGame>();

                memoryGame->start();

                state.activeGame = GameState::ActiveGame::Memory;

                state.showTarget = false;
            }
        }

        // MEMORY GAME
        else if (state.activeGame ==
                 GameState::ActiveGame::Memory &&
                 memoryGame) {

            memoryGame->update(dt);

            if (memoryGame
                    ->isFinished()) {

                const auto& mm =
                    memoryGame
                        ->getMetrics();

                metrics.memoryCorrect =
                    mm.memoryCorrect;

                metrics.memoryIncorrect =
                    mm.memoryIncorrect;

                state.screen =
                    GameState::Screen::Results;
            }
        }
    }

    void handleSpacePressed() {

        switch (state.screen) {

            case GameState::Screen::Menu:
                startGame();
                break;

            case GameState::Screen::Playing:

                if (state.activeGame ==
                    GameState::ActiveGame::Reaction &&
                    reactionGame) {

                    reactionGame
                        ->handleInput(true);
                }

                break;

            case GameState::Screen::Results:
                resetToMenu();
                break;
        }
    }

    void handleNumberPressed(
        int number
    ) {

        if (state.activeGame ==
            GameState::ActiveGame::Memory &&
            memoryGame) {

            memoryGame
                ->handleNumberInput(
                    number
                );
        }
    }

    const GameState& getState() const {
        return state;
    }

    const Metrics& getMetrics() const {
        return metrics;
    }

private:

    void resetToMenu() {

        reactionGame.reset();
        memoryGame.reset();

        metrics = Metrics{};

        state = GameState{};
    }

private:

    GameState state;

    Metrics metrics;

    std::unique_ptr<ReactionGame>
        reactionGame;

    std::unique_ptr<MemoryGame>
        memoryGame;
};