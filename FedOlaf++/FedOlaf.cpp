#include "PluginSDK.h";
#include <locale>
#include "SimpleLib.h"

PluginSetup("FedOlaf - zFederaL")

IMenu* MainMenu;

IMenu* RSettings;

IMenu* ComboSettings;
IMenuOption* ComboQ;
IMenuOption* gotoAxeC;
IMenuOption* ComboW;
IMenuOption* ComboE;

IMenu* HarassSettings;
IMenuOption* gotoAxeH;
IMenuOption* useQHarass;
IMenuOption* useeH;
IMenuOption* minmanaH;

IMenu* MiscSettings;
IMenuOption* userCCed;
IMenuOption* userbeforeCCed;
IMenuOption* RHealthEnemies;
IMenuOption* gotoAxeMaxDist;
IMenuOption* AutoQGapcloser;
IMenuOption* roption;

IMenu* KillstealSettings;
IMenuOption* KillstealE;
IMenuOption* KillstealQ;
IMenuOption* Killsteal;

IMenu* LastHitSettings;
IMenuOption* useQlh;
IMenuOption* useQlhMP;
IMenuOption* qRange;
IMenuOption* useElh;

IMenu* LaneClearSettings;
IMenuOption* useQlc;
IMenuOption* useElc;
IMenuOption* useElclast;
IMenuOption* useWlcMinions;
IMenuOption* useWlc;
IMenuOption* useQlcMinions;
IMenuOption* useWlcHP;

IMenu* JungleClearSettings;
IMenuOption* useQj;
IMenuOption* minMPlc;
IMenuOption* useWj;
IMenuOption* useWjHP;
IMenuOption* useEj;
IMenuOption* minMPlcJ;

IMenu* DrawingSettings;
IMenuOption* DrawReady;
IMenuOption* DrawQ;
IMenuOption* DrawE;
IMenuOption* drawaxe;
IMenuOption* drawcombo;

IMenu* SkinsChange;
IMenuOption* MiscSkin;

IUnit* myHero;
Vec3 lastQCast;
Vec3 lastQpos;

ISpell2* Q;
ISpell2* W;
ISpell2* E;
ISpell2* R;

void InitializeSpells()
{
	Q = GPluginSDK->CreateSpell2(kSlotQ, kLineCast, true, false, kCollidesWithYasuoWall);
	Q->SetSkillshot(0.20f, 75.f, 1500.f, 1000.f); W = GPluginSDK->CreateSpell2(kSlotW, kTargetCast, false, false, kCollidesWithNothing);
	W = GPluginSDK->CreateSpell2(kSlotW, kTargetCast, false, false, kCollidesWithNothing);
	E = GPluginSDK->CreateSpell2(kSlotE, kTargetCast, false, false, kCollidesWithNothing);
	E->SetOverrideRange(325.f);
	R = GPluginSDK->CreateSpell2(kSlotR, kTargetCast, false, false, kCollidesWithNothing);	
}

void InitializeMenu()
{
	MainMenu = GPluginSDK->AddMenu("FedOlaf++");
	
	ComboSettings = MainMenu->AddMenu("Combo Settings");
	{				
		ComboQ = ComboSettings->CheckBox("Use Q", true);
		gotoAxeC = ComboSettings->CheckBox("Catch axe", true);
		ComboW = ComboSettings->CheckBox("Use W", true);		
		ComboE = ComboSettings->CheckBox("Use E", true);
	}

	HarassSettings = MainMenu->AddMenu("Harass Settings");
	{
		useQHarass = HarassSettings->CheckBox("Use Q", true);
		gotoAxeH = HarassSettings->CheckBox("Catch axe", true);
		useeH = HarassSettings->CheckBox("Use E", false);
		minmanaH = HarassSettings->AddInteger("Minimum MP% to Harass", 1, 100, 40);
	}

	KillstealSettings = MainMenu->AddMenu("Killsteal Settings");
	{
		Killsteal = KillstealSettings->CheckBox("Activate KillSteal", true);
		KillstealQ = KillstealSettings->CheckBox("Use Q to KillSteal", true);
		KillstealE = KillstealSettings->CheckBox("Use E to KillSteal", true);
	}

	MiscSettings = MainMenu->AddMenu("Misc Settings");
	{
		AutoQGapcloser = MiscSettings->CheckBox("Automatically Q GapCloser", true);
		userCCed = MiscSettings->CheckBox("Use Q on CC", true);
		userbeforeCCed = MiscSettings->CheckBox("Use before CC", true);		
		gotoAxeMaxDist = MiscSettings->AddInteger("Max dist to catch axe", 200, 600, 450);
		roption = MiscSettings->CheckBox("Option R ToDo", false);
	}

	LastHitSettings = MainMenu->AddMenu("LastHit Settings");
	{
		useQlh = LastHitSettings->CheckBox("Use Q to last hit minions", true);		
		qRange = LastHitSettings->CheckBox("Only use Q if far from minions", true);
		useElh = LastHitSettings->CheckBox("Use E to last hit minions", true);
		useQlhMP = LastHitSettings->AddInteger("Minimum MP% to lasthit", 1, 100, 40);

	}

	LaneClearSettings = MainMenu->AddMenu("LaneClear Settings");
	{
		useQlc = LaneClearSettings->CheckBox("Use Q in laneclear", true);
		useQlcMinions = LaneClearSettings->AddInteger("Minimum minions to Q in laneclear", 1, 10, 3);
		useWlc = LaneClearSettings->CheckBox("Use W in laneclear", false);
		useWlcMinions = LaneClearSettings->AddInteger("Minimum minions to W in laneclear", 1, 10, 6);
		useWlcHP = LaneClearSettings->AddInteger("Minimum HP% to use W to laneclear", 1, 100, 100);
		useElc = LaneClearSettings->CheckBox("Use E in laneclear", true);
		useElclast = LaneClearSettings->CheckBox("Use E only kill minion", true);
		minMPlc = LaneClearSettings->AddInteger("Minimum MP% to laneclear", 1, 100, 40);
	}

	JungleClearSettings = MainMenu->AddMenu("JungleClear Settings");
	{		
		useQj = JungleClearSettings->CheckBox("Use Q to jungle", true);
		useWj = JungleClearSettings->CheckBox("Use W to jungle", true);
		useWjHP = JungleClearSettings->AddInteger("Minimum HP% to use W to jungle", 1, 100, 100);
		useEj = JungleClearSettings->CheckBox("Use E to jungle", true);
		minMPlcJ = JungleClearSettings->AddInteger("Minimum MP% to jungle", 1, 100, 20);
	}

	DrawingSettings = MainMenu->AddMenu("Drawing Settings");
	{
		DrawReady = DrawingSettings->CheckBox("Draw Only Ready Spells", true);
		DrawQ = DrawingSettings->CheckBox("Draw Q", true);
		DrawE = DrawingSettings->CheckBox("Draw E", false);
		drawaxe = DrawingSettings->CheckBox("Draw Axe position", true);
		drawcombo = DrawingSettings->CheckBox("Draw combo damage", true);
	}

	SkinsChange = MainMenu->AddMenu("Skins Changer");
	{
		MiscSkin = SkinsChange->AddInteger("Skins", 1, 7, 1);
	}
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
		if (E->IsReady() && DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), E->Range()); }
	}
	else
	{
		if (DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), Q->Range()); }
		if (DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), E->Range()); }
	}

}

void SkinChanger()
{
	if (myHero->GetSkinId() != MiscSkin->GetInteger())
	{
		myHero->SetSkinId(MiscSkin->GetInteger());
	}
}

bool FoundMinionsNeutral(float range)
{
	for (auto Minions : GEntityList->GetAllMinions(false, false, true))
	{
		if (myHero->IsValidTarget(Minions, range))
			return true;
	}

	return false;
}

bool FoundMinions(float range)
{
	for (auto Minions : GEntityList->GetAllMinions(false, true, false))
	{
		if (myHero->IsValidTarget(Minions, range))
			return true;
	}

	return false;
}

void Automatic()
{
	// Killsteal
	if (Killsteal->Enabled())
	{
		for (auto hero : GEntityList->GetAllHeros(false, true))
		{
			// With Q
			if (KillstealQ->Enabled() && Q->IsReady() && hero != nullptr && hero->IsValidTarget(myHero, Q->Range() - 50) && !hero->IsInvulnerable() && !hero->IsDead())
			{
				auto damage = GHealthPrediction->GetKSDamage(hero, kSlotQ, Q->GetDelay(), false);

				if (damage + 50 > hero->GetHealth()){

					Vec3 position;
					GPrediction->GetFutureUnitPosition(hero, 1, true, position);

					Q->CastOnPosition(position);
				}
			}

			// With E
			else if (KillstealE->Enabled() && E->IsReady() && hero != nullptr && hero->IsValidTarget(myHero, E->Range()) && !hero->IsInvulnerable() && !hero->IsDead())
			{
				auto damage = GHealthPrediction->GetKSDamage(hero, kSlotE, E->GetDelay(), false);

				if (damage + 50 > hero->GetHealth()){

					E->CastOnTarget(hero, kHitChanceMedium);
				}
			}
		}
	}	
}

void CastQ()
{
	if (Q->IsReady())
	{
		auto qTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());
		
		if (qTarget != nullptr && qTarget->IsValidTarget())
		{

			float Distance = SimpleLib::SimpleLib::GetDistance(myHero, qTarget);

			/*float Extend = 0;

			if (Distance < 300) { Extend = 40; }
			else if (Distance >= 300 && Distance < 500){ Extend = 60; }
			else if (Distance >= 500 && Distance < 700){ Extend = 80; }
			else if (Distance >= 700 && Distance < Q->Range()){ Extend = 100; }
			*/

			Vec3 position;
			GPrediction->GetFutureUnitPosition(qTarget, 1, true, position);

			if (Distance < Q->Range())
			{
				Q->CastOnPosition(position);
			}
		}
	}
}

void CastE()
{
	if (E->IsReady())
	{
		auto eTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, E->Range());		

		if (eTarget != nullptr && eTarget->IsValidTarget()
			&& SimpleLib::SimpleLib::GetDistance(myHero, eTarget) <= E->Range())
		{
			E->CastOnTarget(eTarget, kHitChanceMedium);			
		}
	}
}

void Combo()
{
	if (ComboQ->Enabled())
	{
		CastQ();
	}	

	if (ComboE->Enabled())
	{
		CastE();
	}
}

void Mixed()
{
	if (useQHarass->Enabled() && myHero->ManaPercent() >= minmanaH->GetInteger())
	{
		CastQ();
	}	

	if (ComboE->Enabled() && myHero->ManaPercent() >= minmanaH->GetInteger())
	{
		CastE();
	}
}

void LastHit()
{
	if (useQlh->Enabled() && Q->IsReady() && myHero->ManaPercent() >= useQlhMP->GetInteger())
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
						GOrbwalking->ResetAA();
						Q->CastOnUnit(minion);
					}
				}
			}
		}
	}	
	
	if (useElh->Enabled() && E->IsReady() && myHero->ManaPercent() >= useQlhMP->GetInteger())
	{
		for (auto minion : GEntityList->GetAllMinions(false, true, false))
		{
			if (minion != nullptr && minion->IsEnemy(myHero) && !minion->IsDead() && myHero->IsValidTarget(minion, E->Range()))
			{
				auto damage = GHealthPrediction->GetKSDamage(minion, kSlotE, E->GetDelay(), true);

				if (damage > minion->GetHealth())
				{
					GOrbwalking->ResetAA();
					E->CastOnUnit(minion);					
				}
			}
		}
	}
}

void JungleClear()
{
	for (auto minion : GEntityList->GetAllMinions(false, false, true))
	{

		if (useQj->Enabled() && Q->IsReady() && myHero->ManaPercent() >= minMPlcJ->GetInteger() && !FoundMinions(E->Range()) && FoundMinionsNeutral(Q->Range() - 50))
		{

			if (minion != nullptr && !minion->IsDead() && myHero->IsValidTarget(minion, Q->Range()))
			{
				Vec3 posQ;
				int hitQ;
				GPrediction->FindBestCastPosition(Q->Range() - 50, Q->Radius(), true, true, false, posQ, hitQ);

				if (hitQ > 1)
				{
					Q->CastOnPosition(posQ);
				}
				else
				{
					Q->CastOnUnit(minion);
				}
			}
		}

		else if (useEj->Enabled() && E->IsReady() && myHero->ManaPercent() >= minMPlcJ->GetInteger() && !FoundMinions(E->Range()) && FoundMinionsNeutral(Q->Range() - 50))
		{
			if (minion != nullptr && !minion->IsDead() && myHero->IsValidTarget(minion, E->Range()))
			{
				E->CastOnUnit(minion);
			}
		}
	}
}

void LaneClear()
{
	Vec3 posQ, posW;
	int hitQ, hitW;
	GPrediction->FindBestCastPosition(Q->Range() - 50, Q->Radius()+50, true, true, false, posQ, hitQ);
	GPrediction->FindBestCastPosition(500 , 500, false, true, false, posW, hitW);

	if (useQlc->Enabled() && Q->IsReady() && hitQ >= useQlcMinions->GetInteger() && myHero->ManaPercent() >= minMPlc->GetInteger() && !FoundMinionsNeutral(E->Range() + 100))
	{
		Q->CastOnPosition(posQ);		
	}

	if (useWlc->Enabled() && W->IsReady() && hitW >= useWlcMinions->GetInteger() && myHero->ManaPercent() >= minMPlc->GetInteger() && !FoundMinionsNeutral(E->Range() + 100))
	{		
		if (myHero->HealthPercent() <= useWlcHP->GetInteger())
		{
			W->CastOnPlayer();
		}
	}

	if (useElc->Enabled() && E->IsReady() && myHero->ManaPercent() >= minMPlc->GetInteger() && !FoundMinionsNeutral(E->Range() + 100))
	{
		for (auto minion : GEntityList->GetAllMinions(false, true, false))
		{
			if (minion != nullptr && minion->IsEnemy(myHero) && !minion->IsDead() && myHero->IsValidTarget(minion, E->Range()))
			{

				if (useElclast->Enabled())
				{
					auto damage = GHealthPrediction->GetKSDamage(minion, kSlotE, E->GetDelay(), true);

					if (damage > minion->GetHealth())
					{
						GOrbwalking->ResetAA();
						E->CastOnUnit(minion);
					}
				}
				else {
					E->CastOnUnit(minion);
				}
			}
		}
	}	
}

PLUGIN_EVENT(void) OnGameUpdate()
{
	if (myHero->IsDead())
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
	SkinChanger();
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
		if (ComboW->Enabled() && W->IsReady())
		{
			W->CastOnPlayer();
		}
		break;	
	case kModeLaneClear:
		if (useWj->Enabled() && W->IsReady() && FoundMinionsNeutral(myHero->GetRealAutoAttackRange(target)+65))
				{
					if (myHero->HealthPercent() <= useWjHP->GetInteger())
					{
						W->CastOnPlayer();
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


PLUGIN_EVENT(void) OnCreateObject(IUnit* Source)
{
	
}

PLUGIN_EVENT(void) OnProcessSpell(CastedSpell const& Args)

{
	
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
	GEventManager->AddEventHandler(kEventOnSpellCast, OnProcessSpell);
	

	GGame->PrintChat("Olaf by zFederaL loaded!");
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
	GEventManager->RemoveEventHandler(kEventOnSpellCast, OnProcessSpell);
}