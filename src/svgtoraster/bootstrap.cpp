/**
 * \file
 * \brief Файл с определениями функций-членов класса \ref bootstrap "bootstrap"
 */

#include "bootstrap.h"

/**
 * \file
 * Переменные, являющиеся статическими членами класса \ref bootstrap "bootstrap":
 * <BR>
 */

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
 * * \copybrief bootstrap::init()
 */
void bootstrap::init()
{
    /**
     * Алгоритм:
     */

#if defined(Q_OS_WINDOWS) || defined(__DOXYGEN__)
    /**
     * 1 Если программа запущена в консоли Windows, разрешить консольный вывод.
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
     * 2 Установить свой обработчик отладочных/информационных/предупреждающих/аварийных
     * сообщений [qDebug()](https://doc.qt.io/qt-6/qtlogging.html#qDebug),
     * [qInfo()](https://doc.qt.io/qt-6/qtlogging.html#qInfo),
     * [qWarning()](https://doc.qt.io/qt-6/qtlogging.html#qWarning),
     * [qCritical()](https://doc.qt.io/qt-6/qtlogging.html#qCritical),
     * [qFatal()](https://doc.qt.io/qt-6/qtlogging.html#qFatal).
     */
    m_originalMessageHandler = qInstallMessageHandler(myMessageOutput);

    /**
     * 3 Установить локализацию согласно указанному в файле конфигурации языку.
     */
    QLocale currentLocale;
    QLocale systemLocale = QLocale::system();

    qDebug().noquote() << "Default Locale:" << currentLocale.name();
    qDebug().noquote() << "System Locale:" << systemLocale.name();
    qDebug() << "Default language:" << currentLocale.languageToString(currentLocale.language())
      << "(" << currentLocale.nativeLanguageName() << ")";
    qDebug() << "System language:" << systemLocale.languageToString(systemLocale.language())
      << "(" << systemLocale.nativeLanguageName() << ")";
    qDebug() << "Default Region:" << currentLocale.countryToString(currentLocale.country());
    qDebug() << "System Region:" << systemLocale.countryToString(systemLocale.country());
    qDebug() << "Default Territory:" << QLocale::territoryToCode(currentLocale.territory());
    qDebug() << "UI Languages::" << currentLocale.uiLanguages();

    QString locale = QLocale::system().name().left(2);
    setAllTranslators(locale);

#ifdef Q_OS_MACOS
    char *PWD = getenv("PWD");
    QString pwd = PWD;
    qDebug() << "Working directory1: " << pwd;
#else
    qDebug() << "Working directory2: " << QDir::currentPath();
#endif


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
     * 1 Создать поток типа [QTextStream](https://doc.qt.io/qt-6/qtextstream.html)
     * для стандартного вывода.
     */
    QTextStream tsTextStream(stdout);

    /**
     * 2 Вывести в созданный поток информацию о сообщении. Она может быть дополнительно
     * оформлена шаблоном, указанном в переменной окружения [QT_MESSAGE_PATTERN](https://doc.qt.io/qt-6/qtlogging.html#qInstallMessageHandler).
     */
    QString output = qFormatLogMessage(type, context, msg);
    if (!output.isEmpty()) {
        tsTextStream << output << Qt::endl;
    }
}

/**
 * \file
 * * \copybrief bootstrap::setTranslator(const QString &, QTranslator **)
 */
void bootstrap::setTranslator(const QString &translationFileName, QTranslator **translator) {
    /**
     * Алгоритм:
     *
     * 1 Создать новый [транслятор](https://doc.qt.io/qt-6/qtranslator.html).
     */
    QTranslator* new_translator = new QTranslator();

    /**
     * 2 Загрузить указанный аргументом \ref bootstrap::setTranslator(const QString &, QTranslator **) "translationFileName"
     * файл локализации.
     */
    if (!new_translator->load(translationFileName)) {
        /**
         * &nbsp;&nbsp;&nbsp;&nbsp;2.1 В случае неудачи удалить новый [транслятор](https://doc.qt.io/qt-6/qtranslator.html).
         */
        qCritical().noquote() << tr("Error loading translation file") << translationFileName;
        delete new_translator;
    } else {

        /**
         * &nbsp;&nbsp;&nbsp;&nbsp;2.2 В случае успеха удалить предыдущий [транслятор](https://doc.qt.io/qt-6/qtranslator.html),
         * переданный в аргументе-указателе \ref bootstrap::setTranslator(const QString &, QTranslator **) "translator",
         * сохранить под этим указателем новый.
         */
        qApp->removeTranslator(*translator);
        delete *translator;
        *translator = new_translator;

        /**
         * &nbsp;&nbsp;&nbsp;&nbsp;2.3 Установить новый [транслятор](https://doc.qt.io/qt-6/qtranslator.html).
         */
        qApp->installTranslator(*translator);
        qDebug().noquote() << tr("Translation file") << translationFileName << tr("loaded and installed");
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
     * 1 Установить файлы локализации приложения.
     */
    translationFileName = ":/translations/svgtoraster_" + language + ".qm";
    if (QFile::exists(translationFileName)) {
        setTranslator(translationFileName, &m_appTranslator);
    } else {
        qCritical().noquote() << tr("Error loading translation file") << translationFileName;
    }

    /**
     * 2 Установить файлы локализации [Qt](https://doc.qt.io/qt-6/).
     */
    translationFileName = ":/system_translations/qtbase_" + language + ".qm";
    if (QFile::exists(translationFileName)) {
        setTranslator(translationFileName, &m_qtTranslator);
    }
}
