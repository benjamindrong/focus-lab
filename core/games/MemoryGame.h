//
// Created by Benjamin Drong on 5/11/26.
//
#pragma once

#include "IGame.h"
#include "../metrics/Metrics.h"

#include <vector>
#include <cstdlib>

class MemoryGame : public IGame {
public:

    enum class State {
        ShowSequence,
        Input
    };

    void start() override {

        metrics = Metrics{};

        finished = false;
        playerIndex = 0;
        timer = 0.f;

        roundEnded = false;

        metrics.memoryRounds = 0;

        generateSequence();
        state = State::ShowSequence;
    }

    void update(float dt) override {

        if (finished)
            return;

        timer += dt;

        if (state == State::ShowSequence) {

            if (timer >= showDuration) {
                state = State::Input;
                timer = 0.f;
            }
        }
    }

    void handleInput(bool) override {
        // unused
    }

    void handleNumberInput(int number) {

        if (finished || state != State::Input || roundEnded)
            return;

        if (playerIndex == 0)
            inputStartTime = timer;

        if (number == sequence[playerIndex]) {

            playerIndex++;

            if (playerIndex == sequence.size()) {

                metrics.memoryCorrectSequences++;
                metrics.memoryResponseTimes.push_back(
                    timer - inputStartTime
                );

                endRound();
            }
        }
        else {

            metrics.memoryIncorrectSequences++;

            endRound();
        }
    }

    bool isFinished() const override {
        return finished;
    }

    const Metrics& getMetrics() const {
        return metrics;
    }

    const std::vector<int>& getSequence() const {
        return sequence;
    }

    State getState() const {
        return state;
    }

#ifdef TESTING
    void forceStateShow() {
        state = State::ShowSequence;
        timer = 0.f;
        playerIndex = 0;
    }
#endif

private:

    void endRound() {

        roundEnded = true;

        metrics.memoryRounds++;

        if (metrics.memoryRounds >= maxRounds) {
            finished = true;
            return;
        }

        nextRound();
    }

    void nextRound() {

        sequence.clear();
        generateSequence();

        playerIndex = 0;
        timer = 0.f;
        roundEnded = false;

        state = State::ShowSequence;
    }

    void generateSequence() {

        int length = 3 + (rand() % 3);

        for (int i = 0; i < length; i++) {
            sequence.push_back(1 + rand() % 9);
        }
    }

private:

    std::vector<int> sequence;

    State state;

    int playerIndex = 0;

    bool finished = false;
    bool roundEnded = false;

    float timer = 0.f;
    float showDuration = 1.5f;

    float inputStartTime = 0.f;

    Metrics metrics;

    int maxRounds = 5;
};