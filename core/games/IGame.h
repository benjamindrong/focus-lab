//
// Created by Benjamin Drong on 5/10/26.
//

#pragma once

class IGame {
public:
    virtual ~IGame() = default;

    virtual void start() = 0;

    virtual void update(float dt) = 0;

    virtual void handleInput(bool spacePressed) = 0;

    virtual bool isFinished() const = 0;
};