#include "DrumAPI/drum_drumline.h"

using namespace Drum;

// ============================
// == Constructor/Destructor ==
// ============================

DrumLine::DrumLine()
{
}

DrumLine::DrumLine(unsigned startPosition, unsigned endPosition, DrumLineType drumLineType, bool isFloor)
    : m_startPosition(startPosition),
      m_endPosition(endPosition),
      m_drumLineType(drumLineType),
      m_isFloor(isFloor)
{}

DrumLine::DrumLine(const DrumLine& other)
    : m_startPosition(other.m_startPosition),
      m_endPosition(other.m_endPosition),
      m_drumLineType(other.m_drumLineType),
      m_isFloor(other.m_isFloor)
{}

// ======================
// == public functions ==
// ======================

// getters
// -------
unsigned DrumLine::getEndPosition() const
{
    return m_endPosition;
}

unsigned DrumLine::getStartPosition() const
{
    return m_startPosition;
}

DrumLine::DrumLineType DrumLine::getDrumLineType() const
{
    return m_drumLineType;
}

bool DrumLine::isFloor() const
{
    return m_isFloor;
}

bool DrumLine::isEmpty() const
{
    DrumLine emptyDrumLine;
    return emptyDrumLine.m_endPosition   == m_endPosition   ||
           emptyDrumLine.m_startPosition == m_startPosition ||
           emptyDrumLine.m_drumLineType  == m_drumLineType;

}

// =======================
// == private functions ==
// =======================

// setters
// -------
void DrumLine::setStartPosition(int position)
{
    m_startPosition = position;
}

void DrumLine::setEndPosition(int position)
{
    m_endPosition = position;
}

void DrumLine::setDrumLineType(DrumLine::DrumLineType type)
{
    m_drumLineType = type;
}

void DrumLine::setIsFloor(bool isFloor)
{
    m_isFloor = isFloor;
}

void DrumLine::empty()
{
    DrumLine emptyDrumLine;
    this->operator=(emptyDrumLine);
}

DrumLine &DrumLine::operator=(const DrumLine &other)
{
    m_startPosition = other.m_startPosition;
    m_endPosition = other.m_endPosition;
    m_drumLineType = other.m_drumLineType;
    m_isFloor = other.m_isFloor;

    return *this;
}


