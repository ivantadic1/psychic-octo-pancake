# America: No Peace Beyond the Line – Community Fix

Modern compatibility and reverse engineering project for  
*America: No Peace Beyond the Line (2001)*.

# Project Goals
    - Stable modern Windows support
    - Widescreen / high resolution support
    - UI and camera fixes
    - Gameplay fixes
    - Open modding framework

## Features

- Runtime DLL patching
- Pattern scanning system
- Modern Windows compatibility improvements
- Experimental resolution fixes
- Injector-based patch loading
- Legacy DirectDraw compatibility support

---

# Requirements

This project currently relies on cnc-ddraw for stable DirectDraw compatibility on modern Windows systems.

Download cnc-ddraw from:

https://github.com/FunkyFr3sh/cnc-ddraw/releases

---

# Installation

## 1. Install the game

Install *America: No Peace Beyond the Line* normally.

---
### Important Sidenote - Make sure that in America.exe -> properties -> compatibility everything is unchecked. Conflicts may occur when using cnc ddraw + windows compatibility options


## 2. Copy cnc-ddraw files

From the cnc-ddraw release archive, copy these files into the same folder as `America.exe`:

```text
ddraw.dll
ddraw.ini
cnc-ddraw config.exe
```
```text
Set these settings in cnc config (if possible):
```
### Display Settings
    - Presentation: Fullscale Upscaled
    - Maintain aspect ratio: True
    - Enable VSync: False
    - Adjust mouse sensitivity: True
    - Lock cursor to window / screen: True

### Advanced Settings
    - Renderer: Automatic
    - OpenGL shader: Shaders\interpolation\catomull-rom-bilinear.glsl
    - Limit frame rate: True
    - Enable windowboxing / integer scaling: False
    - Show window borders in windowed mode: True
    - Remember window position and size: True

### Compatibility setings
    - Limit game speed: Emulatte 60hz refresh rate monitor
    - Fix broken Alt + Tab: False
    - Unlock additional screen resolutions: False
    - Force high FPS / Fix stuttering on Freesync / G-Sync: False
    - Fix bad performance and sound issues: False
    - Fix invisible videos / UI elements: True (i didnt change the default value)

# Technical Overview

### The project currently includes:
- DLL injection
- Runtime memory patching
- Pattern scanning
- API hooking experiments
- Reverse engineering research
- DirectDraw compatibility research

### Reverse engineering is performed using:
- Ghidra
- Cheat Engine
- Visual Studio