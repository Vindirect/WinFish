# changelog

## [1.2.0] - 2026-03-09
### Fixed
- **Engine:** 
- Updated project to build on Visual Studio 2022 (v143 toolset).

- **Stability:** 
- Resolved critical Stack Overflow errors occurring in the fish menu in the Virtual Tank.
- Changed EditWidget's character addressing, might need to be fixed later

- **Visuals:** 
- Fixed sprite rendering errors where hungry breeder sprite appeared instead of the dead one.

- **Sounds:**
- Fixed the guppies not playing the dying sound where they should.

- **Logic:** 
- Fixed a bug where the star potions would not correctly turn a guppy into the star guppy.
- Fixed a bug where the tier 2 and 3 fish food stopped guppies from growing. (There was an error regarding the operator precedence.)
- 

### Added
- Made it easier for modders to make custom alien waves by introducing new function.
- Added a debug cheat code(skip) that skips the level.

**Technical Note for Modders:**
This version has been updated to **Visual Studio 2022 (v143)**.

If you use VS 2022: It will work immediately "out of the box."
If you use VS 2019: Visual Studio will ask you to "Retarget" the project when you open it. Simply select v142 from the dropdown and click OK.