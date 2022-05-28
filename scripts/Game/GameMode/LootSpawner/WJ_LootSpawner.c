enum WJ_ELootCategory
{
	Civilian,
	Military,
	Medical,
	Industrial,
	Empty
}

[EntityEditorProps(category: "GameScripted/GameMode/LootSpawner", description:"Spawn loot", dynamicBox: true)]
class WJ_LootSpawnerClass: GenericEntityClass
{
}

class WJ_LootSpawner : GenericEntity
{
	// TODO: Make configurable!
	protected const ResourceName LOOT_SPAWN_CONFIG = "{5C73F171217088A5}Configs/LootSpawns/WJ_LootSpawns.conf";
	protected ref WJ_LootSpawnConfig m_LootSpawnConfig;
	
	// Lootable houses
	protected ref map<EntityID, ref WJ_LootableHouse> m_mHouses = new map<EntityID, ref WJ_LootableHouse>();

	// Helper class
	protected ref WJ_LootSpawnHelper m_Helper;
	
	// Constructor
	void WJ_LootSpawner(IEntitySource src, IEntity parent)
	{
		SetEventMask(EntityEvent.INIT | EntityEvent.FRAME);
		SetFlags(EntityFlags.ACTIVE, true);
    }
	
	// Sets up config
	void SetupConfig()
	{
		m_LootSpawnConfig = WJ_LootSpawnConfig.Cast(
		 	BaseContainerTools.CreateInstanceFromContainer(
				BaseContainerTools.LoadContainer(
					LOOT_SPAWN_CONFIG 
				)
					.GetResource()
					.ToBaseContainer() 
			)
		);
	}
	
	// Gets ran when entity is initialized
	override protected void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		SetupConfig();
		
		m_Helper = new WJ_LootSpawnHelper(m_LootSpawnConfig);
		
		// Loads all houses
		GetGame().GetCallqueue().CallLater(LoadHouses, 2000);
		GetGame().GetCallqueue().CallLater(SpawnLoot, 5000);
	}
	
	// Loads all houses that can be looted
	void LoadHouses()
	{
		array<IEntity> entities = m_Helper.GetAllLootableEntitiesInWorld();
		foreach(IEntity entity : entities) {
			m_mHouses.Insert(entity.GetID(), new WJ_LootableHouse(entity, m_LootSpawnConfig))
		}
		
		Log("Loaded " + m_mHouses.Count() + " houses.");
	}
	
	// Tries to spawn loot for each player
	void SpawnLoot()
	{
		// Get all connected player ids
		array<int> playerIds = {};
		GetGame().GetPlayerManager().GetPlayers(playerIds);
		
		// Loop over each player
		foreach(int playerId : playerIds)
		{
			Log("Spawning for PLAYER ID: " + playerId);
			
			//chatComponent
			SpawnLootForPlayer(playerId);
		}

		GetGame().GetCallqueue().CallLater(SpawnLoot, m_LootSpawnConfig.GetScanInterval() * 1000);
	}
	
	// Tries to spawn loot for an individual player
	void SpawnLootForPlayer(int playerId)
	{
		array<IEntity> entities = m_Helper.GetAllLootableEntitiesAroundPlayer(playerId, m_LootSpawnConfig.GetScanRadius());

		Log("Spawning loot in " + entities.Count() + " houses for PLAYER ID " + playerId);
		
		foreach(IEntity entity : entities)
		{
			WJ_LootableHouse lootableHouse = m_mHouses.Get(entity.GetID());
			lootableHouse.SpawnLoot();
		}
	}
	
	// Log stuff
	void Log(string message)
	{
		#ifdef DEBUG
		Print("LOOT: " + message);
		#endif
	}
}

