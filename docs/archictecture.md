# Architecture

## Overview

Focus Lab separates gameplay logic from rendering.

The project is divided into:

- core/
    - platform-independent gameplay logic
- cpp-app/
    - SFML rendering and input
- tests/
    - unit tests for gameplay systems

---

## Core Principles

### GameSession owns flow

GameSession controls:
- active game
- screen transitions
- session lifecycle
- metrics aggregation

Rendering code should never decide gameplay flow.

---

## Game Structure

Each mini-game implements:

IGame

Current games:
- ReactionGame
- MemoryGame

Games should:
- contain rules
- process gameplay state
- update metrics

Games should NOT:
- render graphics
- know about SFML
- access platform APIs

---

## Metrics

Metrics are stored separately from gameplay.

Metrics contain:
- reaction times
- false presses
- memory accuracy
- session timing

This separation allows:
- analytics
- exports
- scoring systems
- future ML/statistical analysis