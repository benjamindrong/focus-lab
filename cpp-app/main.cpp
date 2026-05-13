#include <SFML/Graphics.hpp>
#include "../core/GameSession.h"

#include <cmath>
#include <optional>

int main() {
    sf::RenderWindow window(
        sf::VideoMode({800, 600}),
        "Focus Lab"
    );

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

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto *key =
                    event->getIf<sf::Event::KeyPressed>()) {
                if (key->code ==
                    sf::Keyboard::Key::Space) {
                    game.handleSpacePressed();
                }

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
        if (game.getState().screen ==
            GameState::Screen::Menu) {
            window.draw(title);
            window.draw(start);
        }

        // PLAYING
        else if (
            game.getState().screen ==
            GameState::Screen::Playing
        ) {
            // REACTION GAME
            if (
                game.getState().activeGame ==
                GameState::ActiveGame::Reaction
            ) {
                window.draw(playing);

                if (game.getState().showTarget) {
                    float gameTime =
                            game.getMetrics().sessionTime;

                    float x =
                            200.f +
                            std::sin(gameTime * 3.f) * 100.f;

                    float y =
                            250.f +
                            std::cos(gameTime * 2.f) * 80.f;

                    target.setPosition({x, y});

                    window.draw(target);
                }
            }

            // MEMORY GAME
            else if (
                game.getState().activeGame ==
                GameState::ActiveGame::Memory
            ) {
                const auto& m = game.getMetrics();
                auto seq = game.getMemorySequence();

                std::string text = "MEMORY TEST\n";

                if (game.getMemoryState() ==
                    MemoryGame::State::ShowSequence) {
                    text += "Memorize:\n";

                    for (int n: seq) {
                        text += std::to_string(n) + " ";
                    }
                } else {
                    text += "Enter sequence...";
                }

                memoryText.setString(text);
                window.draw(memoryText);
            }
        }

        // RESULTS
        else if (game.getState().screen ==
                 GameState::Screen::Results) {
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
                    "Rounds: " +
                    std::to_string(m.reactionTimes.size()) +

                    "\nAverage Reaction: " +
                    std::to_string(average) +

                    "\nFalse Presses: " +
                    std::to_string(m.falsePresses) +

                    "\nMemory Correct: " +
                    std::to_string(m.memoryCorrectSequences) +

                    "\nMemory Incorrect: " +
                    std::to_string(m.memoryIncorrectSequences);

            results.setString(text);

            window.draw(results);
        }

        window.display();
    }

    return 0;
}
