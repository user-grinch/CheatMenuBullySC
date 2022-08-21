/// <reference path="../../.config/bully.d.ts" />

class CChar {
    playerControl = true;
    playerInvulnerable = false;

    constructor() {
        if (CChar.self) {
            throw new Error("Singleton classes can't be instantiated more than once.")
        }
        CChar.self = this;
    }

    ShowPage() {
        let sz = ImGui.GetScalingSize("B2Sz", 1, false);
        
        if (ImGui.Button("Clear peds", sz.x, sz.y)) {
            World.ClearAllPeds();
        }
        ImGui.Spacing();

        let tab = ImGui.Tabs("TpTab", "Checkbox,Menu");
        ImGui.Spacing();

        if (tab == 0) {
            ImGui.Columns(2);
            this.playerControl = ImGui.Checkbox("Player control", this.playerControl);
            if (ImGui.IsItemActive("PlayFoc")) {
                Player.SetControllable(!this.playerControl);
            }
            ImGui.NextColumn();
            this.playerInvulnerable = ImGui.Checkbox("Player invulnerable", this.playerControl);
            if (ImGui.IsItemActive("PlayInvul")) {
                Player.SetInvulnerable(!this.playerInvulnerable);
            }
            ImGui.Columns(1);
        }
        
        if (tab == 1) {
            if (ImGui.CollapsingHeader("Money##Header")) {
                let money = ImGui.InputInt("Money", Player.GetMoney(), -9999999, 9999999);
                if (ImGui.IsItemActive("MoneyFocus")) {
                    Player.SetMoney(money);
                }
                ImGui.Spacing();
                ImGui.Separator();
            }

            if (ImGui.CollapsingHeader("Health##Header")) {
                let health = ImGui.InputInt("Health", Player.GetHealth(), 0, 400);
                if (ImGui.IsItemActive("HltFocus")) {
                    Player.SetHealth(health);
                }
                ImGui.Spacing();
                ImGui.Separator();
            }
        }
    }
}

export var Ped = new CChar();