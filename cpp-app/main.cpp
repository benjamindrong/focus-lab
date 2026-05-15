#include <SFML/Graphics.hpp>
#include "../core/GameSession.h"

#include <cmath>
#include <optional>
#include "../core/metrics/Metrics.h"
#include <ctime>

int main() {
    sf::RenderWindow window(
        sf::VideoMode({800, 600}),
        "Focus Lab"
    );

    srand(static_cast<unsigned>(time(nullptr)));

    GameSession game;

    sf::Font font;
    font.openFromFile(
        "/System/Library/Fonts/Supplemental/Arial.ttf"
    );

    sf::Text title(font, "FOCUS LAB", 48);
    title.setPosition({250.f, 150.f});

    sf::Text start(font, "Press SPACE to Start", 24);
    start.setPosition({220.f, 300.f});

    sf::Text playing(font, "REACTION TEST", 32);
    playing.setPosition({250.f, 40.f});

    sf::Text memoryText(
        font,
        "MEMORY TEST\nRepeat: 1 2 3",
        32
    );
    memoryText.setPosition({180.f, 200.f});

    sf::Text results(font, "", 24);
    results.setPosition({180.f, 180.f});

    sf::RectangleShape target({80.f, 80.f});
    target.setFillColor(sf::Color::Red);

    sf::Clock clock;

    sf::Text settingsText(font, "", 28);
    settingsText.setPosition({120.f, 120.f});

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Space) {
                    game.handleSpacePressed();
                }

                if (key->code == sf::Keyboard::Key::Tab) {
                    game.openSettings();
                }

                game.handleSettingsInput(key->code);

                if (key->code >= sf::Keyboard::Key::Num1 &&
                    key->code <= sf::Keyboard::Key::Num9) {
                    int number =
                            static_cast<int>(key->code) -
                            static_cast<int>(sf::Keyboard::Key::Num0);

                    game.handleNumberPressed(number);
                }
            }
        }

        game.update(dt);

        window.clear();

        // MENU
        if (game.getState().screen == GameSession::Screen::Menu) {
            window.draw(title);
            window.draw(start);
        }

        // PLAYING
        else if (
            game.getState().screen == GameSession::Screen::Playing
        ) {
            // REACTION GAME
            if (game.getState().activeGame == GameSession::ActiveGame::Reaction) {
                window.draw(playing);

                if (game.reactionGame && game.reactionGame->getPhase() ==
                   ReactionGame::Phase::StimulusVisible) {
                    target.setPosition({300.f, 250.f});
                    window.draw(target);
                }
            }
            // MEMORY GAME
            else if (game.getState().activeGame == GameSession::ActiveGame::Memory) {
                window.draw(memoryText);

                if (game.memoryGame) {
                    const auto &seq = game.memoryGame->getSequence();
                    std::string text = "MEMORY TEST\n";

                    if (game.memoryGame->getState() == MemoryGame::State::ShowSequence) {
                        // you need this getter
                        text += "Memorize:\n";
                        for (int n: seq) {
                            text += std::to_string(n) + " ";
                        }
                    } else {
                        text += "Enter the sequence...";
                    }

                    memoryText.setString(text);
                }
            }
        } else if (
            game.getState().screen ==
            GameSession::Screen::Settings
        ) {
            std::string text =
                    "SETTINGS\n\n";

            text +=
                    (game.selectedSetting == 0 ? "> " : "  ") +
                    std::string("Memory Rounds: ") +
                    std::to_string(game.tempSettings.memoryRounds) +
                    "\n";

            text +=
                    (game.selectedSetting == 1 ? "> " : "  ") +
                    std::string("Memory Show Time: ") +
                    std::to_string(game.tempSettings.memoryShowDuration) +
                    "\n";

            text +=
                    (game.selectedSetting == 2 ? "> " : "  ") +
                    std::string("Reaction Rounds: ") +
                    std::to_string(game.tempSettings.reactionRounds) +
                    "\n";

            text +=
                    (game.selectedSetting == 3 ? "> " : "  ") +
                    std::string("Reaction Min Delay: ") +
                    std::to_string(game.tempSettings.reactionMinDelay) +
                    "\n";

            text +=
                    (game.selectedSetting == 4 ? "> " : "  ") +
                    std::string("Reaction Max Delay: ") +
                    std::to_string(game.tempSettings.reactionMaxDelay) +
                    "\n\n";

            text +=
                    (game.selectedSetting == 5 ? "> " : "  ") +
                    std::string("Reaction Round Plan: ");

            for (int i = 0; i < game.tempSettings.reactionRoundPlan.size(); i++) {
                if (i == game.selectedRoundIndex)
                    text += "[";

                text +=
                (game.tempSettings.reactionRoundPlan[i] ==
                 ReactionRoundType::Standard
                     ? "S"
                     : "T");

                if (i == game.selectedRoundIndex)
                    text += "] ";

                text += " ";
            }

            text += "\n";

            text +=
                    "UP/DOWN = Select\n"
                    "LEFT/RIGHT = Adjust\n"
                    "ENTER = Apply\n"
                    "ESC = Cancel";

            settingsText.setString(text);

            window.draw(settingsText);
        }

        // RESULTS
        else if (game.getState().screen ==
                 GameSession::Screen::Results) {
            const auto &m = game.getMetrics();

            float average = 0.f;

            for (float t: m.reactionTimes) {
                average += t;
            }

            if (!m.reactionTimes.empty()) {
                average /=
                        static_cast<float>(
                            m.reactionTimes.size()
                        );
            }

            std::string text =
                    "RESULTS\n\n"
                    "Reaction Rounds: " + std::to_string(m.reactionTimes.size()) +
                    "\nAvg Reaction: " + std::to_string(average) +
                    "\nFalse Presses: " + std::to_string(m.reactionFalsePresses) +

                    "\n\nMemory Correct: " + std::to_string(m.memoryCorrectSequences) +
                    "\nMemory Incorrect: " + std::to_string(m.memoryIncorrectSequences);

            results.setString(text);

            window.draw(results);
        }

        window.display();
    }

    return 0;
}
