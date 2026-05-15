//
// Created by Benjamin Drong on 5/10/26.
//

#pragma once

#include <memory>
#include "settings/GameSettings.h"
#include "games/MemoryGame.h"
#include "games/ReactionGame.h"
#include "metrics/Metrics.h"
#include "settings/SettingsPersistence.h"
#include <SFML/Window/Keyboard.hpp>

class GameSession {
public:
    int selectedSetting = 0;
    int selectedRoundIndex = 0;
    bool editingRoundPlan = false;

    void nextSetting() {
        selectedSetting =
                (selectedSetting + 1) % getSettingCount();
    }

    void previousSetting() {
        selectedSetting =
                (selectedSetting - 1 + getSettingCount()) %
                getSettingCount();
    }

    int getReactionRoundCount() const {
        return tempSettings.reactionRoundPlan.size();
    }

    int getSettingCount() const {
        return 6; // or better: derive properly if you later make settings dynamic
    }

    void increaseSelectedSetting() {
        switch (selectedSetting) {
            case 0:
                tempSettings.memoryRounds++;
                break;

            case 1:
                tempSettings.memoryShowDuration += 0.25f;
                break;

            case 2:
                tempSettings.reactionRounds++;
                break;

            case 3:
                tempSettings.reactionMinDelay += 0.25f;
                break;

            case 4:
                tempSettings.reactionMaxDelay += 0.25f;
                break;

            case 5: {
                auto &plan = tempSettings.reactionRoundPlan;
                if (plan.empty()) break;

                auto &type = plan[selectedRoundIndex];

                if (type == ReactionRoundType::Standard)
                    type = ReactionRoundType::TargetStimulus;
                else
                    type = ReactionRoundType::Standard;

                break;
            }
        }
    }

    void decreaseSelectedSetting() {
        switch (selectedSetting) {
            case 0:
                if (tempSettings.memoryRounds > 1)
                    tempSettings.memoryRounds--;
                break;

            case 1:
                if (tempSettings.memoryShowDuration > 0.25f)
                    tempSettings.memoryShowDuration -= 0.25f;
                break;

            case 2:
                if (tempSettings.reactionRounds > 1)
                    tempSettings.reactionRounds--;
                break;

            case 3:
                if (tempSettings.reactionMinDelay > 0.25f)
                    tempSettings.reactionMinDelay -= 0.25f;
                break;

            case 4:
                if (tempSettings.reactionMaxDelay >
                    tempSettings.reactionMinDelay)
                    tempSettings.reactionMaxDelay -= 0.25f;
                break;

            case 5: {
                selectedRoundIndex =
                        (selectedRoundIndex - 1 +
                         tempSettings.reactionRoundPlan.size())
                        % tempSettings.reactionRoundPlan.size();
                break;
            }
        }
    }

    enum class Screen {
        Menu,
        Playing,
        Results,
        Settings
    };

    enum class ActiveGame {
        None,
        Memory,
        Reaction
    };

    struct State {
        Screen screen = Screen::Menu;
        ActiveGame activeGame = ActiveGame::None;
    };

    GameSession() {
        settings = SettingsPersistence::load();
        tempSettings = settings;
    }

    // ---------------- FLOW CONTROL ----------------

    void startMemoryGame() {
        memoryGame = std::make_unique<MemoryGame>(settings);
        memoryGame->start();

        state.screen = Screen::Playing;
        state.activeGame = ActiveGame::Memory;
    }

    void startReactionGame() {
        reactionGame = std::make_unique<ReactionGame>(settings);
        reactionGame->start();

        state.screen = Screen::Playing;
        state.activeGame = ActiveGame::Reaction;
    }

    void update(float dt) {
        if (state.screen == Screen::Settings)
            return;

        if (state.activeGame == ActiveGame::Memory && memoryGame)
            memoryGame->update(dt);

        if (state.activeGame == ActiveGame::Reaction && reactionGame)
            reactionGame->update(dt);

        checkGameEnd();
    }

    // ---------------- INPUT ROUTING ----------------

    void handleSpacePressed() {
        // MENU → start game
        if (state.screen == Screen::Menu) {
            startReactionGame(); // or MemoryGame depending on selection logic
            return;
        }

        // PLAYING → route input
        if (state.activeGame == ActiveGame::Reaction && reactionGame) {
            reactionGame->handleSpacePressed();
        }
    }

    void handleNumberPressed(int n) {
        if (state.activeGame == ActiveGame::Memory && memoryGame)
            memoryGame->handleNumberInput(n);
    }

    // ---------------- STATE ACCESS ----------------

    const State &getState() const {
        return state;
    }

    const Metrics &getMetrics() const {
        static Metrics combined;

        combined = Metrics{}; // reset

        if (memoryGame) {
            const auto &m = memoryGame->getMetrics();
            combined.memoryCorrectSequences = m.memoryCorrectSequences;
            combined.memoryIncorrectSequences = m.memoryIncorrectSequences;
            combined.memoryResponseTimes = m.memoryResponseTimes;
        }

        if (reactionGame) {
            const auto &r = reactionGame->getMetrics();
            combined.reactionTimes = r.reactionTimes;
            combined.reactionFalsePresses = r.reactionFalsePresses;
        }

        return combined;
    }

    const std::vector<int> &getMemorySequence() const {
        return memoryGame->getSequence();
    }

    // ---------------- SETTINGS ----------------

    void openSettings() {
        tempSettings = settings;
        state.screen = Screen::Settings;
    }

    void applySettings() {
        std::cout << "Apply settings called\n";
        settings = tempSettings;

        SettingsPersistence::save(settings);

        state.screen = Screen::Menu;
    }

    void cancelSettings() {
        tempSettings = settings;
        state.screen = Screen::Menu;
    }

    void handleSettingsInput(sf::Keyboard::Key key) {
        if (key == sf::Keyboard::Key::Up)
            previousSetting();

        if (key == sf::Keyboard::Key::Down)
            nextSetting();

        if (key == sf::Keyboard::Key::Left)
            decreaseSelectedSetting();

        if (key == sf::Keyboard::Key::Right)
            increaseSelectedSetting();

        if (key == sf::Keyboard::Key::Enter) {
            applySettings();
        }

        if (key == sf::Keyboard::Key::Escape) {
            cancelSettings();
        }
    }

    bool isReactionTargetVisible() const {
        if (reactionGame && state.activeGame == ActiveGame::Reaction) {
            return reactionGame->getPhase() ==
                   ReactionGame::Phase::StimulusVisible;
        }
        return false;
    }

    float getReactionRoundTimer() const {
        if (reactionGame && state.activeGame == ActiveGame::Reaction) {
            return reactionGame->roundTimer;
        }
        return 0.f;
    }

    bool isReactionReady() const {
        if (reactionGame && state.activeGame == ActiveGame::Reaction) {
            return reactionGame->getPhase() ==
                   ReactionGame::Phase::StimulusVisible;
        }
        return false;
    }

    ReactionGame *getReactionGame() {
        return reactionGame.get();
    }

private:
    void checkGameEnd() {
        if (state.activeGame == ActiveGame::Memory &&
            memoryGame && memoryGame->isFinished()) {
            state.screen = Screen::Results;
        }

        if (state.activeGame == ActiveGame::Reaction &&
            reactionGame && reactionGame->isFinished()) {
            // Auto start Memory after Reaction finishes
            state.activeGame = ActiveGame::Memory;
            memoryGame = std::make_unique<MemoryGame>(settings);
            memoryGame->start();
            return;
        }
    }

public:
    GameSettings settings;
    GameSettings tempSettings;

    State state;

    std::unique_ptr<MemoryGame> memoryGame;
    std::unique_ptr<ReactionGame> reactionGame;
};
