TEMPLATE = app
CONFIG += console
QT += xml

SOURCES += main.cpp \
    gcn.cpp \
    domain.cpp \
    flow.cpp \
    domain_fsm.cpp \
    malware.cpp \
    link.cpp

OTHER_FILES += \
    config.xml \
    net_config.xml

HEADERS += \
    gcn.h \
    domain.h \
    flow.h \
    domain_fsm.h \
    malware.h \
    link.h

