# Documentation - CarGame DLL API

## Dependency graph

{L} - OS-dependent dynamic library extension (.dll, .so, ...)


   /----------------------------\           /--------------------------------\
   |  ${CG_ARCH_PREFIX}/cg.exe  |>> loads >>|  ${CG_ARCH_PREFIX}/mods/*.{L}  |
   \----------------------------/           \--------------------------------/
                  V                                           V
 /----------------------------------\                         V
 |  ${CG_ARCH_PREFIX}/libcgapi.{L}  |<<<<<<<<<<<<<<<<<<<<<<<<</
 \----------------------------------/
                  V
    /---------------------------\
    |  ${CG_ARCH_PREFIX}/*.{L}  |
    \---------------------------/
	              V
      /-----------------------\
      |  System Dependencies  |
      \-----------------------/		
 
 
## Procedures


	[OPTIONAL]
	CGAPI void cgCleanup()
	The function is called by root (cg.exe) when the Game exits main loop.
	
	### Valid pointers
	[ ] Game
	[ ] GameDisplay
	[x] GameLoader
	[x] ModuleManager


	[REQUIRED]
	CGAPI cgGameInit()
	The function is called from Game constructor (before GameDisplay reload).
	You can here fill GameplayObjectManager registers by your objects.
	
	### Valid pointers
	[x] Game
	[x] GameDisplay
	[x] GameLoader
	[x] ModuleManager


	[OPTIONAL]
	CGAPI int cgLoad()
	The function is called immediately after module is loaded, before Game
	starts.
	
	### Valid pointers
	[ ] Game
	[ ] GameDisplay
	[x] GameLoader
	[x] ModuleManager
	