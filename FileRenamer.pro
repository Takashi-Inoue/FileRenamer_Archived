QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AbstractSettings.cpp \
    Path/EntityName.cpp \
    Path/ParentDir.cpp \
    Path/PathRoot.cpp \
    PathsAnalyzer.cpp \
    SearchInDirs.cpp \
    Settings/SearchSettings.cpp \
    StringBuilder/BuilderChain.cpp \
    StringBuilder/File/BuilderChainOnFile.cpp \
    StringBuilder/File/MD5Hash.cpp \
    StringBuilder/File/OriginalName.cpp \
    StringBuilder/InsertString.cpp \
    StringBuilder/Number.cpp \
    StringBuilder/RegExpReplace.cpp \
    StringBuilder/ReplaceString.cpp \
    main.cpp \
    MainWindow.cpp \
    widgets/DialogDroppedDir.cpp \
    widgets/FormFormatSettings.cpp \
    widgets/FormTextFormatSetting.cpp \
    widgets/WidgetInsertTextSetting.cpp \
    widgets/WidgetMD5Setting.cpp \
    widgets/WidgetNumberSetting.cpp \
    widgets/WidgetOnlyPositionFixer.cpp \
    widgets/WidgetOriginalNameSetting.cpp \
    widgets/WidgetPositionFixer.cpp \
    widgets/WidgetReplaceSetting.cpp

HEADERS += \
    AbstractSettings.h \
    MainWindow.h \
    Path/EntityName.h \
    Path/ParentDir.h \
    Path/PathRoot.h \
    PathsAnalyzer.h \
    QStringVector.h \
    SearchInDirs.h \
    Settings/SearchSettings.h \
    StringBuilder/AbstractInsertString.h \
    StringBuilder/AbstractStringBuilder.h \
    StringBuilder/BuilderChain.h \
    StringBuilder/File/AbstractNeedFileInfo.h \
    StringBuilder/File/BuilderChainOnFile.h \
    StringBuilder/File/MD5Hash.h \
    StringBuilder/File/OriginalName.h \
    StringBuilder/InsertString.h \
    StringBuilder/Number.h \
    StringBuilder/RegExpReplace.h \
    StringBuilder/ReplaceString.h \
    widgets/AbstractStringBuilderWidget.h \
    widgets/DialogDroppedDir.h \
    widgets/FormFormatSettings.h \
    widgets/FormTextFormatSetting.h \
    widgets/WidgetInsertTextSetting.h \
    widgets/WidgetMD5Setting.h \
    widgets/WidgetNumberSetting.h \
    widgets/WidgetOnlyPositionFixer.h \
    widgets/WidgetOriginalNameSetting.h \
    widgets/WidgetPositionFixer.h \
    widgets/WidgetReplaceSetting.h

FORMS += \
    MainWindow.ui \
    widgets/DialogDroppedDir.ui \
    widgets/FormFormatSettings.ui \
    widgets/FormTextFormatSetting.ui \
    widgets/WidgetInsertTextSetting.ui \
    widgets/WidgetNumberSetting.ui \
    widgets/WidgetOnlyPositionFixer.ui \
    widgets/WidgetPositionFixer.ui \
    widgets/WidgetReplaceSetting.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
