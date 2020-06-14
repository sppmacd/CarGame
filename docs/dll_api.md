# Documentation - CarGame DLL API

## Dependency graph

{L} - OS-dependent dynamic library extension (.dll, .so, ...)


![Dependency graph](https://raw.githubusercontent.com/sppmacd/CarGame/master/docs/dependency_graph.png "Dependency Graph")	
 
 
## Procedures

````CGAPI void cgCleanup()````

[OPTIONAL]
The function is called by root (cg.exe) when the Game exits main loop.
	
### Valid pointers
[ ] Game
[ ] GameDisplay
[x] GameLoader
[x] ModuleManager


````CGAPI cgGameInit()````
[REQUIRED]
The function is called from Game constructor (before GameDisplay reload).
You can here fill GameplayObjectManager registers by your objects.

### Valid pointers
[x] Game
[x] GameDisplay
[x] GameLoader
[x] ModuleManager


````CGAPI int cgLoad()````
[OPTIONAL]
The function is called immediately after module is loaded, before Game
starts.

### Valid pointers
[ ] Game
[ ] GameDisplay
[x] GameLoader
[x] ModuleManager
	
