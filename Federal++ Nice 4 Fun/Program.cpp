#include "PluginSDK.h"
#include "TextHeader.h"
#include "Color.h"
#include "Champions.h"
#include "AutoSmite.h";
#include "Toxic.h";


#pragma region Events
PLUGIN_EVENT(void) OnOrbwalkBeforeAttack(IUnit* Target)
{

}

PLUGIN_EVENT(void) OnOrbwalkAttack(IUnit* Source, IUnit* Target)
{

}
#pragma endregion

PluginSetup("Federal++ Aio Nice 4 Fun!");

class IChampion
{
public:		
	virtual void OnGameUpdate() = 0;
	virtual void OnAfterAttack(IUnit* Source, IUnit* Target) = 0;
	virtual void OnGapCloser(GapCloserSpell const& Args) = 0;
	virtual void OnInterruptible(InterruptibleSpell const& Args) = 0;
	virtual void OnDash(UnitDash* Args) = 0;
	virtual void OnCreateObject(IUnit* Object) = 0;
	virtual void OnDeleteObject(IUnit* Object) = 0;
	virtual void OnLevelUp(IUnit* Source, int NewLevel) = 0;
	virtual void OnProcessSpell(CastedSpell const& Args) = 0;
	virtual void OnExitVisible(IUnit* Source) = 0;
	virtual void OnUnitDeath(IUnit* Source) = 0;
	virtual void OnLoad() = 0;
	//virtual void OnUnLoad() = 0;
	virtual void OnRender() = 0;
};

class cVarus : public IChampion
{
public:
	virtual void OnLoad() override
	{
		//Message().ADCLaneSeries();
		Message().ChampionLoadMessage();
		Varus().InitializeMenu();
		Varus().LoadSpells();
	}

	virtual void OnRender() override
	{
		Varus().Drawing();

	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() && GEntityList->Player()->IsRecalling())
		{
			return;
	     }
		
			if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
			{
				Varus().Combo();
			}
			if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
			{
				Varus().Harass();
			}
			if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
			{
				Varus().FarmHarass();
				Varus().LaneClear();
				Varus().JungleClear();
			}
			Varus().SemiRLogic();
			Varus().AutoHarass();
			Varus().KillSteal();		
		 
	}
	void OnGapCloser(GapCloserSpell const& Args) override
	{

	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
	{
		
	}
	void OnLevelUp(IUnit* Source, int NewLevel) override
	{

	}	
	void OnInterruptible(InterruptibleSpell const& Args) override
	{
		
	}	

	void OnCreateObject(IUnit* Source) override
	{

	}

	void OnDeleteObject(IUnit* Source) override
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

	void OnUnitDeath(IUnit* Source) override
	{

	}
};

class cZiggs : public IChampion
{
public:

	virtual void OnLoad() override
	{
		//Message().MidLaneSeries();
		Message().ChampionLoadMessage();
		Ziggs().InitializeSpells();
		Ziggs().InitializeMenu();
	}

	virtual void OnRender() override
	{
		
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() && GEntityList->Player()->IsRecalling())
		{
			return;
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{

		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{

		}
		
		//Ziggs().SkinChanger();
		Ziggs().LogicW();
		Ziggs().CheckKeyPresses();
	}

	void OnGapCloser(GapCloserSpell const& Args) override
	{

	}
	void OnInterruptible(InterruptibleSpell const& Args) override
	{
		
	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
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

	void OnDash(UnitDash* Args) override
	{
	}

	void OnProcessSpell(CastedSpell const& Args) override
	{

	}

	void OnExitVisible(IUnit* Source) override
	{

	}

	void OnUnitDeath(IUnit* Source) override
	{

	}
};

class cAshe : public IChampion
{
public:

	virtual void OnLoad() override
	{
		//Message().MidLaneSeries();
		Message().ChampionLoadMessage();
		Ashe().InitializeSpells();
		Ashe().InitializeMenu();
	}

	virtual void OnRender() override
	{
		Ashe().Drawing();
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() && GEntityList->Player()->IsRecalling())
		{
			return;
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{			
			
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			
		}
		Ashe().LogicR();
		Ashe().LogicW();
		Ashe().Automatic();		
		Ashe().SkinChanger();
		Ashe().KeyPressUltimate();
	}

	void OnGapCloser(GapCloserSpell const& Args) override
	{
		
	}
	void OnInterruptible(InterruptibleSpell const& Args) override
	{
		Ashe().OnInterruptible(Args);
	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
	{
		Ashe().OnAfterAttack(Source, Target);
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

	void OnDash(UnitDash* Args) override
	{
	}

	void OnProcessSpell(CastedSpell const& Args) override
	{

	}

	void OnExitVisible(IUnit* Source) override
	{

	}

	void OnUnitDeath(IUnit* Source) override
	{

	}
};

class cDraven : public IChampion
{
public:

	virtual void OnLoad() override
	{
		//Message().MidLaneSeries();
		Message().ChampionLoadMessage();
		Draven().InitializeSpells();
		Draven().InitializeMenu();
	}

	virtual void OnRender() override
	{
		Draven().Drawing();
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() && GEntityList->Player()->IsRecalling())
		{
			return;
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{			
			Draven().LogicW();
			Draven().AxeLogicCombo();
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			Draven().AxeLogicFarm();
		}		
		
		Draven().LogicR();
		Draven().LogicE();
		Draven().KeyPressUltimate();
		Draven().SkinChanger();
	}

	void OnGapCloser(GapCloserSpell const& Args) override
	{
		Draven().OnGapcloser(Args);
	}
	void OnInterruptible(InterruptibleSpell const& Args) override
	{
		
	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
	{
		Draven().OnAfterAttack(Source, Target);
	}
	void OnLevelUp(IUnit* Source, int NewLevel) override
	{

	}

	void OnCreateObject(IUnit* Source) override
	{
		Draven().OnCreateObject(Source);
	}

	void OnDeleteObject(IUnit* Source) override
	{
		Draven().OnDeleteObject(Source);
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

	void OnUnitDeath(IUnit* Source) override
	{

	}
};

class cAhri : public IChampion
{
public:

	virtual void OnLoad() override
	{
		//Message().MidLaneSeries();
		Message().ChampionLoadMessage();
		Ahri().LoadSpells();
		Ahri().InitializeMenu();			
	}

	virtual void OnRender() override
	{
		Ahri().Drawing();		
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() && GEntityList->Player()->IsRecalling())
		{
			return;
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			Ahri().Combo();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{
			Ahri().Harass();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			Ahri().LaneClear();
			Ahri().JungleClear();
		}
		
		Ahri().Automatic();		
		Ahri().SkinChanger();		
	}

	void OnGapCloser(GapCloserSpell const& Args) override
	{
		Ahri().OnGapcloser(Args);
	}
	void OnInterruptible(InterruptibleSpell const& Args) override
	{
		Ahri().OnInterruptible(Args);
	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
	{

	}
	void OnLevelUp(IUnit* Source, int NewLevel) override
	{

	}

	void OnCreateObject(IUnit* Source) override
	{
		Ahri().OnCreateObject(Source);
	}

	void OnDeleteObject(IUnit* Source) override
	{
		Ahri().OnDeleteObject(Source);
	}

	void OnDash(UnitDash* Args) override
	{
	}

	void OnProcessSpell(CastedSpell const& Args) override
	{
		Ahri().OnProcessSpell(Args);
		
	}

	void OnExitVisible(IUnit* Source) override
	{

	}

	void OnUnitDeath(IUnit* Source) override
	{

	}
};

class cOlaf : public IChampion
{
public:

	virtual void OnLoad() override
	{
		//Message().JungleLaneSeries();
		Message().ChampionLoadMessage();
		Olaf().InitializeMenu();
		Olaf().LoadSpells();

		AutoSmite().SpellsSmite();
		AutoSmite().MenuSmite();		
	}

	virtual void OnRender() override
	{
		Olaf().Drawing();
		AutoSmite().DrawingsSmite();
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() && GEntityList->Player()->IsRecalling())
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
		Olaf().SkinChanger();
		AutoSmite().AutomaticSmite();
		AutoSmite().KeyPressSmite();
	}
	void OnGapCloser(GapCloserSpell const& Args) override
	{
		Olaf().OnGapcloser(Args);
	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
	{
		Olaf().OnAfterAttack(Source, Target);
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

	void OnUnitDeath(IUnit* Source) override
	{

	}
};

class cDrMundo : public IChampion
{
public:

	virtual void OnLoad() override
	{
		//Message().JungleLaneSeries();
		Message().ChampionLoadMessage();
		DrMundo().InitializeMenu();
		DrMundo().LoadSpells();

		AutoSmite().SpellsSmite();
		AutoSmite().MenuSmite();
	}

	virtual void OnRender() override
	{
		DrMundo().Drawing();
		AutoSmite().DrawingsSmite();
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() && GEntityList->Player()->IsRecalling())
		{
			return;
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			DrMundo().Combo();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{
			DrMundo().Harass();
			DrMundo().LastHit();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			DrMundo().LaneClear();
			DrMundo().JungleClear();
			DrMundo().AutoStopW();
		}

		DrMundo().Automatic();
		DrMundo().SkinChanger();
		AutoSmite().AutomaticSmite();
		AutoSmite().KeyPressSmite();
	}
	void OnGapCloser(GapCloserSpell const& Args) override
	{
		DrMundo().OnGapcloser(Args);
	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
	{
		DrMundo().OnAfterAttack(Source, Target);
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

	void OnUnitDeath(IUnit* Source) override
	{

	}
};

class cMaokai : public IChampion
{
public:

	virtual void OnLoad() override
	{
		//Message().JungleLaneSeries();
		Message().ChampionLoadMessage();
		Maokai().InitializeMenu();
		Maokai().LoadSpells();

		AutoSmite().SpellsSmite();
		AutoSmite().MenuSmite();
	}

	virtual void OnRender() override
	{
		Maokai().Drawing();
		AutoSmite().DrawingsSmite();
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() && GEntityList->Player()->IsRecalling())
		{
			return;
		}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			Maokai().Combo();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{
			Maokai().Harass();
		}
		if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			Maokai().LaneClear();
			Maokai().JungleClear();
		}		
		
		Maokai().AutoUlt();
		Maokai().SkinChanger();
		AutoSmite().AutomaticSmite();
		AutoSmite().KeyPressSmite();
	}
	void OnGapCloser(GapCloserSpell const& Args) override
	{
		
	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
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

	void OnUnitDeath(IUnit* Source) override
	{

	}
};

class cNocturne : public IChampion
{
public:

	virtual void OnLoad() override
	{
		//Message().JungleLaneSeries();
		Message().ChampionLoadMessage();
		Nocturne().InitializeMenu();
		Nocturne().LoadSpells();

		AutoSmite().SpellsSmite();
		AutoSmite().MenuSmite();
	}

	virtual void OnRender() override
	{
		Nocturne().Drawing();
		AutoSmite().DrawingsSmite();
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() && GEntityList->Player()->IsRecalling())
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
		Nocturne().SkinChanger();
		Nocturne().KeyPressUltimate();
		AutoSmite().AutomaticSmite();
		AutoSmite().KeyPressSmite();
	}
	void OnGapCloser(GapCloserSpell const& Args) override
	{
	
	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
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

	void OnUnitDeath(IUnit* Source) override
	{

	}
};

class cHecarim : public IChampion
{
public:

	virtual void OnLoad() override
	{
		//Message().JungleLaneSeries();
		Message().ChampionLoadMessage();
		Hecarim().InitializeMenu();
		Hecarim().LoadSpells();

		AutoSmite().SpellsSmite();
		AutoSmite().MenuSmite();
	}

	virtual void OnRender() override
	{
		Hecarim().Drawing();
		AutoSmite().DrawingsSmite();
	}

	virtual void OnGameUpdate() override
	{
		if (GEntityList->Player()->IsDead() && GEntityList->Player()->IsRecalling())
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
		Hecarim().SkinChanger();		
		AutoSmite().AutomaticSmite();
		AutoSmite().KeyPressSmite();
	}
	void OnGapCloser(GapCloserSpell const& Args) override
	{
		Hecarim().OnGapcloser(Args);
	}
	void OnAfterAttack(IUnit* Source, IUnit* Target) override
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

	void OnUnitDeath(IUnit* Source) override
	{

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

PLUGIN_EVENT(void) OnUnitDeath(IUnit* Source)
{
	pChampion->OnUnitDeath(Source);
}

void LoadChampion()
{
	std::string szChampion = GEntityList->Player()->ChampionName();

	//ADC
	if (szChampion == "Varus")
		pChampion = new cVarus;
	else if (szChampion == "Ashe")
		pChampion = new cAshe;
	else if (szChampion == "Draven")
		pChampion = new cDraven;
	// Midlane
	else if (szChampion == "Ahri")
		pChampion = new cAhri;
	else if (szChampion == "Ziggs")
		pChampion = new cZiggs;
	// Jungle
	else if (szChampion == "Olaf")
		pChampion = new cOlaf;
	else if (szChampion == "Nocturne")
		pChampion = new cNocturne;
	else if (szChampion == "Hecarim")
		pChampion = new cHecarim;
	// Topline
	else if (szChampion == "DrMundo")
		pChampion = new cDrMundo;
	else if (szChampion == "Maokai")
		pChampion = new cMaokai;
	// Support
	// Not Found
	else
	{
		GGame->PrintChat("Champion Not Supported");
	}	
	
	if (pChampion != nullptr)
	{
		GEventManager->AddEventHandler(kEventOnRender, OnRender);
		GEventManager->AddEventHandler(kEventOnGapCloser, OnGapCloser);
		GEventManager->AddEventHandler(kEventOnInterruptible, OnInterruptible);
		GEventManager->AddEventHandler(kEventOrbwalkAfterAttack, OnAfterAttack);
		GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
		GEventManager->AddEventHandler(kEventOnLevelUp, OnLevelUp);
		//GEventManager->AddEventHandler(kEventOnBuffAdd, OnBuffAdd);
		//GEventManager->AddEventHandler(kEventOnBuffRemove, OnBuffRemove);
		GEventManager->AddEventHandler(kEventOnSpellCast, OnProcessSpell);
		GEventManager->AddEventHandler(kEventOnCreateObject, OnCreateObject);
		GEventManager->AddEventHandler(kEventOnDestroyObject, OnDeleteObject);
		GEventManager->AddEventHandler(kEventOnExitVisible, OnExitVisible);
		GEventManager->AddEventHandler(kEventOnUnitDeath, OnUnitDeath);
	}
}

// Called when plugin is first loaded
PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)
{
	// Initializes global interfaces for core access
	PluginSDKSetup(PluginSDK);
	LoadChampion();
	pChampion->OnLoad();	
	GRender->NotificationEx(Color::LightBlue().Get(), 2, true, true, "Welcome to Federal++ Nice 4 Fun!");
	GRender->NotificationEx(Color::LightBlue().Get(), 3, true, true, "Sup Champions: Ahri, Ashe, Draven, Hecarim, Maokai, Nocturne, Olaf, Dr. Mundo");
}

// Called when plugin is unloaded
PLUGIN_API void OnUnload()
{
	//pChampion->OnUnLoad();

	MainMenu->Remove();
	
	GEventManager->RemoveEventHandler(kEventOnRender, OnRender);
	GEventManager->RemoveEventHandler(kEventOnGapCloser, OnGapCloser);
	GEventManager->RemoveEventHandler(kEventOnInterruptible, OnInterruptible);
	GEventManager->RemoveEventHandler(kEventOrbwalkAfterAttack, OnAfterAttack);
	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->RemoveEventHandler(kEventOnLevelUp, OnLevelUp);
	//GEventManager->RemoveEventHandler(kEventOnBuffAdd, OnBuffAdd);
	//GEventManager->RemoveEventHandler(kEventOnBuffRemove, OnBuffRemove);
	GEventManager->RemoveEventHandler(kEventOnSpellCast, OnProcessSpell);
	GEventManager->RemoveEventHandler(kEventOnCreateObject, OnCreateObject);
	GEventManager->RemoveEventHandler(kEventOnDestroyObject, OnDeleteObject);
	GEventManager->RemoveEventHandler(kEventOnExitVisible, OnExitVisible);
	GEventManager->RemoveEventHandler(kEventOnUnitDeath, OnUnitDeath);
}
