#pragma once

#include <vector>
#include <map>
#include <string>

namespace Drum {

    class DrumTabPart;

    /*
     * class representing a Drum tablature : a sequence of DrumTab parts
     * the tablature has a dedicated file where it is stored
     */

    class DrumTab
    {
    public:
        // constructor, destructor
        explicit DrumTab();
        DrumTab(const DrumTab&) = delete;
        DrumTab(DrumTab&&) = delete;
        DrumTab& operator=(const DrumTab&) = delete;
        DrumTab& operator=(DrumTab&&) = delete;
        ~DrumTab();

        // function to interact with the DrumTab parts
        DrumTabPart* addDrumTabPart(unsigned index);
        void removeDrumTabPart(unsigned drumTabPartNumber);
        void setImplicitExplicit(const DrumTabPart* part,
                                 bool implicitExplicit /*implicit=true, Explicit=false*/);

        std::vector<std::pair<DrumTabPart*,bool>> getDrumTabParts() const;
        unsigned getDrumTabSize() const;

        // serialization functions
        std::string getSerialized() const;
        void fillFromSerialized(const std::string &serializedString);


        std::string getAuthor() const {return m_author;}
        std::string getTitle() const {return m_title;}
        void setAuthor(const std::string author) {m_author = author;}
        void setTitle(const std::string title) {m_title = title;}

    private:
        std::map<unsigned,std::pair<DrumTabPart*,bool>> m_drumTabParts{};

        std::string  m_author;
        std::string  m_title;

    };

}
