#ifndef DRUM_DRUMTABFACTORY_H
#define DRUM_DRUMTABFACTORY_H

#include <memory>
#include <vector>
#include <string>


namespace Drum {

    class DrumTab;

    /*
     * class responsible for creating, saving, and importing the DrumTab
     */

    class DrumTabFactory
    {
    public:
        explicit DrumTabFactory();
        ~DrumTabFactory();
        DrumTabFactory(const DrumTabFactory&) = delete;
        DrumTabFactory(DrumTabFactory&&) = delete;
        DrumTabFactory& operator=(const DrumTabFactory&) = delete;
        DrumTabFactory& operator=(DrumTabFactory&&) = delete;

        DrumTab* loadObject(const std::string& file);
        void dumpToFile() const;

    private:
        std::vector<std::pair<std::unique_ptr<DrumTab>,std::string>> m_AllCreatedDrumTab;

    };
}

#endif // DRUM_DRUMTABFACTORY_H
