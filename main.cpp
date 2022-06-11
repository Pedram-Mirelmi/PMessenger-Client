#define FMT_HEADER_ONLY
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "./include/MainApp.hpp"
#include <fstream>


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    MainApp backend_app(engine);


//    engine.rootContext()->setContextProperty("mainApp", &backend_app);


    const QUrl url(QStringLiteral("qrc:/QML/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
