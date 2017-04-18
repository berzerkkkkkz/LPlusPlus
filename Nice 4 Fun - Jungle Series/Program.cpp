#include "PluginSDK.h"
#include "Champions.h"
#include "AutoSmite.h"
#include <unordered_map>
#include "Toxic.h"
#include "Trinket.h"
#pragma region Events
PLUGIN_EVENT(void) OnOrbwalkBeforeAttack(IUnit* Target)
{

}
PLUGIN_EVENT(void) OnOrbwalkAttack(IUnit* Source, IUnit* Target)
{

}
#pragma endregion

PluginSetup("Nice4Fun - Jungle Series");

class IChampion
{
public:		
	virtual void OnGameUpdate() = 0;
	virtual void OnAfterAttack(IUnit* Source, IUnit* Target) = 0;
	virtual void OnBeforeAttack(IUnit* Target) = 0;
	virtual void OnGapCloser(GapCloserSpell const& Args) = 0;
	virtual void OnInterruptible(InterruptibleSpell const& Args) = 0;
	virtual void OnDash(UnitDash* Args) = 0;
	virtual void OnCreateObject(IUnit* Object) = 0;
	virtual void OnDeleteObject(IUnit* Object) = 0;
	virtual void OnLevelUp(IUnit* Source, int NewLevel) = 0;
	virtual void OnProcessSpell(CastedSpell const& Args) = 0;
	virtual void OnExitVisible(IUnit* Source) = 0;
	virtual void OnLoad() {};
	virtual void OnBuffAdd(IUnit* Source, void* BuffData) = 0;
	virtual void OnBuffRemove(IUnit* Source, void* BuffData) = 0;
	//virtual void OnUnLoad() = 0;
	virtual void OnRender() = 0;
	virtual void OnGameEnd() = 0;
};

class cOlaf : public IChampion
{
public:

	virtual void OnLoad() override
	{		
		Olaf().InitializeMenu();
		Olaf().LoadSpells();
		AutoSmite().SpellsSmite();
		AutoSmite().MenuSmite();		
		Toxic().MenuToxic();
	}

	virtual void OnRender() override
	{
		Olaf().Drawing();
		AutoSmite().DrawingsSmite();
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() || GEntityList->Player()->IsRecalling() || GGame->IsChatOpen() || GGame->IsScoreboardOpen() || GGame->IsShopOpen())
		{
			return;
		}	

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			Olaf().Combo();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{
			Olaf().Harass();
			Olaf().LastHit();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{			
			Olaf().LaneClear();
			Olaf().JungleClear();
		}

		Olaf().Automatic();		
		AutoSmite().AutomaticSmite();
		AutoSmite().KeyPressSmite();
		Toxic().SpamEmote();
	}
	void OnGapCloser(GapCloserSpell const& Args) override
	{
		Olaf().OnGapcloser(Args);
	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
	{
		Olaf().OnAfterAttack(Source, Target);
	}
	void OnBeforeAttack(IUnit* Target) override
	{

	}
	void OnLevelUp(IUnit* Source, int NewLevel) override
	{

	}

	void OnCreateObject(IUnit* Source) override
	{
		Olaf().OnCreateObject(Source);
	}

	void OnDeleteObject(IUnit* Source) override
	{
		Olaf().OnDeleteObject(Source);
	}	

	void OnInterruptible(InterruptibleSpell const& Args) override
	{
		
	}

	void OnDash(UnitDash* Args) override
	{
	}

	void OnProcessSpell(CastedSpell const& Args) override
	{

	}

	void OnExitVisible(IUnit* Source) override
	{

	}	

	void OnBuffAdd(IUnit* Source, void* BuffData) override
	{
	}

	void OnBuffRemove(IUnit* Source, void* BuffData) override
	{
	}

	void OnGameEnd() override
	{
		Toxic().OnGameEnd();
	}
};

class cNocturne : public IChampion
{
public:

	virtual void OnLoad() override
	{		
		Nocturne().InitializeMenu();
		Nocturne().LoadSpells();
		AutoSmite().SpellsSmite();
		AutoSmite().MenuSmite();		
		Toxic().MenuToxic();
	}

	virtual void OnRender() override
	{
		Nocturne().Drawing();
		AutoSmite().DrawingsSmite();
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() || GEntityList->Player()->IsRecalling() || GGame->IsChatOpen() || GGame->IsScoreboardOpen() || GGame->IsShopOpen())
		{
			return;
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			Nocturne().Combo();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{
			Nocturne().Harass();			
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			Nocturne().LaneClear();
			Nocturne().JungleClear();
		}

		Nocturne().RangeUltimate();
		Nocturne().Automatic();		
		Nocturne().KeyPressUltimate();
		AutoSmite().AutomaticSmite();
		AutoSmite().KeyPressSmite();
		Toxic().SpamEmote();
	}
	void OnGapCloser(GapCloserSpell const& Args) override
	{
	
	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
	{
		
	}
	void OnBeforeAttack( IUnit* Target) override
	{

	}
	void OnLevelUp(IUnit* Source, int NewLevel) override
	{

	}

	void OnCreateObject(IUnit* Source) override
	{
		
	}

	void OnDeleteObject(IUnit* Source) override
	{

	}
	void OnInterruptible(InterruptibleSpell const& Args) override
	{
		
	}	

	void OnDash(UnitDash* Args) override
	{
	}

	void OnProcessSpell(CastedSpell const& Args) override
	{

	}

	void OnExitVisible(IUnit* Source) override
	{

	}	

	void OnBuffAdd(IUnit* Source, void* BuffData) override
	{
	}

	void OnBuffRemove(IUnit* Source, void* BuffData) override
	{
	}

	void OnGameEnd() override
	{
		Toxic().OnGameEnd();
	}
};

class cHecarim : public IChampion
{
public:

	virtual void OnLoad() override
	{		
		Hecarim().InitializeMenu();
		Hecarim().LoadSpells();
		AutoSmite().SpellsSmite();
		AutoSmite().MenuSmite();		
		Toxic().MenuToxic();
	}

	virtual void OnRender() override
	{
		Hecarim().Drawing();
		AutoSmite().DrawingsSmite();
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() || GEntityList->Player()->IsRecalling() || GGame->IsChatOpen() || GGame->IsScoreboardOpen() || GGame->IsShopOpen())
		{
			return;
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			Hecarim().Combo();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{
			Hecarim().Harass();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			Hecarim().LaneClear();
			Hecarim().JungleClear();
		}
		
		Hecarim().Automatic();				
		AutoSmite().AutomaticSmite();
		AutoSmite().KeyPressSmite();
		Toxic().SpamEmote();
	}
	void OnGapCloser(GapCloserSpell const& Args) override
	{
		Hecarim().OnGapcloser(Args);
	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
	{

	}
	void OnBeforeAttack(IUnit* Target) override
	{

	}
	void OnLevelUp(IUnit* Source, int NewLevel) override
	{

	}

	void OnCreateObject(IUnit* Source) override
	{

	}

	void OnDeleteObject(IUnit* Source) override
	{

	}
	void OnInterruptible(InterruptibleSpell const& Args) override
	{
		Hecarim().OnInterruptible(Args);
	}

	void OnDash(UnitDash* Args) override
	{
	}

	void OnProcessSpell(CastedSpell const& Args) override
	{

	}

	void OnExitVisible(IUnit* Source) override
	{

	}	

	void OnBuffAdd(IUnit* Source, void* BuffData) override
	{
	}

	void OnBuffRemove(IUnit* Source, void* BuffData) override
	{
	}

	void OnGameEnd() override
	{
		Toxic().OnGameEnd();
	}
};

class cElise : public IChampion
{
public:

	virtual void OnLoad() override
	{
		//Message().JungleLaneSeries();
		//Message().ChampionLoadMessage();
		Elise().InitializeMenu();
		Elise().LoadSpells();
		Elise().LoadTextures();

		AutoSmite().SpellsSmite();
		AutoSmite().MenuSmite();
		//Skins().Menu();
		Toxic().MenuToxic();
	}

	virtual void OnRender() override
	{
		Elise().Drawings();
		AutoSmite().DrawingsSmite();
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() || GEntityList->Player()->IsRecalling() || GGame->IsChatOpen() || GGame->IsScoreboardOpen() || GGame->IsShopOpen())
		{
			return;
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			Elise().Combo();			
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{
			//Elise().Harass();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			//Elise().LaneClear();
			Elise().JungleClear();
		}
		
		//Skins().fedSkinChanger();
		Elise().EliseRappel();
		Elise().Cooldowns();
		AutoSmite().AutomaticSmite();
		AutoSmite().KeyPressSmite();
		Toxic().SpamEmote();
	}
	void OnGapCloser(GapCloserSpell const& Args) override
	{
		Elise().OnGapcloser(Args);
	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
	{
		Elise().OnAfterAttack(Source, Target);
	}
	void OnBeforeAttack(IUnit* Target) override
	{

	}
	void OnLevelUp(IUnit* Source, int NewLevel) override
	{

	}

	void OnCreateObject(IUnit* Source) override
	{

	}

	void OnDeleteObject(IUnit* Source) override
	{

	}
	void OnInterruptible(InterruptibleSpell const& Args) override
	{
		Elise().OnInterruptible(Args);
	}

	void OnDash(UnitDash* Args) override
	{
	}

	void OnProcessSpell(CastedSpell const& Args) override
	{
		Elise().OnProcessSpell(Args);
	}

	void OnExitVisible(IUnit* Source) override
	{

	}	

	void OnBuffAdd(IUnit* Source, void* BuffData) override
	{
	}

	void OnBuffRemove(IUnit* Source, void* BuffData) override
	{
	}

	void OnGameEnd() override
	{
		Toxic().OnGameEnd();
	}
};

class cRengar : public IChampion
{
public:

	virtual void OnLoad() override
	{
		//Message().JungleLaneSeries();
		//Message().ChampionLoadMessage();
		Rengar().Menu();
		Rengar().LoadSpells();

		AutoSmite().SpellsSmite();
		AutoSmite().MenuSmite();
		//Skins().Menu();
		Toxic().MenuToxic();
	}

	virtual void OnRender() override
	{
		Rengar().Drawing();
		AutoSmite().DrawingsSmite();
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() || GEntityList->Player()->IsRecalling() || GGame->IsChatOpen() || GGame->IsScoreboardOpen() || GGame->IsShopOpen())
		{
			return;
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			Rengar().Combo();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{

		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			Rengar().Farm();
		}


		AutoSmite().AutomaticSmite();
		AutoSmite().KeyPressSmite();
		Toxic().SpamEmote();
		//Skins().fedSkinChanger();
	}
	void OnGapCloser(GapCloserSpell const& Args) override
	{

	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
	{
		if (ComboAA->Enabled() && !(ComboA->Enabled()))
		{
			if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
			{
				Rengar().Combo();
			}
		}
	}
	void OnBeforeAttack( IUnit* Target) override
	{

	}
	void OnLevelUp(IUnit* Source, int NewLevel) override
	{

	}

	void OnCreateObject(IUnit* Source) override
	{

	}

	void OnDeleteObject(IUnit* Source) override
	{

	}
	void OnInterruptible(InterruptibleSpell const& Args) override
	{

	}

	void OnDash(UnitDash* Args) override
	{
	}

	void OnProcessSpell(CastedSpell const& Args) override
	{

	}

	void OnExitVisible(IUnit* Source) override
	{

	}	

	void OnBuffAdd(IUnit* Source, void* BuffData) override
	{
	}

	void OnBuffRemove(IUnit* Source, void* BuffData) override
	{
	}

	void OnGameEnd() override
	{
		Toxic().OnGameEnd();
	}
};

class cDiana : public IChampion
{
public:

	virtual void OnLoad() override
	{
		//Message().JungleLaneSeries();
		//Message().ChampionLoadMessage();
		Diana().Menu();
		Diana().LoadSpells();
		AutoSmite().SpellsSmite();
		AutoSmite().MenuSmite();
		//Skins().Menu();
		Toxic().MenuToxic();
	}

	virtual void OnRender() override
	{
		Diana().Drawing();
		AutoSmite().DrawingsSmite();
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() || GEntityList->Player()->IsRecalling() || GGame->IsChatOpen() || GGame->IsScoreboardOpen() || GGame->IsShopOpen())
		{
			return;
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			Diana().Combo();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{
			Diana().Harass();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			Diana().Farm();
		}
		if (R->IsReady())
		{
			Diana().ComboRKill1();
		}
		if (QEnemies1->Enabled())
		{
			Diana().AutoQ();
		}
		Diana().StealR();

		AutoSmite().AutomaticSmite();
		AutoSmite().KeyPressSmite();
		Toxic().SpamEmote();
		//Skins().fedSkinChanger();
	}
	void OnGapCloser(GapCloserSpell const& Args) override
	{
		//Diana().GapCloser(Args);
	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
	{

	}
	void OnBeforeAttack(IUnit* Target) override
	{

	}
	void OnLevelUp(IUnit* Source, int NewLevel) override
	{

	}

	void OnCreateObject(IUnit* Source) override
	{

	}

	void OnDeleteObject(IUnit* Source) override
	{

	}
	void OnInterruptible(InterruptibleSpell const& Args) override
	{

	}

	void OnDash(UnitDash* Args) override
	{
	}

	void OnProcessSpell(CastedSpell const& Args) override
	{

	}

	void OnExitVisible(IUnit* Source) override
	{

	}	

	void OnBuffAdd(IUnit* Source, void* BuffData) override
	{
	}

	void OnBuffRemove(IUnit* Source, void* BuffData) override
	{
	}

	void OnGameEnd() override
	{
		Toxic().OnGameEnd();
	}
};

class cWarwick : public IChampion
{
public:

	virtual void OnLoad() override
	{
		//Message().JungleLaneSeries();
		//Message().ChampionLoadMessage();
		Warwick().Menu();
		Warwick().LoadSpells();

		AutoSmite().SpellsSmite();
		AutoSmite().MenuSmite();
		//Skins().Menu();
		Toxic().MenuToxic();
	}

	virtual void OnRender() override
	{
		Warwick().Drawing();
		AutoSmite().DrawingsSmite();
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() || GEntityList->Player()->IsRecalling() || GGame->IsChatOpen() || GGame->IsScoreboardOpen() || GGame->IsShopOpen())
		{
			return;
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			Warwick().Combo();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{
			Warwick().Harass();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			Warwick().JungleClear();
		}


		AutoSmite().AutomaticSmite();
		AutoSmite().KeyPressSmite();
		Toxic().SpamEmote();
		//Skins().fedSkinChanger();
	}
	void OnGapCloser(GapCloserSpell const& Args) override
	{

	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
	{

	}
	void OnBeforeAttack(IUnit* Target) override
	{

	}
	void OnLevelUp(IUnit* Source, int NewLevel) override
	{

	}

	void OnCreateObject(IUnit* Source) override
	{

	}

	void OnDeleteObject(IUnit* Source) override
	{

	}
	void OnInterruptible(InterruptibleSpell const& Args) override
	{

	}

	void OnDash(UnitDash* Args) override
	{
	}

	void OnProcessSpell(CastedSpell const& Args) override
	{

	}

	void OnExitVisible(IUnit* Source) override
	{

	}	

	void OnBuffAdd(IUnit* Source, void* BuffData) override
	{
	}

	void OnBuffRemove(IUnit* Source, void* BuffData) override
	{
	}

	void OnGameEnd() override
	{
		Toxic().OnGameEnd();
	}
};

class cLeeSin : public IChampion
{
public:

	virtual void OnLoad() override
	{
		//Message().JungleLaneSeries();
		//Message().ChampionLoadMessage();
		LeeSin().InitializeMenu();
		LeeSin().LoadSpells();
		AutoSmite().SpellsSmite();
		AutoSmite().MenuSmite();
		//Skins().Menu();
		Toxic().MenuToxic();

		//GUtility->CreateDebugConsole();
	}

	virtual void OnRender() override
	{
		LeeSin().Drawing();
		AutoSmite().DrawingsSmite();
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() || GEntityList->Player()->IsRecalling() || GGame->IsChatOpen() || GGame->IsScoreboardOpen() || GGame->IsShopOpen())
		{
			return;
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			LeeSin().ComboSelected();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{
			LeeSin().Harass();
			LeeSin().LastHit();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			LeeSin().JungleClear();
			LeeSin().LaneClear();
		}

		LeeSin().StarCombo();
		LeeSin().EscapeJungle();
		LeeSin().InsecRework();
		LeeSin().WardJumpMouse();
		LeeSin().Automatic();
		LeeSin().KickBehind();
		LeeSin().KeyPressComboMode();
		//Skins().fedSkinChanger();
		AutoSmite().AutomaticSmite();
		AutoSmite().KeyPressSmite();
		Toxic().SpamEmote();
	}
	void OnGapCloser(GapCloserSpell const& Args) override
	{

	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
	{

	}
	void OnBeforeAttack( IUnit* Target) override
	{

	}
	void OnLevelUp(IUnit* Source, int NewLevel) override
	{

	}

	void OnCreateObject(IUnit* Source) override
	{
		LeeSin().OnCreateObject(Source);
	}

	void OnDeleteObject(IUnit* Source) override
	{

	}
	void OnInterruptible(InterruptibleSpell const& Args) override
	{

	}

	void OnDash(UnitDash* Args) override
	{
	}

	void OnProcessSpell(CastedSpell const& Args) override
	{
		LeeSin().OnProcessSpell(Args);
	}

	void OnExitVisible(IUnit* Source) override
	{

	}	

	void OnBuffAdd(IUnit* Source, void* BuffData) override
	{
		LeeSin().OnBuffAdd(Source, BuffData);
	}

	void OnBuffRemove(IUnit* Source, void* BuffData) override
	{
		LeeSin().OnBuffRemove(Source, BuffData);
	}

	void OnGameEnd() override
	{
		//Toxic().OnGameEnd();
	}
};

class cKhazix : public IChampion
{
public:

	virtual void OnLoad() override
	{
		//Message().JungleLaneSeries();
		//Message().ChampionLoadMessage();
		Khazix().InitializeMenu();
		Khazix().LoadSpells();
		AutoSmite().SpellsSmite();
		AutoSmite().MenuSmite();
		//Skins().Menu();
		Toxic().MenuToxic();		
	}

	virtual void OnRender() override
	{
		Khazix().Drawing();
		AutoSmite().DrawingsSmite();
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() || GEntityList->Player()->IsRecalling() || GGame->IsChatOpen() || GGame->IsScoreboardOpen() || GGame->IsShopOpen())
		{
			return;
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			Khazix().Combo();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{
			Khazix().LastHit();
			Khazix().Harass();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			Khazix().JungleClear();
			Khazix().LaneClear();
		}
		
		Khazix().Automatic();
		Khazix().AutoEscape();
		AutoSmite().AutomaticSmite();
		AutoSmite().KeyPressSmite();
		Toxic().SpamEmote();
	}
	void OnGapCloser(GapCloserSpell const& Args) override
	{

	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
	{

	}
	void OnBeforeAttack( IUnit* Target) override
	{

	}
	void OnLevelUp(IUnit* Source, int NewLevel) override
	{

	}

	void OnCreateObject(IUnit* Source) override
	{
		
	}

	void OnDeleteObject(IUnit* Source) override
	{

	}
	void OnInterruptible(InterruptibleSpell const& Args) override
	{

	}

	void OnDash(UnitDash* Args) override
	{
	}

	void OnProcessSpell(CastedSpell const& Args) override
	{
		Khazix().OnProcessSpell(Args);
	}

	void OnExitVisible(IUnit* Source) override
	{

	}	

	void OnBuffAdd(IUnit* Source, void* BuffData) override
	{
		
	}

	void OnBuffRemove(IUnit* Source, void* BuffData) override
	{
		
	}

	void OnGameEnd() override
	{
		//Toxic().OnGameEnd();
	}
};

class cGragas : public IChampion
{
public:

	virtual void OnLoad() override
	{
		//Message().JungleLaneSeries();
		//Message().ChampionLoadMessage();
		Gragas().InitializeMenu();
		Gragas().LoadSpells();
		AutoSmite().SpellsSmite();
		AutoSmite().MenuSmite();
		//Skins().Menu();
		Toxic().MenuToxic();
	}

	virtual void OnRender() override
	{
		Gragas().Drawing();
		AutoSmite().DrawingsSmite();
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() || GEntityList->Player()->IsRecalling() || GGame->IsChatOpen() || GGame->IsScoreboardOpen() || GGame->IsShopOpen())
		{
			return;
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			Gragas().Combo();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{			
			Gragas().Harass();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			Gragas().JungleClear();
			Gragas().LaneClear();
		}

		Gragas().Automatic();
		AutoSmite().AutomaticSmite();
		AutoSmite().KeyPressSmite();
		Toxic().SpamEmote();
	}
	void OnGapCloser(GapCloserSpell const& Args) override
	{

	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
	{

	}
	void OnBeforeAttack( IUnit* Target) override
	{

	}
	void OnLevelUp(IUnit* Source, int NewLevel) override
	{

	}

	void OnCreateObject(IUnit* Source) override
	{
		Gragas().OnCreateObject(Source);
	}

	void OnDeleteObject(IUnit* Source) override
	{
		Gragas().OnDeleteObject(Source);
	}
	void OnInterruptible(InterruptibleSpell const& Args) override
	{

	}

	void OnDash(UnitDash* Args) override
	{
	}

	void OnProcessSpell(CastedSpell const& Args) override
	{
		Gragas().OnProcessSpell(Args);
	}

	void OnExitVisible(IUnit* Source) override
	{

	}	

	void OnBuffAdd(IUnit* Source, void* BuffData) override
	{

	}

	void OnBuffRemove(IUnit* Source, void* BuffData) override
	{

	}

	void OnGameEnd() override
	{
		//Toxic().OnGameEnd();
	}
};

class cJax : public IChampion
{
public:

	virtual void OnLoad() override
	{
		//Message().TopLaneSeries();
		//Message().ChampionLoadMessage();
		Jax().Menu();
		Jax().LoadSpells();
		AutoSmite().SpellsSmite();
		AutoSmite().MenuSmite();
		Toxic().MenuToxic();
	}

	virtual void OnRender() override
	{
		AutoSmite().DrawingsSmite();
		Jax().OnRender();
	}

	virtual void OnGameUpdate() override
	{
		Jax().OnGameUpdate();

		AutoSmite().AutomaticSmite();
		AutoSmite().KeyPressSmite();
		Toxic().SpamEmote();
	}
	void OnGapCloser(GapCloserSpell const& Args) override
	{
		//Jax().OnGapCloser(Args);
	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
	{
		Jax().OnOrbwalkAfterAttack(Source, Target);
	}
	void OnBeforeAttack(IUnit* Target) override
	{

	}
	void OnLevelUp(IUnit* Source, int NewLevel) override
	{

	}

	void OnCreateObject(IUnit* Source) override
	{
		Jax().OnCreateObject(Source);
	}

	void OnDeleteObject(IUnit* Source) override
	{

	}
	void OnInterruptible(InterruptibleSpell const& Args) override
	{
	    // Jax).OnInterruptible(Args);
	}

	void OnDash(UnitDash* Args) override
	{
	}

	void OnProcessSpell(CastedSpell const& Args) override
	{
		Jax().OnProcessSpell(Args);
	}

	void OnExitVisible(IUnit* Source) override
	{

	}	

	void OnBuffAdd(IUnit* Source, void* BuffData) override
	{
	}

	void OnBuffRemove(IUnit* Source, void* BuffData) override
	{
	}

	void OnGameEnd() override
	{
		Toxic().OnGameEnd();
	}
};

class cGalio : public IChampion
{
public:

	virtual void OnLoad() override
	{
		//Message().JungleLaneSeries();
		//Message().ChampionLoadMessage();
		Galio().InitializeMenu();
		Galio().LoadSpells();
		AutoSmite().SpellsSmite();
		AutoSmite().MenuSmite();
		//Skins().Menu();
		Toxic().MenuToxic();
	}

	virtual void OnRender() override
	{
		Galio().Drawing();
		AutoSmite().DrawingsSmite();
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() || GEntityList->Player()->IsRecalling() || GGame->IsChatOpen() || GGame->IsScoreboardOpen() || GGame->IsShopOpen())
		{
			return;
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			Galio().ComboSelected();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{
			Galio().Harass();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			Galio().JungleClear();
			Galio().LaneClear();
		}

		Galio().Automatic();
		AutoSmite().AutomaticSmite();
		AutoSmite().KeyPressSmite();
		Toxic().SpamEmote();
	}
	void OnGapCloser(GapCloserSpell const& Args) override
	{

	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
	{

	}
	void OnBeforeAttack(IUnit* Target) override
	{

	}
	void OnLevelUp(IUnit* Source, int NewLevel) override
	{

	}

	void OnCreateObject(IUnit* Source) override
	{
		
	}

	void OnDeleteObject(IUnit* Source) override
	{
		
	}
	void OnInterruptible(InterruptibleSpell const& Args) override
	{
		Galio().OnInterruptible(Args);
	}

	void OnDash(UnitDash* Args) override
	{
	}

	void OnProcessSpell(CastedSpell const& Args) override
	{
		Galio().OnProcessSpell(Args);
	}

	void OnExitVisible(IUnit* Source) override
	{

	}	

	void OnBuffAdd(IUnit* Source, void* BuffData) override
	{

	}

	void OnBuffRemove(IUnit* Source, void* BuffData) override
	{

	}

	void OnGameEnd() override
	{
		//Toxic().OnGameEnd();
	}
};

class cIvern : public IChampion
{
public:

	virtual void OnLoad() override
	{
		//Message().JungleLaneSeries();
		//Message().ChampionLoadMessage();
		Ivern().InitializeMenu();
		Ivern().LoadSpells();
		AutoSmite().SpellsSmite();
		AutoSmite().MenuSmite();
		//Skins().Menu();
		Toxic().MenuToxic();
	}

	virtual void OnRender() override
	{
		Ivern().Drawing();
		AutoSmite().DrawingsSmite();
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() || GEntityList->Player()->IsRecalling() || GGame->IsChatOpen() || GGame->IsScoreboardOpen() || GGame->IsShopOpen())
		{
			return;
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			Ivern().Combo();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{
			Ivern().Harass();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{			
			Ivern().LaneClear();
			Ivern().WUnderTower();
		}

		Ivern().Automatic();
		AutoSmite().AutomaticSmite();
		AutoSmite().KeyPressSmite();
		Toxic().SpamEmote();
	}
	void OnGapCloser(GapCloserSpell const& Args) override
	{

	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
	{

	}
	void OnBeforeAttack(IUnit* Target) override
	{

	}
	void OnLevelUp(IUnit* Source, int NewLevel) override
	{

	}

	void OnCreateObject(IUnit* Source) override
	{		
		Ivern().OnCreateObject(Source);
	}

	void OnDeleteObject(IUnit* Source) override
	{
		Ivern().OnDeleteObject(Source);
	}
	void OnInterruptible(InterruptibleSpell const& Args) override
	{
		
	}

	void OnDash(UnitDash* Args) override
	{
	}

	void OnProcessSpell(CastedSpell const& Args) override
	{
		
	}

	void OnExitVisible(IUnit* Source) override
	{

	}

	void OnBuffAdd(IUnit* Source, void* BuffData) override
	{

	}

	void OnBuffRemove(IUnit* Source, void* BuffData) override
	{

	}

	void OnGameEnd() override
	{
		//Toxic().OnGameEnd();
	}
};

class cKindred : public IChampion
{
public:

	virtual void OnLoad() override
	{
		//Message().JungleLaneSeries();
		//Message().ChampionLoadMessage();
		Kindred().InitializeMenu();
		Kindred().LoadSpells();
		AutoSmite().SpellsSmite();
		AutoSmite().MenuSmite();
		//Skins().Menu();
		Toxic().MenuToxic();
	}

	virtual void OnRender() override
	{
		Kindred().Drawing();
		AutoSmite().DrawingsSmite();
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() || GEntityList->Player()->IsRecalling() || GGame->IsChatOpen() || GGame->IsScoreboardOpen() || GGame->IsShopOpen())
		{
			return;
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			Kindred().Combo();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{
			
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			Kindred().LaneClear();
			Kindred().JungleClear();
		}

		Kindred().Automatic();
		AutoSmite().AutomaticSmite();
		AutoSmite().KeyPressSmite();
		Toxic().SpamEmote();
	}
	void OnGapCloser(GapCloserSpell const& Args) override
	{

	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
	{
		Kindred().OnAfterAttack(Source, Target);
	}
	void OnBeforeAttack(IUnit* Target) override
	{

	}
	void OnLevelUp(IUnit* Source, int NewLevel) override
	{

	}

	void OnCreateObject(IUnit* Source) override
	{
		
	}

	void OnDeleteObject(IUnit* Source) override
	{
		
	}
	void OnInterruptible(InterruptibleSpell const& Args) override
	{

	}

	void OnDash(UnitDash* Args) override
	{
	}

	void OnProcessSpell(CastedSpell const& Args) override
	{
		Kindred().OnProcessSpell(Args);
	}

	void OnExitVisible(IUnit* Source) override
	{

	}

	void OnBuffAdd(IUnit* Source, void* BuffData) override
	{

	}

	void OnBuffRemove(IUnit* Source, void* BuffData) override
	{
		Kindred().OnBuffRemove(Source, BuffData);
	}

	void OnGameEnd() override
	{
		//Toxic().OnGameEnd();
	}
};

IChampion* pChampion = nullptr;

PLUGIN_EVENT(void) OnRender()
{
	pChampion->OnRender();
}

PLUGIN_EVENT(void) OnGameUpdate()
{
	pChampion->OnGameUpdate();
}

PLUGIN_EVENT(void) OnGapCloser(GapCloserSpell const& Args)
{
	pChampion->OnGapCloser(Args);
}

PLUGIN_EVENT(void) OnInterruptible(InterruptibleSpell const& Args)
{
	pChampion->OnInterruptible(Args);
}

PLUGIN_EVENT(void) OnAfterAttack(IUnit* Source, IUnit* Target)
{
	pChampion->OnAfterAttack(Source, Target);
}
PLUGIN_EVENT(void) OnBeforeAttack(IUnit* Target)
{
	pChampion->OnBeforeAttack(Target);
}

PLUGIN_EVENT(void) OnLevelUp(IUnit* Source, int NewLevel)
{
	pChampion->OnLevelUp(Source, NewLevel);
}

PLUGIN_EVENT(void) OnCreateObject(IUnit* Source)
{
	pChampion->OnCreateObject(Source);
}

PLUGIN_EVENT(void) OnDeleteObject(IUnit* Source)
{
	pChampion->OnDeleteObject(Source);
}

PLUGIN_EVENT(void) OnProcessSpell(CastedSpell const& Args)
{
	pChampion->OnProcessSpell(Args);
}

PLUGIN_EVENT(void) OnExitVisible(IUnit* Source)
{
	pChampion->OnExitVisible(Source);
}

PLUGIN_EVENT(void) OnDash(UnitDash* Source)
{
	pChampion->OnDash(Source);
}

PLUGIN_EVENT(void) OnBuffAdd(IUnit* Source, void* BuffData)
{
	pChampion->OnBuffAdd(Source, BuffData);
}

PLUGIN_EVENT(void) OnBuffRemove(IUnit* Source, void* BuffData)
{
	pChampion->OnBuffRemove(Source, BuffData);
}

PLUGIN_EVENT(void) OnGameEnd()
{
	pChampion->OnGameEnd();
}


void LoadChampion()
{
	std::string szChampion = GEntityList->Player()->ChampionName();
	szChampion = ToLower(szChampion);

	// Jungle
	if (szChampion == "olaf")
		pChampion = new cOlaf;
	else if (szChampion == "nocturne")
		pChampion = new cNocturne;
	else if (szChampion == "hecarim")
		pChampion = new cHecarim;
	else if (szChampion == "rengar")
		pChampion = new cRengar;
	else if (szChampion == "diana")
		pChampion = new cDiana;
	else if (szChampion == "warwick")
		pChampion = new cWarwick;
	else if (szChampion == "elise")
		pChampion = new cElise;
	else if (szChampion == "leesin")
		pChampion = new cLeeSin;
	else if (szChampion == "khazix")
		pChampion = new cKhazix;
	else if (szChampion == "gragas")
		pChampion = new cGragas;
	else if (szChampion == "galio")
		pChampion = new cGalio;
	else if (szChampion == "ivern")
		pChampion = new cIvern;
	else if (szChampion == "kindred")
		pChampion = new cKindred;	
	else if (szChampion == "jax")
		pChampion = new cJax;	
	else
	{		
		GGame->PrintChat(szChampion.data());
		GGame->PrintChat("Champion Not Supported");
	}	
	
	if (pChampion != nullptr)
	{
		GEventManager->AddEventHandler(kEventOnRender, OnRender);
		GEventManager->AddEventHandler(kEventOnGapCloser, OnGapCloser);
		GEventManager->AddEventHandler(kEventOnInterruptible, OnInterruptible);
		GEventManager->AddEventHandler(kEventOrbwalkAfterAttack, OnAfterAttack);		
		GEventManager->AddEventHandler(kEventOrbwalkBeforeAttack, OnBeforeAttack);
		GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
		GEventManager->AddEventHandler(kEventOnLevelUp, OnLevelUp);
		GEventManager->AddEventHandler(kEventOnBuffAdd, OnBuffAdd);
		GEventManager->AddEventHandler(kEventOnBuffRemove, OnBuffRemove);
		GEventManager->AddEventHandler(kEventOnSpellCast, OnProcessSpell);
		GEventManager->AddEventHandler(kEventOnCreateObject, OnCreateObject);
		GEventManager->AddEventHandler(kEventOnDestroyObject, OnDeleteObject);
		GEventManager->AddEventHandler(kEventOnExitVisible, OnExitVisible);		
		GEventManager->AddEventHandler(kEventOnDash, OnDash);
		GEventManager->AddEventHandler(kEventOnGameEnd, OnGameEnd);
	}
}

PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)
{	
	PluginSDKSetup(PluginSDK);	
	LoadChampion();
	pChampion->OnLoad();	
	GRender->NotificationEx(Vec4(255,255,0,255), 10, true, true, "Welcome to Nice4Fun - Jungle Series");	

	//GUtility->CreateDebugConsole();	
}

PLUGIN_API void OnUnload()
{
	MainMenu->Remove();
	
	GEventManager->RemoveEventHandler(kEventOnRender, OnRender);
	GEventManager->RemoveEventHandler(kEventOnGapCloser, OnGapCloser);
	GEventManager->RemoveEventHandler(kEventOnInterruptible, OnInterruptible);
	GEventManager->RemoveEventHandler(kEventOrbwalkAfterAttack, OnAfterAttack);	
	GEventManager->RemoveEventHandler(kEventOrbwalkBeforeAttack, OnBeforeAttack);
	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->RemoveEventHandler(kEventOnLevelUp, OnLevelUp);
	GEventManager->RemoveEventHandler(kEventOnBuffAdd, OnBuffAdd);
	GEventManager->RemoveEventHandler(kEventOnBuffRemove, OnBuffRemove);
	GEventManager->RemoveEventHandler(kEventOnSpellCast, OnProcessSpell);
	GEventManager->RemoveEventHandler(kEventOnCreateObject, OnCreateObject);
	GEventManager->RemoveEventHandler(kEventOnDestroyObject, OnDeleteObject);
	GEventManager->RemoveEventHandler(kEventOnExitVisible, OnExitVisible);	
	GEventManager->RemoveEventHandler(kEventOnDash, OnDash);
	GEventManager->RemoveEventHandler(kEventOnGameEnd, OnGameEnd);
}
