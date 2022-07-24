/// <reference path="../.config/bully.d.ts" />
/*
    Basic game overlay

    Required,
        CLEO4
        CLEORedux 1.1-dev
            ImGuiRedux plugin
            IniFile plugin

    Author: Grinch_
*/

HOST === "bully" || exit("This script is only for BullySE");
import { CheatMenu } from "./modules/cheatmenu"

while (true) {
    wait(0);
    CheatMenu.Process();
}