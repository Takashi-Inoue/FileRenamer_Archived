QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AbstractSettings.cpp \
    Path/ParentDir.cpp \
    Path/PathEntity.cpp \
    Path/PathEntityInfo.cpp \
    Path/PathHeaderView.cpp \
    Path/PathModel.cpp \
    Path/PathRoot.cpp \
    Path/PathTableView.cpp \
    PathsAnalyzer.cpp \
    SearchInDirs.cpp \
    Settings/SearchSettings.cpp \
    StringBuilder/BuilderChain.cpp \
    StringBuilder/InsertString.cpp \
    StringBuilder/Number.cpp \
    StringBuilder/RegExpReplace.cpp \
    StringBuilder/ReplaceString.cpp \
    StringBuilderOnFile/BuilderChainOnFile.cpp \
    StringBuilderOnFile/CryptographicHash.cpp \
    StringBuilderOnFile/OriginalName.cpp \
    ThreadCreateNewNames.cpp \
    ThreadRename.cpp \
    main.cpp \
    MainWindow.cpp \
    widgets/DialogDroppedDir.cpp \
    widgets/FormStringBuilder.cpp \
    widgets/FormStringBuilderChain.cpp \
    widgets/WidgetFileHashSetting.cpp \
    widgets/WidgetInsertTextSetting.cpp \
    widgets/WidgetNumberSetting.cpp \
    widgets/WidgetOnlyPositionFixer.cpp \
    widgets/WidgetOriginalNameSetting.cpp \
    widgets/WidgetPositionFixer.cpp \
    widgets/WidgetReplaceSetting.cpp

HEADERS += \
    AbstractSettings.h \
    MainWindow.h \
    Path/ParentDir.h \
    Path/PathEntity.h \
    Path/PathEntityInfo.h \
    Path/PathHeaderView.h \
    Path/PathModel.h \
    Path/PathRoot.h \
    Path/PathTableView.h \
    PathsAnalyzer.h \
    SearchInDirs.h \
    Settings/SearchSettings.h \
    StringBuilder/AbstractInsertString.h \
    StringBuilder/AbstractStringBuilder.h \
    StringBuilder/BuilderChain.h \
    StringBuilder/InsertString.h \
    StringBuilder/Number.h \
    StringBuilder/RegExpReplace.h \
    StringBuilder/ReplaceString.h \
    StringBuilderOnFile/AbstractNeedFileInfo.h \
    StringBuilderOnFile/BuilderChainOnFile.h \
    StringBuilderOnFile/CryptographicHash.h \
    StringBuilderOnFile/IFileInfo.h \
    StringBuilderOnFile/OriginalName.h \
    ThreadCreateNewNames.h \
    ThreadRename.h \
    widgets/AbstractStringBuilderWidget.h \
    widgets/DialogDroppedDir.h \
    widgets/FormStringBuilder.h \
    widgets/FormStringBuilderChain.h \
    widgets/WidgetFileHashSetting.h \
    widgets/WidgetInsertTextSetting.h \
    widgets/WidgetNumberSetting.h \
    widgets/WidgetOnlyPositionFixer.h \
    widgets/WidgetOriginalNameSetting.h \
    widgets/WidgetPositionFixer.h \
    widgets/WidgetReplaceSetting.h

FORMS += \
    MainWindow.ui \
    widgets/DialogDroppedDir.ui \
    widgets/FormStringBuilder.ui \
    widgets/FormStringBuilderChain.ui \
    widgets/WidgetFileHashSetting.ui \
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
