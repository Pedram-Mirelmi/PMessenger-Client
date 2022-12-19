QT += quick core network sql

CONFIG += c++20

LIBS += -lfmt

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    include/DataHandlingPackages/DataBase.cpp \
    include/DataHandlingPackages/DataHandler.cpp \
    include/DataHandlingPackages/NetPendingTaskManager.cpp \
    include/DataHandlingPackages/models/ConversationsListModel.cpp \
    include/DataHandlingPackages/models/MessageListModel.cpp \
    include/MainApp.cpp \
    include/NetworkPackages/NetIOHandler.cpp \
    include/Others/CommonTools.cpp \
    include/Others/stringTools.cpp \
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
    KeyWords.hpp \
    include/AsyncNetFramework/AbstractNetIOManager.h \
    include/AsyncNetFramework/IService.h \
    include/AsyncNetFramework/io/BasicNetMessage.h \
    include/AsyncNetFramework/io/INetMessageBodyDeserializer.h \
    include/AsyncNetFramework/io/ISerializable.h \
    include/DataHandlingPackages/DataBase.hpp \
    include/DataHandlingPackages/DataHandler.hpp \
    include/ClientKeywords.hpp \
    include/CustomTypes/TSQByteArray.h \
    include/DataHandlingPackages/NetPendingTaskManager.h \
    include/DataHandlingPackages/models/ConversationsListModel.hpp \
    include/DataHandlingPackages/models/MessageListModel.hpp \
    include/MainApp.hpp \
    include/NetworkPackages/NetIOHandler.hpp \
    include/NetworkPackages/NetMessages/NetMessageType.hpp \
    include/Others/CommonTools.hpp \
    include/Others/TypeDefs.hpp \
    include/Others/format.hpp \
    include/Others/stringTools.hpp

DISTFILES += \
    todolist.txt
