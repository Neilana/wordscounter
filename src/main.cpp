#include <QApplication>

#include <spdlog/spdlog.h>

#include "Core.h"
#include "loggerDefine.h"

std::shared_ptr<spdlog::logger> g_logger;

void initLogger()
{
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S.%e] [%n] [%L] [thread: %t] %v");

    // file log
    if (!QDir("logs").exists()) QDir().mkdir("logs");

    try
    {
        // FIXME : FILE CREATION
        if (QFile::exists("logs/basic.txt"))
        {
            QFile::remove("logs/basic.txt");
        }
        auto my_logger = spdlog::basic_logger_mt("console_log", "logs/basic.txt");
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        qDebug() << "Log initialization failed: " << ex.what();
    }

    g_logger = spdlog::get("console_log");
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    initLogger();

    Core core;
    core.setUp();
    core.start();

    return a.exec();
}
