//
// Created by Benjamin Drong on 5/11/26.
//

#pragma once

#include "IGame.h"
#include "../metrics/Metrics.h"

#include <vector>

class MemoryGame : public IGame {
public:

    void start() override {

        sequence = {1, 2, 3};

        playerIndex = 0;

        finished = false;

        metrics = Metrics{};
    }

    void update(float dt) override {
    }

    void handleInput(bool spacePressed) override {
    }

    void handleNumberInput(int number) {

        if (finished)
            return;

        if (number ==
            sequence[playerIndex]) {

            playerIndex++;

            metrics.memoryCorrect++;

            if (playerIndex >=
                sequence.size()) {

                finished = true;
                }
            }
        else {

            metrics.memoryIncorrect++;

            finished = true;
        }
    }

    bool isFinished() const override {
        return finished;
    }

    const Metrics& getMetrics() const {
        return metrics;
    }

private:

    std::vector<int> sequence;

    int playerIndex = 0;

    bool finished = false;

    Metrics metrics;
};