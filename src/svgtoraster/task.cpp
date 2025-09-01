/**
 * \file
 * \brief Файл с определениями функций-членов класса \ref Task "Task"
 *
 * <BR>
 */

#include <QtGui/QIcon>
#include <QtCore/QSize>
#include <QImageReader>
#include <QImageWriter>
#include "task.h"
#include "build_ico.h"
#include "arg_parser.h"

/**
 * \file
 * Функции, являющиеся методами класса \ref Task "Task":
 * <BR>
 */

/**
 * \file
 * * \copybrief Task::Task(int, char **, int, QObject *)
 */
Task::Task(int argc, char **argv, int minargs, QObject *parent) : QObject(parent)
{
    m_parse_ok = parse_args(argc, argv, minargs);
}

/**
 * \file
 * * \copybrief Task::parse_args(int, char **, int)
 */
int Task::parse_args(int argc, char **argv, int minargs)
{

    /** 1 Проверить наличие минимально допустимого количества аргументов программы; */
    if (argc < minargs) {
        if (argc > 1) {
            qCritical().noquote() << tr("Command line format error: the number of arguments is less than") << minargs - 1;
        }
        print_help();
        return -1;
    }

    /** 2 Сформировать список директив и их параметров из аргументов командной строки; */
    arg_parser args;
    if (args.process_cmdline(const_cast<const char**>(argv), argc)) { //-V201
        qCritical().noquote() << tr("Command line format error: the first argument '") << argv[1] << tr("' is not a flag");
        return -1;
    }

    /** 3 Обработать директивы в цикле, устанавливая значения управляющих переменных; */
    for (int i = 0; i < args.get_arg_num(); ++i) {
        const QString& curr_flag = args.get_flag(i);
        if (curr_flag.compare("i") == 0) {
          const QStringList& parms = args.get_parameters_set(i);
          if (parms.size() < 1) {
            qCritical().noquote() << tr("Command line format error: missing parameter after the") << "'i'" << tr("flag");
            return -1;
          }
          m_input_file = args.get_parameters(i);

          if (!QFile::exists(m_input_file)) {
              qCritical().noquote() << tr("Error: the specified '") <<  m_input_file << tr("' source file is missing");
              return -1;
          }


          continue;
      
        } else if (curr_flag.compare("o") == 0) {
          const QStringList& parms = args.get_parameters_set(i);
          if (parms.size() < 1) {
            qCritical().noquote() << tr("Command line format error: missing parameter after the") << "'o'" << tr("flag");
            return -1;
          }
          m_output_file = args.get_parameters(i);
          continue;
      
        } else if (curr_flag.compare("s") == 0) {
          const QStringList& parms = args.get_parameters_set(i);
          if (parms.size() < 1) {
            qCritical().noquote() << tr("Command line format error: missing parameter after the") << "'s'" << tr("flag");
            return -1;
          }
          m_bitmap_size = args.get_parameters(i).toInt();
          continue;

        } else {
            qCritical().noquote() << tr("Command line format error: unknown '") << args.get_flag(i) << tr("' flag");
            return -1;
        }
    }

    return 0;
}

/**
 * \file
 * * \copybrief Task::run()
 */
void Task::run()
{

    if (m_parse_ok == 0)
    {
        QIcon icon = QIcon(m_input_file);
        if (icon.isNull())
        {
            qCritical().noquote() << tr("Source file load error");
        } else {

            qDebug().noquote() << tr("Available input file sizes:") << icon.availableSizes();

            QImage image = icon.pixmap(QSize(m_bitmap_size, m_bitmap_size)).toImage();

            QFileInfo fileInfo(m_output_file);
            if (fileInfo.suffix().compare("ico", Qt::CaseInsensitive) == 0)
            {
                QList<int> icon_sizes = {256, 48, 32, 16};
                if (saveIco(icon, m_output_file, icon_sizes) != 0)
                {
                    qCritical() << tr("Cannot open output file '") << m_output_file << tr("' for writing");
                }
            } else {
                image.save(m_output_file);
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
  qInfo().noquote() << "    <S>              -" << tr("output file size");
  qInfo().noquote() << "    <in_file>        -" << tr("source SVG file");
  qInfo().noquote() << "    <out_file>       -" << tr("target PNG file");

  qDebug().noquote() << tr("Supported input formats:") << QImageReader::supportedImageFormats();
  qDebug().noquote() << tr("Supported output formats:") << QImageWriter::supportedImageFormats();
}
