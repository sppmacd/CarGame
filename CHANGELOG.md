# 

DPT - Default Power Time (power time without upgrades / abilities)
levelAcceleration - defined by map car spawn delay (in ticks, on the start of the game).
	
# BETA 0.2 - The Power Update (Planned)

+ New powers: Ball [TODO]

# BETA 0.2 SNAPSHOTS

## v191001
+ Added counter, delaying game unpause when exiting Ingame Menu by 3 seconds.
* Fixed bug - Abilities wasn't resetted when resetting game.
* Improved code documentation.
* Added simple tutorial.
* Cars are drawn in backward direction (from the latest to the earliest)
## v190901
+ Added Shop GUI with powers and abilities.
+ Added abilities: Power Reload Time, Power Time, Damage
* The game speed rises slower.(levelAcceleration/5000 -> levelAcceleration/10000, which means 2x slower)
* You can get information about stat when mouse entered its image.
* Cars are spawned in the same distance, not period of time.
* Game Over GUI and Ingame GUI returns to Map Select GUI when clicked "Return to Menu".
* Power indicator won't be displayed if any power wasn't equipped.
* Made GUI font a bit smaller - 30 pt -> 28 pt
* Made Freeze a bit shorter - 60 s -> 50 s
* Changed Powers' GUI apperrance.
* Point Boost time is shorter - 16.7 s -> 5 s
* Freeze freezes less - 2.5% Speed/tick -> 0.5% Speed/tick and the speed rises shorter - 1/9 DPT -> 1/16 DPT
## v190806
+ You can have now only 2 powers at once.
* Made profile format compatible with other profile format versions. Your powers won't be longer removed!
* Fixed Map Selection bug.
* Coins are earned by full destruction of car (not damage only).
* Cars destroyed by power will give coins.
* Armored Car won't be longer spawned double.
* Oil is better on first level but its force rises slower.
* Changed apperrance of Oil power (on higher levels).
## v190805
* Changed background color of some maps
* Map Selection now is set on last unlocked map (not always on Coutryside)
* Freeze makes game a bit quicker when running out - 0.0625% Speed/tick -> 0.0725% Speed/tick
* Point Boost is shorter - 20 s -> 16.7 s and makes game quicker - 0.005 Speed/tick
* Changed Point Boost behaviour - now adds 2 x [current level] points every 10 ticks.
* Fence is a bit better - 0.01 HP/tick -> 0.025 HP/tick and longer - 10s -> 15s
* Power duration is now shown in Shop.
## v190804
* Added some sounds: upgrade, game_over, coin_mpl.
* Changed freeze behaviour - its effect is now faded.
* Executables moved to folders ->  x86,x64 due to openal32.dll compability
## v190803
+ Added Sounds!!!
* Added point stat animations.
* Settings are loaded on launch!
## v190802
* Settings GUI - new apperrance.
* Fixed Map Select GUI apperrance (the map button size depends on screen size).
* Settings are now saved!
## v190801
+ Added credits screen.
+ Added volume slider (not used).
* Settings can be accessed from ingame menu.
* Changed Powers GUI apperrance.
## v190704
+ Powers aren't bought, but upgraded.
## v190703
* Finally fixed Ingame GUI Escape bug!
* Finally fixed window resizing bugs.
## v190702
+ New translations: full German.

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