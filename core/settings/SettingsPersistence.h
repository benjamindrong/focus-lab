//
// Created by Benjamin Drong on 5/13/26.
//
#pragma once

#include "GameSettings.h"

class SettingsPersistence {
public:

    static GameSettings load();

    static void save(
        const GameSettings& settings
    );
};