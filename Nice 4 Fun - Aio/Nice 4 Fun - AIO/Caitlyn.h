#pragma once
#include "PluginSDK.h"
#include "BaseMenu.h"
#include "Common.h"

IMenuOption* wCCed;
IMenuOption* WTele;
IMenuOption* WRevive;
IMenuOption* WBush;
IMenuOption* WAmmo;
IMenuOption* WSpell;
IMenuOption* WForce;

class Caitlyn
{
public:

	static void InitializeMenu()
	{
		MainMenu = GPluginSDK->AddMenu("Nice4Fun Federal - Caitlyn");

		WSettings = MainMenu->AddMenu("Trap Settings");
		{
			wCCed = WSettings->CheckBox("Auto Trap on CC", true);
			WTele = WSettings->CheckBox("Auto Trap on Teleport", true);
			WRevive = WSettings->CheckBox("Auto Trap on Revive", true);
			WForce = WSettings->CheckBox("Force Trap After E", true);
			TrapKey = WSettings->AddKey("Use Trap", 87);
			TrapDelayDistance = WSettings->AddInteger("Future Prediction Trap", 300, 1500, 800);
		}

		W2Settings = WSettings->AddMenu("::Auto Trap Full Ammo..");
		{
			AutoTrapFull = W2Settings->CheckBox("Auto Trap Full Ammo", true);
			ManaFullAmmo = W2Settings->AddInteger("Min Mana%", 0, 100, 70);
			AutoInGrass = W2Settings->CheckBox("Trap in Grass Pos", true);
			AutoInTowers = W2Settings->CheckBox("Trap in Towers/Route Pos", true);
			DrawCaitSpots = W2Settings->CheckBox("Draw Traps Spots", true);
		}

		WhiteList = WSettings->AddMenu(":: Trap on GaCloser..");
		{
			for (auto enemy : GEntityList->GetAllHeros(false, true))
			{
				std::string szMenuName = "Trap On Gapcloser - " + std::string(enemy->ChampionName());
				ChampionUse[enemy->GetNetworkId()] = WSettings->CheckBox(szMenuName.c_str(), true);
			}
		}
		
		ComboSettings = MainMenu->AddMenu("Combo Settings");
		{
			ComboQ = ComboSettings->CheckBox("Use Q", true);	
			ComboW = ComboSettings->CheckBox("Use W", true);
			//WAmmo = ComboSettings->AddInteger("Use W | x >=", 1, 5, 2);
			ComboE = ComboSettings->CheckBox("Use E", true);
			ComboE2 = ComboSettings->CheckBox("Force Combo E + Q", true);
			ComboR = ComboSettings->CheckBox("Auto R to Kill", true);			
			RWall = ComboSettings->AddInteger("Min Range to R", 1, 1000, 400);
			RMax = ComboSettings->AddInteger("Max Range to R", 1000, 3000, 1500);
			inUnderTower = ComboSettings->CheckBox("Dont R under turret", true);
			SemiManualKey = ComboSettings->AddKey("Dash to Mouse", 90);
			StartComboKey = ComboSettings->AddKey("Combo E + Trap + Q", 71);
						
		}

		HarassSettings = MainMenu->AddMenu("Harass Settings");
		{
			HarassQ = HarassSettings->CheckBox("Use Q", true);
			HarassE = HarassSettings->CheckBox("Use E", false);
			AutoHarass = HarassSettings->CheckBox("Harass Toggle", false);
			HarassMana = HarassSettings->AddInteger("Minimum MP% to Harass", 1, 100, 60);
		}

		KillstealSettings = MainMenu->AddMenu("Killsteal Settings");
		{
			Killsteal = KillstealSettings->CheckBox("Activate KillSteal", true);
			KillstealQ = KillstealSettings->CheckBox("Use Q to KillSteal", true);			
			KillstealE = KillstealSettings->CheckBox("Use E to KillSteal", true);
		}

		LaneClearSettings = MainMenu->AddMenu("LaneClear Settings");
		{
			LaneClearQ = LaneClearSettings->CheckBox("Use Q in laneclear", true);
			MinionsQ = LaneClearSettings->AddInteger("Minimum minions to Q in laneclear", 1, 10, 4);
			LaneClearE = LaneClearSettings->CheckBox("Use E Kill Out Rage & Q cd", false);
			LaneClearMana = LaneClearSettings->AddInteger("Minimum MP% to laneclear", 1, 100, 60);
		}

		JungleClearSettings = MainMenu->AddMenu("JungleClear Settings");
		{
			JungleQ = JungleClearSettings->CheckBox("Use Q in Jungle", true);			
			JungleE = JungleClearSettings->CheckBox("Use E in Jungle", false);			
			JungleMana = JungleClearSettings->AddInteger("Min Mana to Jungle", 1, 100, 40);
		}

		fedMiscSettings = MainMenu->AddMenu("Miscs Settings");
		{					
			Predic = fedMiscSettings->AddSelection("Q Prediction", 2, std::vector<std::string>({ "Medium", "High", "Very High" }));
			CCedQ = fedMiscSettings->CheckBox("Auto Q on CC", true);
			TrinketBush = fedMiscSettings->CheckBox("Reveler Bush", true);
			//AntiDash = fedMiscSettings->CheckBox("Anti Dash", true);
			EGapCloser = fedMiscSettings->CheckBox("E GapCloser | Anti Meele", true);
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
			DrawQ = DrawingSettings->CheckBox("Draw Q", true);			
			DrawW = DrawingSettings->CheckBox("Draw W", false);
			DrawE = DrawingSettings->CheckBox("Draw E", false);
			DrawR = DrawingSettings->CheckBox("Draw R", false);
			DrawComboDamage = DrawingSettings->CheckBox("Draw combo damage", true);
		}		
	}

	static void LoadSpells()
	{
		Q = GPluginSDK->CreateSpell2(kSlotQ, kLineCast, true, false, static_cast<eCollisionFlags>(kCollidesWithYasuoWall));
		Q->SetSkillshot(0.65f, 60.f, 2200.f, 1250.f);
		W = GPluginSDK->CreateSpell2(kSlotW, kCircleCast, true, false, kCollidesWithNothing);
		W->SetSkillshot(1.5f, 100.f, 3200.f, 800.f);
		E = GPluginSDK->CreateSpell2(kSlotE, kLineCast, true, false, static_cast<eCollisionFlags>(kCollidesWithMinions | kCollidesWithYasuoWall | kCollidesWithHeroes));
		E->SetSkillshot(0.25f, 90.f, 1600.f, 750.f);
		R = GPluginSDK->CreateSpell2(kSlotR, kTargetCast, false, false, static_cast<eCollisionFlags>(kCollidesWithYasuoWall | kCollidesWithHeroes));
		R->SetSkillshot(0.25f, 0.f, 1000.f, 3000.f);

		Ward1 = GPluginSDK->CreateItemForId(3340, 600);
		Ward2 = GPluginSDK->CreateItemForId(3363, 900);
	}	

	static void Drawing()
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

		if (DrawCaitSpots->Enabled())
		{
			if (AutoInGrass->Enabled())
			{
				for (auto tg : GrassSpots)
				{
					GRender->DrawOutlinedCircle(tg, Vec4(255, 255, 255, 255), 60);
				}
			}

			if (AutoInTowers->Enabled())
			{
				for (auto tt : TrapTowerSpots)
				{
					GRender->DrawOutlinedCircle(tt, Vec4(255, 255, 0, 255), 60);
				}

			}
		}
	}

	static int PredicChange()
	{
		if (Predic->GetInteger() == 0)
		{
			return mypredic = kHitChanceMedium;
		}
		if (Predic->GetInteger() == 1)
		{
			return mypredic = kHitChanceHigh;
		}
		if (Predic->GetInteger() == 2)
		{
			return mypredic = kHitChanceVeryHigh;
		}

		return mypredic = kHitChanceLow;
	}

	static void CastQ()
	{
		auto qtarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());

		if (!CheckTarget(qtarget)) return;

		if (qtarget->IsValidTarget(GEntityList->Player(), Q->Range()) && Q->IsReady())
		{
			Q->CastOnTarget(qtarget, PredicChange());
		}
	}	

	static void Automatic()
	{
		WAntiMelee();
		TrapRevelerBush();
		ComboWithKey();
		TrapWithKey();
		TrapWhenFull();		
		
		if (CheckTime < GGame->TickCount())
		{			
			putWard = false;
		}
		
		if (AutoHarass->Enabled())
		{
			Harass();
		}

		for (auto target : GEntityList->GetAllHeros(false, true))
		{
			if (!CheckTarget(target)) return;
			
			if (!target->HasBuff("ChronoShift") && Killsteal->Enabled())
			{
				if (KillstealQ->Enabled() && Q->IsReady() && target->IsValidTarget(GEntityList->Player(), Q->Range()) && 
					GHealthPrediction->GetKSDamage(target, kSlotQ, Q->GetDelay(), false) > target->GetHealth())
				{
					Q->CastOnTarget(target, PredicChange());
				}

				if (KillstealE->Enabled() && E->IsReady() && target->IsValidTarget(GEntityList->Player(), E->Range()) && 
					GHealthPrediction->GetKSDamage(target, kSlotE, E->GetDelay(), false) > target->GetHealth())
				{
					E->CastOnTarget(target, PredicChange());
				}

				auto delay = R->GetDelay() + GetDistance(GEntityList->Player(), target) / R->Speed();

				if (ComboR->Enabled() && R->IsReady() && target->IsValidTarget(GEntityList->Player(), RMax->GetInteger()) && 
					GHealthPrediction->GetKSDamage(target, kSlotR, delay, false) > target->GetHealth() + 50)
				{
					if (IsUnderTurret(GEntityList->Player()) && inUnderTower->Enabled()) { return; }

					if (GetDistance(GEntityList->Player(), target) > RWall->GetInteger() && 
						GetDistance(GEntityList->Player(), target) < RMax->GetInteger())
					{
						R->CastOnUnit(target);
					}
				}
			}

			if (CCedQ->Enabled() && target->IsValidTarget(GEntityList->Player(), Q->Range() - 50) && 
				Q->IsReady() && !CanMove(target) && CheckTarget(target) && GEntityList->Player()->GetMana() > Q->ManaCost())
			{
				Q->CastOnTarget(target, PredicChange());
			}

			if (wCCed->Enabled() && target->IsValidTarget(GEntityList->Player(), W->Range() - 50) && W->IsReady() && !CanMove(target) && 
				CheckTarget(target) && GEntityList->Player()->GetMana() > W->ManaCost() && !target->HasBuff("CaitlynYordleTrapInternal"))
			{
				if (GGame->TickCount() - LastWTick > 1500)
				{
					W->CastOnPosition(target->GetPosition());
				}
			}
		}
	}
	
	static void Combo()
	{		
		auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());		

		if (!CheckTarget(target)) return;

		if (!WForce->Enabled() || !E->IsReady())
		{
			//LordWTest(target);
			if (W->IsReady())
			{
				Vec3 position;
				GPrediction->GetFutureUnitPosition(target, (TrapDelayDistance->GetInteger() / 1000 ), true, position);

				W->CastOnPosition(position);
			}
		}

		if (!ComboE2->Enabled() && ComboE->Enabled() && E->IsReady() && target->IsValidTarget(GEntityList->Player(), E->Range()))
		{
			E->CastOnTarget(target, PredicChange());
		}

		if (!ComboE2->Enabled() && ComboQ->Enabled() && Q->IsReady() && target->IsValidTarget(GEntityList->Player(), Q->Range() - 50) && GEntityList->Player()->GetMana() > Q->ManaCost() + E->ManaCost())
		{
			Q->CastOnTarget(target, PredicChange());
		}
		
		if (ComboE2->Enabled() && GEntityList->Player()->GetMana() > Q->ManaCost() + E->ManaCost())
		{
			auto qtarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());
			auto etarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, E->Range());

			if (!CheckTarget(qtarget) || !CheckTarget(etarget)) return;

			if (etarget->IsValidTarget(GEntityList->Player(), E->Range()) && Q->IsReady() && E->IsReady())
			{				
				E->CastOnTarget(etarget, PredicChange());
			}		
			
			else if (!WForce->Enabled() && qtarget->IsValidTarget(GEntityList->Player(), Q->Range()) && Q->IsReady() && !E->IsReady())
			{
				Q->CastOnTarget(qtarget, PredicChange());
			}

			else if (WForce->Enabled() && !E->IsReady())
			{
				if (WSpellStatus && W->IsReady() && qtarget->IsValidTarget(GEntityList->Player(), W->Range()))
				{
					W->CastOnPosition(target->GetPosition());
					WSpellStatus = false;					
				}

				if (Q->IsReady() && qtarget->IsValidTarget(GEntityList->Player(), Q->Range()))
				{					
					GPluginSDK->DelayFunctionCall(10, []() { CastQ(); });	
				}
			}			
		}		
	}

	static void Harass()
	{
		auto qtarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());
		auto etarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, E->Range());

		if (!CheckTarget(qtarget) || !CheckTarget(etarget) || GEntityList->Player()->ManaPercent() <= HarassMana->GetInteger()) return;

		if (HarassE->Enabled() && etarget->IsValidTarget(GEntityList->Player(), E->Range()) && E->IsReady())
		{
			E->CastOnTarget(etarget, PredicChange());
		}

		if (HarassQ->Enabled() && qtarget->IsValidTarget(GEntityList->Player(), Q->Range()) && Q->IsReady())
		{
			Q->CastOnTarget(qtarget, PredicChange());
		}
	}

	static void JungleClear()
	{
		if (JungleMana->GetInteger() < GEntityList->Player()->ManaPercent())
		{
			SArray<IUnit*> Minion = SArray<IUnit*>(GEntityList->GetAllMinions(false, false, true)).Where([](IUnit* m) {return m != nullptr &&
				!m->IsDead() && m->IsVisible() && m->IsValidTarget(GEntityList->Player(), Q->Range()) && m->IsJungleCreep() && !strstr(m->GetObjectName(), "WardCorpse"); });

			if (Minion.Any())
			{
				if (JungleE->Enabled() && E->IsReady() && !FoundMinions(E->Range()) && FoundMinionsNeutral(Q->Range()))
				{
					auto eMinion = Minion.MinOrDefault<float>([](IUnit* i) {return GetDistanceVectors(i->GetPosition(), GGame->CursorPosition()); });

					if (eMinion->IsValidTarget(GEntityList->Player(), E->Range()))
					{
						E->CastOnTarget(eMinion);
					}
				}

				else if (JungleQ->Enabled() && Q->IsReady() && !FoundMinions(E->Range()) && FoundMinionsNeutral(Q->Range()))
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
						Q->CastOnUnit(Minion.MinOrDefault<float>([](IUnit* i) {return GetDistanceVectors(i->GetPosition(), GGame->CursorPosition()); }));
					}
				}
			}
		}
	}

	static void LaneClear()
	{
		if (GEntityList->Player()->ManaPercent() > LaneClearMana->GetInteger())
		{
			if (LaneClearQ->Enabled() && Q->IsReady() && !FoundMinionsNeutral(E->Range()) && GetMinionsInRange(GEntityList->Player()->GetPosition(), Q->Range()) >= MinionsQ->GetInteger())
			{
				if (MinionsQ->GetInteger() >= 3)
				{
					Vec3 pos;
					int count;
					Q->FindBestCastPosition(true, true, pos, count);

					if (count >= 3 && Q->CastOnPosition(pos))
					{
						return;
					}
				}
				else
				{
					Vec3 pos;
					int count;
					Q->FindBestCastPosition(true, true, pos, count);

					if (count >= MinionsQ->GetInteger() && Q->CastOnPosition(pos))
					{
						return;
					}
				}
			}

			for (auto minion : GEntityList->GetAllMinions(false, true, false))
			{

				if (LaneClearE->Enabled() && E->IsReady() && !FoundMinionsNeutral(E->Range() + 100))
				{
					if (!Q->IsReady() || !LaneClearQ->Enabled())
					{
						auto damage = GHealthPrediction->GetKSDamage(minion, kSlotE, E->GetDelay(), false);

						if (minion->IsCreep() && damage > minion->GetHealth() && minion->IsValidTarget(GEntityList->Player(), R->Range()) &&
							GetDistance(GEntityList->Player(), minion) > GEntityList->Player()->GetRealAutoAttackRange(minion) + 50 )
						{
							GOrbwalking->ResetAA();
							E->CastOnTarget(minion);
						}
					}
				}
			}
		}
	}

	static void LordWTest(IUnit* target)
	{
		if (CheckTarget(target) && ComboW->Enabled() && W->IsReady() && /*((GEntityList->Player()->GetSpellBook()->GetAmmo(kSlotW) >= WAmmo->GetInteger())*/  GEntityList->Player()->IsValidTarget(target, W->Range()))
		{
			//auto wTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, W->Range());
			if (GGame->TickCount() - LastWTick > 1500)
			{
				if (target->IsFacing(GEntityList->Player()))
				{
					if (target->IsMelee() && GetDistance(GEntityList->Player(), target) < target->AttackRange() + 100)
					{
						W->CastOnPosition(GEntityList->Player()->ServerPosition());
					}

					else
					{
						auto pred = new AdvPredictionOutput();
						if (W->RunPrediction(target, false, kCollidesWithNothing, pred))
						{
							if (pred->HitChance >= kHitChanceVeryHigh && GEntityList->Player()->IsValidTarget(target, W->Range()))
							{
								W->CastOnPosition(pred->CastPosition);
							}
						}
					}
				}
				else
				{
					auto pred2 = new AdvPredictionOutput();
					if (W->RunPrediction(target, false, kCollidesWithNothing, pred2))
					{
						if (pred2->HitChance >= kHitChanceVeryHigh && GEntityList->Player()->IsValidTarget(target, W->Range()))
						{
							W->CastOnPosition(pred2->CastPosition + ((target->ServerPosition() - GEntityList->Player()->ServerPosition()).VectorNormalize()) * 100);
						}
					}
				}
			}
		}
	}

	static void TrapRevelerBush()
	{
		if (putWard && TrinketBush->Enabled() && (checkWardsTemp() || W->IsReady()))
		{
			auto pPos = GEntityList->Player()->GetPosition();
			auto distance = GetDistanceVectors(pPos, WardPos);
			auto check = 200 / 20;

			for (auto i = 1; i < 20; i++)
			{
				JumpPos = pPos.Extend(WardPos, (distance - 200) + (check * i));

				if (GNavMesh->IsPointGrass(JumpPos))
				{
					if (Ward1->IsReady() && Ward1->IsOwned())
					{
						Ward1->CastOnPosition(JumpPos);
					}
					else if (Ward2->IsReady() && Ward2->IsOwned())
					{
						Ward2->CastOnPosition(JumpPos);
					}
					else if (W->IsReady())
					{
						W->CastOnPosition(JumpPos);
					}
				}
			}
		}
	}

	static bool checkWardsTemp()
	{
		if (Ward1->IsReady() && Ward1->IsOwned())
		{
			return true;
		}

		if (Ward2->IsReady() && Ward2->IsOwned())
		{
			return true;
		}

		return false;
	}

	static void AutoTrap()
	{
		//GetBuffName();

		/*auto Wposition = GetTrapPos(W->Range());

		if (Wposition.x > 0 && Wposition.y > 0 && W->IsReady() && WTele->Enabled() && GGame->TickCount() - LastWTick > 1500)
		{			
			auto finalPosition = Wposition.Extend(GEntityList->Player()->GetPosition(), 50);
			
			W->CastOnPosition(finalPosition);			
		}*/
	}

	static void DashToMouse()
	{
		if (IsKeyDown(SemiManualKey) && E->IsReady())
		{
			auto position = GEntityList->Player()->ServerPosition() - (GGame->CursorPosition() - GEntityList->Player()->ServerPosition());
			E->CastOnPosition(position);
		}
	}

	static void GetBuffName()
	{
		std::vector<void*> vecBuffs;

		for (auto enemy : GEntityList->GetAllHeros(false, true))
		{

			if (enemy->IsValidTarget(GEntityList->Player(), W->Range()))
			{
				enemy->GetAllBuffsData(vecBuffs);
			}

			for (auto i : vecBuffs)
			{
				GBuffData->GetBuffName(i);
				GGame->PrintChat(enemy->ChampionName());
				GGame->PrintChat(GBuffData->GetBuffName(i));

				/*if (GEntityList->Player()->HasBuff("AhriTumble"))
				{
				auto buffTime = GBuffData->GetEndTime(GEntityList->Player()->GetBuffDataByName("AhriTumble"));


				GGame->PrintChat("Tenho Buff do Ult");
				GGame->PrintChat(std::to_string(buffTime - GGame->Time()).data());
				}*/
			}
		}		
	}

	static void OnCreateObject(IUnit* Source)
	{
		if (WRevive->Enabled() && GGame->TickCount() - LastWTick > 1500)
		{
			if (strstr(Source->GetObjectName(), "LifeAura.troy") || strstr(Source->GetObjectName(), "ZileanBase_R_Buf.troy"))
			{
				if (W->IsReady() && Source->IsEnemy(GEntityList->Player()))
				{
					W->CastOnUnit(Source);
				}
			}
		}

		if (WTele->Enabled() && GGame->TickCount() - LastWTick > 1500)
		{
			if (strstr(Source->GetObjectName(), "global_ss_teleport_arrive_red.troy"))
			{				
				if (W->IsReady() && Source->IsEnemy(GEntityList->Player()))
				{
					W->CastOnUnit(Source);
				}
			}
		}		

		//GUtility->LogConsole("Create %s", Source->GetObjectName());
	}	

	static void OnDeleteObject(IUnit* Source)
	{
		//GUtility->LogConsole("Delete %s", Source->GetObjectName());
	}

	static void OnProcessSpell(CastedSpell const& Args)
	{
		if (Args.Caster_ == GEntityList->Player())
		{
			if (strstr(Args.Name_, "TrinketTotemLvl1"))
			{
				if (CheckTime > GGame->TickCount())
				{					
					putWard = false;
				}
			}

			if (strstr(Args.Name_, "TrinketOrbLvl3"))
			{
				if (CheckTime > GGame->TickCount())
				{					
					putWard = false;
				}
			}

			if (strstr(Args.Name_, "CaitlynEntrapment") && WForce->Enabled())
			{
				WSpellStatus = true;
			}

			if (WForce->Enabled() && strstr(Args.Name_, "CaitlynYordleTrap"))
			{

			}

			if (GSpellData->GetSlot(Args.Data_) == kSlotW)
			{
				if (CheckTime > GGame->TickCount())
				{
					putWard = false;
				}

				//GUtility->LogConsole("Vec3(%ff, %ff, %ff),", Args.EndPosition_.x, Args.EndPosition_.y, Args.EndPosition_.z);

				LastWTick = GGame->TickCount();
			}

			if (GSpellData->GetSlot(Args.Data_) == kSlotQ)
			{
				LastQTick = GGame->TickCount();

				if (temp)
				{
					temp = false;
				}
			}
		}
	}

	static void OnDash(UnitDash* Source)
	{
		/*if (AntiDash->Enabled())
		{
			if (GetDistance(GEntityList->Player(), Source->Source) <= E->Range() || GetDistanceVectors(GEntityList->Player()->GetPosition(), Source->EndPosition) <= E->Range() ||
				GetDistanceVectors(GEntityList->Player()->GetPosition(), Source->StartPosition) <= E->Range())
			{
				if (Source->Source->IsValidTarget(GEntityList->Player(), E->Range()))
				{
					E->CastOnTarget(Source->Source, kHitChanceHigh);
				}
			}
		}*/
	}

	static void OnGapcloser(GapCloserSpell const& args)
	{
		if (!CheckTarget(args.Source)) return;
		
		if (EGapCloser->Enabled() && E->IsReady() && !args.IsTargeted && GetDistanceVectors(GEntityList->Player()->GetPosition(), args.EndPosition) < E->Range())
		{
			if (GapCloserList[args.Source->GetNetworkId()]->Enabled())
			{
				E->CastOnTarget(args.Source, PredicChange());
			}
		}

		if (ChampionUse[args.Source->GetNetworkId()]->Enabled() && W->IsReady() && !args.IsTargeted && GetDistanceVectors(GEntityList->Player()->GetPosition(), args.EndPosition) < W->Range())
		{
			W->CastOnPosition(args.EndPosition);
		}
	}

	static void WAntiMelee()
	{
		if (EGapCloser->Enabled() && E->IsReady())
		{
			for (auto target : GEntityList->GetAllHeros(false, true))
			{
				if (!CheckTarget(target)) return;

				if (target->IsMelee() && GetDistance(GEntityList->Player(), target) < 300  && target->IsValidTarget(GEntityList->Player(), GEntityList->Player()->GetRealAutoAttackRange(target)))
				{					
					auto dashpos = GEntityList->Player()->ServerPosition();
					auto extend = dashpos.Extend(target->GetPosition(), -E->Range());					
					
					if (!GNavMesh->IsPointWall(extend) && (CountEnemy(extend, 1000) == 0 || CountAlly(extend, 1000) > CountEnemy(extend, 1000)))
					{						
						if (ChampionAntiMelee[target->GetNetworkId()]->Enabled())
						{							
							E->CastOnUnit(target);
						}
					}
				}
			}
		}
	}

	static void OnExitVisible(IUnit* Source)
	{
		if (TrinketBush->Enabled() && Source->IsHero() && GetDistance(GEntityList->Player(), Source) < 700)
		{		
			WardPos = Source->GetPosition();
			putWard = true;
			CheckTime = GGame->TickCount() + 2000;
			//timeTrinket = GGame->TickCount() + TrinketBushdelay->GetInteger();			
		}		
	}

	static void ComboWithKey()
	{
		if (IsKeyDown(StartComboKey))
		{
			GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
			
			if (GEntityList->Player()->GetMana() > Q->ManaCost() + E->ManaCost() + W->ManaCost())
			{
				auto qtarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());
				auto etarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, E->Range());

				if (!CheckTarget(qtarget) || !CheckTarget(etarget)) return;
				
				if (etarget->IsValidTarget(GEntityList->Player(), E->Range()) && E->IsReady())
				{
					E->CastOnTarget(etarget, PredicChange());
					temp = true;
				}

				else if (!E->IsReady() && temp)
				{
					if (Q->IsReady() && W->IsReady())
					{
						if (WSpellStatus && W->IsReady() && qtarget->IsValidTarget(GEntityList->Player(), W->Range()))
						{
							W->CastOnPosition(qtarget->GetPosition());
							WSpellStatus = false;
						}

						if (Q->IsReady() && qtarget->IsValidTarget(GEntityList->Player(), Q->Range()))
						{
							GPluginSDK->DelayFunctionCall(10, []() 
							{ 
								CastQ();								
							});
						}
					}
					else if (Q->IsReady() && !W->IsReady())
					{
						if (qtarget->IsValidTarget(GEntityList->Player(), Q->Range()))
						{
							CastQ();							
						}
					}
				}
			}
		}
	}

	static void TrapWithKey()
	{
		keystate3 = GetAsyncKeyState(TrapKey->GetInteger());

		if (keystate3 < 0 && LastPress < GGame->TickCount())
		{
			if (harassKeyWasDown == false)
			{
				if (W->IsReady())
				{
					auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, W->Range());
					if (CheckTarget(target))
					{
						//W->CastOnTarget(target, kHitChanceVeryHigh);
						//W->CastOnUnit(target);

						Vec3 position;
						GPrediction->GetFutureUnitPosition(target, (TrapDelayDistance->GetInteger() / 1000), true, position);

						W->CastOnPosition(position);
						LastPress = GGame->TickCount() + 500;
					}
				}			

				harassKeyWasDown = true;
			}
		}
		else
		{
			harassKeyWasDown = false;			
		}
	}

	static int FullAmmoCount()
	{
		int baseAmmo[6] = { 0, 3, 3, 4, 4, 5 };
		return baseAmmo[GEntityList->Player()->GetSpellLevel(kSlotW)];
	}
	
	static void TrapWhenFull()
	{
		if (AutoTrapFull->Enabled() && static_cast<int>(GGame->Time() * 10) % 2 == 0 && W->IsReady() &&
			GEntityList->Player()->GetSpellBook()->GetAmmo(kSlotW) == FullAmmoCount() &&
			GOrbwalking->GetOrbwalkingMode() == kModeNone &&
			GEntityList->Player()->ManaPercent() > ManaFullAmmo->GetInteger())
		{
			if (FullAmmoCount() == 3 && CountCaitTrap(GEntityList->Player()->GetPosition(), 25000) >= 3) return;
			
			//Grass
			if (AutoInGrass->Enabled())
			{
				for (auto grass : GrassSpots)
				{
					if (CountCaitTrap(grass, 60) == 0 && CountEnemy(GEntityList->Player()->GetPosition(), 1000) == 0 && GetDistanceVectors(grass, GEntityList->Player()->GetPosition()) < W->Range())
					{
						W->CastOnPosition(grass);						
					}
				}
			}

			//Towers
			if (AutoInTowers->Enabled())
			{
				for (auto tws : TrapTowerSpots)
				{
					if (CountCaitTrap(tws, 60) == 0 && GetDistanceVectors(tws, GEntityList->Player()->GetPosition()) < W->Range() && CountEnemy(GEntityList->Player()->GetPosition(), 1000) == 0 && (!IsUnderTurretPosAlly(tws) && !IsUnderTurretPos(tws) || IsUnderTurretPos(tws)))
					{
						W->CastOnPosition(tws);
					}
				}
			}
		}	
	}	
};
