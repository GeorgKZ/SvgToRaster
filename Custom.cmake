##############################################################################
# Информация о проекте
##############################################################################

# Имя, версия, описание проекта
set(CUSTOM_PROJECT_NAME "SvgToRaster")
set(CUSTOM_VERSION "1.0")
set(CUSTOM_DESCRIPTION "Простое преобразование файла формата SVG в файл растрового формата на основе возможностей Qt")
set(CUSTOM_HOMEPAGE_URL "https://github.com/GeorgKZ/SvgToRaster")
set(CUSTOM_LANGUAGES CXX)

# Поставщик установочного пакета
set(CUSTOM_PROJECT_VENDOR "GeorgKZ")

# Контактные данные поставщика установочного пакета
set(CUSTOM_PROJECT_MAINTAINER "${PROJECT_VENDOR} https://github.com/GeorgKZ")

# Путь к файлу значка ICO проекта
#set(CUSTOM_ICON_FILE "${CMAKE_SOURCE_DIR}/src/candle/images/candle.ico")

##############################################################################
# Настройки Qt
##############################################################################

# Список необходимых для приложения плюгинов
set(QT_PLUGIN_LIST
#  "accessible"
#  "accessiblebridge"
  "iconengines"
  "imageformats"
#  "permissions"
#  "platforminputcontexts"
  "platforms"
#  "platformthemes"
#  "styles"
#  "xcbglintegrations"
)

# Подсистемы Qt, для которых требуются переводы
set(QT_SYSTEMS "qt;qtbase;qtmultimedia")

# Список необходимых для приложения модулей Qt
set(QT_MODULES_LIST
  "Core"                 
#  "Designer"             
#  "Gui"                  
#  "Multimedia"           
#  "MultimediaWidgets"    
#  "OpenGL"               
#  "OpenGLWidgets"        
#  "Qml"                  
#  "Quick"                
#  "SerialPort"           
   "UiTools"              
#  "UiPlugin"             
#  "Widgets"              
)

# Настройка языков локализации Qt:
# английский, русский
set(QT_LANG "en_US;ru_RU")
