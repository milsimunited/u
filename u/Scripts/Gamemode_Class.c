class GameMode_Editor_Full_Class: SCR_GameModeEditor 
{
	// user script
	protected ref array<IEntity> spawned_blufor = {}; // NOT const!!! // about arrays: scripts/Core/proto/Types.c
	protected ref array<IEntity> spawned_opfor = {};
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
		
		// BLUFOR
		// foreach (int index, int hostile : spawned)
		for (int i = 0; i < spawned_blufor.Count(); i++)
		{
			bool too_far = true;
			foreach (int oneplayerid : allPlayers)
			{
				IEntity player = playerManager.GetPlayerControlledEntity(oneplayerid);
				if ( spawned_blufor[i] != NULL )
				{
					if ( vector.Distance(spawned_blufor[i].GetOrigin(), player.GetOrigin()) < 400 )
					{
						too_far = false;
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
			if ( spawned_blufor.IsIndexValid(to_delete[i]) )
			{
				if ( spawned_blufor[to_delete[i]] != NULL )
				{
					SCR_EntityHelper.DeleteEntityAndChildren( spawned_blufor[to_delete[i]] );
				}
				spawned_blufor.Remove(to_delete[i]);
			}
		}
		
		to_delete.Clear();
		
		// OPFOR
		for (int i = 0; i < spawned_opfor.Count(); i++)
		{
			bool too_far = true;
			foreach (int oneplayerid : allPlayers)
			{
				IEntity player = playerManager.GetPlayerControlledEntity(oneplayerid);
				if ( spawned_opfor[i] != NULL )
				{
					if ( vector.Distance(spawned_opfor[i].GetOrigin(), player.GetOrigin()) < 400 )
					{
						too_far = false;
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
			if ( spawned_opfor.IsIndexValid(to_delete[i]) )
			{
				if ( spawned_opfor[to_delete[i]] != NULL )
				{
					SCR_EntityHelper.DeleteEntityAndChildren( spawned_opfor[to_delete[i]] );
				}
				spawned_opfor.Remove(to_delete[i]);
			}
		}
		
		to_delete.Clear();
			
		foreach (int oneplayerid : allPlayers)
		{
			IEntity player = playerManager.GetPlayerControlledEntity(oneplayerid);
			vector spawn_pos = SCR_Math2D.GenerateRandomPointInRadius(200, 400, player.GetOrigin(), true);
			bool too_close = false;
			foreach (int oneplayerid2 : allPlayers)
			{
				IEntity player2 = playerManager.GetPlayerControlledEntity(oneplayerid2);
				if ( vector.Distance(player2.GetOrigin(), spawn_pos) < 200 )
				{
					too_close = true;
				}
			}
			if ( !too_close )
			{
				params.Transform = {"1 0 0","0 1 0","0 0 1",spawn_pos};
				
				if(spawned_blufor.Count() < 8)
				{
					IEntity blufor = GetGame().SpawnEntityPrefab(resource_blufor, GetGame().GetWorld(), params);
					spawned_blufor.Insert(blufor);
				}
				
				if(spawned_opfor.Count() < 12)
				{
					IEntity opfor = GetGame().SpawnEntityPrefab(resource_opfor, GetGame().GetWorld(), params);
					spawned_opfor.Insert(opfor);
				}
			}
			
			int distance = vector.Distance(player.GetOrigin(), spawn_pos);
		}
			
    }

	override void OnGameStart()
	{
		Print("u loop ext initialized", LogLevel.NORMAL);
		GetGame().GetCallqueue().CallLater(this.u_loop, 120000, true);
	}

};

