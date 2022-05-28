class WJ_LootableHouse
{
	// House entity
	protected IEntity m_eHouseEntity;
	
	// Config
	protected ref WJ_LootSpawnConfig m_Config;
	
	protected ref WJ_HouseConfig m_HouseConfig;
	
	// All entities currently spawned in this house
	protected ref array<IEntity> m_aEntities = new array<IEntity>();
	
	// The tick count since the last time loot was spawned
	protected float m_fLastSpawnTickCount = 0;
	
	void WJ_LootableHouse(IEntity entity, WJ_LootSpawnConfig lootSpawnConfig)
	{
		m_Config = lootSpawnConfig;
		m_eHouseEntity = entity;
		
		m_Config.GetHouseConfigForEntity(entity, m_HouseConfig);
	}
	
 	void SpawnLoot()
	{
		// Releases all touched loot (loot that has been picked up by a player)
		ReleaseTouchedLoot();
		
		float tickCount =  System.GetTickCount();
		
		// Check if enough time has passed since last spawn 
		if (tickCount < (m_fLastSpawnTickCount + m_Config.GetLootCooldownTickCount())) return;
		
		// All spawn points		
		array<vector> spawnPoints = m_HouseConfig.GetSpawnPoints();
		
		// Check if there is no loot items already spawned
		if (m_aEntities.Count() == spawnPoints.Count()) return;
		
		m_fLastSpawnTickCount = tickCount;
		
		// Set of already spawned loot names
		set<ResourceName> spawnedLootNames = GetSpawnedLootNames();
		
		// Loop over remaining amount of slots for this house
		for(int i = 0; i < spawnPoints.Count()-m_aEntities.Count(); i++)
		{
			vector spawnPoint = spawnPoints[i];
			
			// Get the tier
			WJ_TierConfig tierConfig;
			m_Config.GetTierWithName(m_HouseConfig.GetTierName(), tierConfig);
			
			array<ref WJ_ItemConfig> items = m_Config.GetItems(tierConfig.GetRandomCategory());
			if (items.Count() == 0) continue;

			// Get a random item to spawn. Try at least 5 times to get a distinct item. If it pulls the same item 5 times, just accept the fact
			// that in this case we're goingt o have a duplicate item in the same house.
			int spawnAttempts = 1;
			WJ_ItemConfig item = items.GetRandomElement();
			while (spawnedLootNames.Contains(item.GetPrefabName()) && spawnAttempts < 5) {
				item = items.GetRandomElement();
				spawnAttempts++;
			}

			// Entity we are spawning
			IEntity entity = CreateEntityFromItem(item, spawnPoint);
			
			m_aEntities.Insert(entity);
			
			// Add prefab name to already spawned loot
			spawnedLootNames.Insert(item.GetPrefabName());
		}
	}
	
	// Returns a set of all spawned loot's names
	set<ResourceName> GetSpawnedLootNames()
	{
		set<ResourceName> resourceNames = new set<ResourceName>();
		
		foreach(IEntity entity : m_aEntities)
		{
			EntityPrefabData prefabData = entity.GetPrefabData();
			if (prefabData) {
				resourceNames.Insert(prefabData.GetPrefabName());
			}
		}
		
		return resourceNames;
	}
	
	// Creates an entity in the world from our item config
	protected IEntity CreateEntityFromItem(WJ_ItemConfig item, vector spawnPoint)
	{
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.LOCAL;
		params.Parent = m_eHouseEntity;
		
		vector transform[4];
		m_eHouseEntity.GetWorldTransform(transform);
		transform[3] = spawnPoint;
		transform[3][1] = transform[3][1] + 0.3;
		
		params.Transform = transform;
		
		Resource resource = Resource.Load( item.GetPrefabName() );
		IEntity spawnedItem = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);

		spawnedItem.SetAngles(item.GetAngles());
		
		return spawnedItem;
	}
	
	protected void ReleaseTouchedLoot()
	{
		for(int i = 0; i < m_aEntities.Count(); i++)
		{
			vector origin = m_aEntities[i].GetOrigin();
			if (vector.Distance(origin, m_eHouseEntity.GetOrigin()) > 20) m_aEntities.Remove(i);
		}
	}
	
	// Destroys all spawned loot entities
	protected void DestroySpawnedLoot()
	{
		foreach(IEntity entity : m_aEntities)
		{
			delete entity;
		}
		
		m_aEntities = new array<IEntity>();
	}
}

