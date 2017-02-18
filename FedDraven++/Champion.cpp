#include "PluginSDK.h";
#include "SimpleLib.h"
#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>

PluginSetup("FedDraven v2- zFederaL")

IMenu* MainMenu;
IMenu* AxeSettings;
IMenu* QSettings;
IMenu* WSettings;
IMenu* ESettings;
IMenu* RSettings;
IMenu* DrawingSettings;
IMenu* SkinsChange;

IMenuOption* gotoAxeC;
IMenuOption* gotoAxeMaxDist;
IMenuOption* MaxAxes;
IMenuOption* axeKill;
IMenuOption* axeTower;
IMenuOption* axeTower2;
IMenuOption* axeEnemy;
IMenuOption* axePro;
IMenuOption* ComboQ;
IMenuOption* useQlc;
IMenuOption* ComboW;
IMenuOption* slowW;
IMenuOption* ComboE;
IMenuOption* autoE2;
IMenuOption* KillstealE;
IMenuOption* AutoEGapcloser;
IMenuOption* AutoEInterrupt;
IMenuOption* useeCCed;
IMenuOption* AutoR;
IMenuOption* Rdmg;
IMenuOption* ComboR;
IMenuOption* Rcc;
IMenuOption* Raoe;
IMenuOption* keyR;
IMenuOption* DrawReady;
IMenuOption* DrawE;
IMenuOption* drawaxe;
IMenuOption* drawaxerange;
IMenuOption* notiRhelper;
IMenuOption* MiscSkin;

IUnit* myHero;

ISpell* Q;
ISpell* W;
ISpell2* E;
ISpell2* R;

std::vector<IUnit*> axeList;

short keystate;
bool smiteKeyWasDown = false;

void InitializeSpells()
{
	Q = GPluginSDK->CreateSpell(kSlotQ);
	W = GPluginSDK->CreateSpell(kSlotW);

	E = GPluginSDK->CreateSpell2(kSlotE, kLineCast, true, true, kCollidesWithYasuoWall);
	R = GPluginSDK->CreateSpell2(kSlotR, kLineCast, true, true, kCollidesWithNothing);

	E->SetSkillshot(0.25f, 100.f, 1400.f, 1050.f);	
	R->SetSkillshot(0.4f, 160.f, 2000.f, 3000.f);
}

void InitializeMenu()
{	
	MainMenu = GPluginSDK->AddMenu("FedDraven++");	
	
	AxeSettings = MainMenu->AddMenu("Axe Settings");
	{
		gotoAxeC = AxeSettings->CheckBox("Catch axe", true);
		gotoAxeMaxDist = AxeSettings->AddInteger("Max dist to catch axe", 200, 1500, 500);
		MaxAxes = AxeSettings->AddInteger("Maximum Axes", 1, 3, 2);
		axeKill = AxeSettings->CheckBox("Dont Catch Axe if can kill 2 AA", true);
		axeTower = AxeSettings->CheckBox("Dont Catch Axe Under Enemy Turret Combo", true);
		axeTower2 = AxeSettings->CheckBox("Dont Catch Axe Under Enemy Turret Farm", true);
		axeEnemy = AxeSettings->CheckBox("Dont Catch Axe in Enemy grup", true);
		axePro = AxeSettings->CheckBox("if axe timeout: force laneclear", true);		
	}	
	
	QSettings = MainMenu->AddMenu("Q Settings");
	{
		ComboQ = QSettings->CheckBox("Auto Q Combo", true);
		useQlc = QSettings->CheckBox("Auto Q in laneclear", true);
	}
	
	WSettings = MainMenu->AddMenu("W Settings");
	{
		ComboW = WSettings->CheckBox("Auto W", true);
		slowW = WSettings->CheckBox("Auto W slow", true);
	}

	ESettings = MainMenu->AddMenu("E Settings");
	{
		ComboE = ESettings->CheckBox("Auto E", true);
		autoE2 = ESettings->CheckBox("Harras E if can hit 2 targets", true);
		KillstealE = ESettings->CheckBox("Auto E Killsteal", true);
		AutoEGapcloser = ESettings->CheckBox("Auto E GapCloser", true);
		AutoEInterrupt = ESettings->CheckBox("Auto E Interrupt", true);
		useeCCed = ESettings->CheckBox("Auto Use E on CC", true);
	}
		
	RSettings = MainMenu->AddMenu("R Settings");
	{
		AutoR = RSettings->CheckBox("Auto R", true);
		Rdmg = RSettings->CheckBox("Auto R KS", false);
		ComboR = RSettings->CheckBox("Auto R in Combo", true);
		Rcc = RSettings->CheckBox("Auto R in CC", true);
		Raoe = RSettings->CheckBox("R Aoe Combo", true);		
		keyR = RSettings->AddKey("Semi-manual cast R key", 71);		
	}	

	DrawingSettings = MainMenu->AddMenu("Drawing Settings");
	{
		DrawReady = DrawingSettings->CheckBox("Draw Only Ready Spells", true);		
		DrawE = DrawingSettings->CheckBox("Draw E", true);
		drawaxe = DrawingSettings->CheckBox("Draw Axe position", true);
		drawaxerange = DrawingSettings->CheckBox("Draw Axe Catch Range", true);
		notiRhelper = DrawingSettings->CheckBox("Draw R helper", true);
	}

	SkinsChange = MainMenu->AddMenu("Skins Changer");
	{
		MiscSkin = SkinsChange->AddInteger("Skins", 1, 7, 1);
	}
}

void Draw()
{
	if (DrawReady->Enabled())
	{
		if (E->IsReady() && DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), E->Range()); }
	}
	else
	{
		if (DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), E->Range()); }
	}

	if (drawaxe->Enabled()) {

		for (auto obj : axeList)
		{
			if (obj->GetPosition().x > 0 && obj->GetPosition().y > 0)
			{
				if (SimpleLib::SimpleLib::GetDistance(myHero, obj) > 120)
				{
					GRender->DrawOutlinedCircle(obj->GetPosition(), Vec4(255, 255, 0, 255), 150);
				}
				else if (SimpleLib::SimpleLib::GetDistance(myHero, obj) < 150)
				{
					GRender->DrawOutlinedCircle(obj->GetPosition(), Vec4(0, 255, 0, 255), 150);
				}
			}
		}

		if (drawaxerange->Enabled())
		{			
			GRender->DrawOutlinedCircle(GGame->CursorPosition(), Vec4(0, 191, 255, 255), gotoAxeMaxDist->GetInteger());
		}
	}
}

void SkinChanger()
{
	if (myHero->GetSkinId() != MiscSkin->GetInteger())
	{
		myHero->SetSkinId(MiscSkin->GetInteger());
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

bool IsUnderTurret(IUnit* source)
{
	for (auto turret : GEntityList->GetAllTurrets(false, true))
	{
		if (source->IsValidTarget(turret, 950.0f))
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

void CheckKeyPresses()
{
	keystate = GetAsyncKeyState(keyR->GetInteger());

	if (keystate < 0)
	{
		if (smiteKeyWasDown == false)
		{
			smiteKeyWasDown = true;
		}
	}
	else
	{
		smiteKeyWasDown = false;
	}
}

void LogicE()
{	
	if (!E->IsReady() || !ComboE->Enabled()) { return; }
	
	for (auto hero : GEntityList->GetAllHeros(false, true))
	{	
		if (hero != nullptr && hero->IsValidTarget(myHero, E->Range()) && !hero->IsInvulnerable() && !hero->IsDead())
		{

			if (GOrbwalking->GetOrbwalkingMode() == kModeCombo && hero->IsValidTarget() && myHero->GetMana() > R->ManaCost() + E->ManaCost())
			{
				if (SimpleLib::SimpleLib::GetDistance(myHero, hero) > GOrbwalking->GetAutoAttackRange(myHero))
				{
					E->CastOnTarget(hero, kHitChanceHigh);
				}

				if (myHero->HealthPercent() < 50)
				{
					E->CastOnTarget(hero, kHitChanceHigh);
				}
			}

			if (GOrbwalking->GetOrbwalkingMode() == kModeCombo &&  myHero->GetMana() > R->ManaCost() + E->ManaCost() + (Q->GetManaCost() * 2))
			{
				E->CastOnTargetAoE(hero, 2, kHitChanceHigh);
			}

			if (GOrbwalking->GetOrbwalkingMode() == kModeMixed && autoE2->Enabled() && myHero->GetMana() > R->ManaCost() + E->ManaCost() + Q->GetManaCost() + W->GetManaCost())
			{
				E->CastOnTargetAoE(hero, 2, kHitChanceHigh);
			}

			if (SimpleLib::SimpleLib::GetDistance(myHero, hero) < 300 && hero->IsMelee())
			{
				E->CastOnTarget(hero, kHitChanceMedium);
			}

			if (KillstealE->Enabled() && E->IsReady())
			{
				auto damage = GHealthPrediction->GetKSDamage(hero, kSlotE, E->GetDelay(), false);

				if (damage > hero->GetHealth() && SimpleLib::SimpleLib::GetDistance(myHero, hero) > GOrbwalking->GetAutoAttackRange(myHero)){

					E->CastOnTarget(hero, kHitChanceHigh);
				}
			}
		}

	}
	
	// Auto E in Target CC
	if (useeCCed->Enabled() && E->IsReady())
	{
		for (auto hero : GEntityList->GetAllHeros(false, true))
		{
			if (hero == nullptr || !hero->IsValidTarget() || SimpleLib::SimpleLib::GetDistance(myHero, hero) > E->Range() || hero->IsInvulnerable() || hero->IsDead())
			{
				return;
			}

			if (hero->HasBuffOfType(BUFF_Stun) || hero->HasBuffOfType(BUFF_Charm) || hero->HasBuffOfType(BUFF_Snare) || hero->HasBuffOfType(BUFF_Suppression) || hero->HasBuffOfType(BUFF_Fear))
			{
				E->CastOnTarget(hero, kHitChanceHigh);
			}
		}
	}	
}

void CatchAxe(IUnit* target)
{
	auto maxDist = gotoAxeMaxDist->GetInteger();

	auto modokey = GOrbwalking->GetOrbwalkingMode();
	
	if (SimpleLib::SimpleLib::GetDistance(myHero, target) < 100)
	{
		GOrbwalking->SetOverridePosition(Vec3(0, 0, 0));
		return;
	}

	
	if (axeTower->Enabled() && modokey == kModeCombo && IsUnderTurret(target))
	{
		GOrbwalking->SetOverridePosition(Vec3(0, 0, 0));
		return;
	}

	if (axeTower2->Enabled() && modokey == kModeLaneClear  && IsUnderTurret(target))
	{
		GOrbwalking->SetOverridePosition(Vec3(0, 0, 0));
		return;
	}

	if (axeEnemy->Enabled() && CountEnemy(target->GetPosition(), 550) > 2)
	{
		GOrbwalking->SetOverridePosition(Vec3(0, 0, 0));
		return;
	}
	
	
	if (SimpleLib::SimpleLib::GetDistance(myHero, target) < maxDist)
	{
		GOrbwalking->SetOverridePosition(target->GetPosition());
	}
	else
	{
		GOrbwalking->SetOverridePosition(Vec3(0, 0, 0));
	}
}

void LogicW()
{
	if (!W->IsReady()) { return; }
	
	if (ComboW->Enabled() && myHero->GetMana() > R->ManaCost() + E->ManaCost() + W->GetManaCost() + Q->GetManaCost() && FoundEnemies(myHero, 1200) > 0 && !myHero->HasBuff("dravenfurybuff"))
	{
		W->CastOnPlayer();
	}
	else if (slowW->Enabled() && myHero->HasBuffOfType(BUFF_Slow) && myHero->GetMana() > R->ManaCost() + E->ManaCost() + W->GetManaCost())
	{
		W->CastOnPlayer();
	}
}

void LogicR()
{
	for (auto hero : GEntityList->GetAllHeros(false, true))
	{
		if (!hero->IsValidTarget() || hero->IsDead() || !R->IsReady() || myHero->IsWindingUp() || SimpleLib::SimpleLib::GetDistance(myHero, hero) > R->Range()) { return; }

		auto rdamage = GHealthPrediction->GetKSDamage(hero, kSlotR, R->GetDelay(), false);
		auto qdamage = GHealthPrediction->GetKSDamage(hero, kSlotQ, Q->GetDelay(), false);
		auto edamage = GHealthPrediction->GetKSDamage(hero, kSlotE, E->GetDelay(), false);
		auto autodamage = GDamage->GetAutoAttackDamage(myHero, hero, false);		

		if (smiteKeyWasDown)
		{
			if (hero->IsValidTarget())
			{
				R->CastOnTargetAoE(hero, 2, kHitChanceVeryHigh);

				if ((autodamage * 4) + rdamage + qdamage > hero->GetHealth())
				{
					R->CastOnTarget(hero, kHitChanceHigh);
				}
			}
		}

		if (AutoR->Enabled())
		{

			if (Rdmg->Enabled() && rdamage > hero->GetHealth() && SimpleLib::SimpleLib::GetDistance(myHero, hero) > GOrbwalking->GetAutoAttackRange(myHero))
			{
				R->CastOnTarget(hero, kHitChanceHigh);
			}

			else if (GOrbwalking->GetOrbwalkingMode() == kModeCombo && ComboR->Enabled() && SimpleLib::SimpleLib::GetDistance(myHero, hero) < GOrbwalking->GetAutoAttackRange(myHero) &&
				(autodamage * 2) + (rdamage * 2) > hero->GetHealth())
			{
				R->CastOnTarget(hero, kHitChanceHigh);
			}

			else if (Rcc->Enabled() && rdamage * 2 > hero->GetHealth() && SimpleLib::SimpleLib::GetDistance(myHero, hero) <= E->Range())
			{
				if (hero->HasBuffOfType(BUFF_Stun) || hero->HasBuffOfType(BUFF_Charm) || hero->HasBuffOfType(BUFF_Snare) || hero->HasBuffOfType(BUFF_Suppression) || hero->HasBuffOfType(BUFF_Fear))
				{
					R->CastOnTarget(hero, kHitChanceHigh);
				}
			}

			else if (GOrbwalking->GetOrbwalkingMode() == kModeCombo && Raoe->Enabled())
			{
				R->CastOnTargetAoE(hero, 3, kHitChanceVeryHigh);
			}

			else if (SimpleLib::SimpleLib::GetDistance(myHero, hero) <= E->Range() && (rdamage * 2) + qdamage + edamage > hero->GetHealth() && Raoe->Enabled())
			{
				R->CastOnTargetAoE(hero, 2, kHitChanceHigh);
			}
		}
	}
}


void AxeLogic()
{
	if (axeList.size() == 0)
	{
		GOrbwalking->SetOverridePosition(Vec3(0, 0, 0));
		return;
	}

	for (auto hero : GEntityList->GetAllHeros(false, true))
	{
		if (hero == nullptr || !hero->IsValidTarget() || SimpleLib::SimpleLib::GetDistance(myHero, hero) > 800 || hero->IsInvulnerable() || hero->IsDead())
		{
			return;
		}

		if (axeKill->Enabled() && SimpleLib::SimpleLib::GetDistance(myHero, hero) > 400 && (GDamage->GetAutoAttackDamage(myHero, hero, false) * 2) > hero->GetHealth())
		{
			GOrbwalking->SetOverridePosition(Vec3(0, 0, 0));			
			return;
		}
	}

	if (axeList.size() == 1)
	{
		CatchAxe(axeList.front());

		return;
	}
	else
	{
		auto bestAxe = axeList.front();
		for (auto obj : axeList)
		{
			if (SimpleLib::SimpleLib::GetDistanceVectors(GGame->CursorPosition(), bestAxe->GetPosition()) > SimpleLib::SimpleLib::GetDistanceVectors(GGame->CursorPosition(), obj->GetPosition()))
			{
				bestAxe = obj;
			}
		}

		CatchAxe(bestAxe);		
	}
}

	PLUGIN_EVENT(void) OnGameUpdate()
	{
		if (myHero->IsDead())
			return;		

		switch (GOrbwalking->GetOrbwalkingMode())
		{
		case kModeCombo:
			if (gotoAxeC->Enabled())
			{
				AxeLogic();
			}
			LogicW();
			break;
		case kModeMixed:
			if (gotoAxeC->Enabled())
			{
				AxeLogic();
			}
			break;
		case kModeLaneClear:
			if (gotoAxeC->Enabled())
			{
				AxeLogic();
			}
			break;
		}

		LogicR();		
		LogicE();

		SkinChanger();		
		CheckKeyPresses();				
	}

	PLUGIN_EVENT(void) OnGapcloser(GapCloserSpell const& args)
	{
		if (args.Sender->IsEnemy(myHero) && args.Sender->IsHero())
		{
			if (AutoEGapcloser->Enabled() && Q->IsReady() && !args.IsTargeted && SimpleLib::SimpleLib::GetDistanceVectors(myHero->GetPosition(), args.EndPosition) < E->Range())
			{
				E->CastOnTarget(args.Sender);
			}
		}
	}

	PLUGIN_EVENT(void) OnAfterAttack(IUnit* source, IUnit* target)
	{
		if (source != myHero || target == nullptr)
			return;

		auto buffCount = myHero->GetBuffCount("dravenspinningattack");

		switch (GOrbwalking->GetOrbwalkingMode())
		{
		case kModeCombo:
			if (ComboQ->Enabled() && Q->IsReady())
			{
				if (buffCount + axeList.size() == 0)
				{
					Q->CastOnPlayer();
				}
			}
			break;
		case kModeLaneClear:
			if (Q->IsReady() && useQlc->Enabled())
			{
				if (buffCount + axeList.size() == 0 && myHero->GetMana() > R->ManaCost() + E->ManaCost() + W->GetManaCost())
				{
					Q->CastOnPlayer();
				}
				else if (70 < myHero->ManaPercent() && buffCount == 0)
				{
					Q->CastOnPlayer();
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
		if (strstr(Source->GetObjectName(), "Draven_Base_Q_reticle_self.troy"))
		{
			axeList.push_back(Source);			
		}
	}

	PLUGIN_EVENT(void) OnDeleteObject(IUnit* Source)
	{
		if (strstr(Source->GetObjectName(), "Draven_Base_Q_reticle_self.troy"))
		{
			std::vector<IUnit*>::iterator position = std::find(axeList.begin(), axeList.end(), Source);			

			if (position != axeList.end())
			{
				axeList.erase(position);				
			}
		}
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

		InitializeSpells();
		InitializeMenu();

		myHero = GEntityList->Player();

		GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
		GEventManager->AddEventHandler(kEventOnRender, OnRender);
		GEventManager->AddEventHandler(kEventOnBuffAdd, OnBuffAdd);
		GEventManager->AddEventHandler(kEventOnBuffRemove, OnBuffRemove);
		GEventManager->AddEventHandler(kEventOrbwalkAfterAttack, OnAfterAttack);
		GEventManager->AddEventHandler(kEventOnGapCloser, OnGapcloser);
		GEventManager->AddEventHandler(kEventOnSpellCast, OnProcessSpell);
		GEventManager->AddEventHandler(kEventOnCreateObject, OnCreateObject);
		GEventManager->AddEventHandler(kEventOnDestroyObject, OnDeleteObject);

		GGame->PrintChat("Draven by zFederaL loaded!");
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
		GEventManager->RemoveEventHandler(kEventOnCreateObject, OnCreateObject);
		GEventManager->RemoveEventHandler(kEventOnDestroyObject, OnDeleteObject);
	}
