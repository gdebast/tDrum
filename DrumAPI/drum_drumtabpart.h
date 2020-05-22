#pragma once

#include "DrumEnum/drum_drumkit.h"
#include "DrumEnum/drum_drumkithorizontalline.h"
#include "DrumAPI/drum_drumline.h"

#include "vector"
#include "utility"
#include "string"

namespace Tools
{
    class SerializerHelper;
}


namespace Drum {

    /*
     * a DrumtabPart is a sequence of hits on a drum kit.
     * the hit has a sequence number : a number between 1 and m_drumTime
     */

    class DrumTabPart
    {
    public:

        // general def
        typedef std::vector<std::pair<unsigned,DrumKit>>  VecPair_DrumSetPartPosition;

        // constructor/destructor
        explicit DrumTabPart();
        explicit DrumTabPart(const std::string& file);
        explicit DrumTabPart(const DrumTabPart& other);
        ~DrumTabPart();

        // method
        // ------
        int getDrumTime() const;
        std::string getSerialized() const;
        std::string getSaveFileLocation() const;
        // return the drumKits at a position, at a line. it can be Silence
        DrumKit getDrumKit(DrumKitHorizontalLine line, unsigned position) const;


        void addDrumKit(DrumKit drumKit, unsigned position);

        void clear();
        void setDrumTime(int drumtime);
        VecPair_DrumSetPartPosition getDrumKits() { sortDrumKits();  return m_drumKitPositions;}
        VecPair_DrumSetPartPosition getDrumKits() const {return m_drumKitPositions;}
        std::vector<DrumLine*> getDrumLines();

        // remove all drumKits at a position, at a line
        void clearLineAtPosition(DrumKitHorizontalLine line, unsigned position);

        void fillSerializer(Tools::SerializerHelper& helper) const;
        void fillFromSerialized(const Tools::SerializerHelper &helper);

        bool isEmpty() const {return m_drumKitPositions.empty();}

        // operators
        // ---------
        DrumTabPart& operator=(const DrumTabPart& other);
        bool operator==(const DrumTabPart& other) const;



    private:
        VecPair_DrumSetPartPosition m_drumKitPositions{};
        int                         m_drumTime{0}; // number of time in the drum

        std::vector<DrumLine*>      m_drumLines{};
        bool                        m_isDrumLinesUptoDate{false};

        std::string                 m_saveFileLocation{""};
        bool                        m_hasSaveFile{false};

        static const std::vector<std::pair<DrumKit,std::string>> s_stringDrumkitTranslation;

        // functions
        void sortDrumKits();
        DrumKit getDrumKit(const std::string& serializeddrumKit) const;
        std::string getSerializedDrumKit(DrumKit drumKit) const;
        bool isFloorDrumKit(DrumKit drumKit) const;
        void deleteDrumLines();
        void createDrumLines();
    };
}

