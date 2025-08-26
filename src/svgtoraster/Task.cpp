#include "Task.h"
#include "arg_parser.h"
#include <QIcon>
#include <QSize>

/**
 * \brief Минимально возможное количество аргументов (сама программа, исходный и результирующий файлы)
 */
#define MINARGS 3

Task::Task(const int &argc, char **argv, QObject *parent) : QObject(parent)
{
    m_parse_ok = parse_args(argc, argv);
}

int Task::parse_args(const int &argc, char **argv)
{

    /** 1 Проверить наличие минимально допустимого количества аргументов программы; */
    if (argc < MINARGS) {
        if (argc > 1) {
            qCritical() << tr("Ошибка формата командной строки: количество аргументов меньше ") << MINARGS - 1;
        }
        print_help();
        return -1;
    }

    /** 2 Сформировать список директив и их параметров из аргументов командной строки; */
    arg_parser args;
    if (args.process_cmdline(const_cast<const char**>(argv), static_cast<size_t>(argc))) { //-V201
        qCritical() << tr("Ошибка формата командной строки: первый аргумент '") << argv[1] << tr("' не является флагом");
        return -1;
    }

    /** 3 Обработать директивы в цикле, устанавливая значения управляющих переменных; */
    for (size_t i = 0; i < args.get_arg_num(); ++i) {
        const std::string& curr_flag = args.get_flag(i);
        if (curr_flag.compare("infile") == 0) {
          const std::vector<std::string>& parms = args.get_parameters_set(i);
          if (parms.size() < 1) {
            qCritical() << tr("Ошибка формата командной строки: отсутствуют параметры после флага") << " 'infile'";
            return -1;
          }
          m_input_file = args.get_parameters(i).c_str();
          continue;
      
        } else if (curr_flag.compare("outfile") == 0) {
          const std::vector<std::string>& parms = args.get_parameters_set(i);
          if (parms.size() < 1) {
            qCritical() << tr("Ошибка формата командной строки: отсутствует параметр после флага") << " 'outfile'";
            return -1;
          }
          m_output_file = args.get_parameters(i).c_str();
          continue;
      
        } else if (curr_flag.compare("size") == 0) {
          const std::vector<std::string>& parms = args.get_parameters_set(i);
          if (parms.size() < 1) {
            qCritical() << tr("Ошибка формата командной строки: отсутствует параметр после флага") << " 'back'";
            return -1;
          }
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201103L) || __cplusplus >= 201103L)
          m_bitmap_size = std::stoi(args.get_parameters(i));
#else
          m_bitmap_size = atoi(args.get_parameters(i).c_str());
#endif
          continue;

        } else if (curr_flag.compare("debug") == 0) {
          continue;

        } else {
            qCritical() << tr("Ошибка формата командной строки: неизвестный флаг '") << args.get_flag(i) << "'";
            return -1;
        }
    }

    return 0;
}

void Task::run()
{

    if (m_parse_ok == 0) {
        QIcon icon = QIcon(m_input_file);
        QImage image = icon.pixmap(QSize(1024, 1024)).toImage();

        image.save(m_output_file);
    }

    emit finished();
}

/**
 * \file
 * * \copybrief Task::print_help()
 */
void Task::print_help() {
  qInfo() << tr("КРАТКАЯ СПРАВКА");
  qInfo() << "svgtoraster [--size S] [--DEBUG] --infile <inf> --outfile <outf>";
  qInfo() << tr("    S                - размер результирующего файла");
  qInfo() << tr("    --DEBUG          - выводить отладочные сообщения");
  qInfo() << tr("    <inf>            - исходный файл SVG");
  qInfo() << tr("    <outf>           - результирующий файл PNG");
}
