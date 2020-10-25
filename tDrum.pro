QT       += core gui widgets printsupport

RESOURCES += Icons/icons.qrc

CONFIG += c++17


INCLUDEPATH += D:/Development/Boost
LIBS += -LD:/Development/Boost/stage/lib libboost_filesystem-mgw8-mt-d-x64-1_73

HEADERS += \
    DrumAPI/drum_drumkithorizontallinehelper.h \
    DrumAPI/drum_drumtabimpliciter.h \
    DrumAPI/drum_drumtabpdfprinterconfig.h \
    DrumAPI/drum_drumtabpdfprinterconfigfactory.h \
    DrumAPI/drum_idrumfactory.h \
    DrumEnum/drum_drumkithorizontalline.h \
    DrumEnum/drum_drumkit.h \
    DrumAPI/drum_drumline.h \
    DrumAPI/drum_drumtab.h \
    DrumAPI/drum_drumtabfactory.h \
    DrumAPI/drum_drumtabpart.h \
    DrumAPI/drum_drumtabpartfactory.h \
    DrumUiTools/drumui_drumtabpartdrawer.h \
    DrumUiTools/drumui_drumtabpartdrawerhelper.h \
    DrumUiTools/drumui_drumtabpdfcreator.h \
    DrumUiTools/drumui_pdfpagemanager.h \
    DrumWidget/drumui_drummaintoolbar.h \
    DrumWidget/drumui_drumtablistwidget.h \
    DrumWidget/drumui_drumtabpartcreatorwidget.h \
    DrumWidget/drumui_drumtabpartdisplaywidget.h \
    DrumUiTools/drumui_drumtabpartgeometry.h \
    DrumWidget/drumui_drumtabpartwidgetbase.h \
    DrumWidget/drumui_drumtabpdfprinterconfigview.h \
    DrumWidget/drumui_drumtabpdfprinterconfigviewmodel.h \
    DrumWidget/drumui_drumtabwidget.h \
    DrumWidget/drumui_drummainwindow.h \
    Tools/tools_directorymanager.h \
    Tools/tools_file.h \
    Tools/tools_serializerhelper.h \
    Tools/tools_serializerhelper_ext.h \
    Tools/tools_stringfunction.h \
    Tools/tools_stringfunction_ext.h \
    Tools/tools_directory.h \
    Tools/tools_threadsafedata.h \
    Tools/tools_exception.h \
    UI/ui_rightleftpushbutton.h \
    UI/ui_directorylineedit.h

SOURCES += \
    DrumAPI/drum_drumkithorizontallinehelper.cpp \
    DrumAPI/drum_drumline.cpp \
    DrumAPI/drum_drumtab.cpp \
    DrumAPI/drum_drumtabfactory.cpp \
    DrumAPI/drum_drumtabimpliciter.cpp \
    DrumAPI/drum_drumtabpart.cpp \
    DrumAPI/drum_drumtabpartfactory.cpp \
    DrumAPI/drum_drumtabpdfprinterconfig.cpp \
    DrumAPI/drum_drumtabpdfprinterconfigfactory.cpp \
    DrumAPI/drum_idrumfactory.cpp \
    DrumUiTools/drumui_drumtabpartdrawer.cpp \
    DrumUiTools/drumui_drumtabpartdrawerhelper.cpp \
    DrumUiTools/drumui_drumtabpdfcreator.cpp \
    DrumUiTools/drumui_pdfpagemanager.cpp \
    DrumWidget/drumui_drummaintoolbar.cpp \
    DrumWidget/drumui_drumtablistwidget.cpp \
    DrumWidget/drumui_drumtabpartcreatorwidget.cpp \
    DrumWidget/drumui_drumtabpartdisplaywidget.cpp \
    DrumUiTools/drumui_drumtabpartgeometry.cpp \
    DrumWidget/drumui_drumtabpartwidgetbase.cpp \
    DrumWidget/drumui_drumtabpdfprinterconfigview.cpp \
    DrumWidget/drumui_drumtabpdfprinterconfigviewmodel.cpp \
    DrumWidget/drumui_drumtabwidget.cpp \
    DrumWidget/drumui_drummainwindow.cpp \
    Tools/tools_directorymanager.cpp \
    Tools/tools_file.cpp \
    Tools/tools_serializerhelper.cpp \
    Tools/tools_stringfunction.cpp \
    Tools/tools_directory.cpp \
    UI/ui_rightleftpushbutton.cpp \
    UI/ui_directorylineedit.cpp \
    main.cpp
