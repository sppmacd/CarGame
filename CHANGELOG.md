# Definitions

DPT - Default Power Time (power time without upgrades / abilities)
levelAcceleration - defined by map car spawn delay (in ticks, on the start of the game).

# Changelog labels & Github labels

	----------------------------------------------------------------------------------
	Label            Corresponding            Description
	in changelog     Github issue label        
	----------------------------------------------------------------------------------
	[balance]        [balance]                Change of single gameplay value, e.g.
											  game speed.
	[command line]   [command line]           Change of command line arguments or
											  startup scripts
	[cmake]          [cmake]                  Issue releates to CMake build system
	[fix]            [bug]                    Something is/was not working
	[launcher]       [launcher]               Every issue that relates to launcher.
	[performance]    [performance]            Issue that relates to game performance.
	[technical]      [technical]              Issue that is not directly related to
											  gameplay and does not affect it.
	[api]            [api]                    Issue that relates to CG 0.3 API.
	No tag           [enhancement]            Every other issue.
	----------------------------------------------------------------------------------

# BETA 0.3 (Planned)
+ New Anti-Power: Ramp

# BETA 0.3 Progress
* [command line] Added 
* [technical] All std::cout calls are now wrapped in DebugLogger to unify log
  format.
* [technical] Unified Game::displayError() message format.
* [technical] Changed level of some messages
* [technical] Added log coloring (for Windows)
* Added function to specifying log level (e.g. info, error)
* Changed log format
* [api] Added GameplayObjectManager and moved here all registries.
* [technical] Documented code
* [api] Added PlayerDataManager to store player save data.
* [fix] Fixed PowerBall (balls not disappearing after started new game)
* [technical] Renamed /res to /data
* [command line] Added startup script for Unix

# BETA 0.2
Note: Since this version CarGame pack does not include 32-bit Windows in published binaries.

## MAJOR CHANGES
+ New powers: Ball
+ Powers aren't bought, but upgraded.
+ You can have now only 2 powers at once.
+ Bomb doesn't longer making game over - it gives a random Anti-Power when clicked.
+ Added Anti-Powers: Damage Decrease, Car Speed Increase.
+ Added a 3-second delay when starting / resuming game.
+ Added a short power description when hovering equip button or upgrade button.
+ Added Shop with powers and abilities.
+ Added abilities: Power Reload Time, Power Time, Damage. It will ease your game upgrading the best things and degrading the worst.
+ Added an option for resetting settings.
+ Added counter, delaying game unpause when exiting Ingame Menu by 3 seconds.
+ Added credits screen.
+ Added simple tutorial.
+ Added Sounds!!!
+ Created Language GUI.
+ New translations: full German.

## MINOR CHANGES
* Added point stat animations.
* Added some labels to GuiPowers.
* Armored Car won't be longer spawned double in the row.
* Cars are spawned in the same distance, not period of time.
* Cars destroyed by power will give coins.
* Changed apperrance of Oil power (on higher levels).
* Changed background color of some maps.
* Changed freeze behaviour - its effect is now faded (weaker at the beginning and at the end of duration)
* Changed Point Boost behaviour - now adds 2 x [current level] points every 10 ticks.
* Changed power icon textures.
* Changed Powers GUI apperrance.
* Coins are earned by full destruction of car (not damage only).
* Fence is a bit better - 0.01 HP/tick -> 0.025 HP/tick and longer - 10s -> 15s
* Fixed Map Select GUI apperrance (the map button size depends on screen size).
* Freeze freezes less - 2.5% Speed/tick -> 0.5% Speed/tick and the speed rises shorter - 1/9 DPT -> 1/16 DPT
* Freeze makes game a bit quicker when running out - 0.0625% Speed/tick -> 0.0725% Speed/tick
* Game Over GUI and Ingame GUI returns to Map Select GUI when clicked "Return to Menu".
* Made Freeze a bit shorter - 60 s -> 50 s
* Made GUI font a bit smaller - 30 pt -> 28 pt
* Map Selection now is set on last unlocked map (not always on Coutryside)
* New apperrance of Car Speed Increase anti-power.
* Now the game screen is scaled to fit the window. 
* Now the power icon is displayed always when power is running (even Anti-Power).
* Oil is better on first level but its force rises slower.
* Point Boost is shorter - 20 s -> 5 s and makes game quicker - 0.005 Speed/tick
* Power duration is now shown in Shop.
* Power indicator won't be displayed if any power wasn't equipped.
* Settings are loaded on launch!
* Settings are now saved!
* Settings can be accessed from ingame menu.
* Settings GUI - new apperrance.
* The game is now 2x smaller due to not including debug symbols in executables.
* The game speed rises slower. (levelAcceleration/5000 -> levelAcceleration/10000, which means 2x slower)
* Updated credits info - added ziputils library and CG license, updated sounds maker.
* Updated language GUI.
* You can get short information about stat when mouse entered its image.
* Fixed bugs
* Optimized game
* Made other technical changes (see below)

## TECHNICAL & PERFORMANCE CHANGES
* [cmake] SFML extlibs are now automatically copied to "${SFML_ROOT}/lib".
* [command line] Added --help: displays command line help.
* [fix] Added check for resource folder (game doesn't crash when error but shows a "message" - red progress bar while loading)
* [fix] Finally fixed Ingame GUI Escape bug!
* [fix] Finally fixed window resizing bugs.
* [fix] Fixed background bug on GPU that not supports non power of two sizes of textures.
* [fix] Fixed bug - Abilities wasn't reset when resetting game.
* [fix] Fixed fullscreen bug when disabled fullscreen in Options.
* [fix] Fixed Map Selection bug.
* [fix] Fixed very dark screen bug.
* [fix] Power bug - not started when closed level with running power.
* [fix] that Language GUI widgets wasn't resized when toggling fullscreen.
* [fix] that the power remains equipped after game is reset but cannot be used
* [performance] Improved power draw performance (not searching for power in every frame)
* [performance] Optimized game background a bit.
* [technical] Added API to cancel car damage events in correct way.
* [technical] Added API to set damage multiplier.
* [technical] Added some command-line arguments.
* [technical] Cars are drawn in backward direction (from the latest to the earliest)
* [technical] CG API: added function for initializing powers and for reloading textures
* [technical] Created CMake build system.
* [technical] Executables moved to folders ->  x86,x64 due to openal32.dll compability
* [technical] Improved code documentation.
* [technical] Made profile format compatible with other profile format versions. Your powers won't be longer removed!

# BETA 0.1.1
* Loading screen is now splash screen.

# BETA 0.1 - The Big Map Update

## MAIN CHANGES
+ NEW MAPS: SEA, BEACH, SWAMPLAND, CITY, MOTORWAY, FOREST TRAILS
+ NEW CARS: RALLY, FIREMAN, TANK, OLD, BOMB, TRAIN
+ NEW POWERS: POINT BOOST, FENCE
+ BOMB - You cannot click it. It makes the game over!
+ NEW Map Select GUI.
+ Added car animations.
+ Added localization support. Languages are English, Polish and German (not fully).
+ Added point multiplier based on map.

## OTHER CHANGES & FIXES
* New loading screen!
* New textures.
* Made maps 4x more expensive due to point multiplier.
* Now game speed depends on display.
* Now car panes are always blue.
* Now cars are spawning in other timespan in different maps.
* Now the Oil power is working on 2 lanes (not 3).
* Changed toggle button graphics (bigger slider).
* Changed graphics (eg. power time indicator).
* Changed rarity of some cars.
* New Powers GUI.
* Fixed some bugs.
* Fixed button bug on window resize.
* New profile file format.
