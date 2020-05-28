# Documentation - Error Codes

The documentation often refers to game log. You can find it in console that is opening with the game (on Windows).

## Registry Error Codes
C00 - No CarType found
	The error indicates that no car is registered in game. It means that you have corrupted core and you must reinstall the game.
C01 - CarType not found
	This happens when the game tries to spawn Car that is not registered as CarType. This may be caused by invalid mod configuration or broken core. If you encounter that error, try removing all mods or reinstall tha game.
C02 - No spawnable CarType found
	The error occurs when the Car is registered, but no car can be spawned (because it has e.g rarity 0).

## Game Error Codes
G00 - Couldn't load default language
	This happens when the game can't find default (English) translation. The solution is to download the missing file from https://raw.githubusercontent.com/sppmacd/CarGame/master/data/res/lang/en_US.lang and place it to <game dir>/res/lang/en_US.lang
	(for cglauncher, the game dir is <cglauncher dir>/instances/<version>/cargame-<version>)
G01 - not used
G02 - not used
G03 - Duplicate Game instance
	Try removing mods.
G04 - Invalid profile version
	This means that the Game tries to load version of profile that is created in newer version of CG. Try launching the latest stable version of CG to ensure the profile will be converted automatically to newest format. If it doesn't work:
	
	// todo: --info !!
	* ensure that the game is closed!
	* open the profile_1.txt file in some text editor
	* you will see the ":version=X" line in profile.
	* start CMD in game dir (for cglauncher, the game dir is <cglauncher dir>/instances/<version>/cargame-<version>)
	* type "start.bat --info" or "./start.sh --info" (depending on shell) and press Enter. The .bat extension is very important to not confuse with "start" internal command on Windows/DOS.
	* you will see CG debug info. Find the line "profile-version=X". Set the version in the profile to X.
	e.g if "profile-version=6", set ":version=6".
	* start the game.
G05 - Couldn't initialize modules
	This is called when Game isn't able to initialize one or more of modules. The module name will be always visible
	in game log - remove the module if it causes an error.

## Display / ResourceManager Error Codes
D00 - Couldn't load texture
	The error is displayed only in log. If occurred, the unknown texture will be visible instead of not-loaded texture. To fix error,
	reinstall the game or download missing texture and place it to specified path.
D01 - Couldn't find /res folder
	The game cannot find resource folder. If the error occurs, reinstall the game.

## Main Error Codes
M00 - Out of memory
	To fix this error close unnecessary programs that may use RAM.
M01 - Exception while running: xxxxxx
	If the exception is 'stoi', it may mean errors in configuration files.
	See docs/exceptions.txt //todo
M02 - Unexpected exception caught
	This error shouldn't happen, but if it happen, try reinstalling the game without any mods.
M03 - Couldn't load modules
	It occurres always with M01 as description. It means that an error occurred when trying to
	load mods. See log for details and remove mod that causes the error. If that is 'cgcore', contact me -
	e.g. add issue on Github.
	
## Other errors
	* If the game crashes with "cg.exe stopped working..." window, this may be:
		* bug in mod - try removing the mod that probably caused the error
		* bug in game - the only thing you can do is reporting it on Github. 
		https://github.com/sppmacd/CarGame/issues > New issue > select "Bug report" template.
		In report:
			* describe: what happens, when it happens (how to reproduce error), what is displayed
			* if you can, attach some screenshots to issue. 
		
	* If the game is suddenly closing when doing some action, act like it's a crash.
