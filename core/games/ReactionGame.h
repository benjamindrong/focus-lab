#pragma once
#include "IGame.h"
#include "../metrics/Metrics.h"

class ReactionGame : public IGame {
public:
    void start() override {
        timer = 0.f;

        currentRound = 0;
        maxRounds = 5;

        waitingForTarget = true;
        targetShown = false;

        roundDelay = randomDelay();

        metrics = Metrics{};
    }

    void update(float dt) override {
        timer += dt;
        metrics.sessionTime += dt;

        if (finished)
            return;

        if (waitingForTarget && timer >= roundDelay) {
            waitingForTarget = false;
            targetShown = true;
            targetTime = timer;
        }
    }

    void handleInput(bool spacePressed) override {
        if (!spacePressed || finished)
            return;

        // false press
        if (!targetShown) {
            metrics.falsePresses++;
            return;
        }

        // valid reaction
        float reaction = timer - targetTime;
        metrics.reactionTimes.push_back(reaction);

        nextRound();
    }

    bool isFinished() const override {
        return finished;
    }

    bool isTargetVisible() const {
        return targetShown;
    }

    const Metrics& getMetrics() const {
        return metrics;
    }

private:
    Metrics metrics;

    bool finished = false;

    bool waitingForTarget = true;
    bool targetShown = false;

    float timer = 0.f;
    float targetTime = 0.f;

    int currentRound = 0;
    int maxRounds = 5;

    float roundDelay = 2.f;

    void nextRound() {
        currentRound++;

        if (currentRound >= maxRounds) {
            finished = true;
            return;
        }

        waitingForTarget = true;
        targetShown = false;

        timer = 0.f;
        roundDelay = randomDelay();
    }

    float randomDelay() {
        return 1.f + static_cast<float>(rand()) /
            (static_cast<float>(RAND_MAX / 3.f));
    }
};