/**
 * \file
 * \brief Файл с определениями функций-членов класса \ref Task "Task"
 */

#include <QIcon>
#include <QSize>
#include <QImageReader>
#include <QImageWriter>
#include "task.h"
#include "build_ico.h"
#include "build_icns.h"
#include "arg_parser.h"

/**
 * \file
 * Функции, являющиеся методами класса \ref Task "Task":
 * <BR>
 */

/**
 * \file
 * * \copybrief Task::Task(int, char **, QObject *)
 */
Task::Task(int argc, char **argv, QObject *parent) : QObject(parent)
{

#ifdef Q_OS_MACOS
    m_currDir = qEnvironmentVariable("PWD");
#else
    m_currDir = QDir::currentPath();
#endif

//    qDebug() << "Current directory (task pwd): " << QString(getenv("PWD"));
//    qDebug() << "Current directory (task qpwd): " << qEnvironmentVariable("PWD");
//    qDebug() << "Working directory (task currpath): " << QDir::currentPath();

    m_parse_ok = parse_args(argc, argv);
}

/**
 * \file
 * * \copybrief Task::parse_args(int, char **)
 */
int Task::parse_args(int argc, char **argv)
{
    /** Алгоритм: */

    arg_parser args;

    /** 1 Сформировать список директив и их параметров из аргументов командной строки; */
    int err_parse = args.process_cmdline(const_cast<const char**>(argv), argc);

    /** 2 Проверить ошибку отсутствия аргументов программы */
    if (err_parse == -2) {
        print_help();
        return err_parse;
    /** 3 Проверить ошибки разбора командной строки */
    } else if (err_parse == -1) {
        qCritical().noquote() << tr("Command line format error: the first argument") << args.get_flag(0) << tr("is not a flag");
        return err_parse;
    }  else if (err_parse == -3) {
        qCritical().noquote() << tr("Command line format error: the first argument") << args.get_flag(0) << tr("is empty flag");
        return err_parse;
    }

    /** 4 Обработать директивы в цикле, устанавливая значения управляющих переменных; */
    for (qsizetype i = 0; i < args.get_arg_num(); ++i) {
        const QString& curr_flag = args.get_flag(i);
        if (curr_flag.compare("i") == 0) {
          const QStringList& parms = args.get_parameters_set(i);
          if (parms.size() < 1) {
            qCritical().noquote() << tr("Command line format error: missing parameter after the") << "'i'" << tr("flag");
            return -1;
          }
//        m_input_file = QDir::cleanPath(QFileInfo(m_currDir.filePath(args.get_parameters(i))).absoluteFilePath());
          m_input_file = QFileInfo(m_currDir.filePath(args.get_parameters(i))).absoluteFilePath();
//          qDebug() << "Absolute input path:" << m_input_file;

          if (!QFile::exists(m_input_file)) {
              qCritical().noquote() << tr("Error: the specified") << args.get_parameters(i) << tr("source file is missing");
              return -1;
          }
          continue;
      
        } else if (curr_flag.compare("o") == 0) {
          const QStringList& parms = args.get_parameters_set(i);
          if (parms.size() < 1) {
            qCritical().noquote() << tr("Command line format error: missing parameter after the") << "'o'" << tr("flag");
            return -1;
          }
//        m_output_file = QDir::cleanPath(QFileInfo(m_currDir.filePath(args.get_parameters(i))).absoluteFilePath());
          m_output_file = QFileInfo(m_currDir.filePath(args.get_parameters(i))).absoluteFilePath();
//          qDebug() << "Absolute output path:" << m_output_file;
          continue;
      
        } else if (curr_flag.compare("s") == 0) {
          const QStringList& parms = args.get_parameters_set(i);
          if (parms.size() < 1) {
            qCritical().noquote() << tr("Command line format error: missing parameter after the") << "'s'" << tr("flag");
            return -1;
          }
          QStringList size_set = args.get_parameters_set(i);
          for (const QString &size : size_set) {
              m_bitmap_size.append(size.toInt());
          }
          continue;

        } else {
            qCritical().noquote() << tr("Command line format error: unknown") << args.get_flag(i) << tr("flag");
            return -1;
        }
    }

    /** 5 Проверить ошибку отсутствия исходного файла в командной строке; */
    if (m_input_file.isEmpty()) {
        qCritical().noquote() << tr("The source file is not specified");
        return -1;
    }

    /** 5 Проверить ошибку отсутствия результирующего файла в командной строке; */
    if (m_output_file.isEmpty()) {
        qCritical().noquote() << tr("The target file is not specified");
        return -1;
    }
    return err_parse;
}

/**
 * \file
 * * \copybrief Task::run()
 */
void Task::run()
{

    if (m_parse_ok == 0) {
        QIcon icon = QIcon(m_input_file);
        if (icon.isNull()) {
            qCritical().noquote() << tr("Source file load error");
        } else {

            if (m_bitmap_size.empty()) {
              m_bitmap_size.append(256);
            }

            if (QFileInfo(m_output_file).suffix().compare("ico", Qt::CaseInsensitive) == 0) {
                if (saveIco(icon, m_output_file, m_bitmap_size) != 0) {
                    qCritical() << tr("Cannot open output file") << m_output_file << tr("for writing");
                }
            } else if (QFileInfo(m_output_file).suffix().compare("icns", Qt::CaseInsensitive) == 0) {
                if (saveIcns(icon, m_output_file) != 0) {
                    qCritical() << tr("Cannot open output file") << m_output_file << tr("for writing");
                }
            } else {
                QImage image = icon.pixmap(QSize(m_bitmap_size[0], m_bitmap_size[0])).toImage();
                if (!image.save(m_output_file)) {
                    qCritical() << tr("Cannot save output file") << m_output_file;
                }
            }
        }
    }
    emit finished();
}

/**
 * \file
 * * \copybrief Task::print_help()
 */
void Task::print_help() {
  qInfo().noquote() << tr("USAGE:");
  qInfo().noquote() << "svgtoraster [--s <S>] --i <in_file> --o <out_file>";
  qInfo().noquote() << "    <S>              -" << tr("output bitmap sizes");
  qInfo().noquote() << "    <in_file>        -" << tr("source SVG file");
  qInfo().noquote() << "    <out_file>       -" << tr("target PNG, ICO, ICNS file");
  qDebug().noquote() << tr("Supported input formats:") << QImageReader::supportedImageFormats();
  qDebug().noquote() << tr("Supported output formats:") << QImageWriter::supportedImageFormats();
}
