<p align="center">
   <img src="https://i.imgur.com/uXrlAQW.png" alt="Chaos Mod RDR">
</p>

# Chaos Mod RDR
This mod for **Red Dead Redemption 2** activates a random gameplay effect every 45 seconds. This effect can make it harder for you to play, help you, or just be neutral.

You can enable **Twitch Integration** and your viewers will be able to vote for effects.

You can find a list of effects **[here](https://docs.google.com/spreadsheets/d/1Z7dflg-n9VaXPnsqlzortFaUpkuy7XWrB9BGvJ8ilQk)**.

## Installation
- Install **[Script Hook RDR2](http://www.dev-c.com/rdr2/scripthookrdr2/)**.
- Download the latest mod version from the **[Releases page](https://github.com/clixff/ChaosModRDR/releases)**.
- Extract all files from the archive to the **game directory**. 
- Press **F7** in the game to *enable / disable* the mod. Mod is tested on game versions **1.0.1311.23** and **1.0.1436.28**.
## Twitch integration
- Open file **ChaosMod/config.json** and change param **twitch** from **false** to **true**.
- Launch the game, and wait for it to load into the single player
- Visit **[this link](http://127.0.0.1:9148/login)** and authorize access. 
- Re-enable mod with **F7**. Now your viewers can vote for effects in chat.
- If voting doesn't work and the **token** param is empty in **config.json** file, run the game as administrator.
## Twitch poll overlay
In **OBS** add new a **Browser source**. Enter these params:
<hr/>

**URL**: `http://127.0.0.1:9148/`

**Width**: `400`

**Height**: `220`

- [x] `Refresh browser when scene becomes active`
<hr/>

Press **Ctrl+E** when the source is selected and set position to `1490.0`, `105.0`

## Configuration
You can tweak some params in the **ChaosMod/config.json** file.

For example, **effects interval**, **voting time**, **effect status** (enabled/disabled), **effect duration**, **effect chance** (1-10) and **effect name**.

Don't forget to press **F7** in game when you have made changes.

## Controls
**F7** — toggle on/off the mod

**F8** — toggle off/on effects activation

**F10** — test any effect. **Arrows** to navigate, **Enter** to activate.

**F12** — instakill player character (useful when the game breaks)

## Suggestions and bug reports
You can suggest an effect for this mod or send a bug report on the **[Issues page](https://github.com/clixff/ChaosModRDR/issues/new/choose)**.

## Mirrors
**[NexusMods](https://www.nexusmods.com/reddeadredemption2/mods/1269)**

## Known issues
- If mod crashes after reloading savegame or starting a new game, try pressing **F11** in game. After this you can reload your savegame or start new game.
- If Twitch integration doesn't work, try run game as administrator and authorize Twitch access again.