/// <reference path="../../.config/bully.d.ts" />

import { ImGuiCond } from "../../.config/enums";
import { BUILD_NUMBER, MENU_VERSION } from "./defines";

const OVERLAY_TOP_LEFT = 0;
const OVERLAY_TOP_RIGHT = 1;
const OVERLAY_BOTTOM_LEFT = 2;
const OVERLAY_BOTTOM_RIGHT = 3;

class CMenu {
    constructor() {
        if (CMenu.self) {
            throw new Error("Singleton classes can't be instantiated more than once.")
        }
        CMenu.self = this;
        this.selectedPos = OVERLAY_BOTTOM_RIGHT;
        this.offset = 10;
        this.windowSize = [0, 0];
        this.showCoord = false;
        this.showFPS = false;
    }

    CalcPosition() {
        let displaySize = ImGui.GetDisplaySize();
        if (this.selectedPos == OVERLAY_TOP_LEFT) {
          return [this.offset, this.offset];
        }
      
        if (this.selectedPos == OVERLAY_TOP_RIGHT) {
          return [displaySize.width - this.offset - this.windowSize[0], this.offset];
        }
      
        if (this.selectedPos == OVERLAY_BOTTOM_LEFT) {
          return [this.offset, displaySize.height - this.offset - this.windowSize[1]];
        }
      
        if (this.selectedPos == OVERLAY_BOTTOM_RIGHT) {
          return [
            displaySize.width - this.offset - this.windowSize[0],
            displaySize.height - this.offset - this.windowSize[1],
          ];
        }
    }

    DrawOverlay() {
        if (!(this.showCoord || this.showFPS)) {
            return;
        }

        let pos = this.CalcPosition();

        // ImGui Window
        ImGui.SetNextWindowPos(pos[0], pos[1], ImGuiCond.Always);
        ImGui.SetNextWindowTransparency(CMenu.self.noBG ? 0.0 : 0.5);

        ImGui.Begin("Overlay", true, true, true, true, true);
        let sz = ImGui.GetWindowSize("WinodwSXZ");
        this.windowSize = [sz.width, sz.height];
        
        if (CMenu.self.showCoord) {
            let coord = Player.GetCoordinates();
            ImGui.Text(`Coord: ${coord.x.toFixed(0)}, ${coord.y.toFixed(0)}, ${coord.z.toFixed(0)}`);
        }

        if (CMenu.self.showFPS) {
            ImGui.Text("Frames: " + Game.GetFramerate());
        }
        ImGui.End();
    }

    ShowPage() {
        let tab = ImGui.Tabs("MyTab", "Overlay,About");
        ImGui.Spacing();
        ImGui.BeginChild("MyTab");
        
        if (tab == 0)
        {
            ImGui.Spacing();
            ImGui.SameLine();
            CMenu.self.selectedPos = ImGui.ComboBox("Position", "Top left,Top right,Bottom left, Bottom right", CMenu.self.selectedPos)
            ImGui.Spacing();
            ImGui.Columns(2);
            CMenu.self.noBG = ImGui.Checkbox("No background", CMenu.self.noBG);
            CMenu.self.showCoord = ImGui.Checkbox("Show coordinates", CMenu.self.showCoord);
            ImGui.NextColumn();
            CMenu.self.showFPS = ImGui.Checkbox("Show framerate", CMenu.self.showFPS);
            ImGui.Columns(1);
        }

        if (tab == 1)
        {
            let sz = ImGui.GetScalingSize("B2sz", 2, true);
        
            if (ImGui.Button("Discord server", sz.x, sz.y)) {
                //ShellExecute(nullptr, "open", DISCORD_INVITE, nullptr, nullptr, SW_SHOWNORMAL);
            }
            ImGui.SameLine();
            if (ImGui.Button("GitHub", sz.x, sz.y)) {
                //ShellExecute(nullptr, "open", GITHUB_LINK, nullptr, nullptr, SW_SHOWNORMAL);
            }
            ImGui.Spacing();
            ImGui.Columns(2);
            ImGui.Text("Author: Grinch_");
            ImGui.Text(`Version: ${MENU_VERSION}`);
            ImGui.NextColumn();
            ImGui.Text(`ImGui: ${ImGui.GetVersion()}`);
            ImGui.Text(`Build number: ${BUILD_NUMBER}`);
            ImGui.Columns(1);

            ImGui.Dummy(0, 10);
            ImGui.TextWrapped("If you find bugs or have suggestions, let me know on discord.");
            ImGui.Dummy(0, 10);
            ImGui.TextCentered("Copyright Grinch_ 2022. All rights reserved.");
        }
        ImGui.EndChild();
    }
}

export var Menu = new CMenu();