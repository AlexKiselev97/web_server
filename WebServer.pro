TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lboost_system -lpthread -fopenmp
SOURCES += \
        main.cpp \
    server.cpp

HEADERS += \
    server.h
