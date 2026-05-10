#include <SFML/Graphics.hpp>
#include "../core/GameSession.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Focus Lab");

    GameSession game;
    float gameTime = game.getMetrics().sessionTime;

    sf::Font font;
    font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf");

    sf::Text title(font, "FOCUS LAB", 48);
    title.setPosition({250.f, 150.f});

    sf::Text start(font, "Press SPACE to Start", 24);
    start.setPosition({220.f, 300.f});

    sf::Text playing(font, "GAME RUNNING...", 32);
    playing.setPosition({250.f, 250.f});

    sf::Clock clock;

    sf::Text results(font, "", 24);
    results.setPosition({200.f, 200.f});

    sf::RectangleShape target({80.f, 80.f});
    target.setFillColor(sf::Color::Red);
    target.setPosition({-200.f, -200.f});

    float x = 200.f + std::sin(gameTime * 3.f) * 100.f;
    float y = 250.f + std::cos(gameTime * 2.f) * 80.f;

    target.setPosition({x, y});

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Space &&
                    game.getState().screen == GameState::Screen::Menu) {
                    game.startGame();
                } else
                    game.handleInput(
                        key->code == sf::Keyboard::Key::Space
                    );
            }
        }

        game.update(dt);

        window.clear();

        if (game.getState().screen == GameState::Screen::Menu) {
            window.draw(title);
            window.draw(start);
        } else if (game.getState().screen == GameState::Screen::Playing) {
            window.draw(playing);

            auto *rg = game.getReactionGame();
            if (rg && rg->isTargetVisible()) {
                target.setPosition({300.f, 250.f});
                window.draw(target);
            }
        } else if (game.getState().screen == GameState::Screen::Results) {
            const auto &m = game.getMetrics();

            float average = 0.f;

            for (float t: m.reactionTimes) {
                average += t;
            }

            if (!m.reactionTimes.empty()) {
                average /= m.reactionTimes.size();
            }

            std::string text =
                    "Results:\n"
                    "Rounds: " + std::to_string(m.reactionTimes.size()) + "\n"
                    "Average Reaction: " + std::to_string(average) + "\n"
                    "False Presses: " + std::to_string(m.falsePresses) + "\n"
                    "Session Time: " + std::to_string(m.sessionTime);

            results.setString(text);
            window.draw(results);
        }

        window.display();
    }

    return 0;
}
