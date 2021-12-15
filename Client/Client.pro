QT += quick core network sql

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    include/DataHandlingPackages/DataBase.cpp \
    include/DataHandlingPackages/DataHandler.cpp \
    include/DataHandlingPackages/MainDataModel.cpp \
    include/MainApp.cpp \
    include/NetworkPackages/NetReceiver.cpp \
    include/NetworkPackages/NetSender.cpp \
    include/NetworkPackages/NetworkHandler.cpp \
    main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    include/DataHandlingPackages/DataBase.hpp \
    include/DataHandlingPackages/DataHandler.hpp \
    include/ClientKeywords.hpp \
    include/CustomTypes/TSQByteArray.h \
    include/DataHandlingPackages/MainDataModel.hpp \
    include/MainApp.hpp \
    include/NetworkPackages/NetReceiver.hpp \
    include/NetworkPackages/NetSender.hpp \
    include/NetworkPackages/NetworkHandler.hpp \
    include/Others/format.hpp

DISTFILES +=
