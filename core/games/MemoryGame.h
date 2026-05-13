#pragma once

#include "../settings/GameSettings.h"
#include "../metrics/Metrics.h"
#include <vector>

class MemoryGame {
public:

    enum class State {
        ShowSequence,
        Input
    };

    State getState() const { return state; }

    explicit MemoryGame(const GameSettings& settings)
        : config(settings) {
    }

    void start() {

        metrics = Metrics{};
        finished = false;

        playerIndex = 0;
        roundEnded = false;

        currentRound = 0;
        maxRounds = config.memoryRounds;

        generateSequence();
        state = State::ShowSequence;
        timer = 0.f;
    }

    void update(float dt) {

        if (finished)
            return;

        timer += dt;

        if (state == State::ShowSequence &&
            timer >= config.memoryShowDuration) {

            state = State::Input;
            timer = 0.f;
        }
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
                metrics.memoryResponseTimes.push_back(timer - inputStartTime);

                endRound();
            }
        }
        else {
            metrics.memoryIncorrectSequences++;
            endRound();
        }
    }

    bool isFinished() const {
        return finished;
    }

    const Metrics& getMetrics() const {
        return metrics;
    }

    const std::vector<int>& getSequence() const {
        return sequence;
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
        currentRound++;

        if (currentRound >= maxRounds) {
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

        int length =
            config.memoryMinSequence +
            (rand() % (config.memoryMaxSequence - config.memoryMinSequence + 1));

        for (int i = 0; i < length; i++) {
            sequence.push_back(1 + rand() % 9);
        }
    }

private:

    GameSettings config;

    std::vector<int> sequence;

    State state;

    Metrics metrics;

    int playerIndex = 0;
    int currentRound = 0;
    int maxRounds = 5;

    bool finished = false;
    bool roundEnded = false;

    float timer = 0.f;
    float inputStartTime = 0.f;
};