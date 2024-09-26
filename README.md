# u

This is a collection of scenarios for Arma Reforger that resemble Ravage and DayZ in terms of persistence and survival. 

Persistent position and loadout. Game crashes don't bother you much.
Dynamic spawn and despawn of hostiles and allies for ambient combat and loot.
Gamemaster to embed handcrafted scenarios within a dynamic environment.
Players need to stay alert  all the time because this is a war zone. Gamemasters can focus on details.
Ideal for training and exploring new terrains and equipment as it is easily customizable and  completely open source.

I hope this will be helpful for anyone to do first steps in scripting automatation in Reforger, who is struggling with hours of video tutorials, when he only wants some lines of code.
It's not overly elaborate and beautiful, but it's quick.

Most of the magic happens when you place the prefab gamemode "gamemaster editor" and add an OnGameStart and a user script to it, like this: 

```
class Gamemode_Class: SCR_BaseGameMode 
{
    // user script
    protected ref array<IEntity> spawned = {}; // NOT const!!!
    protected ref array<int> to_delete = {};
    
    
    void u_loop()
    {
        PlayerManager playerManager = GetGame().GetPlayerManager();
        array<int> allPlayers = {};
        playerManager.GetPlayers(allPlayers);
        
        Resource resource_blufor = Resource.Load("{84E5BBAB25EA23E5}Prefabs/Groups/BLUFOR/Group_US_FireTeam.et");
        Resource resource_opfor = Resource.Load("{30ED11AA4F0D41E5}Prefabs/Groups/OPFOR/Group_USSR_FireGroup.et");
        EntitySpawnParams params = new EntitySpawnParams();
        params.TransformMode = ETransformMode.WORLD;
        
        Print("u loop ext execute", LogLevel.NORMAL);
        Print("spawned: " + spawned, LogLevel.NORMAL);
        Print("to_delete: " + to_delete, LogLevel.NORMAL);
        
        //foreach (int index, int hostile : spawned)
        for (int i = 0; i < spawned.Count(); i++)
        {
            Print("index = " + i + "; value = " + spawned[i], LogLevel.NORMAL);
            
            bool too_far = true;
            foreach (int oneplayerid : allPlayers)
            {
                IEntity player = playerManager.GetPlayerControlledEntity(oneplayerid);
                if ( spawned[i] != NULL )
                {
                    if ( vector.Distance(spawned[i].GetOrigin(), player.GetOrigin()) < 600 )
                    {
                        too_far = false;
                        Print("Distance to player: " + vector.Distance(spawned[i].GetOrigin(), player.GetOrigin()) + "; too far: " + too_far, LogLevel.NORMAL);
                    }
                }
            }
            if ( too_far )
            {
                to_delete.Insert(i);
            }
        }
        
        for (int i = 0; i < to_delete.Count(); i++)
        {
            Print("to delete: " + to_delete[i], LogLevel.NORMAL);
            if ( spawned.IsIndexValid(to_delete[i]) )
            {
                if ( spawned[to_delete[i]] != NULL )
                {
                    SCR_EntityHelper.DeleteEntityAndChildren( spawned[to_delete[i]] );
                }
                spawned.Remove(to_delete[i]);
            }
        }
        
        to_delete.Clear();
        
        foreach (int oneplayerid : allPlayers)
        {
            IEntity player = playerManager.GetPlayerControlledEntity(oneplayerid);
            vector spawn_pos = SCR_Math2D.GenerateRandomPointInRadius(200, 500, player.GetOrigin(), true);
            bool too_close = false;
            foreach (int oneplayerid2 : allPlayers)
            {
                IEntity player2 = playerManager.GetPlayerControlledEntity(oneplayerid2);
                if ( vector.Distance(player2.GetOrigin(), spawn_pos) < 300 )
                {
                    too_close = true;
                }
            }
            if ( !too_close )
            {
                params.Transform = {"1 0 0","0 1 0","0 0 1",spawn_pos};
                IEntity friendly = GetGame().SpawnEntityPrefab(resource_blufor, GetGame().GetWorld(), params);
                spawned.Insert(friendly);
                IEntity hostile = GetGame().SpawnEntityPrefab(resource_opfor, GetGame().GetWorld(), params);
                spawned.Insert(hostile);
            }
            
            int distance = vector.Distance(player.GetOrigin(), spawn_pos);
            Print("Player: " + oneplayerid + " " + playerManager.GetPlayerName(oneplayerid) + " Position: " + player.GetOrigin(), LogLevel.NORMAL);
            Print("Player vector transformation: " + SCR_Math2D.GenerateRandomPointInRadius(200, 500, player.GetOrigin(), true), LogLevel.NORMAL);
            Print("Distance to player: " + distance + "; too close: " + too_close, LogLevel.NORMAL);
        }
    }


    override void OnGameStart()
    {
        Print("u loop ext initialized", LogLevel.NORMAL);
        GetGame().GetCallqueue().CallLater(this.u_loop, 7000, true);
    }

};
```
