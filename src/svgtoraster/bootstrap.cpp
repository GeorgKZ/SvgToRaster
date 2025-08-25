/**
 * \file
 * \brief Файл с определениями членов класса \ref bootstrap "bootstrap"
 *
 * <BR>
 */
#include <QtCore/QSettings>
#ifdef Q_OS_WINDOWS
  #include <windows.h> 
#endif
#include "bootstrap.h"

/**
 * \file
 * Переменные, являющиеся статическими членами класса \ref bootstrap "bootstrap":
 * <BR>
 */

/**
 * \file
 * * \copybrief bootstrap::m_debugOutput
 */
bool bootstrap::m_debugOutput = false;

/**
 * \file
 * * \copybrief bootstrap::m_originalMessageHandler
 */
QtMessageHandler bootstrap::m_originalMessageHandler = nullptr;

/**
 * \file
 * * \copybrief bootstrap::m_appTranslator
 */
QTranslator *bootstrap::m_appTranslator = nullptr;

/**
 * \file
 * * \copybrief bootstrap::m_qtTranslator
 */
QTranslator *bootstrap::m_qtTranslator = nullptr;

/**
 * \file
 * Функции, являющиеся статическими методами класса \ref bootstrap "bootstrap":
 * <BR>
 */

/**
 * \file
 * * \copybrief bootstrap::init(int &, char **)
 */
void bootstrap::init(int &argc, char **argv)
{
    /**
     * Алгоритм:
     */

#ifdef Q_OS_WINDOWS
    /**
     * * Если программа запущена в консоли, разрешить консольный вывод.
     */
    auto stdout_type = GetFileType(GetStdHandle(STD_OUTPUT_HANDLE));
    if (stdout_type == FILE_TYPE_UNKNOWN) {
        if (AttachConsole(ATTACH_PARENT_PROCESS)) {
            freopen("CONOUT$", "w", stdout);
            freopen("CONOUT$", "w", stderr);
        }
    }
#endif

    /**
     * * При наличии в командной строке ключа -DEBUG включить вывод отладочной информации
     */
    for (auto i = 0; i < argc; i++) {
        if (QString(argv[i]).toUpper() == "-DEBUG") {
            m_debugOutput = true;
            break;
        }
    }

    /**
     * * Установить свой обработчик отладочных/информационных/предупреждающих/аварийных
     * сообщений [qDebug()](https://doc.qt.io/qt-6/qtlogging.html#qDebug),
     * [qInfo()](https://doc.qt.io/qt-6/qtlogging.html#qInfo),
     * [qWarning()](https://doc.qt.io/qt-6/qtlogging.html#qWarning),
     * [qCritical()](https://doc.qt.io/qt-6/qtlogging.html#qCritical),
     * [qFatal()](https://doc.qt.io/qt-6/qtlogging.html#qFatal)
     */
    m_originalMessageHandler = qInstallMessageHandler(myMessageOutput);

    /**
     * * Установить локализацию согласно указанному в файле конфигурации языку.
     */
    setAllTranslators("en");
}

/**
 * \file
 * * \copybrief bootstrap::myMessageOutput(QtMsgType, const QMessageLogContext &, const QString &)
 */
void bootstrap::myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    /**
     * Алгоритм:
     */

    /**
     * * Если не установлен флаг разрешения консольного вывода, то ничего не делать.
     */
    if (!m_debugOutput) {
        return;
    }

    /**
     * * Создать поток типа [QTextStream](https://doc.qt.io/qt-6/qtextstream.html)
     * для стандартного вывода.
     */
    QTextStream tsTextStream(stdout);

    /**
     * * Вывести в созданный поток информацию о сообщении. Она может быть дополнительно
     * оформлена шаблоном, указанном в переменной окружения QT_MESSAGE_PATTERN.
     */
    tsTextStream << qFormatLogMessage(type, context, msg) << Qt::endl;
}

/**
 * \file
 * * \copybrief bootstrap::setTranslator(const QString &, QTranslator **)
 */
void bootstrap::setTranslator(const QString &translationFileName, QTranslator **translator) {
    /**
     * Алгоритм:
     *
     * * Создать новый [транслятор](https://doc.qt.io/qt-6/qtranslator.html).
     */
    QTranslator* new_translator = new QTranslator();

    /**
     * * Загрузить указанный аргументом \ref bootstrap::setTranslator(const QString &, QTranslator **) "translationFileName"
     * файл локализации.
     */
    if (!new_translator->load(translationFileName)) {
        /**
         * * В случае неудачи удалить новый [транслятор](https://doc.qt.io/qt-6/qtranslator.html).
         */
        qCritical() << "Error loading" << translationFileName << "translation";
        delete new_translator;
    } else {

        /**
         * * В случае успеха удалить предыдущий [транслятор](https://doc.qt.io/qt-6/qtranslator.html),
         * переданный в аргументе-указателе \ref bootstrap::setTranslator(const QString &, QTranslator **) "translator",
         * сохранить под этим указателем новый.
         */
        qApp->removeTranslator(*translator);
        delete *translator;
        *translator = new_translator;

        /**
         * * Установить новый [транслятор](https://doc.qt.io/qt-6/qtranslator.html).
         */
        qApp->installTranslator(*translator);
        qInfo() << "Translation" << translationFileName << "loaded abd installed";
    }
}

/**
 * \file
 * * \copybrief bootstrap::setAllTranslators(const QString &)
 */
void bootstrap::setAllTranslators(const QString &language) {
    /**
     * Алгоритм:
     */

    QString translationFileName;

    /**
     * * Установить файлы локализации приложения.
     */
    translationFileName = ":/translations/candle_" + language + ".qm";
    if (QFile::exists(translationFileName)) {
        setTranslator(translationFileName, &m_appTranslator);
    } else {
        qCritical() << "Error loading translation file" << translationFileName;
    }

    /**
     * * Установить файлы локализации [Qt](https://doc.qt.io/qt-6/).
     */
    translationFileName = ":/system_translations/qtbase_" + language + ".qm";
    if (QFile::exists(translationFileName)) {
        setTranslator(translationFileName, &m_qtTranslator);
    }
}

