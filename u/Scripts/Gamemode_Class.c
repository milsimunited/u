class GameMode_Editor_Full_Class: SCR_GameModeEditor 
{
	// user script
	protected ref array<IEntity> spawned = {}; // NOT const!!!
	protected ref array<int> to_delete = {};
	
	
	void u_loop()
    {
		PlayerManager playerManager = GetGame().GetPlayerManager();
		array<int> allPlayers = {};
		playerManager.GetPlayers(allPlayers);
		
		Resource resource_blufor = Resource.Load("{938AAB9744DC2546}Prefabs/Groups/BLUFOR/Group_BW_Squad_Flecktarn.et");
		Resource resource_opfor = Resource.Load("{0ACD74AD27EEEE7D}Prefabs/Groups/OPFOR/RHS_AFRF/MSV/VKPO_Demiseason/Group_RHS_RF_MSV_VKPO_DS_FireGroup.et");
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
			vector spawn_pos = SCR_Math2D.GenerateRandomPointInRadius(250, 500, player.GetOrigin(), true);
			bool too_close = false;
			foreach (int oneplayerid2 : allPlayers)
			{
				IEntity player2 = playerManager.GetPlayerControlledEntity(oneplayerid2);
				if ( vector.Distance(player2.GetOrigin(), spawn_pos) < 250 )
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
			Print("Player vector transformation: " + SCR_Math2D.GenerateRandomPointInRadius(250, 500, player.GetOrigin(), true), LogLevel.NORMAL);
			Print("Distance to player: " + distance + "; too close: " + too_close, LogLevel.NORMAL);
		}
    }

	override void OnGameStart()
	{
		Print("u loop ext initialized", LogLevel.NORMAL);
		GetGame().GetCallqueue().CallLater(this.u_loop, 7000, true);
	}

};

