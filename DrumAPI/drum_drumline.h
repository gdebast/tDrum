#ifndef DRUMLINE_H
#define DRUMLINE_H

namespace Drum {

    class DrumTabPart;

    /*
     * a drumLine is the horizontal line linking multiple drum kits
     * it is either a Croche or a DoubleCroche, with a start position and end position
     * The line is either linking floor drum kits or non-floor drum kits
     */

    class DrumLine
    {

    public:
    enum DrumLineType
    {
        Croche,DoubleCroche,None
    };

    friend DrumTabPart;

    private:
        DrumLine(); //empty drum line
        DrumLine(unsigned startPosition, unsigned endPosition, DrumLineType drumLineType, bool isFloor);
    public:
        DrumLine(const DrumLine& other);

    public:
        // getter
        unsigned getStartPosition() const;
        unsigned getEndPosition() const;
        DrumLineType getDrumLineType() const;
        bool isFloor() const;
        bool isEmpty() const;

    private:
        // setter
        void setStartPosition(int position);
        void setEndPosition(int position);
        void setDrumLineType(DrumLineType type);
        void setIsFloor(bool isFloor);
        void empty();

        // operator
        DrumLine& operator=(const DrumLine& other);



    private:
        unsigned     m_startPosition{0};
        unsigned     m_endPosition{0};
        DrumLineType m_drumLineType{DrumLineType::None};
        bool         m_isFloor{false};

    };
}

#endif // DRUMLINE_H
