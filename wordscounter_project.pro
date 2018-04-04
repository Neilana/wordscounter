#-------------------------------------------------
#
# Project created by QtCreator 2017-01-13T22:40:41
#
#-------------------------------------------------

QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += qml quick quickwidgets xmlpatterns
CONFIG += c++11

TARGET = wordscounter_project
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += src/main.cpp \
#    src/configs/Config.cpp \
    src/databases/LibraryDatabase.cpp \
    src/databases/WordsDatabase.cpp \
    src/managers/LibraryManager.cpp \
    src/managers/ReadingManager.cpp \
    src/managers/WordsManager.cpp \
    src/qml_connection/MySqlQueryModel.cpp \
    src/qml_connection/QmlCppInterface.cpp \
    src/reader/LibraryBook.cpp \
    src/reader/PagesGenerator.cpp \
    #src/reader/WordClass.t.cpp \
    src/Core.cpp \
    src/qml_connection/SearchWordModel.cpp \
    src/managers/NetworkManager.cpp


HEADERS  += \
#    src/configs/Config.h \
    src/databases/LibraryDatabase.h \
    src/databases/WordsDatabase.h \
    src/managers/LibraryManager.h \
    src/managers/ReadingManager.h \
    src/managers/WordsManager.h \
    src/qml_connection/CursorPosProvider.h \
    src/qml_connection/MySqlQueryModel.h \
    src/qml_connection/QmlCppInterface.h \
    src/reader/LibraryBook.h \
    src/reader/PagesGenerator.h \
    src/Core.h \
    src/loggerDefine.h \
    src/qml_connection/SearchWordModel.h \
    src/managers/NetworkManager.h \
    src/WordItem.h

INCLUDEPATH += \
    src \
    src/databases \
    src/managers \
    src/reader \
    src/qml_connection \
    ../libs/spdlog-0.16.3/include # https://github.com/gabime/spdlog

# Google Test integration
#INCLUDEPATH += /usr/include/gtest

#LIBS += -lgtest -L/usr/include/gtest

RESOURCES += \
    book_resources.qrc \
    ui/style_resources.qrc \
    ui/qml_forms/forms_resources.qrc

message($$OUT_PWD)
