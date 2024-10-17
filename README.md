# u

## Description
Arma Reforger scenario with player and vehicle persistence and dynamic spawn. 

- Persistent position and loadout. Game crashes don't bother you much.
- Dynamic spawn and despawn of hostiles and allies for ambient combat and loot.
- Gamemaster to embed handcrafted scenarios within a dynamic environment.
- Players need to stay alert  all the time because this is a war zone. Gamemasters have more time to focus on details.
- Ideal for training and exploring new terrains and equipment.
- Ideal scenario template as it is easily customizable and completely open source.
 
 
 
## Workshop
https://reforger.armaplatform.com/workshop/61E8761D4AC85A90-u-Everon
 
 
 
 
## Workbench
I hope this will be helpful for everyone who wants to do first steps in scripting and automatation in Reforger, but is struggling with hours of video tutorials, when only looking for some lines of code. It's not overly elaborate and beautiful, but it's quick.

Just throw the u directory in your C:\Users\Your Username\Documents\My Games\ArmaReforgerWorkbench\addons along with all the dependencies from the workshop.
 
 
 
 
## Under the hood
Most of the magic happens when you place the prefab gamemode "SCR_GameModeEditor" and add an OnGameStart and a user script to it, like this: 

https://github.com/milsimunited/u/blob/main/u/Scripts/Gamemode_Class.c
 
 
 
 
You also need:
 
 
SCR_GameModeEditor
- disable spawn stuff
- add EPF_BasicRespawnSystemComponent
- add EPF_PersistenceManagerComponent: select connection info Json
- add EPF_SpawnPoint
 
 
Besides the gamemode:
- Perception Manager
- Radio Manager
- Faction Manager
- SCR_AIWorld_Eden
- ScriptedChatEntity
