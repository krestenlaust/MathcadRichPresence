# MathcadRichPresence
A plugin for PTC Mathcad Prime 4.0, that enables Discord's rich presence, by using Mathcads Custom Functions plugin-system.

## What does it do


## How to use it
- Simply download the latest release files (Note to self: insert link) from releases and drag them into `C:\Program Files\PTC\Mathcad Prime 4.0\Custom Functions`. It does require administrator to drag it into the folder, if you installed using administrator.

## APIs used
The main focus of the project was using Discords new [Game Sdk](https://discord.com/developers/docs/game-sdk/sdk-starter-guide), which is used in many games to communicate using RPC with the local Discord client.

As for the implementation method used, I looked into the different APIs offered by Mathcad (and considering hooking), but ultimately went with [Mathcad Prime Custom Functions](http://support.ptc.com/help/mathcad/en/index.html#page/PTC_Mathcad_Help/about_custom_functions.html)
