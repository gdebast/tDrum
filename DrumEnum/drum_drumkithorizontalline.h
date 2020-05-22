#pragma once

namespace Drum {

    /*
     * class representing an horizontal line where we can put a drumKit
     *  map of the lines:
     *
     *                   Crash
     *  ---------------- HiHat
     *  ---------------- HighTom
     *  ---------------- SnareTom
     *  - - - - - - - -  LowTom -> semi line
     *  ---------------- FloorTom
     *  ---------------- BassDrum
     *                   HiHatFloorLine
     */


    enum class DrumKitHorizontalLine
    {
        CrashLine,
        HiHatLine,
        HighTomLine,
        SnareTomLine,
        LowTomLine,
        FloorTomLine,
        BassDrumLine,
        HiHatFloorLine,
        None
    };

}

