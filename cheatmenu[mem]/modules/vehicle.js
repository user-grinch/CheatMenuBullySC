/// <reference path="../../.config/bully.d.ts" />

import vehicleList from "../data/vehicles.json"

class CVehicle {

    constructor() {
        if (CVehicle.self) {
            throw new Error("Singleton classes can't be instantiated more than once.")
        }
        CVehicle.self = this;
    }

    ShowPage() {
        let sz = ImGui.GetScalingSize("B2Sz", 1, false);
        if (ImGui.Button("Clear vehicles", sz.x, sz.y)) {
            World.ClearAllVehicles();
        }
        ImGui.Spacing();
        let tab = ImGui.Tabs("VehTab", "Spawn");
        ImGui.Spacing();

        if (tab == 0) {
            let text = ImGui.InputText("Search");
            ImGui.BeginChild("Child232");
            
            ImGui.Spacing();
            for (let key in vehicleList) {
                if (key.toLowerCase().includes(text.toLowerCase())) {
                    if (ImGui.Selectable(key, false)) {
                        let coord = Player.GetCoordinates();
                        let model = vehicleList[key];
                        if (Streaming.RequestVehicleModel(model)) {
                            let car = Car.Create(model, coord.x, coord.y, coord.z);
                            coord.z += 1.0;
                            Player.SetCoordinates(coord.x, coord.y, coord.z);
                            Streaming.ModelNotNeeded(model);
                        }
                    }
                }
            }
            ImGui.EndChild()
        }
    }
}

export var Vehicle = new CVehicle();