#pragma once
#include "PluginSDK.h"
#include "BaseMenu.h"
#include "Common.h"

class Maokai
{
public:

	static void InitializeMenu()
	{	
		MainMenu = GPluginSDK->AddMenu("Federal Maokai");
		
		ComboSettings = MainMenu->AddMenu("Combo Settings");
		{
			
			ComboQ = ComboSettings->CheckBox("Use Q", true);			
			RangeQ = ComboSettings->AddInteger("Q max Range", 0, 600, 600);
			ComboW = ComboSettings->CheckBox("Use W", true);
			ComboE = ComboSettings->CheckBox("Use E", true);
			ComboR = ComboSettings->CheckBox("Use R", true);			
			REnemies = ComboSettings->AddInteger("Use R min", 1, 5, 2);
			ComboRKill = ComboSettings->CheckBox("Cancel Ult If Target Killable", true);
			UltPercent = ComboSettings->AddInteger("Cancel Ult If Mana", 0, 100, 30);
		}

		HarassSettings = MainMenu->AddMenu("Harass Settings");
		{
			HarassQ = HarassSettings->CheckBox("Use Q", true);
			HarassE = HarassSettings->CheckBox("Use E", false);			
			HarassMana = HarassSettings->AddInteger("Minimum MP% to Harass", 1, 100, 60);
		}

		DrawingSettings = MainMenu->AddMenu("Drawing Settings");
		{
			DrawReady = DrawingSettings->CheckBox("Draw Only Ready Spells", true);
			DrawQ = DrawingSettings->CheckBox("Draw Q", true);			
			DrawW = DrawingSettings->CheckBox("Draw W", false);
			DrawE = DrawingSettings->CheckBox("Draw E", false);			
			DrawComboDamage = DrawingSettings->CheckBox("Draw combo damage", true);
		}

		SkinsChange = MainMenu->AddMenu("Skins Changer");
		{
			MiscSkin = SkinsChange->AddInteger("Skins", 1, 7, 1);
		}
	}

	static void LoadSpells()
	{
		Q = GPluginSDK->CreateSpell2(kSlotQ, kLineCast, false, true, kCollidesWithYasuoWall);
		Q->SetSkillshot(0.50f, 110.f, 1200.f, 600.f);
		W = GPluginSDK->CreateSpell2(kSlotW, kTargetCast, false, false, kCollidesWithNothing);
		W->SetOverrideRange(500.f);
		E = GPluginSDK->CreateSpell2(kSlotE, kCircleCast, false, false, kCollidesWithNothing);
		E->SetSkillshot(1.f, 250.f, 1500.f, 1100.f);
		R = GPluginSDK->CreateSpell2(kSlotR, kTargetCast, false, false, kCollidesWithNothing);
		R->SetOverrideRange(450.f);
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
	}

	static void Combo()
	{
		auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, E->Range());
		if (target != nullptr && target->IsValidTarget() && !target->IsInvulnerable() && !target->IsDead())
		{

			/*Obj_AI_Hero targetR = TargetSelector.GetTarget(R.Range, TargetSelector.DamageType.Magical);
			var rmana = Config.Item("Rmana").GetValue<Slider>().Value;
			if (R.IsReady() && player.ManaPercent < rmana && targetR.IsValidTarget(R.Range - 30))
			{
			bool enoughEnemies = Config.Item("Rene").GetValue<Slider>().Value <= player.CountEnemiesInRange(R.Range - 50);

			if (maoR && ((Config.Item("Rkill").GetValue<bool>() && Damage.GetSpellDamage(player, targetR, SpellSlot.R) + player.CalcDamage(target, Damage.DamageType.Magical, maoRStack) > targetR.Health) || (!enoughEnemies)))
			{
			R.Cast();
			}
			if (targetR != null && !maoR && player.ManaPercent > rmana && (enoughEnemies || R.IsInRange(targetR)))
			{
			R.Cast();
			}
			}*/

			if (W->IsReady() && target->IsValidTarget(GEntityList->Player(), W->Range()) && ComboW->Enabled())
			{
				W->CastOnUnit(target);
			}

			else if (E->IsReady() && target->IsValidTarget(GEntityList->Player(), E->Range()) && ComboE->Enabled())
			{
				E->CastOnTarget(target, kHitChanceHigh);
			}

			else if (!W->IsReady() && Q->IsReady() && ComboQ->Enabled() && target->IsValidTarget(GEntityList->Player(), Q->Range()))
			{
				Q->CastOnTarget(target, kHitChanceHigh);
			}

			/*var rDmg = player.GetSpellDamage(target, SpellSlot.R);
			if (Config.Item("Rkill").GetValue<bool>() && target.HealthPercent <= rDmg)
			{
			R.Cast();
			}*/
		}
		
	}


	static void Harass()
	{		
		if (GEntityList->Player()->ManaPercent() < HarassMana->GetInteger())
		{
			return;
		}
		auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, E->Range());

		if (target != nullptr && target->IsValidTarget() && !target->IsInvulnerable() && !target->IsDead())
		{
			if (HarassW && W->IsReady() && target->IsValidTarget(GEntityList->Player(), W->Range()))
			{
				W->CastOnUnit(target);
			}
			
			else if (HarassQ && Q->IsReady() && target->IsValidTarget(GEntityList->Player(), RangeQ->GetInteger()))
			{
				Q->CastOnTarget(target, kHitChanceHigh);
			}

			else if (HarassE->Enabled() && E->IsReady() && target->IsValidTarget(GEntityList->Player(), E->Range() - 50))
			{
				E->CastOnTarget(target, kHitChanceHigh);
			}			
		}
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
		if (DrawReady->Enabled())
		{
			if (Q->IsReady() && DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), Q->Range()); }
			if (W->IsReady() && DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), W->Range()); }
			if (E->IsReady() && DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), E->Range()); }			
		}
		else
		{
			if (DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), Q->Range()); }
			if (DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), W->Range()); }
			if (DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), E->Range()); }			
		}		
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
