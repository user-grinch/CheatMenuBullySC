/// <reference path="../../.config/bully.d.ts" />

import locationList from "../data/locations.json"

class CTeleport {
    constructor() {
        if (CTeleport.self) {
            throw new Error("Singleton classes can't be instantiated more than once.")
        }
        CTeleport.self = this;
    }
    
    SetPlayerPos (text) {
        let coord = text.trim().split(",");
        Streaming.SetAreaVisible(coord[0]);
        Player.SetCoordinates(coord[1], coord[2], coord[3]);
    }

    ShowPage() {
        let tab = ImGui.Tabs("TpTab", "Teleport,Location");
        ImGui.Spacing();

        if (tab == 0) {
            let sz = ImGui.GetScalingSize("B2SzTp", 2, false);
            let coord = ImGui.InputText("Coordiantes");
            ImGui.Spacing();
            if (ImGui.Button("Teleport to coordinates", sz.x, sz.y)) {
                CTeleport.self.SetPlayerPos(coord);
            }
            ImGui.SameLine();
            if (ImGui.Button("Teleport to center", sz.x, sz.y)) {
                CTeleport.self.SetPlayerPos("0, 0, 0, 3");
            }
        }
        
        if (tab == 1) {
            ImGui.BeginChild("LocTP");
            
            let search = ImGui.InputText("Search");
            ImGui.Spacing();
            for (const name in locationList) {
                if (name.toLowerCase().includes(search.toLowerCase())) {
                    if (ImGui.Selectable(name, false)) {
                        CTeleport.self.SetPlayerPos(locationList[name]);
                    }
                }
            }
            ImGui.EndChild();
        }
       
    }
}

export var Teleport = new CTeleport();