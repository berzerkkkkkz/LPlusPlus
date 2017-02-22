#pragma once
#include "PluginSDK.h"
#include "BaseMenu.h"
#include "Common.h"

class Ashe
{
public:

	static void InitializeMenu()
	{
		MainMenu = GPluginSDK->AddMenu("Federal Ashe");

		ComboSettings = MainMenu->AddMenu("Combo Settings");
		{
			ComboQ = ComboSettings->CheckBox("Use Q", true);			
			ComboW = ComboSettings->CheckBox("Use W", true);			
			ComboR = ComboSettings->CheckBox("Use R KS combo R + W + AA", true);
			UltEnemies = ComboSettings->CheckBox("Auto R Aoe", true);
			SemiManualKey = ComboSettings->AddKey("Semi-manual cast R key", 74);
		}

		HarassSettings = MainMenu->AddMenu("Harass Settings");
		{			
			HarassQ = HarassSettings->CheckBox("Use Q", false);
			HarassW = HarassSettings->CheckBox("Use W", true);			
			AutoHarass = HarassSettings->CheckBox("Harass Toggle", false);
			HarassMana = HarassSettings->AddInteger("Minimum MP% to Harass", 1, 100, 60);
		}

		KillstealSettings = MainMenu->AddMenu("Killsteal Settings");
		{
			Killsteal = KillstealSettings->CheckBox("Activate KillSteal", true);
			KillstealW = KillstealSettings->CheckBox("Use W to KillSteal", true);
			KillstealR = KillstealSettings->CheckBox("Use R to KillSteal", true);			
		}

		LaneClearSettings = MainMenu->AddMenu("LaneClear Settings");
		{
			LaneClearQ = LaneClearSettings->CheckBox("Use Q in laneclear", true);
			MinionsQ = LaneClearSettings->AddInteger("Minimum minions to Q in laneclear", 1, 10, 5);
			LaneClearW = LaneClearSettings->CheckBox("Use W in laneclear", true);
			MinionsW = LaneClearSettings->AddInteger("Minimum minions to W in laneclear", 1, 10, 4);
			LaneClearMana = LaneClearSettings->AddInteger("Minimum MP% to laneclear", 1, 100, 60);
		}

		JungleClearSettings = MainMenu->AddMenu("JungleClear Settings");
		{
			JungleQ = JungleClearSettings->CheckBox("Use Q in Jungle", true);
			JungleW = JungleClearSettings->CheckBox("Use W in Jungle", true);			
			JungleMana = JungleClearSettings->AddInteger("Min Mana to Jungle", 1, 100, 40);
		}
		
		MiscSettings = MainMenu->AddMenu("Misc Settings");
		{			
			RInterrupter = MiscSettings->CheckBox("Automatically R Interrupt Spell", true);
			RGapCloser = MiscSettings->CheckBox("Automatically R GapCloser", true);
			HealthR = MiscSettings->AddInteger("If My HealthPercent <= %", 1, 90, 30);
			CCedW = MiscSettings->CheckBox("Auto W When Enemies Cant Move", true);
			CCedR = MiscSettings->CheckBox("Auto R When Enemies Cant Move", false);
			AutoE = MiscSettings->CheckBox("Auto E target Brush", true);
			Predic = MiscSettings->CheckBox("HitChance - Off: Medium | On: Hight", true);
		}

		DrawingSettings = MainMenu->AddMenu("Drawing Settings");
		{
			DrawReady = DrawingSettings->CheckBox("Draw Only Ready Spells", true);			
			DrawW = DrawingSettings->CheckBox("Draw W", true);
			DrawE = DrawingSettings->CheckBox("Draw E", false);
			DrawR = DrawingSettings->CheckBox("Draw R", false);
			DrawComboDamage = DrawingSettings->CheckBox("Draw combo damage", true);
		}

		SkinsChange = MainMenu->AddMenu("Skins Changer");
		{
			MiscSkin = SkinsChange->AddInteger("Skins", 1, 8, 1);
		}
	}

	static void InitializeSpells()
	{
		Q = GPluginSDK->CreateSpell2(kSlotQ, kTargetCast, false, false, kCollidesWithNothing);		
		W = GPluginSDK->CreateSpell2(kSlotW, kConeCast, false, true, kCollidesWithYasuoWall);
		W->SetSkillshot(0.25f, 60.f, 2000.f, 1240.f);		
		E = GPluginSDK->CreateSpell2(kSlotE, kLineCast, false, false, kCollidesWithNothing);
		E->SetSkillshot(0.25f, 300.f, 1400.f, 2500.f);
		R = GPluginSDK->CreateSpell2(kSlotR, kLineCast, false, false, kCollidesWithYasuoWall);
		R->SetSkillshot(0.25f, 130.f, 1600.f, 5000.f);
	}

	static void SkinChanger()
	{
		if (GEntityList->Player()->GetSkinId() != MiscSkin->GetInteger())
		{
			GEntityList->Player()->SetSkinId(MiscSkin->GetInteger());
		}
	}

	static void Automatic()
	{
		if (AutoE->Enabled() && E->IsReady())
		{
			for (auto hero : GEntityList->GetAllHeros(false, true))
			{
				if (!CheckTarget(hero) || GetDistance(GEntityList->Player(), hero) > 1500) return;

				Vec3 position;
				auto delay = E->GetDelay() + GetDistance(GEntityList->Player(), hero) / E->Speed();
				GPrediction->GetFutureUnitPosition(hero, delay, true, position);

				if (GNavMesh->IsPointGrass(position) && !hero->IsVisible())
				{
					E->CastOnPosition(position);
				}
			}
		}
	}	

	static void LogicW()
	{
		auto t = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, W->Range());

		if (CheckTarget(t))
		{

			if (t->IsValidTarget() && t != nullptr && !t->IsDead() && !t->IsInvulnerable())
			{
				if (ComboW->Enabled() && W->IsReady() && GOrbwalking->GetOrbwalkingMode() == kModeCombo && GEntityList->Player()->GetMana() > R->ManaCost() + W->ManaCost())
				{
					if (Predic->Enabled())
					{
						W->CastOnTarget(t, kHitChanceHigh);
					}
					else
					{
						W->CastOnTarget(t, kHitChanceMedium);
					}
				}

				else if (HarassW->Enabled() && W->IsReady() && GOrbwalking->GetOrbwalkingMode() == kModeMixed  && GEntityList->Player()->GetMana() > R->ManaCost() + W->ManaCost() && GEntityList->Player()->ManaPercent() > HarassMana->GetInteger())
				{
					if (Predic->Enabled())
					{
						W->CastOnTarget(t, kHitChanceHigh);
					}
					else
					{
						W->CastOnTarget(t, kHitChanceMedium);
					}
				}

				else if (AutoHarass->Enabled() && W->IsReady() && GEntityList->Player()->GetMana() > R->ManaCost() + W->ManaCost() && GEntityList->Player()->ManaPercent() > HarassMana->GetInteger())
				{
					if (Predic->Enabled())
					{
						W->CastOnTarget(t, kHitChanceHigh);
					}
					else
					{
						W->CastOnTarget(t, kHitChanceMedium);
					}
				}

				else if (Killsteal->Enabled() && KillstealW->Enabled() && W->IsReady() && GHealthPrediction->GetKSDamage(t, kSlotW, W->GetDelay(), false) > t->GetHealth())
				{
					if (Predic->Enabled())
					{
						W->CastOnTarget(t, kHitChanceHigh);
					}
					else
					{
						W->CastOnTarget(t, kHitChanceMedium);
					}
				}

				else if (GOrbwalking->GetOrbwalkingMode() == kModeNone && CCedW->Enabled() && t->IsValidTarget(GEntityList->Player(), W->Range() - 50) && W->IsReady() && !CanMove(t) && !t->IsDead() && !t->IsInvulnerable() && GEntityList->Player()->GetMana() > W->ManaCost() + R->ManaCost())
				{
					if (Predic->Enabled())
					{
						W->CastOnTarget(t, kHitChanceHigh);
					}
					else
					{
						W->CastOnTarget(t, kHitChanceMedium);
					}
				}
			}
		}

		if ((LaneClearW->Enabled() || JungleW->Enabled()) && GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			for (auto Minion : GEntityList->GetAllMinions(false, true, false))
			{
				if (Minion != nullptr && !Minion->IsDead())
				{
					if (LaneClearW->Enabled() && W->IsReady() && GEntityList->Player()->GetMana() > R->ManaCost() + Q->ManaCost() + W->ManaCost() && GEntityList->Player()->IsValidTarget(Minion, W->Range()) && !FoundMinionsNeutral(W->Range() - 300))
					{
						if (GEntityList->Player()->ManaPercent() > LaneClearMana->GetInteger())
						{
							Vec3 pos;
							int count;
							W->FindBestCastPosition(true, true, pos, count);

							if (count >= MinionsW->GetInteger() && W->CastOnPosition(pos))
							{
								return;
							}
						}
					}
				}
			}

			for (auto jMinion : GEntityList->GetAllMinions(false, false, true))
			{
				if (jMinion != nullptr && !jMinion->IsDead())
				{
					if (JungleW->Enabled() && W->IsReady() && GEntityList->Player()->GetMana() > R->ManaCost() + Q->ManaCost() + W->ManaCost() && GEntityList->Player()->IsValidTarget(jMinion, W->Range()) && !FoundMinions(W->Range() - 300))
					{
						if (GEntityList->Player()->ManaPercent() > JungleMana->GetInteger())
						{
							Vec3 pos;
							int count;
							W->FindBestCastPosition(true, true, pos, count);

							if (count >= MinionsW->GetInteger() && W->CastOnPosition(pos))
							{
								return;
							}
						}
					}
				}
			}
		}
	}

	static void Combo()
	{
	}

	static void Harass()
	{
	}

	static void LastHit()
	{
	}

	static void JungleClear()
	{
	}

	static void LaneClear()
	{
	}

	static void Drawing()
	{
	}

	static void OnGapcloser(GapCloserSpell const& args)
	{

	}

	static void OnAfterAttack(IUnit* source, IUnit* target)
	{
		if (source != GEntityList->Player() || target == nullptr)
			return;

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo && Q->IsReady() && ComboQ->Enabled() && GEntityList->Player()->GetMana() > R->ManaCost() + Q->ManaCost())
		{
			Q->CastOnPlayer();
		}

		else if (GOrbwalking->GetOrbwalkingMode() == kModeMixed && HarassQ->Enabled() && GEntityList->Player()->GetMana() > R->ManaCost() + Q->ManaCost() + W->ManaCost())
		{
			Q->CastOnPlayer();
		}

		else if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			for (auto Minion : GEntityList->GetAllMinions(false, true, false))
			{
				if (Minion != nullptr && !Minion->IsDead())
				{
					if (LaneClearQ->Enabled() && Q->IsReady() && GEntityList->Player()->GetMana() > R->ManaCost() + Q->ManaCost() + W->ManaCost() && GEntityList->Player()->IsValidTarget(Minion, W->Range()) && !FoundMinionsNeutral(W->Range() - 300))
					{
						if (CountMinions(GEntityList->Player()->GetPosition(), W->Range() >= MinionsQ->GetInteger()) && GEntityList->Player()->ManaPercent() > LaneClearMana->GetInteger())
							Q->CastOnPlayer();
					}
				}
			}

			for (auto jMinion : GEntityList->GetAllMinions(false, false, true))
			{
				if (jMinion != nullptr && !jMinion->IsDead())
				{
					if (JungleQ->Enabled() && Q->IsReady() && GEntityList->Player()->GetMana() > R->ManaCost() + Q->ManaCost() + W->ManaCost() && GEntityList->Player()->IsValidTarget(jMinion, W->Range()) && !FoundMinions(W->Range() - 300))
					{
						if (GEntityList->Player()->ManaPercent() > JungleMana->GetInteger())
						{
							W->CastOnUnit(jMinion);
						}
					}
				}
			}

		}
	}
			
	static void OnExitVisible(IUnit* Source)
	{
		
	}
	

	
};
