# Testing

## Philosophy

Core gameplay logic must be testable independently from rendering.

Tests target:
- gameplay rules
- state transitions
- metrics generation

SFML rendering is currently not unit tested.

---

## Current Coverage

### ReactionGame
- startup state
- false presses
- target visibility
- reaction recording
- max round completion

---

## Planned Coverage

- MemoryGame
- GameSession transitions
- metrics aggregation
- input routing