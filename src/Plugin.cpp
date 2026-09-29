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

    // Unser Hook greift nun versionssicher über das offizielle HOOKSPACE-Makro
    HOOKSPACE(GOTHIC_NAMESPACE, GetGameVersion() == ENGINE);

    HOOK Hook_Union_MinDamage_OnDamage_Hit PATCH(&oCNpc::OnDamage_Hit, &Union_MinDamage_OnDamage_Hit);

    void __fastcall Union_MinDamage_OnDamage_Hit(oCNpc* _this, void* vtable, oSDamageDescriptor& desc) {
        zerr.Message("[MinDamage] === OnDamage_Hit aufgerufen ===");

        // 1. INI-Wert auslesen
        int settingValue = zoptions->ReadInt("UNION_MINIMUM_DAMAGE", "MinDamageValue", 0);
        zerr.Message("[MinDamage] INI MinDamageValue ausgelesen: " + ZString(settingValue));

        int targetMinDamage = 0;

        // 2. Weiche für Dynamisch vs. Fest
        if (settingValue < 0) {
            zerr.Message("[MinDamage] INI < 0 -> Modus: DYNAMISCH aktiv.");
            
            int bonus = 0;
            if (desc.pNpcAttacker) {
                zerr.Message("[MinDamage] Angreifer (pNpcAttacker) vorhanden.");
                
                bool isRanged = (desc.enuModeWeapon == NPC_WEAPON_BOW || desc.enuModeWeapon == NPC_WEAPON_CBOW);
                if (isRanged) {
                    zerr.Message("[MinDamage] Waffentyp: Fernkampf (Bogen/Armbrust).");
                    bonus = (desc.pNpcAttacker->attribute[NPC_ATR_DEXTERITY] / 10) - 1;
                } else {
                    zerr.Message("[MinDamage] Waffentyp: Nahkampf (oder unbewaffnet/Magie).");
                    bonus = (desc.pNpcAttacker->attribute[NPC_ATR_STRENGTH] / 10) - 1;
                }
                zerr.Message("[MinDamage] Berechneter Attribut-Bonus: " + ZString(bonus));
            } else {
                zerr.Message("[MinDamage] KEIN Angreifer (pNpcAttacker ist nullptr)!");
            }

            targetMinDamage = 5 + bonus;
            if (targetMinDamage < 0) {
                targetMinDamage = 0;
                zerr.Message("[MinDamage] TargetMinDamage war < 0, auf 0 korrigiert.");
            } else {
                zerr.Message("[MinDamage] TargetMinDamage (dynamisch) ergibt: " + ZString(targetMinDamage));
            }
        } else {
            zerr.Message("[MinDamage] INI >= 0 -> Modus: FESTER WERT aktiv.");
            targetMinDamage = settingValue;
            zerr.Message("[MinDamage] TargetMinDamage (fest) gesetzt auf: " + ZString(targetMinDamage));
        }

        // 3. Daedalus-Symbol ansprechen
        zCPar_Symbol* sym = parser->GetSymbol("NPC_MINIMAL_DAMAGE");
        int oldMinDamage = 5;
        if (sym) {
            oldMinDamage = sym->single_intdata;
            zerr.Message("[MinDamage] NPC_MINIMAL_DAMAGE Symbol gefunden. Alter Wert: " + ZString(oldMinDamage));
            sym->single_intdata = targetMinDamage;
            zerr.Message("[MinDamage] NPC_MINIMAL_DAMAGE überschrieben auf: " + ZString(targetMinDamage));
        } else {
            zerr.Message("[MinDamage] FEHLER: NPC_MINIMAL_DAMAGE Symbol nicht im Parser gefunden!");
        }

        // 4. Originalen Code ausführen
        zerr.Message("[MinDamage] Springe in die originale OnDamage_Hit Routine...");
        Hook_Union_MinDamage_OnDamage_Hit(_this, vtable, desc);
        zerr.Message("[MinDamage] Ausführung der originalen Routine beendet.");

        // 5. Wiederherstellen
        if (sym) {
            sym->single_intdata = oldMinDamage;
            zerr.Message("[MinDamage] NPC_MINIMAL_DAMAGE wiederhergestellt auf alten Wert: " + ZString(oldMinDamage));
        }
        zerr.Message("[MinDamage] === Ende OnDamage_Hit ===");
    }

    void App_Init() {
        zerr.Message("==================================================");
        zerr.Message("[Union_MinimumDamage] PLUGIN START: Erfolgreich geladen!");
        zerr.Message("==================================================");
    }

    cInitApp Documents_Init(App_Init);
}

#undef GOTHIC_NAMESPACE
#undef ENGINE
#endif

// Globaler Fallback für Union-Initialisierung
HOOKSPACE(Global, true);