DISTFILES += \
    ../Expression_Calculator/Makefile \

HEADERS += \
    ../Expression_Calculator/catch.h \
    ../Expression_Calculator/env.h \
    ../Expression_Calculator/expr.h \
    ../Expression_Calculator/parse.h \
    ../Expression_Calculator/pointer.h \
    ../Expression_Calculator/val.h \
    msdwidget.h

SOURCES += \
    ../Expression_Calculator/env.cpp \
    ../Expression_Calculator/exec.cpp \
    ../Expression_Calculator/expr.cpp \
    ../Expression_Calculator/parse.cpp \
    ../Expression_Calculator/val.cpp \
    main.cpp \
    msdwidget.cpp

QT += widgets
