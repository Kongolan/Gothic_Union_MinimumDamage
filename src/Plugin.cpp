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
#define ENGINE Engine_G1_Addon
#endif

// --- GOTHIC 2 CLASSIC ---
#ifdef __G2
#define GOTHIC_NAMESPACE Gothic_II_Classic
#define ENGINE Engine_G2
#endif

// --- GOTHIC 2 ADDON (Nacht des Raben) ---
#ifdef __G2A
#define GOTHIC_NAMESPACE Gothic_II_Addon
#define ENGINE Engine_G2_Addon
#endif

// Wenn eine gültige Engine aktiv ist, registrieren wir den Hook im korrekten Namespace
#if defined(GOTHIC_NAMESPACE) && defined(ENGINE)

namespace GOTHIC_NAMESPACE {

    // Eigene Logging-Funktion fuer den Bildschirm (nur wenn DebugMode=1 in INI)
    void LogDebug(const ZString& text) {
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
        LogDebug("INI MinDamageValue: " + ZString(settingValue));

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
                LogDebug("Attribut-Bonus: " + ZString(bonus));
            } else {
                LogDebug("KEIN Angreifer (nullptr)!");
            }
            targetMinDamage = 5 + bonus;
            if (targetMinDamage < 0) {
                targetMinDamage = 0;
                LogDebug("TargetMinDamage < 0, auf 0 korrigiert.");
            } else {
                LogDebug("TargetMinDamage (dyn): " + ZString(targetMinDamage));
            }
        } else {
            // Fester Wert - greift direkt auf den INI-Wert zu (der exakt dem Index entspricht!)
            LogDebug("Modus: FEST");
            targetMinDamage = settingValue;
            LogDebug("TargetMinDamage (fest): " + ZString(targetMinDamage));
        }

        LogDebug("Ziel-Schaden: " + ZString(targetMinDamage));

        // 3. Daedalus-Symbol ansprechen
        zCPar_Symbol* sym = parser->GetSymbol("NPC_MINIMAL_DAMAGE");
        int oldMinDamage = 5;
        if (sym) {
            oldMinDamage = sym->single_intdata;
            LogDebug("Alter NPC_MINIMAL_DAMAGE: " + ZString(oldMinDamage));
            sym->single_intdata = targetMinDamage;
            LogDebug("Überschrieben auf: " + ZString(targetMinDamage));
        } else {
            LogDebug("FEHLER: NPC_MINIMAL_DAMAGE nicht gefunden!");
        }

        // 4. Originale Schadensberechnung der Engine ausführen
        LogDebug("Führe originalen OnDamage_Hit aus...");
        Hook_Union_MinDamage_OnDamage(_this, vtable, desc);
        LogDebug("Originale Routine beendet.");

        // 5. Symbol sofort wiederherstellen, damit andere Vanilla-Berechnungen ungestört bleiben
        if (sym) {
            sym->single_intdata = oldMinDamage;
            LogDebug("NPC_MINIMAL_DAMAGE wiederhergestellt: " + ZString(oldMinDamage));
        }
        LogDebug("=== Ende OnDamage_Hit ===");
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

    // Dies registriert die Mod tief in der Union-Engine. Ohne diesen Block lädt nichts!
    #define AppDefault True
    CApplication* lpApplication = !CHECK_THIS_ENGINE ? Null : CApplication::CreateRefApplication(
        Enabled( AppDefault ) Game_Entry,
        Enabled( AppDefault ) Game_Init,
        Enabled( AppDefault ) Game_Exit,
        Enabled( AppDefault ) Game_PreLoop,
        Enabled( AppDefault ) Game_Loop,
        Enabled( AppDefault ) Game_PostLoop,
        Enabled( AppDefault ) Game_MenuLoop,
        Enabled( AppDefault ) Game_SaveBegin,
        Enabled( AppDefault ) Game_SaveEnd,
        Enabled( AppDefault ) Game_LoadBegin_NewGame,
        Enabled( AppDefault ) Game_LoadEnd_NewGame,
        Enabled( AppDefault ) Game_LoadBegin_SaveGame,
        Enabled( AppDefault ) Game_LoadEnd_SaveGame,
        Enabled( AppDefault ) Game_LoadBegin_ChangeLevel,
        Enabled( AppDefault ) Game_LoadEnd_ChangeLevel,
        Enabled( AppDefault ) Game_LoadBegin_Trigger,
        Enabled( AppDefault ) Game_LoadEnd_Trigger,
        Enabled( AppDefault ) Game_Pause,
        Enabled( AppDefault ) Game_Unpause,
        Enabled( AppDefault ) Game_DefineExternals,
        Enabled( AppDefault ) Game_ApplyOptions
    );
}

#undef GOTHIC_NAMESPACE
#undef ENGINE
#endif