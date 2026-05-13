#pragma once

#include "../settings/GameSettings.h"
#include "../metrics/Metrics.h"

#include <cstdlib>
#include <iostream>

class ReactionGame {
public:

    float roundTimer = 0.f;
    float waitDuration = 0.f;

    explicit ReactionGame(const GameSettings& settings)
        : config(settings) {
    }

    void start() {
        metrics = Metrics{};
        finished = false;

        currentRound = 0;
        maxRounds = config.reactionRounds;

        resetRound();   // This is key
    }

    void update(float dt) {

        if (finished) return;

        roundTimer += dt;

        if (!waitingForInput && roundTimer >= waitDuration) {
            waitingForInput = true;
            reactionStartTime = roundTimer;
        }
    }

    void handleSpacePressed() {

        if (finished)
            return;

        if (!waitingForInput) {
            metrics.reactionFalsePresses++;
            return;
        }

        float reactionTime = roundTimer - reactionStartTime;
        metrics.reactionTimes.push_back(reactionTime);

        endRound();
    }

    bool isReady() const {
        return waitingForInput;
    }

    bool isFinished() const {
        return finished;
    }

    const Metrics& getMetrics() const {
        return metrics;
    }

private:

    void endRound() {

        currentRound++;

        if (currentRound >= maxRounds) {
            finished = true;
            return;
        }

        resetRound();
    }

    void resetRound() {
        waitingForInput = false;
        roundTimer = 0.f;
        waitDuration = getRandomDelay();
        reactionStartTime = 0.f;
    }

    float getRandomDelay() {
        return config.reactionMinDelay +
               (rand() / (float)RAND_MAX) *
               (config.reactionMaxDelay - config.reactionMinDelay);
    }

private:

    GameSettings config;
    Metrics metrics;

    int currentRound = 0;
    int maxRounds = 5;

    bool finished = false;
    bool waitingForInput = false;

    float timer = 0.f;
    float targetTime = 0.f;
    float reactionStartTime = 0.f;
};
