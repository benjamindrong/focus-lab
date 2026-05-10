//
// Created by Benjamin Drong on 5/10/26.
//

#pragma once
#include "games/ReactionGame.h"
#include <memory>
#include "metrics/Metrics.h"

struct GameState {
    enum class Screen {
        Menu,
        Playing,
        Results
    };

    Screen screen = Screen::Menu;
};

class GameSession {
public:
    void startGame() {
        state.screen = GameState::Screen::Playing;
        game = std::make_unique<ReactionGame>();
        game->start();
    }

    void update(float dt) {
        if (game && !game->isFinished()) {
            game->update(dt);
        } else if (game && game->isFinished()) {
            if (auto* rg = game.get()) {
                metrics = rg->getMetrics();
            }
            state.screen = GameState::Screen::Results;
        }
    }

    void handleInput(bool spacePressed) {
        if (game) game->handleInput(spacePressed);
    }

    const GameState& getState() const {
        return state;
    }

    const Metrics& getMetrics() const {
        return metrics;
    }

    bool isTargetActive() const {
        return game && !game->isFinished();
    }

    ReactionGame* getReactionGame() {
        return game.get();
    }

private:
    GameState state;
    std::unique_ptr<ReactionGame> game;
    Metrics metrics;
};