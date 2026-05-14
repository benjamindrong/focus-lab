# Focus Lab

Focus Lab is a cognitive mini-game prototype exploring:
- reaction timing
- impulse control
- memory
- sustained attention
- task switching

The project is designed as an experimental architecture-first codebase with portable gameplay logic separated from rendering and platform concerns.

---

# Current Features

## Reaction Test
- randomized target delay
- moving visual stimulus
- false press tracking
- multi-round timing collection

## Memory Test
- number sequence recall
- success/failure tracking

## Results System
- aggregated metrics
- reaction averages
- memory accuracy reporting

## Architecture
- platform-independent core logic
- SFML rendering frontend
- centralized session flow
- unit-tested gameplay systems

---

# Project Structure

```text
FocusLab/
│
├── core/
│   ├── games/
│   │   ├── IGame.h
│   │   ├── MemoryGame.h
│   │   └── ReactionGame.h
│   │
│   ├── metrics/
│   │   └── Metrics.h
│   │
│   ├── platform/
│   │   ├── FileSystem.h
│   │   ├── PlatformPaths.h
│   │   └── PlatformPaths_mac.mm
│   │
│   ├── settings/
│   │   ├── GameSettings.h
│   │   ├── SettingsPersistence.cpp
│   │   └── SettingsPersistence.h
│   │
│   ├── GameSession.h
│   └── GameState.h
│
├── cpp-app/
│   └── main.cpp
│
├── data/
│   └── settings.json
│
├── docs/
│   ├── architecture.md
│   ├── gameplay.md
│   ├── roadmap.md
│   └── tests.md
│
├── tests/
│   ├── GameSessionTests.cpp
│   ├── MemoryGameTests.cpp
│   ├── MetricsTests.cpp
│   └── ReactionGameTests.cpp
│
├── .gitignore
├── CMakeLists.txt
└── README.md