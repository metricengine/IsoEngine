# IsoEngine
IsoEngine is a 2D engine dedicated for old-school isometric games. The project has mostly educational purposes, but it seeks to create an easy to use, cross-platform engine for 2D games. The engine aims at bringing modern engine features (e.g. advanced lightning) to retro isometric style. Currently, the engine uses SFML (a wrapper library for OpenGL) for graphics, but in the future it will be swapped for Vulkan.

## Goals
The ultimate goal is to create a high performance, lightweight, cross-platform engine dedicated to 2D isometric games. The principles can be summarized as follows:

- High performance
- Lightweight
- Simple to use
- Usage of modern C++
- Cross-platform

## Features
The following list includes the planned and already implemented features:

- Event handling (keyboard, mouse)
- Command queues
- Creating and using animations from sprites
- Resource manager
- Compile-time hashed strings for ids

## Usage
Create and instance of engine (highlevel/engine.h), then add textures, animations and register commands. Then add entities to the engine, set their animations. With setting up done, just call the run method of the engine instance.
