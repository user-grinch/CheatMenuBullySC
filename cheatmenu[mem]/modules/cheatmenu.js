import { ImGuiCond, KeyCode } from "../../.config/enums";

/*
    The Core class of the menu
    Handles window drawing & page selection
*/
import { Teleport } from "./teleport"
import { Ped } from "./character"
import { Game } from "./game"
import { Menu } from "./menu"
import { Vehicle } from "./vehicle"
import { Weapon } from "./weapon"
import { MENU_TITLE } from "./defines";

class CCheatMenu {
    isOpen = false;
    _pageList = {"Teleport" : Teleport.ShowPage, "Character" : Ped.ShowPage, 
                "Game" : Game.ShowPage, "Vehicle" : Vehicle.ShowPage, 
                "Weapon" : Weapon.ShowPage, "Menu" : Menu.ShowPage}
    _pageFunc = 0
    constructor() {
        if (CCheatMenu.self) {
            throw new Error("Singleton classes can't be instantiated more than once.")
        }
        CCheatMenu.self = this;
    }

    #ShowWelcomePage() {
        ImGui.NewLine();

        ImGui.TextCentered("Welcome to Cheat Menu");
        ImGui.TextCentered("Author: Grinch_");

        ImGui.NewLine();
        ImGui.TextWrapped("Please ensure you have the latest version from GitHub");
        ImGui.NewLine();
        let sz = ImGui.GetScalingSize("B2sz", 2, true);
        
        if (ImGui.Button("Discord server", sz.x, sz.y)) {
            //ShellExecute(nullptr, "open", DISCORD_INVITE, nullptr, nullptr, SW_SHOWNORMAL);
        }
        ImGui.SameLine();
        if (ImGui.Button("GitHub", sz.x, sz.y)) {
            //ShellExecute(nullptr, "open", GITHUB_LINK, nullptr, nullptr, SW_SHOWNORMAL);
        }
        ImGui.NewLine();
        ImGui.TextWrapped("If you find bugs or have suggestions, let me know on discord.");
        ImGui.Dummy(0, 30);
        ImGui.TextCentered("Copyright Grinch_ 2022. All rights reserved.");
    }

    #ProcessPages() {
        let hBtnSz = ImGui.GetScalingSize("HeaderSz", 3, false);
        let count = 0
        
        ImGui.PushStyleVar2(14, 0, 1); // ImGuiStyleVar_ItemSpacing

        for (const key in this._pageList) {
            let selected = (CCheatMenu.self._pageFunc == this._pageList[key])

            if (selected) {
                ImGui.PushStyleColor(21, 15, 135, 250, 255) // ImGuiCol_Button
            }

            if (ImGui.Button(key, hBtnSz.x, hBtnSz.y)) {
                CCheatMenu.self._pageFunc = this._pageList[key];
            }

            if (selected) {
                ImGui.PopStyleColor(1);
            }

            if (count % 3 != 2) {
                ImGui.SameLine();
            }
            count++;
        }
        ImGui.PopStyleVar(1);

        ImGui.Dummy(0, 10);
        ImGui.BeginChild("PgeChild");
        if (this._pageFunc) {
            this._pageFunc();
        } else {
            this.#ShowWelcomePage();
        }
        ImGui.EndChild();
    }

    #DrawFrame() {
        ImGui.BeginFrame("GRINCH_MENU");
  
        if (this.isOpen) {
            let size = ImGui.GetDisplaySize();
            let width = size.width/4.0;
            let height = size.height/1.2;
            ImGui.SetNextWindowSize(width, height, ImGuiCond.Always);
            this.isOpen = ImGui.Begin(MENU_TITLE, this.isOpen, false, false, false, false);
            ImGui.SetCursorVisible(true);
            
            this.#ProcessPages();

            ImGui.End();
        }
        
        Menu.DrawOverlay();
        ImGui.EndFrame();
    }

    Process() {
        this.#DrawFrame();

        if (Pad.IsKeyPressed(KeyCode.Ctrl) && Pad.IsKeyPressed(KeyCode.M)) {
            while (Pad.IsKeyPressed(KeyCode.Ctrl) && Pad.IsKeyPressed(KeyCode.M)) {
                wait(0);
            }
            this.isOpen = !this.isOpen;
        }
    }
}

export var CheatMenu = new CCheatMenu();