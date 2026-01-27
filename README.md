# Insaniquarium Deluxe Decompilation

Based on reverse engineering of Windows executable distributed on PopCap.com.

Status: Fully playable, still has bugs and inaccuracies.

### Dependencies

- Visual Studio
- BASS 1.8 DLL, shipped with original game, if you're compiling a 32-bit version.

### Notes

- Most of the variable/function/parameter names are guessed or unnamed.

### Compiling

Before compiling make sure to copy bass.dll and other data containing folders (data, properties, fishsongs...) from your game files to the root folder (where .sln is).

Current .sln file is made with Build tools: Visual Studio 2022 (v142 toolset).

Open WinFish.sln in Visual Studio and press F5 to build.

### Miscellaneous notes

Legal notes:
- The source code of game itself (everything in `source/WinFish/`) is provided as-is.
- Any modifications are licensed under MIT.
- The main focus of the project is bug-fixing, modding and porting and is done in a good faith - we don't condone 
  piracy, we just essentially want to give the game a "second life".
- The decompilation project does not include any assets - you need to get them from your own copy of the game - if 
  you don't own it, Steam version is easily available: https://store.steampowered.com/app/3320/Insaniquarium_Deluxe/