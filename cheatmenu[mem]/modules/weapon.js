/// <reference path="../../.config/bully.d.ts" />

class CWeapon {
    constructor() {
        if (CWeapon.self) {
            throw new Error("Singleton classes can't be instantiated more than once.")
        }
        CWeapon.self = this;
    }
    
    ShowPage() {
        let tab = ImGui.Tabs("weapnTab", "Checkbox");
        ImGui.Spacing();

        if (tab == 0) {
            ImGui.Columns(2);
            this.unlimitedAmmo = ImGui.Checkbox("Unlimited Ammo", this.unlimitedAmmo);
            if (ImGui.IsItemActive("Ammounlimite")) {
                if (this.unlimitedAmmo){
                    Memory.WriteU32(0x45CA3A, 0x90909090, true);
                    Memory.WriteU32(0x45CA3E, 0x90909090, true);
                    Memory.WriteU8(0x45CA42, 0x90, true);
                } else {
                    Memory.WriteU32(0x45CA3A, 0x0F068966, true);
                    Memory.WriteU32(0x45CA3E, 0x00008384, true);
                    Memory.WriteU8(0x45CA42, 0x00, true);
                }
            }
            ImGui.NextColumn();
         
            ImGui.Columns(1);
        }
    }
}

export var Weapon = new CWeapon();