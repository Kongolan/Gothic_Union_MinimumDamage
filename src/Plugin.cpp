#include <Union/Hook.h>
#include <ZenGin/zGothicAPI.h>

// --- GOTHIC 1 ---
#ifdef __G1
#define GOTHIC_NAMESPACE Gothic_I
#define ENGINE Engine_G1
#endif

// --- GOTHIC 1.08k ---
#ifdef __G1A
#define GOTHIC_NAMESPACE Gothic_I_Addon
#define ENGINE Engine_G1A
#endif

// --- GOTHIC 2 CLASSIC ---
#ifdef __G2
#define GOTHIC_NAMESPACE Gothic_II_Classic
#define ENGINE Engine_G2
#endif

// --- GOTHIC 2 ADDON (Nacht des Raben) ---
#ifdef __G2A
#define GOTHIC_NAMESPACE Gothic_II_Addon
#define ENGINE Engine_G2A
#endif

// Wenn eine gültige Engine aktiv ist, registrieren wir den Hook im korrekten Namespace
#if defined(GOTHIC_NAMESPACE) && defined(ENGINE)

namespace GOTHIC_NAMESPACE {

    // Eigene Logging-Funktion (Nutzt jetzt das native zSTRING statt dem Wizard-Makro ZString)
    void LogDebug(const zSTRING& text) {
        int debugMode = zoptions->ReadInt("UNION_MINIMUM_DAMAGE", "DebugMode", 0);
        if (debugMode > 0) {
            // Loggt unsichtbar in die zSpy Konsole mit
            zerr.Message("[MinDamage] " + text);
            
            // Nutzt die native Gothic-Textausgabe (wie "10 Erz erhalten"), damit es sicher lesbar aufploppt!
            if (ogame && ogame->GetTextView()) {
                ogame->GetTextView()->Printwin(text);
            }
        }
    }

    // Unser Hook greift nun versionssicher über das offizielle HOOKSPACE-Makro
    HOOKSPACE(GOTHIC_NAMESPACE, GetGameVersion() == ENGINE);

    // ==========================================================
    // 1. SCHADENSBERECHNUNG (OnDamage Root Hook)
    // ==========================================================
    // Da oCNpc::OnDamage überladen ist, nutzen wir einen static_cast, um den 
    // Pointer auf die exakte Signatur (mit oSDamageDescriptor) zu zwingen.
    HOOK Hook_Union_MinDamage_OnDamage PATCH( static_cast<void(oCNpc::*)(oSDamageDescriptor&)>(&oCNpc::OnDamage), &Union_MinDamage_OnDamage );

    void __fastcall Union_MinDamage_OnDamage(oCNpc* _this, void* vtable, oSDamageDescriptor& desc) {
        LogDebug("--- NEUER TREFFER (OnDamage Root) ---");

        // 1. INI-Werte auslesen (Standard: Dynamisch = 1, Wert = 0)
        int isDynamic    = zoptions->ReadInt("UNION_MINIMUM_DAMAGE", "DynamicMode", 1);
        int settingValue = zoptions->ReadInt("UNION_MINIMUM_DAMAGE", "MinDamageValue", 0);
        LogDebug("INI MinDamageValue: " + zSTRING(settingValue));

        int targetMinDamage = 0;

        // 2. Kristallklare Logik
        if (isDynamic == 1) {
            LogDebug("Modus: DYNAMISCH");
            
            int bonus = 0;
            if (desc.pNpcAttacker) {
                LogDebug("Angreifer (pNpcAttacker) vorhanden.");
                
                bool isRanged = (desc.enuModeWeapon == NPC_WEAPON_BOW || desc.enuModeWeapon == NPC_WEAPON_CBOW);
                if (isRanged) {
                    LogDebug("Waffentyp: Fernkampf (Bogen/Armbrust).");
                    bonus = (desc.pNpcAttacker->attribute[NPC_ATR_DEXTERITY] / 10) - 1;
                } else {
                    LogDebug("Waffentyp: Nahkampf.");
                    bonus = (desc.pNpcAttacker->attribute[NPC_ATR_STRENGTH] / 10) - 1;
                }
                LogDebug("Attribut-Bonus: " + zSTRING(bonus));
            } else {
                LogDebug("KEIN Angreifer (nullptr)!");
            }
            targetMinDamage = 5 + bonus;
            if (targetMinDamage < 0) {
                targetMinDamage = 0;
                LogDebug("TargetMinDamage < 0, auf 0 korrigiert.");
            } else {
                LogDebug("TargetMinDamage (dyn): " + zSTRING(targetMinDamage));
            }
        } else {
            // Fester Wert - greift direkt auf den INI-Wert zu
            LogDebug("Modus: FEST");
            targetMinDamage = settingValue;
            LogDebug("TargetMinDamage (fest): " + zSTRING(targetMinDamage));
        }

        LogDebug("Ziel-Schaden: " + zSTRING(targetMinDamage));

        // 3. Daedalus-Symbol ansprechen
        zCPar_Symbol* sym = parser->GetSymbol("NPC_MINIMAL_DAMAGE");
        int oldMinDamage = 5;
        if (sym) {
            oldMinDamage = sym->single_intdata;
            LogDebug("Alter NPC_MINIMAL_DAMAGE: " + zSTRING(oldMinDamage));
            sym->single_intdata = targetMinDamage;
            LogDebug("Überschrieben auf: " + zSTRING(targetMinDamage));
        } else {
            LogDebug("FEHLER: NPC_MINIMAL_DAMAGE nicht gefunden!");
        }

        // 4. Originale Schadensberechnung der Engine ausführen
        LogDebug("Führe originalen OnDamage aus...");
        Hook_Union_MinDamage_OnDamage(_this, vtable, desc);
        LogDebug("Originale Routine beendet.");

        // 5. Symbol sofort wiederherstellen
        if (sym) {
            sym->single_intdata = oldMinDamage;
            LogDebug("NPC_MINIMAL_DAMAGE wiederhergestellt: " + zSTRING(oldMinDamage));
        }
        LogDebug("=== Ende OnDamage ===");
    }

    // ==========================================================
    // 2. OBLIGATORISCHES UNION-LIFECYCLE-GERÜST
    // ==========================================================
    void Game_Entry() {}
    
    void Game_Init() {
        // Hier feuern wir unser Log! Wenn das im zSpy auftaucht, laeuft die DLL!
        zerr.Message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        zerr.Message("!!! UNION MINIMUM DAMAGE DLL WURDE GELADEN !!!");
        zerr.Message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    }
    
    void Game_Exit() {}
    void Game_PreLoop() {}
    void Game_Loop() {}
    void Game_PostLoop() {}
    void Game_MenuLoop() {}
    void Game_SaveBegin() {}
    void Game_SaveEnd() {}
    void LoadBegin() {}
    void LoadEnd() {}
    void Game_LoadBegin_NewGame() {}
    void Game_LoadEnd_NewGame() {}
    void Game_LoadBegin_SaveGame() {}
    void Game_LoadEnd_SaveGame() {}
    void Game_LoadBegin_ChangeLevel() {}
    void Game_LoadEnd_ChangeLevel() {}
    void Game_LoadBegin_Trigger() {}
    void Game_LoadEnd_Trigger() {}
    void Game_Pause() {}
    void Game_Unpause() {}
    void Game_DefineExternals() {}
    void Game_ApplyOptions() {}

    // Registrierung als natives C++ ohne VS-Makros. 
    // Wird nur geladen, wenn die gebaute DLL-Version mit der aktiven Engine übereinstimmt.
    CApplication* lpApplication = (Union::GetEngineVersion() == ENGINE) ? CApplication::CreateRefApplication(
        Game_Entry, Game_Init, Game_Exit, Game_PreLoop, Game_Loop, Game_PostLoop, Game_MenuLoop,
        Game_SaveBegin, Game_SaveEnd, Game_LoadBegin_NewGame, Game_LoadEnd_NewGame,
        Game_LoadBegin_SaveGame, Game_LoadEnd_SaveGame, Game_LoadBegin_ChangeLevel,
        Game_LoadEnd_ChangeLevel, Game_LoadBegin_Trigger, Game_LoadEnd_Trigger,
        Game_Pause, Game_Unpause, Game_DefineExternals, Game_ApplyOptions
    ) : nullptr;
}

#undef GOTHIC_NAMESPACE
#undef ENGINE
#endif

// Globaler Fallback
HOOKSPACE(Global, true);