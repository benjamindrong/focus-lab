//
// Created by Benjamin Drong on 5/13/26.
//

#include "SettingsPersistence.h"

#include <fstream>
#include <filesystem>
#include <iostream>

#include <nlohmann/json.hpp>
#include "../platform/PlatformPaths.h"

using json = nlohmann::json;

namespace fs = std::filesystem;

#pragma once

#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

inline fs::path getAppDataDir() {

#if defined(_WIN32)
    const char* appdata = std::getenv("APPDATA");
    return fs::path(appdata ? appdata : "") / "FocusLab";

#elif defined(__APPLE__)
    const char* home = std::getenv("HOME");
    return fs::path(home ? home : "") / "Library/Application Support/FocusLab";

#else
    const char* home = std::getenv("HOME");
    return fs::path(home ? home : "") / ".focuslab";
#endif
}

static int encodeRoundType(ReactionRoundType t) {
    return static_cast<int>(t);
}

static ReactionRoundType decodeRoundType(int v) {
    return static_cast<ReactionRoundType>(v);
}

inline fs::path getSettingsPath() {
    fs::path dir = getAppDataDir();
    fs::create_directories(dir);
    return dir / "data/settings.json";
}

GameSettings SettingsPersistence::load() {
    GameSettings settings;

    fs::path path = PlatformPaths::settingsFile();

    std::ifstream file(path);

    std::cout << "Loading from: "
              << getSettingsPath()
              << "\n";

    if (!fs::exists(path))
        return settings;

    // std::ifstream file(path);

    if (!file.is_open())
        return settings;

    json j;
    file >> j;

    settings.memoryRounds =
        j.value("memoryRounds", 5);

    settings.memoryShowDuration =
        j.value("memoryShowDuration", 1.5f);

    settings.reactionRounds =
        j.value("reactionRounds", 5);

    settings.reactionMinDelay =
        j.value("reactionMinDelay", 1.0f);

    settings.reactionMaxDelay =
        j.value("reactionMaxDelay", 3.0f);

    if (j.contains("reactionRoundPlan")) {

        settings.reactionRoundPlan.clear();

        for (auto& v : j["reactionRoundPlan"]) {
            settings.reactionRoundPlan.push_back(
                decodeRoundType(v.get<int>())
            );
        }
    }

    if (settings.reactionRoundPlan.empty()) {

        settings.reactionRoundPlan = {
            ReactionRoundType::Standard,
            ReactionRoundType::Standard,
            ReactionRoundType::Standard,
            ReactionRoundType::Standard,
            ReactionRoundType::Standard
        };
    }

    return settings;
}

void SettingsPersistence::save(
    const GameSettings& settings
) {

    fs::path path = PlatformPaths::settingsFile();

    std::ofstream file(path);

    std::cout << "Saving to: "
              << path
              << "\n";

    json j;

    j["memoryRounds"] =
        settings.memoryRounds;

    j["memoryShowDuration"] =
        settings.memoryShowDuration;

    j["reactionRounds"] =
        settings.reactionRounds;

    j["reactionMinDelay"] =
        settings.reactionMinDelay;

    j["reactionMaxDelay"] =
        settings.reactionMaxDelay;

    json plan = json::array();

    for (auto t : settings.reactionRoundPlan) {
        plan.push_back(encodeRoundType(t));
    }

    j["reactionRoundPlan"] = plan;

    std::cout << j.dump(4) << "\n";

    file << j.dump(4);
}