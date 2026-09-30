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
    // 1. SCHADENSBERECHNUNG (Die absolute Wurzel!)
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

    void App_Init() {
        // Beim Start einmalig prüfen, ob wir den Debug-Text triggern können
        zerr.Message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        zerr.Message("!!! UNION MINIMUM DAMAGE DLL WURDE GELADEN !!!");
        zerr.Message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    }
    }

    cInitApp Documents_Init(App_Init);
}

#undef GOTHIC_NAMESPACE
#undef ENGINE
#endif

// Globaler Fallback für Union-Initialisierung
HOOKSPACE(Global, true);