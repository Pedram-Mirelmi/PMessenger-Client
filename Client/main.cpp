#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "./include/MainApp.hpp"



int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    MainApp backend_app(engine);


    engine.rootContext()->setContextProperty("mainApp", &backend_app);
    engine.rootContext()->setContextProperty("netConn", backend_app.m_network_handler);
    engine.rootContext()->setContextProperty("dataHolder", backend_app.m_data_handler);

    const QUrl url(QStringLiteral("qrc:/QML/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
