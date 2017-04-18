#pragma once
#include "PluginSDK.h"
#include "BaseMenu.h"
#include "Common.h"

class Kindred
{
public:

	static void InitializeMenu()
	{
		MainMenu = GPluginSDK->AddMenu("Nice4Fun - Kindred");

		RSettings = MainMenu->AddMenu("R Settings");
		{
			ComboR = RSettings->AddSelection("When receive Attack..", 3, std::vector<std::string>({ "Off", "Enemys", "Towers and Jungle Mobs", "Both" }));			 
			RHP = RSettings->CheckBox("R MySelf", true);
			HealthR = RSettings->AddInteger("MySelf Min Hp%", 1, 100, 20);			
			REnemies = RSettings->AddInteger("Max Enemies in Range R", 1, 5, 4);			
			UltPercent = RSettings->AddInteger("Min HP% To R Allys", 1, 100, 15);
			RAllys = RSettings->AddInteger("Min Allys in Range R", 1, 5, 1);
			for (auto ally : GEntityList->GetAllHeros(true, false))
			{
				if (ally != GEntityList->Player())
				{
					std::string szMenuName = "Use R on - " + std::string(ally->ChampionName());
					ChampionUse[ally->GetNetworkId()] = RSettings->CheckBox(szMenuName.c_str(), true);
				}
			}
		}

		QSettings = MainMenu->AddMenu("Q Settings");
		{
			ComboQH = QSettings->AddSelection("Q Modes", 2, std::vector<std::string>({ "Q To Cursor", "Q Side", "Default" }));
			EnemyCheck = QSettings->AddInteger("Block Q in x enemies", 1, 5, 3);
			WallCheck = QSettings->CheckBox("Block Q in wall", true);
			TurretCheck = QSettings->CheckBox("Block Q under turret", false);
			AAcheck = QSettings->CheckBox("Q only in AA range", false);
			QAntiMelee = QSettings->AddSelection("Auto Q Anti Melee", 2, std::vector<std::string>({ "Off", "After Hit Me", "Near Me" }));
			QCancelAnimation = QSettings->CheckBox("Q Cancel Animation", false);
		}

		ESettings = MainMenu->AddMenu("E Settings");
		{
			EOrder = ESettings->CheckBox("Focus E Target", true);
			for (auto enemys : GEntityList->GetAllHeros(false, true))
			{
				std::string szMenuName = "Use E on - " + std::string(enemys->ChampionName());
				ChampionUseE[enemys->GetNetworkId()] = ESettings->CheckBox(szMenuName.c_str(), true);
			}
		}
		
		ComboSettings = MainMenu->AddMenu("Combo Settings");
		{					
			ComboQ = ComboSettings->CheckBox("Use Q", true);
			ComboW = ComboSettings->CheckBox("Use W", true);
			ComboE = ComboSettings->CheckBox("Use E", true);			
		}

		LaneClearSettings = MainMenu->AddMenu("LaneClear Settings");
		{
			LaneClearQ = LaneClearSettings->CheckBox("Q laneclear", true);
			LaneClearW = LaneClearSettings->CheckBox("W laneclear", true);
			MinionsQ = LaneClearSettings->AddInteger("LaneClear minimum minions", 1, 6, 3);
			LaneClearMana = LaneClearSettings->AddInteger("Minimum MP% to laneclear", 1, 100, 40);
		}

		JungleClearSettings = MainMenu->AddMenu("JungleClear Settings");
		{
			JungleQ = JungleClearSettings->CheckBox("Use Q in Jungle", true);
			JungleW = JungleClearSettings->CheckBox("Use W in Jungle", true);
			JungleE = JungleClearSettings->CheckBox("Use E in Jungle", true);
			JungleEBig = JungleClearSettings->CheckBox("W & E only Big Mobs", true);
			JungleMana = JungleClearSettings->AddInteger("Min Mana to Jungle", 0, 100, 30);
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
		Q = GPluginSDK->CreateSpell2(kSlotQ, kLineCast, false, false, (kCollidesWithNothing));
		Q->SetSkillshot(0.25, 500, 1400, 340);

		W = GPluginSDK->CreateSpell2(kSlotW, kTargetCast, false, false, (kCollidesWithNothing));
		W->SetSkillshot(0.25, 0, 1400, 800);

		E = GPluginSDK->CreateSpell2(kSlotE, kTargetCast, false, false, (kCollidesWithNothing));
		E->SetSkillshot(0.25, 0, 1000, 500);

		R = GPluginSDK->CreateSpell2(kSlotR, kTargetCast, false, false, (kCollidesWithNothing));
		R->SetSkillshot(0.25, 0, 1000, 500);
	}

	static bool CheckAutoAttackRange(Vec3 point)
	{
		if (!AAcheck->Enabled())
		{
			return true;
		}
		else if (GOrbwalking->GetLastTarget() != nullptr && GOrbwalking->GetLastTarget()->IsHero())
		{
			return GetDistanceVectors(point, GOrbwalking->GetLastTarget()->GetPosition()) < GEntityList->Player()->AttackRange();
		}
		else
		{
			return CountEnemy(point, GEntityList->Player()->AttackRange()) > 0;
		}
	}

	static bool CheckPosition(Vec3 Pos)
	{
		if (TurretCheck->Enabled() && IsUnderTurretPos(Pos) && !IsUnderTurret(GEntityList->Player()))
		{
			return false;
		}

		if (EnemyCheck->GetInteger() > CountEnemy(Pos, 600))
		{
			return true;
		}

		if (CountEnemy(Pos, 600) <= CountEnemy(GEntityList->Player()->GetPosition(), 450))
		{
			return true;
		}

		if (WallCheck->Enabled())
		{
			float check = Q->Range() + 50 / 5;

			for (int i = 1; i <= 5; i++)
			{
				auto pPos = GEntityList->Player()->GetPosition();

				if (GPrediction->IsPointWall(pPos.Extend(Pos, i * check)))
				{
					return false;
				}
			}
		}

		return false;
	}

	static Vec3 QDashPosition(IUnit* target = nullptr)
	{
		Vec3 DashPos;
		auto pPos = GEntityList->Player()->GetPosition();
		auto cPos = GGame->CursorPosition();

		if (ComboQH->GetInteger() == 0)
		{
			DashPos = pPos.Extend(cPos, Q->Range());
		}
		else if (ComboQH->GetInteger() == 1)
		{
			Vec2 sPos = pPos.To2D();
			Vec2 endPos = target->GetPosition().To2D();

			auto dir = (endPos - sPos).VectorNormalize();
			auto pDir = dir.Perpendicular();

			auto right = endPos + pDir * GetDistance(GEntityList->Player(), target);
			auto left = endPos - pDir * GetDistance(GEntityList->Player(), target);

			auto rEPosistion = To3D(right);
			auto lEPosistion = To3D(left);

			if (GetDistanceVectors(cPos, rEPosistion) < GetDistanceVectors(cPos, lEPosistion))
			{
				DashPos = pPos.Extend(rEPosistion, Q->Range());
			}
			else
			{
				DashPos = pPos.Extend(lEPosistion, Q->Range());
			}
		}
		else
		{
			auto after = GEntityList->Player()->GetPosition() + (GGame->CursorPosition() - GEntityList->Player()->GetPosition()).VectorNormalize() * Q->Range();

			if (GetDistanceVectors(target->GetPosition(), after)  < 630 && GetDistanceVectors(target->GetPosition(), after) > 150)
			{
				return cPos;
			}
			if (GetDistanceVectors(target->GetPosition(), GEntityList->Player()->GetPosition()) > 630 && GetDistanceVectors(target->GetPosition(), after) < 630)
			{
				return cPos;
			}
		}

		if (DashPos == Vec3(0, 0, 0))
		{
			return Vec3(0, 0, 0);
		}		

		if (CheckPosition(DashPos) && CheckAutoAttackRange(DashPos))
		{
			return DashPos;
		}

		return Vec3(0, 0, 0);
	}	

	static void Automatic()
	{	
		FocusTargetE();	
	}

	static void FocusTargetE()
	{
		if (EOrder->Enabled())
		{
			SArray<IUnit*> enemy = SArray<IUnit*>(GEntityList->GetAllHeros(false,true)).Where([](IUnit* m) {return m != nullptr &&
				!m->IsDead() && m->IsVisible() && m->IsValidTarget(GEntityList->Player(), GEntityList->Player()->AttackRange()) &&
				m->HasBuff("KindredERefresher"); });

			SArray<IUnit*> minions = SArray<IUnit*>(GEntityList->GetAllMinions(false, true, true)).Where([](IUnit* m) {return m != nullptr &&
				!m->IsDead() && m->IsVisible() && m->IsValidTarget(GEntityList->Player(), GEntityList->Player()->AttackRange()) &&
				m->HasBuff("KindredERefresher"); });

			if (enemy.Any())
			{
				GOrbwalking->SetOverrideTarget(enemy.FirstOrDefault());
			}

			if (minions.Any())
			{
				GOrbwalking->SetOverrideTarget(minions.FirstOrDefault());
			}
		}
	}

	static void Combo()
	{
		if (Q->IsReady() && GEntityList->Player()->GetMana() > R->ManaCost() + Q->ManaCost() && ComboQ->Enabled())
		{
			auto qtarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, GEntityList->Player()->AttackRange() + 300);

			if (CheckTarget(qtarget))
			{
				if (GetDistance(GEntityList->Player(), qtarget) > GEntityList->Player()->AttackRange() &&
					GetDistanceVectors(qtarget->GetPosition(), GGame->CursorPosition()) < GetDistance(GEntityList->Player(), qtarget) && !qtarget->IsFacing(GEntityList->Player()))
				{
					auto pPos = GEntityList->Player()->GetPosition();
					auto dash = pPos.Extend(qtarget->GetPosition(), Q->Range());

					if (CheckPosition(dash))
					{
						if (QAntiMelee->GetInteger() == 2 && GetDistance(GEntityList->Player(), qtarget) <= qtarget->GetRealAutoAttackRange(GEntityList->Player())
							&& qtarget->IsMelee() && qtarget->IsFacing(GEntityList->Player()))
						{
							Q->CastOnPosition(pPos.Extend(qtarget->GetPosition(), -Q->Range()));
							return;
						}

						Q->CastOnPosition(dash);
					}
				}				
			}
		}

		auto wTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, W->Range());

		if (!CheckTarget(wTarget)) return;

		if (W->IsReady() && GEntityList->Player()->GetMana() > R->ManaCost() + W->ManaCost() && ComboW->Enabled())
		{
			if (wTarget->IsValidTarget(GEntityList->Player(), 650) && !Q->IsReady())
			{
				W->CastOnPlayer();
			}
		}

		if (E->Range() && GEntityList->Player()->GetMana() > R->ManaCost() + E->ManaCost() && ComboE->Enabled())
		{
			if (wTarget->IsValidTarget(GEntityList->Player(), E->Range()) && ChampionUseE[wTarget->GetNetworkId()]->Enabled())
			{
				E->CastOnUnit(wTarget);
			}
		}
	}	

	static void JungleClear()
	{
		if (GEntityList->Player()->ManaPercent() < JungleMana->GetInteger()) return;		

		SArray<IUnit*> Minion = SArray<IUnit*>(GEntityList->GetAllMinions(false, false, true)).Where([](IUnit* m) {return m != nullptr &&
			!m->IsDead() && m->IsVisible() && m->IsValidTarget(GEntityList->Player(), W->Range()) && m->IsJungleCreep() && !strstr(m->GetObjectName(), "WardCorpse"); });

		if (Minion.Any())
		{
			for (auto jMonster : Minion.ToVector())
			{

				if (JungleE->Enabled() && E->IsReady())
				{
					if (GEntityList->Player()->IsValidTarget(jMonster, E->Range()))
					{
						if (JungleEBig->Enabled())
						{
							if (strstr(jMonster->GetObjectName(), "Dragon") ||
								strstr(jMonster->GetObjectName(), "Baron") ||
								strstr(jMonster->GetObjectName(), "Red") ||
								strstr(jMonster->GetObjectName(), "Blue") ||
								strstr(jMonster->GetObjectName(), "RiftHerald") ||
								strstr(jMonster->GetObjectName(), "Gromp") ||
								strstr(jMonster->GetObjectName(), "Crab") ||
								strstr(jMonster->GetObjectName(), "Razorbeak3") ||
								strstr(jMonster->GetObjectName(), "SRU_Krug") ||
								strstr(jMonster->GetObjectName(), "SRU_Murkwolf2"))
							{
								E->CastOnUnit(jMonster);
							}
						}
						else
						{
							E->CastOnUnit(jMonster);
						}
					}
				}

				if (JungleQ->Enabled() && Q->IsReady() && GEntityList->Player()->IsValidTarget(jMonster, 600))
				{
					Q->CastOnPosition(GGame->CursorPosition());
				}

				if (JungleW->Enabled() && W->IsReady() && GEntityList->Player()->IsValidTarget(jMonster, W->Range()))
				{
					if (JungleEBig->Enabled())
					{
						if (strstr(jMonster->GetObjectName(), "Dragon") ||
							strstr(jMonster->GetObjectName(), "Baron") ||
							strstr(jMonster->GetObjectName(), "Red") ||
							strstr(jMonster->GetObjectName(), "Blue") ||
							strstr(jMonster->GetObjectName(), "RiftHerald") ||
							strstr(jMonster->GetObjectName(), "Gromp") ||
							strstr(jMonster->GetObjectName(), "Crab") ||
							strstr(jMonster->GetObjectName(), "Razorbeak3") ||
							strstr(jMonster->GetObjectName(), "SRU_Krug") ||
							strstr(jMonster->GetObjectName(), "SRU_Murkwolf2"))
						{
							W->CastOnPlayer();
						}
					}
					else
					{
						W->CastOnPlayer();
					}
				}
			}
		}
	}

	static void LaneClear()
	{
		if (GEntityList->Player()->ManaPercent() < LaneClearMana->GetInteger()) return;

		if (Q->IsReady() && LaneClearQ->Enabled())
		{
			if (CountMinions(GEntityList->Player()->GetPosition(), 450) >= MinionsQ->GetInteger())
			{
				Q->CastOnPosition(GGame->CursorPosition());
			}
		}

		if (W->IsReady() && LaneClearW->Enabled())
		{
			if (CountMinions(GEntityList->Player()->GetPosition(), 600) >= MinionsQ->GetInteger())
			{
				W->CastOnPlayer();
			}
		}
	}

	static void Drawing()
	{
		if (DrawReady->Enabled())
		{
			if (Q->IsReady() && DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }
			if (E->IsReady() && DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }
			if (W->IsReady() && DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), W->Range()); }
			if (R->IsReady() && DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), R->Range()); }
		}
		else
		{
			if (DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }
			if (DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }
			if (DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), W->Range()); }
			if (DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), R->Range()); }
		}
	}

	static void OnGapcloser(GapCloserSpell const& args)
	{

	}

	static void OnAfterAttack(IUnit* source, IUnit* target)
	{		
		if (!target->IsHero() || !Q->IsReady()) return;

		if (ComboQ->Enabled() && GEntityList->Player()->GetMana() > R->ManaCost() + Q->ManaCost())
		{			
			if (target->IsValidTarget())
			{			
				if (QDashPosition(target) != Vec3(0, 0, 0) && CountEnemy(QDashPosition(target), 600) > 0)
				{
					if (QAntiMelee->GetInteger() == 2 && GetDistance(GEntityList->Player(), target) <= target->GetRealAutoAttackRange(GEntityList->Player())
						&& target->IsMelee() && target->IsFacing(GEntityList->Player()))
					{
						auto pPos = GEntityList->Player()->GetPosition();
						Q->CastOnPosition(pPos.Extend(target->GetPosition(), -Q->Range()));
						return;
					}
					Q->CastOnPosition(QDashPosition(target));
				}
			}
		}		
	}	

	static void OnBuffRemove(IUnit* Source, void* BuffData)
	{
		if (strstr(GBuffData->GetBuffName(BuffData), "KindredERefresher"))
		{
			GOrbwalking->SetOverrideTarget(nullptr);
		}
	}

	static void OnProcessSpell(CastedSpell const& Args)
	{
		if (Args.Caster_ == GEntityList->Player())
		{
			if (GSpellData->GetSlot(Args.Data_) == kSlotQ)
			{
				if (QCancelAnimation->Enabled())
				{
					GGame->Taunt(kLaugh);
				}
			}
		}

		if (Args.Caster_->IsEnemy(GEntityList->Player()) && Args.Caster_->IsHero() && Args.Target_ == GEntityList->Player() && Args.Caster_->IsMelee() && Args.AutoAttack_)
		{
			if (!CheckTarget(Args.Caster_)) return;

			if (QAntiMelee->GetInteger() == 1 && Q->IsReady())
			{
				auto pPos = GEntityList->Player()->GetPosition();
				Q->CastOnPosition(pPos.Extend(Args.Caster_->GetPosition(), -Q->Range()));
			}
		}
		
		if (GEntityList->Player()->IsDead() || ComboR->GetInteger() == 0 || !R->IsReady()) return;
				
		auto enemy = Args.Caster_;
		auto ally = Args.Target_;		

		if (enemy == nullptr || !enemy->IsValidTarget() || !enemy->IsVisible()) return;
		if (ally == nullptr || !ally->IsValidTarget() || !ally->IsVisible()) return;
		
		if (!enemy->IsEnemy(GEntityList->Player())
			|| ally->IsEnemy(GEntityList->Player()) 
			|| GetDistance(GEntityList->Player(), ally) > R->Range())
		{
			return;
		}

		if (ally->IsHero() && ((ComboR->GetInteger() == 1 || ComboR->GetInteger() == 3) && enemy->IsHero() || (ComboR->GetInteger() == 2 || ComboR->GetInteger() == 3) && (enemy->IsTurret() || enemy->IsJungleCreep())))
		{
			auto predictedHealth = ally->GetHealth() - Args.Damage_;
			auto hpp = predictedHealth / ally->GetMaxHealth() * 100;

			if (ally != GEntityList->Player())
			{
				if (predictedHealth < 0 || hpp < UltPercent->GetInteger())
				{
					if (CountAlly(ally->GetPosition(), 500) + 1 >= RAllys->GetInteger() &&
						CountEnemy(ally->GetPosition(), 500) <= REnemies->GetInteger())
					{						
						if (ChampionUse[ally->GetNetworkId()]->Enabled())
						{
							R->CastOnPlayer();
						}
					}
				}
			}
			else
			{
				if (RHP->Enabled() && (predictedHealth < 0 || hpp < HealthR->GetInteger()))
				{
					if (CountEnemy(ally->GetPosition(), 500) <= REnemies->GetInteger())
					{
						R->CastOnPlayer();						
					}
				}
			}
		}		
	}
};
