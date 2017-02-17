#include "PluginSDK.h";
#include <locale>
#include "SimpleLib.h"

PluginSetup("FedHecarim - zFederaL")

IMenu* MainMenu;

IMenu* ComboSettings;
IMenuOption* useQCombo;
IMenuOption* useWCombo;
IMenuOption* useECombo;
IMenuOption* useRCombo;
IMenuOption* RCount;
IMenuOption* Rsolo;
IMenuOption* RAllys;

IMenu* HarassSettings;
IMenuOption* useQHarassToggle;
IMenuOption* useQHarass;
IMenuOption* minmanaH;

IMenu* MiscSettings;
IMenuOption* userbeforeCCed;
IMenuOption* AutoRInterrupter;
IMenuOption* AutoEInterrupter;
IMenuOption* AutoEGapcloser;

IMenu* KillstealSettings;
IMenuOption* KillstealQ;
IMenuOption* Killsteal;
IMenuOption* KillstealR;
IMenuOption* KillstealRrange;

IMenu* LastHitSettings;
IMenuOption* useQlh;
IMenuOption* useQlhHP;
IMenuOption* minMPlh;

IMenu* LaneClearSettings;
IMenuOption* useQlc;
IMenuOption* useElc;
IMenuOption* useWHP;
IMenuOption* useWlc;
IMenuOption* useQlcMinions;
IMenuOption* useWlcMinions;

IMenu* JunglerSettings;
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
IMenuOption* DrawW;
IMenuOption* DrawR;

IMenu* SkinsChange;
IMenuOption* MiscSkin;

IUnit* myHero;

ISpell2* Q;
ISpell2* W;
ISpell2* E;
ISpell2* R;

void InitializeSpells()
{
	Q = GPluginSDK->CreateSpell2(kSlotQ, kCircleCast, false, true, kCollidesWithNothing);
	Q->SetOverrideRange(350.f);
	W = GPluginSDK->CreateSpell2(kSlotW, kTargetCast, false, false, kCollidesWithNothing);
	W->SetOverrideRange(525.f);
	E = GPluginSDK->CreateSpell2(kSlotE, kTargetCast, false, false, kCollidesWithNothing);
	E->SetOverrideRange(1000.f);
	R = GPluginSDK->CreateSpell2(kSlotR, kLineCast, false, true, kCollidesWithNothing);	
	R->SetSkillshot(0.25f, 300.f, 1200.f, 1230.f);	
}

void InitializeMenu()
{
	MainMenu = GPluginSDK->AddMenu("FedHecarim++");
	
	ComboSettings = MainMenu->AddMenu("Combo Settings");
	{				
		useQCombo = ComboSettings->CheckBox("Use Q", true);
		useWCombo = ComboSettings->CheckBox("Use W", true);
		useECombo = ComboSettings->CheckBox("Use E", true);
		useRCombo = ComboSettings->CheckBox("Use R", true);
		Rsolo = ComboSettings->CheckBox("R 1v1 Mode Kill Easy", true);
		RAllys = ComboSettings->CheckBox("Use R if Allys > Enemys", true);
		RCount = ComboSettings->AddInteger("Use R if Counts Enemies >=", 1, 5, 3);		
	}		

	HarassSettings = MainMenu->AddMenu("Harass Settings");
	{
		useQHarass = HarassSettings->CheckBox("Use Q", true);
		useQHarassToggle = HarassSettings->CheckBox("Auto Q Harass", false);
		minmanaH = HarassSettings->AddInteger("Minimum MP% to Harass", 1, 100, 60);
	}

	KillstealSettings = MainMenu->AddMenu("Killsteal Settings");
	{
		Killsteal = KillstealSettings->CheckBox("Activate KillSteal", true);
		KillstealQ = KillstealSettings->CheckBox("Use Q to KillSteal", true);
		KillstealR = KillstealSettings->CheckBox("Use R to KillSteal", true);
		KillstealRrange = KillstealSettings->AddInteger("Use R | If Target Distance >=", 100, 1230, 600);
	}

	MiscSettings = MainMenu->AddMenu("Misc Settings");
	{
		AutoEGapcloser = MiscSettings->CheckBox("Use E Anti GapCloset", true);
		AutoEInterrupter = MiscSettings->CheckBox("Use E Interrupt Spell", true);
		AutoRInterrupter = MiscSettings->CheckBox("Use R Interrupt Spell", true);
		userbeforeCCed = MiscSettings->CheckBox("Use R on CC (ToDO )", false);
	}

	LastHitSettings = MainMenu->AddMenu("Last Hit Settings");
	{
		useQlh = LastHitSettings->CheckBox("Use Q to last hit minions", true);
		minMPlh = LastHitSettings->AddInteger("Minimum MP% to use Q to lasthit", 1, 100, 60);

	}

	LaneClearSettings = MainMenu->AddMenu("Lane Clear Settings");
	{
		useQlc = LaneClearSettings->CheckBox("Use Q in laneclear", true);
		useQlcMinions = LaneClearSettings->AddInteger("Minimum minions to Q in laneclear", 1, 6, 3);
		useWlc = LaneClearSettings->CheckBox("Use W in laneclear", true);
		useWlcMinions = LaneClearSettings->AddInteger("Minimum minions to W in laneclear", 1, 10, 5);
		minMPlc = LaneClearSettings->AddInteger("Minimum MP% to laneclear", 1, 100, 50);
	}

	JunglerSettings = MainMenu->AddMenu("Jungle Clear Settings");
	{		
		useQj = JunglerSettings->CheckBox("Use Q to jungle", true);
		useWj = JunglerSettings->CheckBox("Use W to jungle", true);		
		minMPlcJ = JunglerSettings->AddInteger("Minimum MP% to jungle", 1, 100, 20);
	}

	DrawingSettings = MainMenu->AddMenu("Drawing Settings");
	{
		DrawReady = DrawingSettings->CheckBox("Draw Only Ready Spells", true);
		DrawQ = DrawingSettings->CheckBox("Draw Q", true);
		DrawW = DrawingSettings->CheckBox("Draw W", true);
		DrawE = DrawingSettings->CheckBox("Draw E", true);
		DrawR = DrawingSettings->CheckBox("Draw R", true);		
	}

	SkinsChange = MainMenu->AddMenu("Skins Changer");
	{
		MiscSkin = SkinsChange->AddInteger("Skins", 1, 7, 3);
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

int CountEnemy(Vec3 Location, int range)
{
	int Count = 0;

	for (auto Enemy : GEntityList->GetAllHeros(false, true))
	{
		if ((Enemy->GetPosition() - Location).Length() < range && Enemy->IsValidTarget() && !Enemy->IsDead())
		{
			Count++;
		}
	}
	return (Count);
}

int CountAlly(Vec3 Location, int range)
{
	int Count = 0;

	for (auto Ally : GEntityList->GetAllHeros(true, false))
	{
		if ((Ally->GetPosition() - Location).Length() < range && Ally->IsValidTarget() && !Ally->IsDead() && Ally != myHero)
		{
			Count++;
		}
	}
	return (Count);
}

void Draw()
{
	if (DrawReady->Enabled())
	{
		if (Q->IsReady() && DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), Q->Range()); }
		if (W->IsReady() && DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), W->Range()); }
		if (E->IsReady() && DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), E->Range()); }
		if (R->IsReady() && DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), R->Range()); }
	}
	else
	{
		if (DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), Q->Range()); }
		if (DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), W->Range()); }
		if (DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), E->Range()); }
		if (DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), R->Range()); }
	}
}

void SkinChanger()
{
	if (myHero->GetSkinId() != MiscSkin->GetInteger())
	{
		myHero->SetSkinId(MiscSkin->GetInteger());
	}
}

double ComboDamage(IUnit* enemy)

{
	float AutoDmg = GDamage->GetAutoAttackDamage(myHero, enemy, true) * (1 + myHero->Crit());
	double dmg = 0;

	if (Q->IsReady())
	{
		dmg += GDamage->GetSpellDamage(myHero, enemy, kSlotQ);
	}

	if (W->IsReady())
	{
		dmg += GDamage->GetSpellDamage(myHero, enemy, kSlotW);
	}

	if (E->IsReady())
	{
		dmg += GDamage->GetSpellDamage(myHero, enemy, kSlotE);
	}

	if (R->IsReady())
	{
		dmg += GDamage->GetSpellDamage(myHero, enemy, kSlotR);
	}
		
		dmg += AutoDmg * 2;
	
		//thunder
		//dmg += (10 * myHero->GetLevel()) + (myHero->PhysicalDamage() * 0.30f) + (myHero->MagicDamage() * 0.10f);

	return dmg;
}

void Automatic()
{
	// Killsteal
	if (Killsteal->Enabled())
	{
		for (auto hero : GEntityList->GetAllHeros(false, true))
		{
			if (KillstealQ->Enabled() && Q->IsReady() && hero != nullptr && hero->IsValidTarget(myHero, Q->Range() - 50) && !hero->IsInvulnerable())
			{
				auto damage = GHealthPrediction->GetKSDamage(hero, kSlotQ, Q->GetDelay(), false);

				if (damage+50 > hero->GetHealth()){

					Q->CastOnPlayer();
				}
			}
			else if (KillstealR->Enabled() && R->IsReady() && Q->IsReady() && hero != nullptr && hero->IsValidTarget(myHero, KillstealRrange->GetInteger()) && !hero->IsInvulnerable())
			{
				auto damage = GHealthPrediction->GetKSDamage(hero, kSlotR, R->GetDelay(), false);

				if (damage+50 > hero->GetHealth()){

					R->CastOnTarget(hero, kHitChanceHigh);
				}
			}
		}
	}

	//Auto Q Harass
	if (useQHarassToggle->Enabled() && useQHarass->Enabled() && Q->IsReady() && myHero->ManaPercent() >= minmanaH->GetInteger())
	{
		auto qTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());
		if (qTarget != nullptr && qTarget->IsValidTarget()
			&& SimpleLib::SimpleLib::GetDistance(myHero, qTarget) < Q->Range())
		{
			Q->CastOnTarget(qTarget, kHitChanceMedium);
		}
	}	

}

void Combo()
{
	if (useQCombo->Enabled() && Q->IsReady())
	{
		auto qTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());
		if (qTarget != nullptr && qTarget->IsValidTarget()
			&& SimpleLib::SimpleLib::GetDistance(myHero, qTarget) < Q->Range())
		{
			Q->CastOnTarget(qTarget, kHitChanceMedium);
		}
	}

	if (useWCombo->Enabled() && W->IsReady())
	{
		auto qTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, W->Range());
		if (qTarget != nullptr && qTarget->IsValidTarget()
			&& SimpleLib::SimpleLib::GetDistance(myHero, qTarget) < W->Range())
		{
			W->CastOnPlayer();
		}
	}
	
	if (useECombo->Enabled() && E->IsReady())
	{
		auto qTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, E->Range());
		if (qTarget != nullptr && qTarget->IsValidTarget()
			&& SimpleLib::SimpleLib::GetDistance(myHero, qTarget) < E->Range() )
		{
			E->CastOnPlayer();
		}
	}

	if (useRCombo->Enabled() && R->IsReady())
	{
		auto qTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, R->Range());
		
		int Min = RCount->GetInteger();		
		int Enemies = 0;
		Vec3 Position = Vec3();

		GPrediction->FindBestCastPosition(1000, 250, false, false, true, Position, Enemies, R->GetDelay());

		if (qTarget != nullptr && qTarget->IsValidTarget()
			&& SimpleLib::SimpleLib::GetDistance(myHero, qTarget) < R->Range())
		{
			if (Rsolo->Enabled() && Q->IsReady() && CountEnemy(myHero->GetPosition(), 3000) < 2 && CountAlly(qTarget->GetPosition(), 2000) < 1)
			{	
				if (ComboDamage(qTarget) >= qTarget->GetHealth())
				{
					R->CastOnTarget(qTarget, kHitChanceHigh);
				}
			}
			else if (Enemies >= Min)
			{
				R->CastOnPosition(Position);				
			}
			else if (RAllys->Enabled() && Q->IsReady() && CountEnemy(myHero->GetPosition(), 3000) < CountAlly(myHero->GetPosition(), 3000) + 1)
			{
				R->CastOnPosition(Position);
			}
		}
	}	
}

void Mixed()
{
	if (useQHarass->Enabled() && Q->IsReady() && myHero->ManaPercent() >= minmanaH->GetInteger())
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
	if (useQlh->Enabled() && Q->IsReady() && myHero->ManaPercent() >= minMPlh->GetInteger())
	{
		for (auto minion : GEntityList->GetAllMinions(false, true, false))
		{
			if (minion != nullptr && !minion->IsDead() && myHero->IsValidTarget(minion, Q->Range() - 50))
			{
				auto damage = GHealthPrediction->GetKSDamage(minion, kSlotQ, Q->GetDelay(), false);

				if (damage > minion->GetHealth())
				{					
					GOrbwalking->ResetAA();
					Q->CastOnPlayer();
				}
			}
		}
	}
}

void JungleClear()
{
	if (useQj->Enabled() && Q->IsReady() && myHero->ManaPercent() >= minMPlcJ->GetInteger() && !FoundMinions(W->Range()) && FoundMinionsNeutral(Q->Range() -50))
	{
		Q->CastOnPlayer();
	}

	if (useWj->Enabled() && W->IsReady() && myHero->ManaPercent() >= minMPlcJ->GetInteger() && !FoundMinions(W->Range()) && FoundMinionsNeutral(W->Range() - 100))
	{
		W->CastOnPlayer();
	}
}

void LaneClear()
{
	Vec3 posQ, posW;
	int hitQ, hitW;
	GPrediction->FindBestCastPosition(Q->Range()-50, Q->Radius()-50, false, true, false, posQ, hitQ);
	GPrediction->FindBestCastPosition(W->Range() - 80, W->Radius() - 80, false, true, false, posW, hitW);

	if (useQlc->Enabled() && Q->IsReady() && hitQ >= useQlcMinions->GetInteger() && myHero->ManaPercent() >= minMPlc->GetInteger() && !FoundMinionsNeutral(W->Range()+100))
	{
		Q->CastOnPlayer();
	}

	if (useWlc->Enabled() && W->IsReady() && hitW >= useWlcMinions->GetInteger() && myHero->ManaPercent() >= minMPlc->GetInteger() && !FoundMinionsNeutral(W->Range() + 100))
	{
		W->CastOnPlayer();
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
	if (!myHero->IsEnemy(args.Sender)) return;
	
	if (E->IsReady() && myHero->IsValidTarget(args.Sender, E->Range()) && AutoEGapcloser->Enabled())
	{
		E->CastOnPlayer();
		GTargetSelector->SetOverrideFocusedTarget(args.Sender);
		GOrbwalking->SetOverrideTarget(args.Sender);
	}
}

PLUGIN_EVENT(void) OnInterruptible(InterruptibleSpell const& Args)

{	
	if (!myHero->IsEnemy(Args.Target)) return;
	
	if (E->IsReady() && GEntityList->Player()->IsValidTarget(Args.Target, E->Range()) && AutoEInterrupter->Enabled())
	{
		E->CastOnPlayer();
		GTargetSelector->SetOverrideFocusedTarget(Args.Target);
		GOrbwalking->SetOverrideTarget(Args.Target);		
	}

	if (!E->IsReady() && R->IsReady() && GEntityList->Player()->IsValidTarget(Args.Target, R->Range()) && AutoRInterrupter->Enabled())
	{
		R->CastOnTarget(Args.Target, kHitChanceHigh);
	}
}

PLUGIN_EVENT(void) OnAfterAttack(IUnit* source, IUnit* target)
{
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
	if (Args.Caster_ == myHero)
	{
		GGame->PrintChat(Args.Name_);
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
	GEventManager->AddEventHandler(kEventOnSpellCast, OnProcessSpell);
	GEventManager->AddEventHandler(kEventOnInterruptible, OnInterruptible);
	

	GGame->PrintChat("Hecarim by zFederaL loaded!");
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
	GEventManager->RemoveEventHandler(kEventOnInterruptible, OnInterruptible);
}