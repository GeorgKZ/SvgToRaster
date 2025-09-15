set(qt_mirrors "mirrors.ukfast.co.uk/sites/qt.io;ftp.fau.de/qtproject;mirrors.sau.edu.cn/qt;mirrors.ocf.berkeley.edu/qt")
set(qt_version "6.9.2")

string(REPLACE "." "" qt_version_dotless "${qt_version}")

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
  set(ARCH_PATH "T:/archive")
  set(url_os "windows_x86")
  set(compiler_id "win64_msvc2022_64")
  set(qt_dir "C:/QT_${qt_version_dotless}")
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  set(ARCH_PATH "/var/tmp/archive")
  set(url_os "linux_x64")
  set(compiler_id "linux_gcc_64")
  set(qt_dir "/opt/qt${qt_version_dotless}")
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
  set(ARCH_PATH "/tmp/archive")
  set(url_os "mac_x64")
  set(compiler_id "clang_64")
  set(qt_dir "/opt/qt${qt_version_dotless}")
endif()

# Пробовать загружать последовательно с каждого зеркала
foreach(mirror ${qt_mirrors})
  set(qt_base_url "https://${mirror}/online/qtsdkrepository/${url_os}/desktop/qt6_${qt_version_dotless}/qt6_${qt_version_dotless}")
  foreach(retry RANGE 10)
    message("Загрузка Updates.xml")
    cmake_language(EVAL CODE
      "file(DOWNLOAD \"${qt_base_url}/Updates.xml\" \"${ARCH_PATH}/Updates.xml\")"
    )
    file(SIZE ${ARCH_PATH}/Updates.xml fileSize)
    if (fileSize GREATER 0)
      break()
    endif()
  endforeach()

  # Проверить размер загруженного файла Updates.xml,
  # если 0 (не загрузился) перейти к следующему зеркалу
  file(SIZE ${ARCH_PATH}/Updates.xml fileSize)
  if (fileSize EQUAL 0)
    continue()
  endif()

  file(READ ${ARCH_PATH}/Updates.xml updates_xml)
  # get the package sections, only for given compiler, without the "debug info" ones
  string(REGEX REPLACE "<PackageUpdate>" ";<PackageUpdate>" sections "${updates_xml}")
  list(FILTER sections EXCLUDE REGEX "<Name>.*debug.*</Name>")
  list(FILTER sections INCLUDE REGEX "<Name>.*${compiler_id}.*</Name>")

  function(get_sub_url outvar module)
    set(filtered ${sections})
    list(FILTER filtered INCLUDE REGEX "<DownloadableArchives>.*${module}.*</DownloadableArchives>")
    list(LENGTH filtered count)
    if (count LESS 1)
      set(${outvar} "<notfound>" PARENT_SCOPE)
      return()
    endif()
    list(GET filtered 0 section)
    string(REGEX MATCH "<Name>(.*)</Name>" match "${section}")
    set(name ${CMAKE_MATCH_1})
    string(REGEX MATCH "<Version>(.*)</Version>" match "${section}")
    set(version ${CMAKE_MATCH_1})
    string(REGEX MATCH "<DownloadableArchives>.*(${module}[^,]+).*</DownloadableArchives>" match "${section}")
    set(archive ${CMAKE_MATCH_1})
    set(${outvar} "${name}/${version}${archive}" PARENT_SCOPE)
  endfunction()

  message("Загрузка Qt в ${qt_dir}")
  # Загрузка и разжатие файлов типа ${module}.7z
  function(downloadAndExtract module subdir)
    file(MAKE_DIRECTORY "${qt_dir}/${subdir}")
    set(archive "${module}.7z")
    get_sub_url(sub_url ${module})
    set(url "${qt_base_url}/${sub_url}")
    foreach(retry RANGE 10)
      message("Загрузка модуля ${module}")
      cmake_language(EVAL CODE
        "file(DOWNLOAD \"${url}\" \"${ARCH_PATH}/${archive}\")"
      )
      file(SIZE "${ARCH_PATH}/${archive}" fileSize)
      if (fileSize GREATER 0)
        break()
      endif()
    endforeach()
    message("Разжатие модуля ${module} в директорию ${qt_dir}/${subdir}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xvf "${ARCH_PATH}/${archive}"
      WORKING_DIRECTORY "${qt_dir}/${subdir}"
      OUTPUT_QUIET
    )
  endfunction()

  # Обязательные:
  # +base (Core, Gui, Widgets) OpenGL +serialport Qml OpenGLWidgets  UiTools Quick +multimedia(Multimedia, MultimediaWidgets)
  set(qt_packages
    "qt3d"
    "qt5compat"
    "qtcharts"
    "qtconnectivity"
    "qtdatavis3d"
    "qtgraphs"
    "qtgrpc"
    "qthttpserver"
    "qtimageformats"
    "qtlanguageserver"
    "qtlocation"
    "qtlottie"
    "qtmultimedia"
    "qtnetworkauth"
    "qtpositioning"
    "qtquick3d"
    "qtquick3dphysics"
    "qtquickeffectmaker"
    "qtquicktimeline"
    "qtremoteobjects"
    "qtscxml"
    "qtsensors"
    "qtserialbus"
    "qtserialport"
    "qtshadertools"
    "qtspeech"
    "qtvirtualkeyboard"
    "qtwayland"
    "qtwebchannel"
    "qtwebsockets"
    "qtwebview"
    "qtbase"
    "qtdeclarative"
    "qtsvg"
    "qttools"
    "qttranslations"
    "qtwayland"
  )

  foreach(package ${qt_packages})
    downloadAndExtract(${package} "")
  endforeach(package)

  # uic depends on libicu*.so
  if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    downloadAndExtract("icu" "lib")
  endif()

  # Work is done, no need to try the next mirror.
  break()
endforeach(mirror)

# для Windows поместить системные библиотеки в директорию Qt, чтобы их было проще собрать в установочный пакет
if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    foreach(file vcruntime140.dll concrt140.dll msvcp140_1.dll msvcp140_2.dll
                 msvcp140_codecvt_ids.dll vcruntime140_1.dll msvcp140.dll)
    file(INSTALL "C:/Windows/System32/${file}"
      DESTINATION "qt/bin")
    endforeach()
endif()
