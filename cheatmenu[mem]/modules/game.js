/// <reference path="../../.config/bully.d.ts" />

class CGame {
    constructor() {
        if (CGame.self) {
            throw new Error("Singleton classes can't be instantiated more than once.")
        }
        CGame.self = this;
    }

    ShowPage() {
        let tab = ImGui.Tabs("TpTab", "Checkbox,Menu");
        ImGui.Spacing();

        if (tab == 0) {
            ImGui.Columns(2);
            this.freezeClock = ImGui.Checkbox("Freeze clock", this.freezeClock);
            if (ImGui.IsItemActive("ClockFrzAct")) {
                if (this.freezeClock){
                    Clock.Pause();
                } else {
                    Clock.Unpause();
                }
            }
            this.hideHUD = ImGui.Checkbox("Hide hud", this.hideHUD);
            if (ImGui.IsItemActive("hideHudFoc")) {
                if (this.hideHUD){
                    Memory.WriteU16(0x53CFEC, 0xD2FF, true);
                } else {
                    Memory.WriteU16(0x53CFEC, 0x9090, true);
                }
            }
            ImGui.NextColumn();
            this.wideScreen = ImGui.Checkbox("Widescreen", this.wideScreen);
            if (ImGui.IsItemActive("ClockFrzAct")) {
                Camera.SetWidescreen(this.wideScreen)
            }
            ImGui.Columns(1);
        }

        if (tab == 1) {
            if (ImGui.CollapsingHeader("Field of view")) {
                this.fov = ImGui.InputInt("FOV", this.fov, 10, 120);
                if (ImGui.IsItemActive("fovFoc")) {
                    Camera.SetFov(this.fov)
                }
                ImGui.Spacing();
                ImGui.Separator();
            }

            if (ImGui.CollapsingHeader("Time")) {
                this.hour = ImGui.InputInt("Hour", this.hour, 0, 23);
                if (ImGui.IsItemActive("HourFo")) {
                    Clock.SetTimeOfDay(this.hour, this.minute);
                }
                this.minute = ImGui.InputInt("Minute", this.minute, 0, 59);
                if (ImGui.IsItemActive("HourFo")) {
                    Clock.SetTimeOfDay(this.hour, this.minute);
                }
                ImGui.Spacing();
                ImGui.Separator();
            }
        }
    }
}

export var Game = new CGame();