#pragma once

#include "../settings/GameSettings.h"
#include "../metrics/Metrics.h"

#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

class ReactionGame {
public:

    float roundTimer = 0.f;
    float waitDuration = 0.f;

    std::vector<float> roundDelays;

    explicit ReactionGame(const GameSettings& settings)
        : config(settings) {
    }

    void start() {
        metrics = Metrics{};
        finished = false;

        currentRound = 0;
        maxRounds = config.reactionRounds;

        generateRoundDelays();

        resetRound();
    }

    const std::vector<float>& getRoundDelays() const {
        return roundDelays;
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
        waitDuration = roundDelays[currentRound];
        reactionStartTime = 0.f;
    }

    void generateRoundDelays() {

        roundDelays.clear();

        if (maxRounds <= 1) {
            roundDelays.push_back(config.reactionMinDelay);
            return;
        }

        float range =
            config.reactionMaxDelay -
            config.reactionMinDelay;

        for (int i = 0; i < maxRounds; i++) {

            float t =
                static_cast<float>(i) /
                static_cast<float>(maxRounds - 1);

            float delay =
                config.reactionMinDelay +
                (range * t);

            roundDelays.push_back(delay);
        }

        std::random_device rd;
        std::mt19937 g(rd());

        std::shuffle(
            roundDelays.begin(),
            roundDelays.end(),
            g
        );
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
