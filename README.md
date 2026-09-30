# Time to Breach

A CRPG with a player-only time rewind mechanic, made by an 8-person team in 4 weeks at
Futuregames. Built in Unreal Engine 5.6 (C++, Blueprint, Python).

- Play: https://futuregames.itch.io/time-to-breach
- Write-up: https://ossiandackfors.com/time-to-breach/

## What's in this repository

The full Unreal project the team made: C++ source (`Source/`), config, Blueprints, maps,
materials, VFX, UI, input, sound effects and the editor Python tools (`Content/Python/`).

The rewind system is `URewindComponent` in `Source/GP4_Team7/Gameplay/Time/TimeReversal/`.

## What's not included

Third-party assets are licensed for use in the project, but may not be redistributed, so they
are left out (see `.gitignore`). The project compiles and opens without them, but maps that use
them show missing meshes and materials.

To open the maps fully, add these packs to `Content/` under the same folder names:

| Folder | Source |
|---|---|
| `Animated_Nebulas_Starter_Pack` | Fab |
| `Chestnuts_Pack` | Fab |
| `Clinic` | Fab |
| `Cyberpunk_RPG_UI_27` | Fab |
| `Drone` | Fab |
| `Fab` | Fab (various free assets) |
| `JVAD3D_SimpleWaterPuddles` | Fab |
| `kb3d_missiontominerva` | KitBash3D, Mission to Minerva |
| `ModularSciFiPrison` | Fab |
| `Namaqualand` | Megascans |
| `ParagonLtBelica`, `ParagonWraith` | Epic Games, Paragon characters |
| `PrisonLifeAnimsPack` | Fab |
| `RockEnv_Pack` | Fab |
| `Scene_Junkyard` | Megascans |
| `SciFiInterior04` | Fab |
| `SciFiSoldier02` | Fab |
| `Spaceman` | Fab |
| `SpaceshipInterior` | Fab |
| `Survival_Character` | Fab |
| `UndergroundSciFi` | Fab |
| `Vefects` | Vefects (Fab) |
| `Mixamo`, `Characters/Prison`, `Characters/Prison1`, and the cinematic animations | Mixamo |
| `Audio/Music` | Licensed music |

## Opening the project

1. Install Unreal Engine 5.6.
2. Right-click `GP4_Team7.uproject` and choose **Generate Visual Studio project files**.
3. Open the solution and build `GP4_Team7Editor` (Development Editor), or just open the
   `.uproject` and let Unreal compile the module.

The start map is `Content/Blueprints/Prototypes/john/L_Menu`.
