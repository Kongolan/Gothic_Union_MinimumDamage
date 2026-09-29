// 1. Zwingend erforderlich: Der Gothic API mitteilen, welche Engine wir kompilieren
#define GOTHIC_NAMESPACE Gothic_II_Addon

// 2. Offizieller Header der Gothic API (Beinhaltet zoptions, oCNpc, etc.)
#include <ZenGin/zGothicAPI.h>

// 3. Offizielles Modul der Union API für das Hook-System (Existiert nachweislich im Union-Ordner)
#include <Union/Hook.h>

// 4. Wir öffnen den exakten Namensraum, den die Gothic API durch unser #define generiert hat
namespace Gothic_II_Addon {

    // Eindeutiger Name für die Hook-Variable und die neue Funktion
    HOOK Hook_Union_MinDamage_OnDamage_Hit PATCH(&oCNpc::OnDamage_Hit, &Union_MinDamage_OnDamage_Hit);

    void __fastcall Union_MinDamage_OnDamage_Hit(oCNpc* _this, void* vtable, oSDamageDescriptor& desc) {
        // Die INI-Sektion "UNION_MINIMUM_DAMAGE" auslesen
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

        // Temporäres Überschreiben der Daedalus-Konstante
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