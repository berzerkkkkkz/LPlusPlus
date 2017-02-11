#include "PluginSDK.h";
#include <locale>
#include "SimpleLib.h"

PluginSetup("FedMundo - zFederaL")

IMenu* MainMenu;

IMenu* ComboSettings;
IMenuOption* ComboQ;
IMenuOption* QHealthCombo;
IMenuOption* WHealthCombo;
IMenuOption* ComboW;
IMenuOption* ComboE;

IMenu* HarassSettings;
IMenuOption* useQHarassHP;
IMenuOption* useQHarass;
IMenuOption* useQHarassToggle;
IMenuOption* HarassE;

IMenu* MiscSettings;
IMenuOption* handleW;
IMenuOption* useR;
IMenuOption* RHealth;
IMenuOption* RHealthEnemies;
IMenuOption* AutoQGapcloser;

IMenu* KillstealSettings;
IMenuOption* KillstealQ;
IMenuOption* Killsteal;

IMenu* LastHitSettings;
IMenuOption* useQlh;
IMenuOption* useQlhHP;
IMenuOption* qRange;

IMenu* LaneClearSettings;
IMenuOption* useQlc;
IMenuOption* useQlcHP;
IMenuOption* useWlcHP;
IMenuOption* useWlc;
IMenuOption* useWlcMinions;
IMenuOption* useQj;
IMenuOption* useQjHP;
IMenuOption* useWj;
IMenuOption* useWjHP;
IMenuOption* useEj;

IMenu* DrawingSettings;
IMenuOption* DrawReady;
IMenuOption* DrawQ;

IUnit* myHero;

ISpell2* Q;
ISpell2* W;
ISpell2* E;
ISpell2* R;

void InitializeSpells()
{
	Q = SimpleLib::SimpleLib::LoadSkillshot('Q', 0.275, 1000, 2000, 60, kLineCast, true, false, static_cast<eCollisionFlags>(kCollidesWithHeroes | kCollidesWithMinions | kCollidesWithYasuoWall));	
	W = GPluginSDK->CreateSpell2(kSlotW, kTargetCast, false, false, kCollidesWithNothing);
	E = GPluginSDK->CreateSpell2(kSlotE, kTargetCast, false, false, kCollidesWithNothing);
	R = GPluginSDK->CreateSpell2(kSlotR, kTargetCast, false, false, kCollidesWithNothing);
}

void InitializeMenu()
{
	MainMenu = GPluginSDK->AddMenu("FedMundo++");

	ComboSettings = MainMenu->AddMenu("Combo Settings");
	{		
		ComboQ = ComboSettings->CheckBox("Use Q", true);
		QHealthCombo = ComboSettings->AddInteger("Minimum HP% to use Q", 1, 20, 10);
		ComboW = ComboSettings->CheckBox("Use W", true);
		WHealthCombo = ComboSettings->AddInteger("Minimum HP% to use W", 1, 25, 20);
		ComboE = ComboSettings->CheckBox("Use E", true);
	}

	HarassSettings = MainMenu->AddMenu("Harass Settings");
	{
		useQHarass = HarassSettings->CheckBox("Use Q", true);
		useQHarassHP = HarassSettings->AddInteger("Minimum HP% to use Q", 1, 20, 10);
		useQHarassToggle = HarassSettings->CheckBox("Auto Q on enemies", false);
		HarassE = HarassSettings->CheckBox("Use E", false);
	}

	KillstealSettings = MainMenu->AddMenu("Killsteal Settings");
	{
		Killsteal = KillstealSettings->CheckBox("Activate KillSteal", true);
		KillstealQ = KillstealSettings->CheckBox("Use Q to KillSteal", true);		
	}

	MiscSettings = MainMenu->AddMenu("Misc Settings");
	{
		AutoQGapcloser = MiscSettings->CheckBox("Automatically Q GapCloser", true);
		handleW = MiscSettings->CheckBox("Automatically handle W", true);
		useR = MiscSettings->CheckBox("Automatically use R", true);		
		RHealthEnemies = MiscSettings->CheckBox("If enemies nearby", true);
		RHealth = MiscSettings->AddInteger("Minimum HP% to use R", 1, 60, 20);
	}

	LastHitSettings = MainMenu->AddMenu("Last Hit Settings");
	{
		useQlh = LastHitSettings->CheckBox("Use Q to last hit minions", true);
		useQlhHP = LastHitSettings->AddInteger("Minimum HP% to use Q to lasthit", 1, 60, 40);
		qRange = LastHitSettings->CheckBox("Only use Q if far from minions", true);
	}

	LaneClearSettings = MainMenu->AddMenu("Lane Clear Settings");
	{
		useQlc = LaneClearSettings->CheckBox("Use Q to last hit in laneclear", true);
		useQlcHP = LaneClearSettings->AddInteger("Minimum HP% to use Q to laneclear", 1, 60, 40);
		useWlc = LaneClearSettings->CheckBox("Use W in laneclear", true);
		useWlcHP = LaneClearSettings->AddInteger("Minimum HP% to use W to laneclear", 1, 60, 40);
		useWlcMinions = LaneClearSettings->AddInteger("Minimum minions to W in laneclear", 1, 10, 4);
		useQj = LaneClearSettings->CheckBox("Use Q to jungle", true);
		useQjHP = LaneClearSettings->AddInteger("Minimum HP% to use Q in jungle", 1, 60, 20);
		useWj = LaneClearSettings->CheckBox("Use W to jungle", true);
		useWjHP = LaneClearSettings->AddInteger("Minimum HP% to use W to jungle", 1, 60, 20);
		useEj = LaneClearSettings->CheckBox("Use E to jungle", true);		
	}

	DrawingSettings = MainMenu->AddMenu("Drawing Settings");
	{
		DrawReady = DrawingSettings->CheckBox("Draw Only Ready Spells", true);
		DrawQ = DrawingSettings->CheckBox("Draw Q", true);		
	}
}

bool IsBurning()
{
	return myHero->HasBuff("BurningAgony");
}

bool FoundEnemies(IUnit* source, float range)
{	
	for (auto enemys : GEntityList->GetAllHeros(false, true))
	{
		if (source->IsValidTarget(enemys, range))
			return true;
	}

	return false;				
}

void Draw()
{
	if (DrawReady->Enabled())
	{
		if (Q->IsReady() && DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), Q->Range()); }		
	}
	else
	{
		if (DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), Q->Range()); }		
	}

}

void Automatic()
{
	// Killsteal
	if (Killsteal->Enabled() && KillstealQ->Enabled() && Q->IsReady())
	{
		for (auto hero : GEntityList->GetAllHeros(false, true))
		{
			if (hero != nullptr && hero->IsValidTarget(myHero, Q->Range()) && !hero->IsInvulnerable())
			{
				auto damage = GHealthPrediction->GetKSDamage(hero, kSlotQ, Q->GetDelay(), true);

				if (damage > hero->GetHealth()){

					Q->CastOnTarget(hero, kHitChanceMedium);
				}
			}
		}
	}

	// Auto Harass
	if (useQHarassToggle->Enabled() && useQHarass->Enabled() && Q->IsReady() && useQHarassHP->GetInteger() < myHero->HealthPercent())
	{
		auto qTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());
		if (qTarget != nullptr && qTarget->IsValidTarget()
			&& SimpleLib::SimpleLib::GetDistance(myHero, qTarget) < Q->Range())
		{
			Q->CastOnTarget(qTarget, kHitChanceHigh);
		}
	}

	// BurningManager
	if (handleW->Enabled() && IsBurning() && W->IsReady())
	{
		W->CastOnPlayer();
	}

	// Auto R Low % HP
	if (R->IsReady() && useR->Enabled() && myHero->HealthPercent() <= RHealth->GetInteger())
	{
		if (!RHealthEnemies->Enabled())
		{
			R->CastOnPlayer();			
		}
		else
		{			
			if (RHealthEnemies->Enabled() && FoundEnemies(myHero, Q->Range()))
			{
				R->CastOnPlayer();				
			}			
		}		
	}	
}

void Combo()
{
	if (ComboQ->Enabled() && Q->IsReady() && myHero->HealthPercent() > QHealthCombo->GetInteger())
	{
		auto qTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());
		if (qTarget != nullptr && qTarget->IsValidTarget()
			&& SimpleLib::SimpleLib::GetDistance(myHero, qTarget) < Q->Range())
		{
			Q->CastOnTarget(qTarget, kHitChanceMedium);
		}
	}

	if (ComboW->Enabled() && W->IsReady() && myHero->HealthPercent() > WHealthCombo->GetInteger() && !IsBurning() && FoundEnemies(myHero, 400))
	{
		W->CastOnPlayer();
	}
	else if (ComboW->Enabled() && W->IsReady() && IsBurning() && !FoundEnemies(myHero, 500))
	{
		W->CastOnPlayer();
	}
}

void Mixed()
{
	if (useQHarass->Enabled() && Q->IsReady() && myHero->HealthPercent() > useQHarassHP->GetInteger())
	{
		auto qTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());
		if (qTarget != nullptr && qTarget->IsValidTarget()
			&& SimpleLib::SimpleLib::GetDistance(myHero, qTarget) < Q->Range())
		{
			Q->CastOnTarget(qTarget, kHitChanceMedium);
		}
	}
}

void LastHit()
{
	if (useQlh->Enabled() && Q->IsReady() && myHero->HealthPercent() >= useQlhHP->GetInteger())
	{
		for (auto minion : GEntityList->GetAllMinions(false, true, false))
		{ 
			if (minion != nullptr && minion->IsEnemy(myHero) && !minion->IsDead() && myHero->IsValidTarget(minion, Q->Range()))
			{
				auto damage = GHealthPrediction->GetKSDamage(minion, kSlotQ, Q->GetDelay(), true);

				if (damage > minion->GetHealth())
				{
					if (qRange->Enabled() && SimpleLib::SimpleLib::GetDistance(myHero, minion) > 400)
					{
						Q->CastOnUnit(minion);
					}
					else if (!qRange->Enabled())
					{
						Q->CastOnUnit(minion);
					}
				}
			}
		}
	}
}

void JungleClear()
{
	if (useQj->Enabled() && Q->IsReady() || useWj->Enabled() && W->IsReady())
	{
		for (auto jMinion : GEntityList->GetAllMinions(false, false, true))
		{
			if (jMinion != nullptr && !jMinion->IsDead())
			{
				if (useQj->Enabled() && Q->IsReady() && myHero->IsValidTarget(jMinion, Q->Range()) && myHero->HealthPercent() >= useQjHP->GetInteger())
				{					
					Q->CastOnUnit(jMinion);					
				}

				if (useWj->Enabled() && W->IsReady() && !IsBurning() && myHero->IsValidTarget(jMinion, W->Range()) && myHero->HealthPercent() >= useWjHP->GetInteger())
				{
					W->CastOnPlayer();
				}
				else if (useWj->Enabled() && W->IsReady() && IsBurning() && !myHero->IsValidTarget(jMinion, W->Range() + 100))
				{
					W->CastOnPlayer();
				}
			}
		}
	}
}
	

void LaneClear()
{
	if (useQlc->Enabled() && Q->IsReady() || useWlc->Enabled() && W->IsReady())
	{
		for (auto minion : GEntityList->GetAllMinions(false, true, false))
		{
			if (minion != nullptr && minion->IsEnemy(myHero) && !minion->IsDead() && myHero->IsValidTarget(minion, Q->Range()))
			{
				Vec3 pos;
				int hit;
				GPrediction->FindBestCastPosition(W->Range(), W->Radius() + 50, false, true, false, pos, hit);

				if (useWlc->Enabled() && W->IsReady() && !IsBurning() && hit >= useWlcMinions->GetInteger() && myHero->HealthPercent() >= useWlcHP->GetInteger())
				{
					W->CastOnPlayer();
				}
				else if (useWlc->Enabled() && W->IsReady() && IsBurning() && hit < useWlcMinions->GetInteger())
				{
					W->CastOnPlayer();
				}

				if (useQlc->Enabled() && Q->IsReady() && myHero->HealthPercent() > useQlcHP->GetInteger())
				{
					auto damage = GHealthPrediction->GetKSDamage(minion, kSlotQ, Q->GetDelay(), true);

					if (damage > minion->GetHealth())
					{
						Q->CastOnUnit(minion);
					}
				}
			}
		}
	}
}

PLUGIN_EVENT(void) OnGameUpdate()
{
	if (GEntityList->Player()->IsDead())
		return;
	
	switch (GOrbwalking->GetOrbwalkingMode())
	{
	case kModeCombo:
		Combo();
		break;
	case kModeMixed:
		Mixed();
		LastHit();
		break;
	case kModeLaneClear:
		LaneClear();
		JungleClear();
		break;
	}

	Automatic();
}

PLUGIN_EVENT(void) OnGapcloser(GapCloserSpell const& args)
{
	if (args.Sender->IsEnemy(myHero) && args.Sender->IsHero())
	{
		if (AutoQGapcloser->Enabled() && Q->IsReady() && !args.IsTargeted && SimpleLib::SimpleLib::GetDistanceVectors(myHero->GetPosition(), args.EndPosition) < Q->Range())
		{
			Q->CastOnTarget(args.Sender);
		}
	}
}

PLUGIN_EVENT(void) OnAfterAttack(IUnit* source, IUnit* target)
{
	if (source != myHero || target == nullptr)
		return;

	switch (GOrbwalking->GetOrbwalkingMode())
	{
	case kModeCombo:
		if (ComboE->Enabled() && E->IsReady())
		{
			E->CastOnPlayer();
		}
		break;
	case kModeMixed:
		if (HarassE->Enabled() && E->IsReady())
		{
			E->CastOnPlayer();
		}
		break;
	case kModeLaneClear:
		for (auto jMinion : GEntityList->GetAllMinions(false, false, true))
		{
			if (jMinion != nullptr && !jMinion->IsDead())
			{
				if (useEj->Enabled() && E->IsReady() && myHero->IsValidTarget(jMinion, W->Range()))
				{
					E->CastOnPlayer();					
				}
			}
		}
		break;
	default:;
	}
}

PLUGIN_EVENT(void) OnBuffAdd(IUnit* source, void* BuffData)
{	
}

PLUGIN_EVENT(void) OnBuffRemove(IUnit* source, void* BuffData)
{	
}

PLUGIN_EVENT(void) OnCreateObject(IUnit* source)
{
	for (IUnit* object : GEntityList->GetAllUnits())
	{
		if (object->IsCreep())
			return;
	}
}

PLUGIN_EVENT(void) OnRender()
{
	Draw();
}

PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)
{
	PluginSDKSetup(PluginSDK);
	InitializeMenu();
	InitializeSpells();
	myHero = GEntityList->Player();

	GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->AddEventHandler(kEventOnRender, OnRender);
	GEventManager->AddEventHandler(kEventOnBuffAdd, OnBuffAdd);
	GEventManager->AddEventHandler(kEventOnBuffRemove, OnBuffRemove);
	GEventManager->AddEventHandler(kEventOrbwalkAfterAttack, OnAfterAttack);
	GEventManager->AddEventHandler(kEventOnGapCloser, OnGapcloser);
}

PLUGIN_API void OnUnload()
{
	MainMenu->Remove();
	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->RemoveEventHandler(kEventOnRender, OnRender);
	GEventManager->RemoveEventHandler(kEventOnBuffAdd, OnBuffAdd);
	GEventManager->RemoveEventHandler(kEventOnBuffRemove, OnBuffRemove);
	GEventManager->RemoveEventHandler(kEventOrbwalkAfterAttack, OnAfterAttack);
	GEventManager->RemoveEventHandler(kEventOnGapCloser, OnGapcloser);
}