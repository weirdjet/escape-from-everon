class WJ_LootSpawnHelper
{
	// Map of all WJ_LootableHouses
	protected ref array<IEntity> m_eEntities;
	
	// Config
	protected ref WJ_LootSpawnConfig m_Config;
	
	// Lootable house prefab set
	protected ref set<ResourceName> m_sLootableHouseNames;
	
	void WJ_LootSpawnHelper(WJ_LootSpawnConfig config)
	{
		m_Config = config;
		m_sLootableHouseNames = m_Config.GetHouseResourceNames();
	}
	
	array<IEntity> GetAllLootableEntitiesInWorld()
	{
		m_eEntities = new array<IEntity>();
		
		BaseWorld world = GetGame().GetWorld(); 
		if (!world) return m_eEntities;

		vector regionMins, regionMaxs;
		world.GetBoundBox(regionMins, regionMaxs);
		
		world.QueryEntitiesByAABB(regionMins, regionMaxs, AddEntity);
		
		return m_eEntities;
	}
	
	array<IEntity> GetAllLootableEntitiesAroundPlayer(int playerId, float radius)
	{
		m_eEntities = new array<IEntity>();
		
		BaseWorld world = GetGame().GetWorld(); 
		if (!world) return m_eEntities;
		
		IEntity entity = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId);
		if (!entity) return m_eEntities;
		
		world.QueryEntitiesBySphere(entity.GetOrigin(), radius, AddEntity);
		
		return m_eEntities;
	}
	
	protected bool AddEntity(IEntity entity)
	{
		EntityPrefabData prefabData = entity.GetPrefabData();
		if (!prefabData) return true;

		if (m_sLootableHouseNames.Contains(prefabData.GetPrefabName())) {
			m_eEntities.Insert(entity);
		}

		return true;
	}
}