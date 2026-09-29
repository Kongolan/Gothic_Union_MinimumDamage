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
        int settingValue = zoptions->ReadInt("UNION_MINIMUM_DAMAGE", "MinDamageValue", 0);
        int targetMinDamage = 5;

        if (settingValue == 0) { 
            int bonus = 0;
            if (desc.pNpcAttacker) {
                bool isRanged = (desc.enuModeWeapon == NPC_WEAPON_BOW || desc.enuModeWeapon == NPC_WEAPON_CBOW);
                if (isRanged) {
                    bonus = (desc.pNpcAttacker->attribute[NPC_ATR_DEXTERITY] / 10) - 1;
                } else {
                    bonus = (desc.pNpcAttacker->attribute[NPC_ATR_STRENGTH] / 10) - 1;
                }
            }
            targetMinDamage = 5 + bonus;
            if (targetMinDamage < 0) {
                targetMinDamage = 0; 
            }
        } else {
            targetMinDamage = settingValue - 1;
        }

        zCPar_Symbol* sym = parser->GetSymbol("NPC_MINIMAL_DAMAGE");
        int oldMinDamage = 5;
        if (sym) {
            oldMinDamage = sym->single_intdata;
            sym->single_intdata = targetMinDamage;
        }

        Hook_Union_MinDamage_OnDamage_Hit(_this, vtable, desc);

        if (sym) {
            sym->single_intdata = oldMinDamage;
        }
    }
}

#undef GOTHIC_NAMESPACE
#undef ENGINE
#endif

// Globaler Fallback für Union-Initialisierung
HOOKSPACE(Global, true);