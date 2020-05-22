#pragma once


namespace Drum  {

    /*
     * class representing a Drum kit : a part of a drum set
     */

    enum class DrumKit{
        BassDrum,FloorTom,SnareTom,LowTom,HighTom,   // tom and drum
        HiHatClosed,HiHatOpened,Crash,Ride,RideBell,HitHatFloor, // cybal
        Silence                                      // none
    };

}
