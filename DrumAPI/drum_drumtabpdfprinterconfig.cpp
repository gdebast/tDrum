#include "DrumAPI/drum_drumtabpdfprinterconfig.h"

#include <Tools/tools_serializerhelper_ext.h>

namespace
{
    static const std::string EXPLICITDRUMTABPRINTING  = "ExplicitDrumTabPrinting";
    static const std::string PDFEXPORTDIRECTORY = "PdfExportDirectory";
    static const std::string NUMBEROFDRUMTABPARTSPERROW = "NumberOfDrumTabPartsPerRow";
}

using namespace Drum;

DrumTabPdfPrinterConfig::DrumTabPdfPrinterConfig()
{
}

void DrumTabPdfPrinterConfig::fillFromSerialized(const std::string &serializedString)
{
    Tools::SerializerHelper helper(serializedString);
    helper.deserialize(EXPLICITDRUMTABPRINTING,m_explicitDrumTabPrinting);
    helper.deserialize(PDFEXPORTDIRECTORY,m_pdfExportDirectory);
    helper.deserialize(NUMBEROFDRUMTABPARTSPERROW,m_numberOfDrumTabPartsPerRow);
}

std::string DrumTabPdfPrinterConfig::getSerialized() const
{
    // add the settings
    Tools::SerializerHelper helper;
    helper.addToSerializeString(EXPLICITDRUMTABPRINTING,m_explicitDrumTabPrinting);
    helper.addToSerializeString(PDFEXPORTDIRECTORY,m_pdfExportDirectory);
    helper.addToSerializeString(NUMBEROFDRUMTABPARTSPERROW,m_numberOfDrumTabPartsPerRow);

    return helper.getSerializedString();
}
