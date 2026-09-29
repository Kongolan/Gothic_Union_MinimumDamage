// 1. Zwingend: Dem Compiler mitteilen, dass wir das Addon-Modul laden wollen
#define __G2A
#define GOTHIC_NAMESPACE Gothic_II_Addon

// Zwingend notwendig, damit die Gothic API oCNpc und oSDamageDescriptor voll auswertet:
#define ENGINE_O_NPC
#define ENGINE_O_DAMAGE

// 2. Gothic API (oCNpc, zoptions, parser etc. sind jetzt erfolgreich geladen!)
#include <ZenGin/zGothicAPI.h>

// 3. Spezifische Engine-Header explizit einbinden, damit oCNpc und oSDamageDescriptor vollständig definiert sind
#include <ZenGin/Gothic_II_Addon/API/oNpc.h>
#include <ZenGin/Gothic_II_Addon/API/oDamage.h>

// 4. Union API erst einbinden, wenn alle Engine-Klassen voll bekannt sind, damit das HOOK-Makro greifen kann
#include <Union/Hook.h>

namespace Gothic_II_Addon {

    // Eindeutiger Name für die Hook-Variable und die neue Funktion
    HOOK Hook_Union_MinDamage_OnDamage_Hit PATCH(&oCNpc::OnDamage_Hit, &Union_MinDamage_OnDamage_Hit);

    void __fastcall Union_MinDamage_OnDamage_Hit(oCNpc* _this, void* vtable, oSDamageDescriptor& desc) {
        // Die INI-Sektion "UNION_MINIMUM_DAMAGE" mit deinem festen Key "MinDamageValue"
        int settingValue = zoptions->ReadInt("UNION_MINIMUM_DAMAGE", "MinDamageValue", 0);
        int targetMinDamage = 5;

        if (settingValue == 0) { 
            int bonus = 0;
            if (desc.pNpcAttacker) {
                // Waffentyp prüfen: 5 = Bogen, 6 = Armbrust
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

        // Temporäres Überschreiben der Daedalus-Konstante (NPC_MINIMAL_DAMAGE)
        zCPar_Symbol* sym = parser->GetSymbol("NPC_MINIMAL_DAMAGE");
        int oldMinDamage = 5;
        if (sym) {
            oldMinDamage = sym->single_intdata;
            sym->single_intdata = targetMinDamage;
        }

        // Originale Schadensberechnung aufrufen
        Hook_Union_MinDamage_OnDamage_Hit(_this, vtable, desc);

        // Konstante sofort wiederherstellen
        if (sym) {
            sym->single_intdata = oldMinDamage;
        }
    }
}