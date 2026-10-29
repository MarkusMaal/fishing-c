# Fishing in C

Text-based fishing game, which is a port of my old batch game "Fishing Batch" to the C programming language. The project is currently in development, but all the maps from the original game are playable.

However, unlike the original batch game, this version makes heavy use of data-driving. The following things are data-driven:

- Fish
    - Appearance
    - Attack patterns
    - Fighting inputs
    - Stats (weight/size)
    - Looping cycles (parts of the fight that may loop several times)
    - Names and livewell information
- Maps
    - Names
    - Background graphic
    - Water bounds
    - Map goal and game type
    - Music filenames
    - Background/foreground color
    - List of fish, spawn locations and luck rates
    - Boat/rod position
    - Status messages and their color/position
    - Lore
- Credits/copyright
- Profiles
    - CRC32 verification
    - Names
    - Save date/time
    - Last fishing session (able to restore previous session on reload)
    - Scouting/fish despawn information
    - Settings
    - Unlocks
    - "Stories told"

## Compiling

Compiling instructions will not be provided until final version is publically available, however there is a makefile, so you may be able to make use of that to get the beta version -_-

## Features currently implemented

- Profile system
- "Awesome" menu system
- Difficulty, rod color and delete profile options
- Maps: Beginner's lake, Experience river, Moon lake, Expert sea
- Some of the settings (all the toggles and name changing)
- Saving and autosaving
- Main progression flow for unlocking new maps
- Loading screen tips
- Fishing menu
- Fish fights
- Boss fish fight
- Scouting map
- Credits screen
- Livewell
- Splash and title screen screens
- Cheat Codes
- Secret map and creepypasta features
- Timeouts/demo mode
- Game over screen
- Scoreboard
- Arcade mode

## Features to be implemented

### Porting

- Custom game mode

### New features

- Maps: Swampy Lake, River Falls, Jelly Fun Place
- Lures
- Player stats (stuff like how much fish you've caught in total, what's your luck been like, etc.)
- Map editor (maybe in the future for release v2.1)

## Scouting map

Exclusive to this port is the scouting map feature - it shows on a grid that spots you've caught fish/boots from as well as spots that probably don't have fish.