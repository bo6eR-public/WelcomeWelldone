# WelcomeWelldone

A competitive 1v1 multiplayer shooter developed in Unreal Engine 5. The project focuses on networked gameplay, Gameplay Ability System integration, modular equipment architecture, and dynamic combat modifiers that alter player abilities during a match.

## Overview

WelcomeWelldone is a fast-paced multiplayer game where two players compete in a closed arena while collecting temporary power-ups that can either enhance their own capabilities or negatively affect their opponent.

The project was built to explore advanced Unreal Engine gameplay systems, including GAS, equipment management inspired by Lyra Starter Game, animation layering, and event-driven architecture.

## Technologies

* Unreal Engine 5
* C++
* Blueprint
* Gameplay Ability System (GAS)
* Multiplayer Networking & Replication
* Animation Layers
* Custom Plugin Development

## Core Features

### Multiplayer Gameplay

* Server-authoritative architecture
* Replicated combat mechanics
* Replicated power-up system
* Client-server synchronization
* RPC-based gameplay interactions

### Dynamic Buff & Debuff System

Power-ups spawn throughout the arena and can be collected by players during combat.

Examples include:

* Invisibility
* Enlarged head hitbox
* Temporary combat modifiers
* Gameplay-altering status effects

The system is implemented using Gameplay Effects and Gameplay Tags through GAS.

### Gameplay Ability System (GAS)

Implemented gameplay functionality using Unreal Engine's Gameplay Ability System:

* Gameplay Abilities
* Gameplay Effects
* Gameplay Tags
* Attribute management
* Buff and debuff handling
* Ability activation and execution

### Equipment System

Developed a modular equipment architecture inspired by Lyra Starter Game.

Features include:

* Equipment Component
* Weapon equipping and unequipping
* Extensible equipment framework
* Data-driven equipment definitions

### Weapons

The project includes three distinct weapon types, each with unique gameplay behavior and combat characteristics.

### Animation System

Implemented character animation architecture using Animation Layers.

Features include:

* Layered animation workflow
* Weapon-specific animation logic
* Reusable animation architecture
* Separation of gameplay and animation systems

### Message Bus System Plugin

Created a custom Message Bus System plugin to enable decoupled communication between gameplay systems.

Benefits:

* Reduced system dependencies
* Event-driven architecture
* Improved scalability and maintainability
* Cleaner communication between gameplay modules

## Technical Challenges

During development, particular attention was given to:

* Multiplayer replication
* GAS integration with networked gameplay
* Modular equipment architecture
* Animation system extensibility
* Decoupled gameplay communication

## Author

bo6eR — Unreal Engine Developer
