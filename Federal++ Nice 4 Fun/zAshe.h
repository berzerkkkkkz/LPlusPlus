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
			SaveMana = ComboSettings->CheckBox("Save Mana To Cast W&R", true);
			ComboW = ComboSettings->CheckBox("Use W", true);			
			ComboR = ComboSettings->CheckBox("Use R KS combo R + W + AA", true);
			UltEnemies = ComboSettings->CheckBox("Auto R Aoe", true);
			SemiManualKey = ComboSettings->AddKey("Semi-manual cast R key", 74);
		}

		HarassSettings = MainMenu->AddMenu("Harass Settings");
		{			
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
			LaneClearQ = LaneClearSettings->CheckBox("Use W in laneclear", true);
			MinionsQ = LaneClearSettings->AddInteger("Minimum minions to W in laneclear", 1, 10, 4);			
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
			MiscSkin = SkinsChange->AddInteger("Skins", 1, 7, 1);
		}
	}

	static void LoadSpells()
	{
		//Spell
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
				if (!CheckTarget(hero) || GetDistance(GEntityList->Player(), hero) > 1200) return;

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

	}

	static void OnCreateObject(IUnit* Source)
	{

	}

	static void OnDeleteObject(IUnit* Source)
	{

	}
};
