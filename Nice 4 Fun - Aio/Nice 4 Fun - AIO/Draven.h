#pragma once
#include "PluginSDK.h"
#include "BaseMenu.h"
#include "Common.h"
#include <list>

class Draven
{
public:

	static void InitializeSpells()
	{

		Q = GPluginSDK->CreateSpell2(kSlotQ, kTargetCast, false, false, kCollidesWithNothing);
		W = GPluginSDK->CreateSpell2(kSlotW, kTargetCast, false, false, kCollidesWithNothing);
		E = GPluginSDK->CreateSpell2(kSlotE, kLineCast, true, true, kCollidesWithYasuoWall);
		R = GPluginSDK->CreateSpell2(kSlotR, kLineCast, true, true, kCollidesWithNothing);
		E->SetSkillshot(0.25f, 100.f, 1400.f, 1050.f);
		R->SetSkillshot(0.4f, 160.f, 2000.f, 3000.f);
	}

	static void InitializeMenu()
	{
		MainMenu = GPluginSDK->AddMenu("Nice4Fun Federal - Draven");

		AxeSettings = MainMenu->AddMenu("Axe Settings");
		{
			gotoAxeC = AxeSettings->CheckBox("Catch axe", true);
			axeMode = AxeSettings->AddSelection("Catch axe Mode: ", 0, std::vector<std::string>({ "Mode 1", "Mode 2" }));
			axeSoften = AxeSettings->CheckBox("Beta Soften Orbwalk", true);
			//UseWToAxe = AxeSettings->CheckBox("Use W if Axe too far", true);
			gotoAxeMaxDist = AxeSettings->AddFloat("Max dist to catch axe", 200, 1500, 800);
			axeKill = AxeSettings->CheckBox("No Catch Axe if can kill 2 AA", true);
			axeTower = AxeSettings->CheckBox("No Catch Axe Under Turret Combo", true);
			axeTower2 = AxeSettings->CheckBox("No Catch Axe Under Turret Farm", true);
			axeEnemy = AxeSettings->CheckBox("No Catch Axe in Enemy grup", true);
		}

		QSettings = MainMenu->AddMenu("Q Settings");
		{
			ComboQ = QSettings->CheckBox("Auto Q Combo", true);
			HarassQ = QSettings->CheckBox("Auto Q Harass", true);
			LaneClearQ = QSettings->CheckBox("Auto Q in laneclear", true);
			LastHitQ = QSettings->CheckBox("Auto Q in LastHit", true);
		}

		WSettings = MainMenu->AddMenu("W Settings");
		{
			ComboW = WSettings->CheckBox("Auto W", true);
			slowW = WSettings->CheckBox("Auto W Slow", true);
		}

		ESettings = MainMenu->AddMenu("E Settings");
		{
			ComboE = ESettings->CheckBox("Auto E", true);
			AutoE = ESettings->CheckBox("Harras E if can hit 2 targets", true);
			KillstealE = ESettings->CheckBox("Auto E Killsteal", true);
			EGapCloser = ESettings->CheckBox("Auto E GapCloser", true);
			EInterrupter = ESettings->CheckBox("Auto E Interrupt", true);
			CCedE = ESettings->CheckBox("Auto Use E on CC", true);
		}

		RSettings = MainMenu->AddMenu("R Settings");
		{
			AutoUlt = RSettings->CheckBox("Auto R", true);
			ComboRKill = RSettings->CheckBox("Auto R KS", false);
			ComboR = RSettings->CheckBox("Auto R in Combo", true);
			CCedR = RSettings->CheckBox("Auto R in CC", true);
			Raoe = RSettings->CheckBox("R Aoe Combo", true);
			HealthR = RSettings->AddInteger("Only if Target HP% <", 1, 100, 90);
			SemiManualKey = RSettings->AddKey("Semi-manual cast R key", 71);
		}

		DrawingSettings = MainMenu->AddMenu("Drawing Settings");
		{
			DrawReady = DrawingSettings->CheckBox("Draw Only Ready Spells", true);
			DrawE = DrawingSettings->CheckBox("Draw E", true);
			DrawAxe = DrawingSettings->CheckBox("Draw Axe position", true);
			DrawAxerange = DrawingSettings->CheckBox("Draw Axe Catch Range", true);
			Drawkill = DrawingSettings->CheckBox("Draw R helper", true);
		}		
	}

	static void Drawing()
	{
		if (DrawReady->Enabled())
		{
			if (E->IsReady() && DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), E->Range()); }
		}
		else
		{
			if (DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), E->Range()); }
		}

		if (DrawAxe->Enabled()) {

			for (auto obj : DravenAxes.ToVector())
			{
				if (obj->GetPosition() != Vec3(0,0,0))
				{
					GRender->DrawOutlinedCircle(obj->GetPosition(), Vec4(0, 255, 0, 255), 150);

					if (GetDistanceVectors(GGame->CursorPosition(), GMissileData->GetEndPosition(obj)) > gotoAxeMaxDist->GetInteger() || IsUnderTurretPos(GMissileData->GetEndPosition(obj)))
					{
						GRender->DrawOutlinedCircle(GMissileData->GetEndPosition(obj), Vec4(255, 0, 0, 255), 150);
					}
					else if (GetDistanceVectors(GEntityList->Player()->GetPosition(), GMissileData->GetEndPosition(obj)) > 150)
					{
						GRender->DrawOutlinedCircle(GMissileData->GetEndPosition(obj), Vec4(255, 69, 0, 255), 150);
					}
					else if (GetDistanceVectors(GEntityList->Player()->GetPosition(), GMissileData->GetEndPosition(obj)) <= 150)
					{
						GRender->DrawOutlinedCircle(GMissileData->GetEndPosition(obj), Vec4(0, 255, 0, 255), 150);						
					}
				}
			}

			if (DrawAxerange->Enabled())
			{
				GRender->DrawOutlinedCircle(GGame->CursorPosition(), Vec4(0, 191, 255, 255), gotoAxeMaxDist->GetFloat());
			}
		}
	}	

	static void KeyPressUltimate()
	{
		if (IsKeyDown(SemiManualKey))
		{

			if (!R->IsReady())
			{
				return;
			}
			auto TargetR = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, R->Range());

			if (TargetR == nullptr || TargetR->IsDead() || TargetR->IsInvulnerable() || !TargetR->IsValidTarget(GEntityList->Player(), R->Range()))
			{
				return;
			}

			auto rdamage = GHealthPrediction->GetKSDamage(TargetR, kSlotR, R->GetDelay(), false);
			auto qdamage = GHealthPrediction->GetKSDamage(TargetR, kSlotQ, Q->GetDelay(), false);
			auto edamage = GHealthPrediction->GetKSDamage(TargetR, kSlotE, E->GetDelay(), false);
			auto autodamage = GDamage->GetAutoAttackDamage(GEntityList->Player(), TargetR, false);

			if (R->CastOnTargetAoE(TargetR, 2, kHitChanceVeryHigh))
			{
				return;
			}

			if ((autodamage * 4) + rdamage + qdamage > TargetR->GetHealth())
			{
				R->CastOnTarget(TargetR, kHitChanceHigh);
			}
		}
	}

	static void LogicE()
	{
		if (!E->IsReady() || !ComboE->Enabled()) { return; }

		auto hero = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, R->Range());

		if (hero != nullptr && hero->IsValidTarget(GEntityList->Player(), E->Range()) && !hero->IsInvulnerable() && !hero->IsDead())
		{
			if (GOrbwalking->GetOrbwalkingMode() == kModeCombo && hero->IsValidTarget() && GEntityList->Player()->GetMana() > R->ManaCost() + E->ManaCost())
			{
				if (GetDistance(GEntityList->Player(), hero) > GEntityList->Player()->GetRealAutoAttackRange(hero))
				{
					E->CastOnTarget(hero, kHitChanceHigh);
				}

				if (GEntityList->Player()->HealthPercent() < 50)
				{
					E->CastOnTarget(hero, kHitChanceHigh);
				}
			}

			if (GOrbwalking->GetOrbwalkingMode() == kModeCombo &&  GEntityList->Player()->GetMana() > R->ManaCost() + E->ManaCost() + (Q->ManaCost() * 2))
			{
				E->CastOnTargetAoE(hero, 2, kHitChanceHigh);
			}

			if (GOrbwalking->GetOrbwalkingMode() == kModeMixed && AutoE->Enabled() && GEntityList->Player()->GetMana() > R->ManaCost() + E->ManaCost() + Q->ManaCost() + W->ManaCost())
			{
				E->CastOnTargetAoE(hero, 2, kHitChanceHigh);
			}

			if (GetDistance(GEntityList->Player(), hero) < 300 && hero->IsMelee())
			{
				E->CastOnTarget(hero, kHitChanceMedium);
			}
		}

		for (auto target : GEntityList->GetAllHeros(false, true))
		{
			if (hero != nullptr && hero->IsValidTarget(GEntityList->Player(), E->Range()) && !hero->IsInvulnerable() && !hero->IsDead())
			{
				if (KillstealE->Enabled() && E->IsReady())
				{
					auto damage = GHealthPrediction->GetKSDamage(hero, kSlotE, E->GetDelay(), false);

					if (damage > hero->GetHealth() && GetDistance(GEntityList->Player(), hero) > GEntityList->Player()->GetRealAutoAttackRange(hero))
					{
						E->CastOnTarget(hero, kHitChanceHigh);
					}
				}
				// Auto E in Target CC
				if (CCedE->Enabled() && E->IsReady())
				{
					if (hero->HasBuffOfType(BUFF_Stun) || hero->HasBuffOfType(BUFF_Charm) || hero->HasBuffOfType(BUFF_Snare) || hero->HasBuffOfType(BUFF_Suppression) || hero->HasBuffOfType(BUFF_Fear))
					{
						E->CastOnTarget(hero, kHitChanceHigh);
					}
				}
			}
		}
	}	

	static void LogicW()
	{
		if (!W->IsReady()) { return; }

		if (ComboW->Enabled() && GEntityList->Player()->GetMana() > R->ManaCost() + E->ManaCost() + W->ManaCost() + Q->ManaCost() && FoundEnemies(GEntityList->Player(), 1200) > 0 && !GEntityList->Player()->HasBuff("dravenfurybuff"))
		{
			W->CastOnPlayer();
		}
		else if (slowW->Enabled() && GEntityList->Player()->HasBuffOfType(BUFF_Slow) && GEntityList->Player()->GetMana() > R->ManaCost() + E->ManaCost() + W->ManaCost())
		{
			W->CastOnPlayer();
		}
	}

	static void LogicR()
	{
		auto TargetR = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, R->Range());

		if (TargetR == nullptr || !TargetR->IsValidTarget() || TargetR->IsDead() || !R->IsReady() || GEntityList->Player()->IsWindingUp() || GetDistance(GEntityList->Player(), TargetR) > R->Range()) { return; }

		auto rdamage = GHealthPrediction->GetKSDamage(TargetR, kSlotR, R->GetDelay(), false);
		auto qdamage = GHealthPrediction->GetKSDamage(TargetR, kSlotQ, Q->GetDelay(), false);
		auto edamage = GHealthPrediction->GetKSDamage(TargetR, kSlotE, E->GetDelay(), false);
		auto autodamage = GDamage->GetAutoAttackDamage(GEntityList->Player(), TargetR, false);

		if (AutoUlt->Enabled() && TargetR->HealthPercent() < HealthR->GetInteger())
		{
			if (ComboRKill->Enabled() && rdamage > TargetR->GetHealth() && GetDistance(GEntityList->Player(), TargetR) > GEntityList->Player()->GetRealAutoAttackRange(TargetR))
			{
				R->CastOnTarget(TargetR, kHitChanceHigh);
			}

			else if (GOrbwalking->GetOrbwalkingMode() == kModeCombo && ComboR->Enabled() && GetDistance(GEntityList->Player(), TargetR) < GEntityList->Player()->GetRealAutoAttackRange(TargetR) &&
				(autodamage * 2) + (rdamage * 2) > TargetR->GetHealth())
			{
				R->CastOnTarget(TargetR, kHitChanceHigh);
			}

			else if (CCedR->Enabled() && rdamage * 2 > TargetR->GetHealth() && GetDistance(GEntityList->Player(), TargetR) <= E->Range())
			{
				if (TargetR->HasBuffOfType(BUFF_Stun) || TargetR->HasBuffOfType(BUFF_Charm) || TargetR->HasBuffOfType(BUFF_Snare) || TargetR->HasBuffOfType(BUFF_Suppression) || TargetR->HasBuffOfType(BUFF_Fear))
				{
					R->CastOnTarget(TargetR, kHitChanceHigh);
				}
			}

			else if (GOrbwalking->GetOrbwalkingMode() == kModeCombo && Raoe->Enabled())
			{
				R->CastOnTargetAoE(TargetR, 3, kHitChanceVeryHigh);
			}

			else if (GetDistance(GEntityList->Player(), TargetR) <= E->Range() && (rdamage * 2) + qdamage + edamage > TargetR->GetHealth() && Raoe->Enabled())
			{
				R->CastOnTargetAoE(TargetR, 2, kHitChanceHigh);
			}
		}
	}

	static void CatchAxe(IUnit* target)
	{
		auto maxDist = gotoAxeMaxDist->GetFloat();
		auto modokey = GOrbwalking->GetOrbwalkingMode();

		if (axeSoften->Enabled() && GetDistanceVectors(GEntityList->Player()->GetPosition(), target->GetPosition()) < 250)
		{
			GOrbwalking->SetOverridePosition(GGame->CursorPosition());

			return;
		}		

		if (axeTower->Enabled() && modokey == kModeCombo && IsUnderTurretPos(target->GetPosition()))
		{
			GOrbwalking->SetOverridePosition(Vec3(0, 0, 0));
			return;
		}

		if (axeTower2->Enabled() && modokey == kModeLaneClear && CountMinionsAlly(returnNearPosTower(target), 900) == 0 && IsUnderTurretPos(target->GetPosition()))
		{
			GOrbwalking->SetOverridePosition(Vec3(0, 0, 0));
			return;
		}

		if (axeEnemy->Enabled() && CountEnemy(target->GetPosition(), 550) > 2)
		{
			GOrbwalking->SetOverridePosition(Vec3(0, 0, 0));
			return;
		}

		if (GetDistanceVectors(GGame->CursorPosition(), target->GetPosition()) <= maxDist && QMissile != nullptr)
		{
			if (axeMode->GetInteger() == 0)
			{
				GOrbwalking->SetOverridePosition(Extend(GMissileData->GetEndPosition(target), GGame->CursorPosition(), 100));
			}
			else
			{
				GOrbwalking->SetOverridePosition(GMissileData->GetEndPosition(target));
			}

			if (GetDistanceVectors(GMissileData->GetEndPosition(target), GEntityList->Player()->GetPosition()) > 100)
			{
				auto delay = 1000 * (GetDistanceVectors(GMissileData->GetEndPosition(target), GEntityList->Player()->GetPosition()) / GEntityList->Player()->MovementSpeed());
				/*auto expireTime = GMissileData-> - Environment.TickCount;

				if (eta >= expireTime && this.Menu.Item("UseWForQ").IsActive())
				{
					this.W.Cast();
				}*/
			}
		}
		else
		{
			GOrbwalking->SetOverridePosition(Vec3(0, 0, 0));
		}
	}

	static void AxeLogicFarm()
	{
		if (!gotoAxeC->Enabled())
		{
			return;
		}

		if (DravenAxes.Count() == 0)
		{
			GOrbwalking->SetOverridePosition(Vec3(0, 0, 0));
			return;
		}

		if (DravenAxes.Count() == 1)
		{
			CatchAxe(DravenAxes.FirstOrDefault());
			return;
		}
		else
		{
			if (axeMode->GetInteger() == 0)
			{
				auto bestAxe = DravenAxes.FirstOrDefault();

				for (auto obj : DravenAxes.ToVector())
				{
					if (GetDistanceVectors(GGame->CursorPosition(), bestAxe->GetPosition()) > GetDistanceVectors(GGame->CursorPosition(), obj->GetPosition()))
					{
						bestAxe = obj;
					}
				}

				GOrbwalking->DisableNextAttack();
				CatchAxe(bestAxe);
			}
			else
			{
				auto bestAxe = DravenAxes.FirstOrDefault();

				GOrbwalking->DisableNextAttack();
				CatchAxe(bestAxe);
			}
		}
	}

	static void AxeLogicCombo()
	{
		if (!gotoAxeC->Enabled())
		{
			return;
		}

		if (DravenAxes.Count() == 0)
		{
			GOrbwalking->SetOverridePosition(Vec3(0, 0, 0));
			return;
		}

		for (auto hero : GEntityList->GetAllHeros(false, true))
		{
			if (axeKill->Enabled() && GetDistance(GEntityList->Player(), hero) > 400 && GDamage->GetAutoAttackDamage(GEntityList->Player(), hero, false) * 2 > hero->GetHealth() && !hero->IsDead())
			{
				GOrbwalking->SetOverridePosition(Vec3(0, 0, 0));
				return;
			}
		}

		if (DravenAxes.Count() == 1)
		{
			CatchAxe(DravenAxes.FirstOrDefault());
			return;
		}
		else
		{
			if (axeMode->GetInteger() == 0)
			{
				auto bestAxe = DravenAxes.FirstOrDefault();

				for (auto obj : DravenAxes.ToVector())
				{
					if (GetDistanceVectors(GGame->CursorPosition(), bestAxe->GetPosition()) > GetDistanceVectors(GGame->CursorPosition(), obj->GetPosition()))
					{
						bestAxe = obj;
					}
				}

				GOrbwalking->DisableNextAttack();
				CatchAxe(bestAxe);
			}
			else
			{
				auto bestAxe = DravenAxes.FirstOrDefault();

				GOrbwalking->DisableNextAttack();
				CatchAxe(bestAxe);
			}
		}
	}

	static void OnGapcloser(GapCloserSpell const& args)
	{
		if (EGapCloser->Enabled() && E->IsReady() && !args.IsTargeted && GetDistanceVectors(GEntityList->Player()->GetPosition(), args.EndPosition) < E->Range())
		{
			E->CastOnTarget(args.Source, kHitChanceMedium);
		}
	}

	static void OnAfterAttack(IUnit* source, IUnit* target)
	{
		if (source != GEntityList->Player() || target == nullptr)
			return;

		auto buffCount = GEntityList->Player()->GetBuffCount("dravenspinningattack");

		switch (GOrbwalking->GetOrbwalkingMode())
		{
		case kModeCombo:
			if (ComboQ->Enabled() && Q->IsReady())
			{
				if (buffCount + axeListTeste.size() == 0)
				{
					Q->CastOnPlayer();
				}
			}
			break;
		case kModeMixed:
			if (HarassQ->Enabled() && Q->IsReady())
			{
				if (buffCount + axeListTeste.size() == 0)
				{
					Q->CastOnPlayer();
				}
			}
			break;			
		case kModeLastHit:
				if (Q->IsReady() && LastHitQ->Enabled())
				{
					if (buffCount + axeListTeste.size() == 0 && GEntityList->Player()->GetMana() > R->ManaCost() + E->ManaCost() + W->ManaCost())
					{
						Q->CastOnPlayer();
					}
					else if (60 < GEntityList->Player()->ManaPercent() && buffCount == 0)
					{
						Q->CastOnPlayer();
					}
				}
			break;
		case kModeLaneClear:
			if (Q->IsReady() && LaneClearQ->Enabled())
			{

				if (buffCount + axeListTeste.size() == 0 && GEntityList->Player()->GetMana() > R->ManaCost() + E->ManaCost() + W->ManaCost())
				{
					Q->CastOnPlayer();
				}
				else if (60 < GEntityList->Player()->ManaPercent() && buffCount == 0)
				{
					Q->CastOnPlayer();
				}
			}
			break;
		default:;
		}
	}

	static void OnCreateObject(IUnit* Source)
	{
		if (Source->IsMissile() && GMissileData->GetCaster(Source)->GetNetworkId() == GEntityList->Player()->GetNetworkId())
		{
			if (strstr(GMissileData->GetName(Source), "DravenSpinningReturnCatch"))
			{
				DravenAxes.Add(Source);
				QMissile = Source;
			}

			if (!strstr(GMissileData->GetName(Source), "DravenSpinningReturnCatch") && strstr(GMissileData->GetName(Source), "DravenSpinningReturn"))
			{
				//GUtility->LogConsole("Name: %s - Time: %f", GMissileData->GetName(Source), GMissileData->GetSpellCastTime(Source));
			}
		}
	}

	static void OnDeleteObject(IUnit* Source)
	{
		if (Source->IsMissile() && GMissileData->GetCaster(Source)->GetNetworkId() == GEntityList->Player()->GetNetworkId())
		{
			if (strstr(GMissileData->GetName(Source), "DravenSpinningReturnCatch"))
			{
				DravenAxes.RemoveAll([&](IUnit* i) {return i->GetNetworkId() == Source->GetNetworkId(); });
				QMissile = nullptr;
			}
		}
		if (!strstr(GMissileData->GetName(Source), "DravenSpinningReturnCatch") && strstr(GMissileData->GetName(Source), "DravenSpinningReturn"))
		{
			//GUtility->LogConsole("Nome: %s - Time: %f", GMissileData->GetName(Source), GMissileData->GetSpellCastTime(Source));
		}
	}

};
