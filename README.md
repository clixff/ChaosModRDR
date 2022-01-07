<p align="center">
   <img src="misc/logo.png" alt="Chaos Mod RDR">
</p>

# Chaos Mod RDR
This mod for **Red Dead Redemption 2** activates random game effects sometimes. These effects can make your game difficult or help you.

You can enable **Twitch integration** and your viewers will be able choose these effects.

You can find a list of effects **[here](https://docs.google.com/spreadsheets/d/1Z7dflg-n9VaXPnsqlzortFaUpkuy7XWrB9BGvJ8ilQk)**.

## Installation
### Install ScriptHook
Install **[Script Hook RDR2](http://www.dev-c.com/rdr2/scripthookrdr2/)** (extract **dinput8.dll** and **ScriptHookRDR2.dll** to game directory). Mod is tested on version **v1.0.1436.25**.
### Download mod
Download latest mod version from **[Releases page](https://github.com/clixff/ChaosModRDR/releases)**.
### Install mod
Extract all files from archive to **game directory**. 
### Activation
Press **F7** in game to *enable / disable* mod. Mod is tested on game version **1.0.1311.23**.
### Twitch integration
When game is started, visit **[this link](http://127.0.0.1:9148/login)** and authorize access. Re-enable mod with **F7**. Now your viewers can vote for effects.
### Twitch poll overlay
In **OBS** add new **Browser source**. Enter these params:
<hr/>

**URL**: `http://127.0.0.1:9148/`

**Width**: `400`

**Height**: `220`

- [x] `Refresh browser when scene becomes active`
<hr/>

Press **Ctrl+E** when source is selected and set position to `1490.0`, `105.0`

### Configuration
You can tweak some params in **ChaosMod/config.json** file.

For example, **effects interval**, **voting time**, **effect status** (enabled/disabled), **effect duration**, **effect chance** (1-10) and **effect name**.

Don't forget to press **F7** in game when you have made changes.

### Additional
You can disable and re-enable effect activation for testing purposes by pressing **F8**. Also by pressing **F10** you can activate and test any effect.

### Known issues
- If mod crashes after reloading savegame or starting new game, try pressing **F11** in game. After this you can reload your savegame or start new game.
- If Twitch integration doesn't work, try run game as administrator and authorize Twitch access again.