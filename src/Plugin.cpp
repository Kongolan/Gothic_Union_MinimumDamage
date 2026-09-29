#include "UnionAfx.h"

namespace GOTHIC_ENGINE {

    HOOK Hook_oCNpc_OnDamage_Hit PATCH(&oCNpc::OnDamage_Hit, &oCNpc_OnDamage_Hit_New);

    void __fastcall oCNpc_OnDamage_Hit_New(oCNpc* _this, void* vtable, oSDamageDescriptor& desc) {
        int settingValue = zoptions->ReadInt("ZMODMINDAMAGE", "MinDamageValue", 0);
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

        Hook_oCNpc_OnDamage_Hit(_this, vtable, desc);

        if (sym) {
            sym->single_intdata = oldMinDamage;
        }
    }
}
