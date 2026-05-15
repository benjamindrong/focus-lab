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

    ReactionRoundType currentRoundType =
            ReactionRoundType::Standard;

    ReactionRoundType getCurrentRoundType() const {
        return currentRoundType;
    }

    bool stimulusVisible = false;
    bool currentStimulusIsTarget = false;

    float stimulusVisibleTimer = 0.f;
    float stimulusVisibleDuration = 1.0f;

    std::mt19937 rng;

    enum class Phase {
        Waiting,
        StimulusVisible,
        Finished
    };

    Phase getPhase() const {
        if (finished) return Phase::Finished;
        if (stimulusVisible) return Phase::StimulusVisible;
        return Phase::Waiting;
    }

    int getCurrentRoundIndex() const {
        return currentRound;
    }

    explicit ReactionGame(
        const GameSettings &settings,
        std::mt19937::result_type seed =
                std::random_device{}()
    )
        : config(settings), rng(seed) {
    }

#ifdef TESTING
    void forceAdvanceToStimulus() {
        roundTimer = waitDuration;
        update(0.f);
    }
#endif

    void start() {
        metrics = Metrics{};
        finished = false;

        currentRound = 0;
        maxRounds = config.reactionRounds;

        generateRoundDelays();

        resetRound();
    }

    const std::vector<float> &getRoundDelays() const {
        return roundDelays;
    }

    void update(float dt) {
        if (finished)
            return;

        roundTimer += dt;

        if (!stimulusVisible &&
            roundTimer >= waitDuration) {
            stimulusVisible = true;

            reactionStartTime = roundTimer;

            currentStimulusIsTarget =
            (
                currentRoundType ==
                ReactionRoundType::TargetStimulus
            );
        }

        if (stimulusVisible) {
            stimulusVisibleTimer += dt;

            if (
                stimulusVisibleTimer >=
                stimulusVisibleDuration
            ) {
                if (currentStimulusIsTarget) {
                    metrics.reactionMissedTargets++;
                }

                endRound();
            }
        }
    }

    void handleSpacePressed() {
        if (finished)
            return;

        if (!canAcceptInput()) {
            metrics.reactionFalsePresses++;
            return;
        }

        if (
            currentRoundType ==
            ReactionRoundType::TargetStimulus &&
            !currentStimulusIsTarget
        ) {
            metrics.reactionFalsePresses++;
            return;
        }

        float reactionTime =
                roundTimer - reactionStartTime;

        metrics.reactionTimes.push_back(
            reactionTime
        );

        endRound();
    }

    bool isFinished() const {
        return finished;
    }

    const Metrics &getMetrics() const {
        return metrics;
    }

    bool isStimulusVisible() const {
        return stimulusVisible;
    }

    bool isCurrentStimulusTarget() const {
        return currentStimulusIsTarget;
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
        stimulusVisible = false;

        stimulusVisibleTimer = 0.f;

        roundTimer = 0.f;

        waitDuration = roundDelays[currentRound];

        if (
            currentRound <
            config.reactionRoundPlan.size()
        ) {
            currentRoundType =
                    config.reactionRoundPlan[currentRound];
        } else {
            currentRoundType =
                    ReactionRoundType::Standard;
        }

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

        std::shuffle(
            roundDelays.begin(),
            roundDelays.end(),
            rng
        );
    }

private:
    GameSettings config;
    Metrics metrics;

    int currentRound = 0;
    int maxRounds = 5;

    bool finished = false;

    bool canAcceptInput() const {
        return stimulusVisible;
    }

    float timer = 0.f;
    float targetTime = 0.f;
    float reactionStartTime = 0.f;
};
