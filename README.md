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
│   ├── metrics/
│   ├── GameSession.h
│   └── GameState.h
│
├── cpp-app/
│   └── main.cpp
│
├── tests/
│
├── docs/
│
└── CMakeLists.txt