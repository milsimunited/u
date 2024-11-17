class GameMode_Editor_Full_Class: SCR_GameModeEditor 
{
	// user script
	protected ref array<IEntity> spawned_blufor = {}; // NOT const!!! // about arrays: scripts/Core/proto/Types.c
	protected ref array<IEntity> spawned_opfor = {};
	protected ref array<int> to_delete = {};
	
	
	void u_loop()
    {
		protected PlayerManager playerManager = GetGame().GetPlayerManager();
		protected array<int> allPlayers = {};
		playerManager.GetPlayers(allPlayers);
		protected EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		
		protected Resource resource_blufor = Resource.Load("{938AAB9744DC2546}Prefabs/Groups/BLUFOR/Group_BW_Squad_Flecktarn.et");
		protected Resource resource_blufor_02 = Resource.Load("{DA70F249A8B7A366}Prefabs/Groups/BLUFOR/RHS_USAF/RHS_USAF_MARSOC/Group_USAF_USMC_MARSOC_RifleSquad.et");
		protected Resource resource_opfor = Resource.Load("{0ACD74AD27EEEE7D}Prefabs/Groups/OPFOR/RHS_AFRF/MSV/VKPO_Demiseason/Group_RHS_RF_MSV_VKPO_DS_FireGroup.et");
		protected Resource resource_opfor_02 = Resource.Load("{0ACD74AD27EEEE7D}Prefabs/Groups/OPFOR/RHS_AFRF/MSV/VKPO_Demiseason/Group_RHS_RF_MSV_VKPO_DS_FireGroup.et");
		
		Print("u loop ext execute", LogLevel.NORMAL);
		
		// BLUFOR
		// foreach (int index, int hostile : spawned)
		for (int i = 0; i < spawned_blufor.Count(); i++)
		{
			bool too_far = true;
			foreach (int oneplayerid : allPlayers)
			{
				protected IEntity player = playerManager.GetPlayerControlledEntity(oneplayerid);
				if ( spawned_blufor[i] != NULL )
				{
					if ( vector.Distance(spawned_blufor[i].GetOrigin(), player.GetOrigin()) < 350 )
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
				protected IEntity player = playerManager.GetPlayerControlledEntity(oneplayerid);
				if ( spawned_opfor[i] != NULL )
				{
					if ( vector.Distance(spawned_opfor[i].GetOrigin(), player.GetOrigin()) < 350 )
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
			protected IEntity player = playerManager.GetPlayerControlledEntity(oneplayerid);
			protected vector spawn_pos = SCR_Math2D.GenerateRandomPointInRadius(175, 350, player.GetOrigin(), true);
			bool too_close = false;
			foreach (int oneplayerid2 : allPlayers)
			{
				protected IEntity player2 = playerManager.GetPlayerControlledEntity(oneplayerid2);
				if ( vector.Distance(player2.GetOrigin(), spawn_pos) < 150 )
				{
					too_close = true;
				}
			}
			
			if ( vector.Distance(GetGame().GetWorld().FindEntityByName("u_base").GetOrigin(), spawn_pos) < 500 )
			{
				too_close = true;
			}
			
			if ( !too_close )
			{
				//params.Transform = {"1 0 0","0 1 0","0 0 1",spawn_pos};
				params.Transform[3] = spawn_pos;
				protected int random = Math.RandomInt(0, 2);
				
				// SCR_DebugEditorComponent.c
				// ResourceName wp_prefab = "{FFF9518F73279473}PrefabsEditable/Auto/AI/Waypoints/E_AIWaypoint_Move.et";
				ResourceName wp_prefab = "{D9C14ECEC9772CC6}PrefabsEditable/Auto/AI/Waypoints/E_AIWaypoint_Defend.et";
				EntitySpawnParams spawnParams = new EntitySpawnParams();
				spawnParams.Transform[3] = spawn_pos; // player.GetOrigin()
				AIWaypoint wp =  AIWaypoint.Cast(GetGame().SpawnEntityPrefab(Resource.Load(wp_prefab), GetGame().GetWorld(), spawnParams));
				
				if(spawned_blufor.Count() < 18)
				{
					protected IEntity blufor = GetGame().SpawnEntityPrefab(resource_blufor, GetGame().GetWorld(), params);
					spawned_blufor.Insert(blufor);
					
					protected AIGroup group_blufor = AIGroup.Cast(blufor);
					group_blufor.AddWaypoint(wp);
					
					if(random == 0)
					{
						protected IEntity blufor_02 = GetGame().SpawnEntityPrefab(resource_blufor_02, GetGame().GetWorld(), params);
						spawned_blufor.Insert(blufor_02);
						
						protected AIGroup group_blufor_02 = AIGroup.Cast(blufor_02);
						group_blufor_02.AddWaypoint(wp);
					}
				}
				
				if(spawned_opfor.Count() < 20)
				{
					protected IEntity opfor = GetGame().SpawnEntityPrefab(resource_opfor, GetGame().GetWorld(), params);
					spawned_opfor.Insert(opfor);
					
					protected AIGroup group_opfor = AIGroup.Cast(opfor);
					group_opfor.AddWaypoint(wp);
					
					if(random == 1)
					{
						protected IEntity opfor_02 = GetGame().SpawnEntityPrefab(resource_opfor_02, GetGame().GetWorld(), params);
						spawned_opfor.Insert(opfor_02);
					
						protected AIGroup group_opfor_02 = AIGroup.Cast(opfor);
						group_opfor_02.AddWaypoint(wp);
					}
					//Print("random: " + random, LogLevel.NORMAL);
				}
			}
			
			int distance = vector.Distance(player.GetOrigin(), spawn_pos);
		}
			
    }

	override void OnGameStart()
	{
		Print("u loop ext initialized", LogLevel.NORMAL);
		GetGame().GetCallqueue().CallLater(this.u_loop, 240000, true);
	}

};

