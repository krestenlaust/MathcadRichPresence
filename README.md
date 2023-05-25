# MathcadRichPresence
A plugin for PTC Mathcad Prime 4.0, that enables Discord's rich presence, by using Mathcads Custom Functions plugin-system.

## What does it do
Not much currently. It just shows the same text 'Editing document' and 'In file'.
I haven't figured out, how to retrieve the document title, or whether it's even possible using the plugin API.

## How to use it
Since the project relies on the built-in plugin system, simply moving the plugin file (DLL) into the plugins folder, enables the plugin.

- Simply download the latest release files (Note to self: insert link)
- Drag them into the `Custom Functions`-folder, most likely located here: `C:\Program Files\PTC\Mathcad Prime 4.0\Custom Functions`
(It does require administrator to drag it into the folder, if you installed using administrator.)

## APIs used
The main focus of the project was learning to use Discords new [Game Sdk](https://discord.com/developers/docs/game-sdk/sdk-starter-guide), which is used in many games to communicate (using RPC) with the local Discord client.

As for the implementation method used, I looked into the different APIs offered by Mathcad (and considering hooking), but ultimately went with [Mathcad Prime Custom Functions](http://support.ptc.com/help/mathcad/en/index.html#page/PTC_Mathcad_Help/about_custom_functions.html)

## Project structure
The project was just a fun side-project, so I didn't plan on releasing it, which resulted in relaxed naming of the projects. None of the projects depend on each other, they are just all related to the overall topic of Mathcad / Rich Presence.

Folders / projects:
- MathcadRichPresencePlugin (main project)

This is the project which compiles the plugin file. It combines the efforts of the other projects, to enable Mathcad Rich Presence.

State: Should compile fine.

- DiscordBasicApplication

This project is simply demonstrating, how the Game Sdk works. It being a console project enabled faster development, instead of waiting for Mathcad to open.

State: Doesn't seem to compile.

- MathcadDemoPlugin

This project is demonstrating use of the Mathcad plugin API. I used it to better understand, how the API works and what it allows.

State: Doesn't seem to compile.

- MathcadDiscordCombinedPlugin

This project combined the Mathcad Plugin (a multiply function specifically) with the Discord rich presence.

State: Should compile fine.

- MathcadRichPresencePluginOld

This project was the original attempt at implementing rich presence.

State: Doesn't seem to compile.

- discord-files (folder)

The discord C/C++ API used by intellisense.