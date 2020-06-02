# CG Debug Options

The --debug-options string is a space-separated option array:

    option1 option2 option3 ...
	
example:
	
	cglaunch --debug-options "st:global:debug=1 st:graphics:fullscreen=0 pp:power:level_cgcore$ball=5"
	
You can change settings or player profile temporarily to debug the game.

## Namespaces
````lg:```` - language config file
````pp:```` - player profile
````so:```` - specific options
````st:```` - settings

## Changing Profile
Prefix the option with the "pp:" namespace:

Example: Give player Damage ability at level 15 544 and 2 000 000 000 coins:

    "pp:ability:damage=15544 pp:main:coins=2000000000"

## Changing Settings
The settings changes are applied in the same way as previously:

Example: Force game to mute the sound.

    "st:sound:volume=0"
	
Example: Force game to start in non-Fullscreen Mode
	
	"st:graphics:fullscreen=0"
	
## Changing Language Config

Example: Change language to Polish:

	"lg:lang:current=pl_PL"
	
## Using Specific Options
The specific options aren't used in any of in-game registries - they are only simple switches to use in debug options.

    so:newPlayer= <0 or 1>      Initialize player profile with default values. It is NOT overridden by ````pp:```` options.
