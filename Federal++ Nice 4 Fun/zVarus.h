#pragma once
#include "PluginSDK.h"
#include "BaseMenu.h"
#include "Common.h"

class Varus
{
public:

	void InitializeMenu()
	{
		MainMenu = GPluginSDK->AddMenu("Varus");
		ComboSettings = MainMenu->AddMenu("Combo");
		HarassSettings = MainMenu->AddMenu("Harass");
		LaneClearSettings = MainMenu->AddMenu("LaneClear");
		JungleClearSettings = MainMenu->AddMenu("JungleClear");
		KillstealSettings = MainMenu->AddMenu("Killsteal");
		MiscSettings = MainMenu->AddMenu("Miscs");
		DrawingSettings = MainMenu->AddMenu("Drawings");

		ComboQ = ComboSettings->CheckBox("Use Q", true);
		ComboE = ComboSettings->CheckBox("Use E", true);
		ComboR = ComboSettings->CheckBox("Use R", true);
		LaneClearQ = ComboSettings->CheckBox("Use R|Logic", true);
		QEnemies = ComboSettings->AddFloat("Use R|Counts Enemies In Range >= x", 1, 5, 3);
		QEnemies1 = ComboSettings->AddFloat("Use Spell|When target Have x Passive", 0, 3, 3);

		ComboE2 = HarassSettings->CheckBox("Use Q", true);
		ComboW = HarassSettings->CheckBox(" Use E", true);
		HarassE = HarassSettings->CheckBox("Auto Harass", true); //toggle
		HarassManaE = HarassSettings->AddFloat("Harass Mana %", 0, 100, 60);


		//AutoUlt = LaneClearSettings->CheckBox("Use Q", true);
		//REnemies = LaneClearSettings->AddFloat("If Q CanHit Counts >= x", 1, 5, 3);
		ComboRKill = LaneClearSettings->CheckBox("Use E", true);
		ComboREnemies = LaneClearSettings->AddFloat("If E CanHit Counts >= x", 1, 5, 3);
		HarassManaQ = LaneClearSettings->AddFloat("Harass Mana %", 0, 100, 60);

		//HarassManaW = JungleClearSettings->CheckBox("Use Q", true);
		FarmW = JungleClearSettings->CheckBox("Use E", true);
		HarassW = JungleClearSettings->AddFloat("Harass Mana %", 0, 100, 60);

		ComboQH = KillstealSettings->CheckBox("Use Q", true);
		ComboEA = KillstealSettings->CheckBox("Use E", true);

		LaneClearR = MiscSettings->AddKey("Semi-manual R Key", 0x54);
		HarassManaW = MiscSettings->CheckBox("Use Spell Harass(In LaneClear Mode)", true);

		DrawReady = DrawingSettings->CheckBox("Draw Only Ready Spells", true);
		DrawQ = DrawingSettings->CheckBox("Draw Q", true);
		DrawE = DrawingSettings->CheckBox("Draw E", true);
		DrawR = DrawingSettings->CheckBox("Draw R", true);
	}

	void LoadSpells()
	{
		Q = GPluginSDK->CreateSpell2(kSlotQ, kLineCast, false, false, static_cast<eCollisionFlags>(kCollidesWithYasuoWall));
		Q->SetCharged(925.f, 1600.f, 1.5f);
		W = GPluginSDK->CreateSpell2(kSlotW, kTargetCast, false, false, static_cast<eCollisionFlags>(kCollidesWithNothing));
		E = GPluginSDK->CreateSpell2(kSlotE, kCircleCast, false, true, static_cast<eCollisionFlags>(kCollidesWithYasuoWall));
		R = GPluginSDK->CreateSpell2(kSlotR, kLineCast, false, true, static_cast<eCollisionFlags>(kCollidesWithYasuoWall));
		Q->SetOverrideRange(1600);
		Q->SetOverrideDelay(0.25f);
		Q->SetOverrideRadius(70);
		Q->SetOverrideSpeed(1650);
		E->SetOverrideRange(975);
		E->SetOverrideRadius(120);
		E->SetOverrideDelay(0.35f);
		E->SetOverrideSpeed(1500);
	    R->SetOverrideRange(1050);
		R->SetOverrideRadius(120);
		R->SetOverrideDelay(0.25f);
		R->SetOverrideSpeed(1950);
		 
	}
	int GetEnemiesInRange2(float range)
	{
		auto enemies = GEntityList->GetAllHeros(false, true);
		auto enemiesInRange = 1;

		for (auto enemy : enemies)
		{
			if (enemy != nullptr && enemy->GetTeam() != GEntityList->Player()->GetTeam())
			{
				auto flDistance = (enemy->GetPosition() - GEntityList->Player()->GetPosition()).Length();
				if (flDistance < range)	
				{
					enemiesInRange++;
				}
			}
		}
		return enemiesInRange;
	}
	void CastQ(IUnit* target)
	{
		if (target != nullptr && Q->IsReady() && !target->IsDead())
		{
			//target = GTargetSelector->FindTarget(QuickestKill,PhysicalDamage, Q->Range());
			if (target != nullptr && !target->IsInvulnerable() && GetEnemiesInRange2(1600) >= 1 && GEntityList->Player()->IsValidTarget(target, Q->Range()))
			{
				Q->StartCharging();
			}
			if (target != nullptr && Q->IsCharging() && Q->GetChargePercent())
			{
				 
				//target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());
				Q->FindTarget(PhysicalDamage);
				if (Q->GetChargePercent() >= 5)
				{
					Q->CastOnTarget(target, 5);
				}
				 
			}
			return;
		}
	}
	void CastQ2(IUnit* target)
	{
		//auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());


		if (target != nullptr && !target->IsDead() && !target->IsInvulnerable() && GetEnemiesInRange2(1600) >= 1 && GEntityList->Player()->IsValidTarget(target, Q->Range()))
		{
			Q->StartCharging();
		}
			target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());
			//if (target != nullptr)

			Q->FindTarget(PhysicalDamage);
			Q->CastOnTarget(target, 5);
	}
	
	
	void SemiRLogic()
	{
			if(R->IsReady() && IsKeyDown(LaneClearR))
			{
			auto target = GTargetSelector->GetFocusedTarget()!= nullptr ? GTargetSelector->GetFocusedTarget(): 
					GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, R->Range());
			if (target != nullptr &&  GEntityList->Player()->IsValidTarget(target, R->Range()))
			{
				R->CastOnTarget(target, 5);
			}
			}
	}
	void AutoHarass()
	{
		if (InFountain(GEntityList->Player()))
		{
			return;
		}
				
		if (HarassE->Enabled() && GOrbwalking->GetOrbwalkingMode() != kModeCombo && GOrbwalking->GetOrbwalkingMode() != kModeMixed && !GEntityList->Player()->IsRecalling())
		{
			Harass();
		}

	}

	void Harass()
	{
		
		auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());
		if (GEntityList->Player()->ManaPercent() >= HarassManaQ->GetFloat())
		{		
			if (target != nullptr && !target->IsDead() && GEntityList->Player()->IsValidTarget(target, Q->Range()) && target->IsEnemy(GEntityList->Player()) && GetDistance(GEntityList->Player(), target) <= Q->Range())
			{
				if (ComboE2->Enabled())
				{
					CastQ(target);
					
				}
			}

			if (target != nullptr && ComboW->Enabled() && E->IsReady() && GEntityList->Player()->IsValidTarget(target,E->Range()))
			{
				E->CastOnTarget(target, 5);
			}
		}
	}

	 int GetPassiveCount(IUnit* target)
	{
		return target->HasBuff("varuswdebuff") ? target->GetBuffCount("varuswdebuff") : 0;
	}
	void KillSteal()
	{

		{
			for (auto enemy : GEntityList->GetAllHeros(false, true))
			{
				if (enemy->IsEnemy(GEntityList->Player()) && GetDistance(GEntityList->Player(), enemy) <= Q->Range() && ComboQH->Enabled() && GEntityList->Player()->IsValidTarget(enemy, Q->Range()))
				{
					if (enemy != nullptr && enemy->IsDead() || enemy->IsInvulnerable() || enemy->HasBuffOfType(BUFF_SpellShield) || enemy->HasBuffOfType(BUFF_SpellImmunity))
						return;

					if (GHealthPrediction->GetKSDamage(enemy, kSlotQ, Q->GetDelay(), true) > enemy->GetHealth())
					{
						CastQ(enemy);
					}
				}
			}
		}
		for (auto enemy : GEntityList->GetAllHeros(false, true))
		{
			if (enemy->IsEnemy(GEntityList->Player()) && GetDistance(GEntityList->Player(), enemy) <= E->Range() && ComboEA->Enabled() && GEntityList->Player()->IsValidTarget(enemy, E->Range()))
			{
				if (enemy != nullptr && enemy->IsDead() || enemy->IsInvulnerable() || enemy->HasBuffOfType(BUFF_SpellShield) || enemy->HasBuffOfType(BUFF_SpellImmunity))
					return;

				if (GHealthPrediction->GetKSDamage(enemy, kSlotE, E->GetDelay(), true) > enemy->GetHealth())
				{
					E->CastOnTarget(enemy, 5);
				}
			}
		}
	}
	
	void Combo()
	{
		auto target = GTargetSelector->GetFocusedTarget() != nullptr ? GTargetSelector->GetFocusedTarget() :
			GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());
		if (target != nullptr && GEntityList->Player()->IsValidTarget(target, Q->Range()) && !target->IsInvulnerable() && !target->IsDead())
		{
			if (ComboR->Enabled() && R->IsReady() && GEntityList->Player()->IsValidTarget(target, R->Range()))
			{
				if (LaneClearQ->Enabled() && R->IsReady() && GetEnemiesInRange2(1000) <= 2)
				{
					if (((target->GetHealth() + target->HPRegenRate() * 2) <= (GDamage->GetSpellDamage(GEntityList->Player(), target , kSlotR) + GDamage->GetSpellDamage(GEntityList->Player(), target, kSlotW) + (E->IsReady() ? GDamage->GetSpellDamage(GEntityList->Player(),target, kSlotE) : 0)) + (Q->IsReady() ? GDamage->GetSpellDamage(GEntityList->Player(), target, kSlotQ) : 0) + (GDamage->GetAutoAttackDamage(GEntityList->Player(), target, true) * 3)))
					{
						R->CastOnTarget(target, 5);
					}
				}

				if(GetEnemiesInRange2(R->Range()) >= QEnemies->GetFloat())
				{
					R->CastOnTarget(target, 5);
				}
					
			}
			if (ComboQ->Enabled() && GEntityList->Player()->IsValidTarget(target, Q->Range()))
			{
				
					if ( GEntityList->Player()->IsValidTarget(target, Q->Range()) || (GEntityList->Player()->GetPosition() - target->GetPosition()).Length() > GEntityList->Player()->GetRealAutoAttackRange(GEntityList->Player()) + 200 || GetPassiveCount(target) >= QEnemies1->GetFloat() || GEntityList->Player()->GetSpellLevel(kSlotW) == 0 || target->GetHealth() < GDamage->GetSpellDamage(GEntityList->Player(), target, kSlotQ))
					{
						if (target != nullptr && target->IsDead() || target->IsInvulnerable() || target->HasBuffOfType(BUFF_SpellShield) || target->HasBuffOfType(BUFF_SpellImmunity))
							return;

						if (GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range()))
						{
							CastQ(target);
						}
					}
				}
			
			if (ComboE->Enabled() && GEntityList->Player()->IsValidTarget(target, E->Range()))
			{
				if (GetPassiveCount(target) >= QEnemies1->GetFloat() || GEntityList->Player()->GetSpellLevel(kSlotW) == 0 || target->GetHealth() < GDamage->GetSpellDamage(GEntityList->Player(), target, kSlotQ))
				{
					E->CastOnTarget(target, 5);
				}
			}
		}
	}
	void FarmHarass()
	{
		if (HarassManaW->Enabled())
		{
			Harass();
		}
	}
	void LaneClear()
	{
		if (GEntityList->Player()->ManaPercent() >= HarassManaQ->GetFloat())
		/*{

			auto QMinionsInRange = GetMinionsInRange(GEntityList->Player()->GetPosition(), Q->Range());
			if (AutoUlt->Enabled() && Q->IsReady())
				if (QMinionsInRange >= REnemies->GetFloat())
					for (auto minion : GEntityList->GetAllMinions(false, true, false))
					{
						if (minion != nullptr && GEntityList->Player()->IsValidTarget(minion, Q->Range()) && minion->IsEnemy(GEntityList->Player()) && GetDistance(GEntityList->Player(), minion) <= Q->Range())
						{
							CastQ(minion);
						}
					}
		}*/
		if (ComboRKill->Enabled() && E->IsReady())
		{
			auto EMinionsInRange = GetMinionsInRange(GEntityList->Player()->GetPosition(), E->Range());	
				if (EMinionsInRange >= ComboREnemies->GetFloat())
					for (auto minion : GEntityList->GetAllMinions(false, true, false))
					{
						if (minion != nullptr && GEntityList->Player()->IsValidTarget(minion, E->Range()))
						{
							E->CastOnTarget(minion, 5);
						}
					}
		}
	}

	void JungleClear()
	{
		if (GEntityList->Player()->ManaPercent() >= HarassW->GetFloat())
		{
			for (auto mob : GEntityList->GetAllMinions(false, false, true))
			{
				if (mob == nullptr)
					return;
				if (GEntityList->Player()->IsValidTarget(mob, Q->Range()))
				{
					/*if (HarassManaW->Enabled() && Q->IsReady() && mob->IsValidTarget(mob, Q->Range()))
					{
						CastQ(mob);
					}*/

					if (FarmW->Enabled() && E->IsReady() && mob->IsValidTarget(mob, E->Range()))
					{
						E->CastOnTarget(mob);
					}
				}
			}
		}
	}
				
		
	
 	
	
	
		void Drawing()
		{

			if (DrawReady->Enabled())
			{
				if (Q->IsReady() && DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }

				if (E->IsReady() && DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }
	
				if (R->IsReady() && DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), R->Range()); }

			
			}
			else
			{
				if (DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }

				if (DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }
				
				if (DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), R->Range()); }

				
			}
		}


	};