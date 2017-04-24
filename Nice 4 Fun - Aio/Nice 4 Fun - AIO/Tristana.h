#pragma once
#include "PluginSDK.h"
#include "BaseMenu.h"
#include "Common.h"
#include <iomanip>

class Tristana
{
public:
	 
	static void InitializeMenu()
	{		
		MainMenu = GPluginSDK->AddMenu("Nice4Fun Federal - Tristana");

		WSettings = MainMenu->AddMenu("W Settings");
		{
			AutoW = WSettings->CheckBox("Menu ToDO", false);
		}
		
		ComboSettings = MainMenu->AddMenu("Combo Settings");
		{
			ComboQ = ComboSettings->CheckBox("Use Q", true);
			QEnemies = ComboSettings->CheckBox("Use Q only with E", true);

			ComboE2 = ComboSettings->CheckBox("Use E", true);
			ComboE = ComboSettings->AddSelection("E Mode:", 0, std::vector<std::string>({ "E", "Use E After AA" }));
			ComboR = ComboSettings->CheckBox("Use R ", true);
			ComboAA = ComboSettings->CheckBox("Focus Target with E", true);
			killstealR = ComboSettings->CheckBox("R Killsteal", false);
			ComboRKill = ComboSettings->CheckBox("Use E + R Finally", true);
			RAllys = ComboSettings->CheckBox("R Target to Allys", true);
			inUnderTower = ComboSettings->CheckBox("R Target to Ally Tower", true);
			for (auto enemy : GEntityList->GetAllHeros(false, true))
			{
				std::string szMenuName = "Use E on - " + std::string(enemy->ChampionName());
				MenuDontUlt[enemy->GetNetworkId()] = ComboSettings->CheckBox(szMenuName.c_str(), true);				
			}
		}

		HarassSettings = MainMenu->AddMenu("Harass Settings");
		{
			HarassQ = HarassSettings->CheckBox("Use Q", true);
			ComboEA = HarassSettings->CheckBox("Use Q only with E", true);
			HarassE = HarassSettings->CheckBox("Use E", true);
			RangeE = HarassSettings->CheckBox("Use E only After AA", true);
			HarassMana = HarassSettings->AddInteger("Minimum MP% to E", 1, 100, 50);
			for (auto enemy : GEntityList->GetAllHeros(false, true))
			{
				std::string szMenuName = "Use E on - " + std::string(enemy->ChampionName());
				ChampionUse[enemy->GetNetworkId()] = HarassSettings->CheckBox(szMenuName.c_str(), true);
			}
		}

		LaneClearSettings = MainMenu->AddMenu("LaneClear Settings");
		{
			LaneClearQ = LaneClearSettings->CheckBox("Use Q in laneclear", false);
			MinionsQ = LaneClearSettings->AddInteger("Minimum minions to Q in laneclear", 1, 10, 6);
			LaneClearE = LaneClearSettings->CheckBox("Use E in laneclear", false);
			MinionsE = LaneClearSettings->AddInteger("Minimum Minions Close to E", 1, 10, 6);
			AutoE = LaneClearSettings->CheckBox("Use E on Tower", true);
			LaneClearMana = LaneClearSettings->AddInteger("Minimum MP% to laneclear", 1, 100, 70);
		}

		JungleClearSettings = MainMenu->AddMenu("JungleClear Settings");
		{
			JungleQ = JungleClearSettings->CheckBox("Use Q in Jungle", true);			
			JungleE = JungleClearSettings->CheckBox("Use E in Jungle", true);			
			JungleMana = JungleClearSettings->AddInteger("Min Mana to Jungle", 1, 100, 40);
		}

		fedMiscSettings = MainMenu->AddMenu("Miscs Settings");
		{
			RInterrupter = fedMiscSettings->CheckBox("Automatically R Interrupt Spell", true);
			RGapCloser = fedMiscSettings->CheckBox("GapCloser | Anti Meele", true);			
			for (auto enemy : GEntityList->GetAllHeros(false, true))
			{
				std::string szMenuName = "Anti Gapcloser - " + std::string(enemy->ChampionName());
				GapCloserList[enemy->GetNetworkId()] = fedMiscSettings->CheckBox(szMenuName.c_str(), true);				
			}

			for (auto enemy : GEntityList->GetAllHeros(false, true))
			{
				if (enemy->IsMelee())
				{
					std::string szMenuName = "Anti Melee - " + std::string(enemy->ChampionName());
					ChampionAntiMelee[enemy->GetNetworkId()] = fedMiscSettings->CheckBox(szMenuName.c_str(), true);
				}				
			}
		}

		DrawingSettings = MainMenu->AddMenu("Drawing Settings");
		{
			DrawReady = DrawingSettings->CheckBox("Draw Only Ready Spells", true);			
			DrawW = DrawingSettings->CheckBox("Draw W", false);
			DrawE = DrawingSettings->CheckBox("Draw E", false);
			DrawR = DrawingSettings->CheckBox("Draw R", false);
			DrawE2 = DrawingSettings->CheckBox("Draw E Info", true);
			DrawComboDamage = DrawingSettings->CheckBox("Draw combo damage", true);
		}	
	}

	static void LoadSpells()
	{
		Q = GPluginSDK->CreateSpell2(kSlotQ, kTargetCast, false, false, static_cast<eCollisionFlags>(kCollidesWithNothing));
		Q->SetOverrideRange(870);
		W = GPluginSDK->CreateSpell2(kSlotW, kCircleCast, false, false, static_cast<eCollisionFlags>(kCollidesWithNothing));
		W->SetOverrideDelay(0.50f);
		W->SetOverrideRadius(270);
		W->SetOverrideRange(900);
		W->SetOverrideSpeed(1500.f);
		E = GPluginSDK->CreateSpell2(kSlotE, kTargetCast, false, true, static_cast<eCollisionFlags>(kCollidesWithYasuoWall));
		E->SetOverrideDelay(0.25f);
		E->SetOverrideRange(550);
		R = GPluginSDK->CreateSpell2(kSlotR, kTargetCast, false, true, static_cast<eCollisionFlags>(kCollidesWithYasuoWall));		
	}

	static void Combo()
	{
		if (ComboE2->Enabled() && ComboE->GetInteger() == 0)
		{
			if (E->IsReady())
			{
				auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, E->Range());
				if (CheckTarget(target) && target->IsValidTarget(GEntityList->Player(), E->Range()))
				{
					if (MenuDontUlt[target->GetNetworkId()]->Enabled())
					{
						E->CastOnTarget(target);
					}
				}
			}
		}

		if (ComboQ->Enabled())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, GOrbwalking->GetAutoAttackRange(GEntityList->Player()));

			if (Q->IsReady() && QEnemies->Enabled())
			{
				if (CheckTarget(target) && target->IsValidTarget(GEntityList->Player(), GEntityList->Player()->GetRealAutoAttackRange(target)))
				{
					if (!E->IsReady() && target->HasBuff("tristanaechargesound"))
					{
						Q->CastOnPlayer();
					}					
				}
			}
			else if (Q->IsReady() && !QEnemies->Enabled())
			{				
				if (CheckTarget(target) && target->IsValidTarget(GEntityList->Player(), GEntityList->Player()->GetRealAutoAttackRange(target)))
				{
					Q->CastOnPlayer();
				}
			}			
		}
	}

	static void Harass()
	{
		if (GEntityList->Player()->MagicDamage() <= HarassMana->GetInteger()) { return; }

		if (HarassE->Enabled() && !RangeE->Enabled())
		{
			if (E->IsReady())
			{
				auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, E->Range());
				if (CheckTarget(target) && target->IsValidTarget(GEntityList->Player(), E->Range()))
				{
					if (ChampionUse[target->GetNetworkId()]->Enabled())
					{
						E->CastOnTarget(target);
					}
				}
			}
		}

		if (HarassQ)
		{
			if (Q->IsReady() && ComboEA->Enabled())
			{
				auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
				if (!E->IsReady() && target->HasBuff("tristanaechargesound"))
				{
					if (CheckTarget(target) && target->IsValidTarget(GEntityList->Player(), E->Range()))
					{
						Q->CastOnPlayer();
					}
				}
			}
			else
			{
				auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
				if (CheckTarget(target) && target->IsValidTarget(GEntityList->Player(), E->Range()))
				{
					Q->CastOnPlayer();
				}
			}
		}
	}

	static void LaneClear()
	{
		AttackETower();

		if (LaneClearMana->GetInteger() < GEntityList->Player()->ManaPercent())
		{
			SArray<IUnit*> Minion = SArray<IUnit*>(GEntityList->GetAllMinions(false, true, false)).Where([](IUnit* m) {return m != nullptr &&
				!m->IsDead() && m->IsVisible() && m->IsValidTarget(GEntityList->Player(), E->Range()) && m->IsCreep() && !strstr(m->GetObjectName(), "WardCorpse"); });

			if (Minion.Any())
			{
				for (auto minion : Minion.ToVector())
				{

					if (LaneClearE->Enabled() && E->IsReady() && CountMinions(minion->GetPosition(), E->Radius()) >= MinionsE->GetInteger())
					{
						if (GEntityList->Player()->IsValidTarget(minion, E->Range()))
						{
							E->CastOnUnit(minion);
						}
					}

					if (LaneClearQ->Enabled() && Q->IsReady() && CountMinions(minion->GetPosition(), E->Range()) >= MinionsQ->GetInteger())
					{
						if (GEntityList->Player()->IsValidTarget(minion, E->Range()))
						{
							Q->CastOnPlayer();
						}
					}
				}
			}
		}
	}

	static void JungleClear()
	{
		if (JungleMana->GetInteger() < GEntityList->Player()->ManaPercent())
		{
			SArray<IUnit*> Minion = SArray<IUnit*>(GEntityList->GetAllMinions(false, false, true)).Where([](IUnit* m) {return m != nullptr &&
				!m->IsDead() && m->IsVisible() && m->IsValidTarget(GEntityList->Player(), E->Range()) && m->IsJungleCreep() && !strstr(m->GetObjectName(), "WardCorpse"); });

			if (Minion.Any())
			{
				for (auto minion : Minion.ToVector())
				{

					if (JungleE->Enabled() && E->IsReady() && !FoundMinions(E->Range()))
					{
						if (GEntityList->Player()->IsValidTarget(minion, E->Range()))
						{
							E->CastOnUnit(minion);
						}
					}

					if (JungleQ->Enabled() && Q->IsReady() && !FoundMinions(E->Range()))
					{
						if (GEntityList->Player()->IsValidTarget(minion, E->Range()) && minion->HasBuff("tristanaechargesound"))
						{
							Q->CastOnPlayer();
						}
					}
				}
			}
		}		
	}

	static void Drawing()
	{
		if (DrawReady->Enabled())
		{			
			if (W->IsReady() && DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), W->Range()); }
			if (E->IsReady() && DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), E->Range()); }
			if (R->IsReady() && DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), R->Range()); }
		}
		else
		{			
			if (DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), W->Range()); }
			if (DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), E->Range()); }
			if (DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), R->Range()); }
		}
		
		if (DrawE2->Enabled())
		{
			for (auto enemy : GEntityList->GetAllHeros(false, true))
			{
				if (GetDistance(GEntityList->Player(), enemy) <= 2000 && enemy->IsValidTarget(GEntityList->Player(), 2000) &&
					enemy->HasBuff("tristanaechargesound") && enemy->IsEnemy(GEntityList->Player()))
				{
					static auto message = GRender->CreateFontW("Comic Sans", 16.f, kFontWeightBold);
					message->SetColor(Vec4(255, 255, 0, 255));
					message->SetOutline(true);

					if (CalcEDmg(enemy) > enemy->GetHealth())
					{
						message->Render(enemy->ServerPosition().x, enemy->ServerPosition().y, "IS DEAD");
					}
					else
					{
						int btime = (enemy->GetBuffStartTime("tristanaechargesound") - GGame->Time() + 5 - (GGame->Latency() / 1000));
						auto bCount = enemy->GetBuffCount("tristanaecharge") + 1;						
						
						std::string text = "Stacks: ";
						text += std::to_string(bCount).data();					

						std::string text2 = "E Time: ";
						std::setprecision(2);
						text2 += std::to_string(btime).data();

						Vec2 pos;
						GGame->Projection(enemy->GetPosition(), &pos);

						message->Render(pos.x - 30, pos.y - 180, text.c_str());
						message->Render(pos.x - 30, pos.y - 200, text2.c_str());
					}
				}
			}
		}
	}

	static void FocusTargetE()
	{
		//GetBuffName();
		
		if (ComboAA->Enabled())
		{
			for (auto enemy : GEntityList->GetAllHeros(false, true))
			{
				if (!CheckTarget(enemy)) return;

				if (GetDistance(GEntityList->Player(), enemy) <= GEntityList->Player()->GetRealAutoAttackRange(enemy) && enemy->IsValidTarget(GEntityList->Player(), GEntityList->Player()->GetRealAutoAttackRange(enemy)) &&
					enemy->HasBuff("tristanaechargesound") && enemy->IsEnemy(GEntityList->Player()))
				{
					GTargetSelector->SetOverrideFocusedTarget(enemy);
				}				
			}
			
			SArray<IUnit*> Minion = SArray<IUnit*>(GEntityList->GetAllMinions(false, true, false)).Where([](IUnit* m) {return m != nullptr &&
				!m->IsDead() && m->IsVisible() && m->IsValidTarget(GEntityList->Player(), GEntityList->Player()->GetRealAutoAttackRange(m)) && m->IsCreep() && !strstr(m->GetObjectName(), "WardCorpse") && m->HasBuff("tristanaechargesound"); });

			if (Minion.Any())
			{
				GTargetSelector->SetOverrideFocusedTarget(Minion.FirstOrDefault());
			}
		}
		
	}

	static void AttackETower()
	{
		if (AutoE->Enabled())
		{
			for (auto tower : GEntityList->GetAllTurrets(false, true))
			{

				if (!tower->IsDead() && tower->GetHealth() > 100 && tower->IsValidTarget(GEntityList->Player(), GEntityList->Player()->GetRealAutoAttackRange(tower)) && tower->IsVisible())
				{
					E->CastOnUnit(tower);
				}
			}
		}
	}

	static void GetBuffName()
	{
		std::vector<void*> vecBuffs;
		

		for (auto minion : GEntityList->GetAllMinions(false, true, true))
		{
			if (minion->IsValidTarget(GEntityList->Player(), 500))
			{
				minion->GetAllBuffsData(vecBuffs);			

				for (auto i : vecBuffs)
				{
					GBuffData->GetBuffName(i);
					GGame->PrintChat(GBuffData->GetBuffName(i));
				}
			}
		}
	}
	
	static float CalcEDmg(IUnit* target)
	{
		if (target->HasBuff("tristanaechargesound"))
		{
			auto dmgE = GDamage->GetSpellDamage(GEntityList->Player(), target, kSlotE);
			auto buffCount = target->GetBuffCount("tristanaecharge");
			dmgE += (dmgE * 0.3f * (buffCount - 1));
			return dmgE - (target->HPRegenRate() * 4);
		}

		return 0;
	}
	
	static void RMiscs()
	{	
		float lvl = 7 * (GEntityList->Player()->GetLevel() - 1);
		auto Range = 620 + lvl;

		E->SetOverrideRange(Range);
		R->SetOverrideRange(Range);
		
		if (R->IsReady() && ComboR->Enabled())
		{
			for (auto target : GEntityList->GetAllHeros(false, true))
			{
				if (CheckTarget(target) && target->IsValidTarget(GEntityList->Player(), R->Range()))
				{
					R->FindTarget(PhysicalDamage);
					R->SetRangeCheckFrom(target->GetPosition());

					AdvPredictionOutput prediction;
					R->RunPrediction(target, false, kCollidesWithYasuoWall, &prediction);

					auto push = 400 + (GEntityList->Player()->GetSpellBook()->GetLevel(kSlotR) * 200);
					auto finalPosition = prediction.TargetPosition.Extend(GEntityList->Player()->GetPosition(), -push);

					if (RAllys->Enabled() && CountAlly(finalPosition, 500) > 1 && 
						CountAlly(target->GetPosition(), 400) == 0 && 
						 CountEnemy(finalPosition, 500) + 1 < CountAlly(finalPosition, 500))
					{
						R->CastOnUnit(target);
						//GGame->PrintChat("Target go to Allys");
					}

					if (inUnderTower->Enabled() && IsUnderTurretPosAlly(finalPosition) && !IsUnderTurretPosAlly(target->GetPosition()) && !IsUnderTurretPosAlly(GEntityList->Player()->GetPosition()))
					{
						R->CastOnUnit(target);
						//GGame->PrintChat("Target go to Tower");
					}
					
					if (ComboRKill->Enabled())
					{
						auto rdamage = GHealthPrediction->GetKSDamage(target, kSlotR, R->GetDelay(), false);
						auto edmg = CalcEDmg(target);
						rdamage = rdamage + edmg;

						if (rdamage > target->GetHealth() && edmg < target->GetHealth() && ValidUlt(target) && target->HasBuff("tristanaechargesound"))
						{
							R->CastOnUnit(target);
							//GGame->PrintChat("Target EQ Combo to Kill");
						}
					}

					if (killstealR->Enabled())
					{
						auto rdamage = GHealthPrediction->GetKSDamage(target, kSlotR, R->GetDelay(), false);

						if (rdamage > target->GetHealth() && CalcEDmg(target) < target->GetHealth() && ValidUlt(target))
						{
							R->CastOnUnit(target);
							//GGame->PrintChat("Target Killsteal");
						}
					}
				}
			}
		}
	}

	static void WAntiMelee()
	{
		if (RGapCloser->Enabled() && W->IsReady())
		{
			for (auto target : GEntityList->GetAllHeros(false, true))
			{				
				if (target->IsMelee() && GetDistance(GEntityList->Player(), target) < 300 && !target->IsDead() && target->IsValidTarget(GEntityList->Player(), GEntityList->Player()->GetRealAutoAttackRange(target)))
				{
					auto dashpos = GEntityList->Player()->ServerPosition();
					auto extend = dashpos.Extend(target->GetPosition(), -W->Range());
					
					if (!GNavMesh->IsPointWall(extend) && (CountEnemy(extend, 1000) == 0 || CountAlly(extend, 1000) > CountEnemy(extend, 1000)))
					{						
						if (ChampionAntiMelee[target->GetNetworkId()]->Enabled() && CheckTarget(target))
						{							
							W->CastOnPosition(extend);
						}
					}
				}
			}
		}
	}

	static void OnGapcloser(GapCloserSpell const& args)
	{
		if (RGapCloser->Enabled() && R->IsReady() && !args.IsTargeted && GetDistanceVectors(GEntityList->Player()->GetPosition(), args.Source->GetPosition()) < R->Range())
		{
			if (args.Source->IsValidTarget(GEntityList->Player(), R->Range()) && GapCloserList[args.Source->GetNetworkId()]->Enabled() && CheckTarget(args.Source))
			{
				R->CastOnUnit(args.Source);
			}
		}
	}

	static void OnInterruptible(InterruptibleSpell const& Args)
	{
		if (RInterrupter->Enabled() && GetDistance(GEntityList->Player(), Args.Source) < R->Range() && R->IsReady() && CheckTarget(Args.Source))
		{
			R->CastOnUnit(Args.Source);
		}
	}

	static void OnAfterAttack(IUnit* source, IUnit* target)
	{
		if (source != GEntityList->Player() || target == nullptr || !target->IsValidTarget(GEntityList->Player(), E->Range()) || !target->IsVisible())
			return;

		switch (GOrbwalking->GetOrbwalkingMode())
		{
		case kModeCombo:
			if (ComboE2->Enabled() && E->IsReady() && ComboE->GetInteger() == 1 )
			{
				E->CastOnUnit(target);
			}
			break;
		case kModeMixed:
			if (RangeE->Enabled() && E->IsReady() && target->IsHero())
			{
				if (GEntityList->Player()->ManaPercent() >= HarassMana->GetInteger())
				{
					E->CastOnUnit(target);
				}
			}
			break;
		default:;
		}
	}
	
};
