[EntityEditorProps(category: "GameScripted/GameMode/LootSpawner", description: "Loot Spawn Trigger", color: "0 0 255 255")]
class WJ_TriggerLootComponentClass: ScriptComponentClass
{
};

class WJ_TriggerLootComponent : ScriptComponent
{
	[Attribute("100", UIWidgets.EditBox, "Radius from user to check for lootable houses")]
	protected float m_fTriggerRadius;
	
	[Attribute("100", UIWidgets.EditBox, "How often we should try spawning loot (minutes)")]
	protected int m_iScanIntervalMinutes;
	
	// A map of all lootable houses in the area since last check
	//protected map<EntityID, WJ_House> m_mHouses = new map<EntityID, WJ_House>;
	
	// Ref to world
	//protected ref BaseWorld m_World;
	
	// Ref to player
	//protected ref PlayerController m_Player;
	
	override protected void OnPostInit(IEntity owner)
	{
		//super.OnPostInit(owner);
		//SpawnLoot();
	}
	
	protected void SpawnLoot()
	{

	}
	
	bool AddHouse(IEntity entity)
	{
		
	}
	
	override protected void OnDelete(IEntity owner)
	{
		//GetGame().GetCallqueue().Remove(SpawnLoot);
	}
}