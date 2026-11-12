# Fishing in C

**WARNING**: The current Linux release has memory leak issues. Please don't use it and wait for version 2.1 release!

Text-based fishing game, which is a port of my old batch game "Fishing Batch" to the C programming language. The features from that game have all been ported over, but some unique features are planned for future updates.

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

### Linux

Install libao and libmpg123 dependencies and gcc compiler. Then you should be able to run `make` to automatically compile the game.

### Windows

Compilation on Windows should be done with [Cygwin](https://cygwin.com/). Install it with the following packages: gcc-core, libao and libmpg123. Then you can run Cygwin64 terminal and use the `make` command to build an executable.

To resolve DLL errors, copy the DLL files mentioned on the error messages from `<cygwin64 install dir>\bin` to the game folder.

Note: It's recommended to disable legacy console for colors to be displayed correctly.

### macOS

Install libao and libmpg123 using [Homebrew](https://brew.sh). Then run `make brew` to automatically compile the game.

This will use cmake by default, but you can replace it with gcc if you want to.

## Converting legacy profiles

To import profiles from Fishing Batch (1.1 or later), you can copy the contents of save folder from Fishing Batch to the save folder found in Fishing C and then run it with the `--convert` argument, which will convert the saves to the correct format.

If you have save data from version 1.0 (with the .cmd extension), this must be converted to 1.1+ format (with the .save extension) by first loading it in version 1.1+ and then saving the profile. This will produce .save files that this conversion process supports. The latest version of Fishing Batch can be downloaded from here: [https://markustegelane.eu/app/fishing.zip](https://markustegelane.eu/app/fishing.zip).

Note that livewell and statistics data will not be imported this way. In addition, if you were fishing at the moon lake previously, the fish you've caught so far will be reset and if you had unlocked Expert Sea in previous versions, you have to now complete Swampy Lake to have it be unlocked again.

The arcade mode scores data will get converted however and you can verify this by pressing **BACKSPACE** on the title screen (not Escape, which was the key used in Fishing Batch).

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
- Custom game mode
- New maps: Swampy Lake, River Falls, Jelly Fun Place (custom mode environment)
- Lures
- Player stats (stuff like how much fish you've caught in total, what's your luck been like, etc.)
- Map editor (in developer menu)


## Scouting map

Exclusive to this port is the scouting map feature - it shows on a grid that spots you've caught fish/boots from as well as spots that probably don't have fish.
