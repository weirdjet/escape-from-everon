[BaseContainerProps(configRoot: true)]
class WJ_LootSpawnConfig
{
	[Attribute("100", UIWidgets.EditBox, "Radius from user to check for lootable houses")]
	protected float m_fScanRadius;
	
	[Attribute("2", UIWidgets.EditBox, "How often we should try spawning loot (minutes)")]
	protected int m_iScanIntervalSeconds;
	
	[Attribute("10", UIWidgets.EditBox, "Cooldown between each loot spawn for each house")]
	protected int m_fLootCooldownMinutes;
	
	[Attribute("", UIWidgets.Object, "Houses")]
	protected ref array<ref WJ_HouseConfig> m_aHouses;
	
	[Attribute("", UIWidgets.Object, "Items")]
	ref array<ref WJ_ItemConfig> m_aItems;
	
	[Attribute("", UIWidgets.Object, "Tiers")]
	ref array<ref WJ_TierConfig> m_aTiers;
	
	protected ref set<ResourceName> m_sHouseResourceNames;

	set<ResourceName> GetHouseResourceNames()
	{
		if (m_sHouseResourceNames) return m_sHouseResourceNames;
		
		m_sHouseResourceNames = new set<ResourceName>();
		foreach(WJ_HouseConfig houseConfig : m_aHouses) {
			m_sHouseResourceNames.Insert(houseConfig.GetPrefabName());
		}
		return m_sHouseResourceNames;
	}
	
	// Gets the scan radius for lootable houses
	float GetScanRadius()
	{
		return m_fScanRadius;
	}
	
	// Gets how often we should run the scan
	int GetScanInterval()
	{
		return m_iScanIntervalSeconds;
	}
	
	// Get all configured loot items
	array<ref WJ_ItemConfig> GetItems()
	{
		return m_aItems;
	}
	
	// Get all items by category name
	array<ref WJ_ItemConfig> GetItems(WJ_ELootCategory category)
	{
		array<ref WJ_ItemConfig> items = new array<ref WJ_ItemConfig>();
		foreach(WJ_ItemConfig item : m_aItems)
		{
			if (item.GetCategory() == category) items.Insert(item)
		}
		return items;
	}
	
	// Gets the house config for given entity
	bool GetHouseConfigForEntity(IEntity entity, out WJ_HouseConfig outHouseConfig)
	{
		EntityPrefabData prefabData = entity.GetPrefabData();
		
		foreach(WJ_HouseConfig houseConfig : m_aHouses)
		{
			if (houseConfig.GetPrefabName() == prefabData.GetPrefabName())
			{
				outHouseConfig = houseConfig;
				return true;
			}
		}
		
		return false;
	}
	
	// Gets the tier with specified tier name
	bool GetTierWithName(string tierName, out WJ_TierConfig outTierConfig)
	{
		foreach(WJ_TierConfig tierConfig : m_aTiers)
		{
			if (tierConfig.GetTierName() == tierName)
			{
				outTierConfig = tierConfig;
				return true;
			}
		}
		
		return false;
	}
	
	// Gets the amount of ticks between each loot cooldown
	float GetLootCooldownTickCount()
	{
		return m_fLootCooldownMinutes * 60000;
	}
	
	void ~WJ_LootSpawnConfig()
	{
		m_aHouses = null;
		m_aItems = null;
		m_aTiers = null;
	}
};

//------------------------------------------------------------------------------------------------
[BaseContainerProps("House")]
class WJ_HouseConfig
{
	[Attribute(defvalue: "", uiwidget: UIWidgets.ResourceNamePicker, desc: "Prefab for this house")]
	protected ResourceName m_sPrefabName;

	[Attribute(defvalue: "", uiwidget: UIWidgets.EditBox, desc: "Loot Tier name this house spawns")]
	string m_sTierName;
	
	[Attribute("", uiwidget: UIWidgets.Coords, desc: "Position of this spawn point relative to the house prefab")]
	protected ref array<vector> m_vCoords;
	
	ResourceName GetPrefabName()
	{
		return m_sPrefabName;
	}
	
	// Returns all coordinates
	array<vector> GetSpawnPoints()
	{
		return m_vCoords;
	}
	
	// Get tier name
	string GetTierName()
	{
		return this.m_sTierName;
	}
};

//------------------------------------------------------------------------------------------------
[BaseContainerProps("Item")]
class WJ_ItemConfig
{
	[Attribute(defvalue: "", uiwidget: UIWidgets.ResourceNamePicker, desc: "Prefab for this item")]
	protected ResourceName m_sPrefabName;
	
	[Attribute("0", UIWidgets.ComboBox, "", enums: ParamEnumArray.FromEnum(WJ_ELootCategory))]
	protected WJ_ELootCategory m_ELootCategory;
	
	[Attribute("", uiwidget: UIWidgets.Coords, desc: "Orientation of the item")]
	protected vector m_vAngles;
	
	// Gets the intended orienttation of this item
	vector GetAngles()
	{
		return m_vAngles;
	}
	
	ResourceName GetPrefabName()
	{
		return m_sPrefabName;
	}
	
	WJ_ELootCategory GetCategory()
	{
		return m_ELootCategory;
	}
};

//------------------------------------------------------------------------------------------------
[BaseContainerProps("Tier")]
class WJ_TierConfig
{
	[Attribute(defvalue: "", uiwidget: UIWidgets.EditBox, desc: "Tier Name")]
	protected string m_sName;
	
	[Attribute(defvalue: "", uiwidget: UIWidgets.Slider, desc: "Spawn Civilian loot percentage", "0 100 1")]
	protected int m_bCivilian;
	
	[Attribute(defvalue: "", uiwidget: UIWidgets.Slider, desc: "Spawn Military loot percentage", "0 100 1")]
	protected int m_bMilitary;
	
	[Attribute(defvalue: "", uiwidget: UIWidgets.Slider, desc: "Spawn Industrial loot percentage", "0 100 1")]
	protected int m_bIndustrial;
	
	[Attribute(defvalue: "", uiwidget: UIWidgets.Slider, desc: "Spawn Industrial loot percentage", "0 100 1")]
	protected int m_bMedical;
	
	[Attribute(defvalue: "", uiwidget: UIWidgets.Slider, desc: "Spawn Empty loot percentage", "0 100 1")]
	protected int m_bEmpty;
	
	// Get a random category based on this tiers percentage sliders
	WJ_ELootCategory GetRandomCategory()
	{
		int sum = GetCivilian() + GetMedical() + GetMilitary() + GetIndustrial() + GetEmpty();
		int rand = Math.RandomInt(0, sum);
		
		int total = GetCivilian();
		if (rand <= total) return WJ_ELootCategory.Civilian;
		else total += GetMedical();
		
		if (rand <= total) return WJ_ELootCategory.Medical;
		else total += GetMilitary();
		
		if (rand <= total) return WJ_ELootCategory.Military;
		else total += GetIndustrial();
		
		if (rand <= total) return WJ_ELootCategory.Industrial;
		
		return WJ_ELootCategory.Empty;
	}
	
	// Get tier name
	string GetTierName()
	{
		return m_sName;
	}
	
	int GetCivilian()
	{
		return m_bCivilian;
	}
	
	int GetMilitary()
	{
		return m_bMilitary;
	}
	
	int GetIndustrial()
	{
		return m_bIndustrial;
	}
	
	int GetMedical()
	{
		return m_bMedical;
	}
	
	int GetEmpty()
	{
		return m_bEmpty;
	}
};
