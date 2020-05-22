#pragma once

#include <memory>
#include <vector>
#include <string>

namespace Drum {


    class DrumTabPart;

    /*
     * class responsible for creating, saving, and importing
     * the standalone DrumTabParts
     */

    class DrumTabPartFactory
    {
    public:
        explicit DrumTabPartFactory();
        DrumTabPartFactory(const DrumTabPartFactory&) = delete;
        DrumTabPartFactory(DrumTabPartFactory&&) = delete;
        DrumTabPartFactory& operator=(const DrumTabPartFactory&) = delete;
        DrumTabPartFactory& operator=(DrumTabPartFactory&&) = delete;
        ~DrumTabPartFactory();

        DrumTabPart* loadObject(const std::string& file);
        void dumpToFile() const;

    private:
        std::vector<std::unique_ptr<DrumTabPart>> m_AllCreatedDrumTabPart;


    };
}

