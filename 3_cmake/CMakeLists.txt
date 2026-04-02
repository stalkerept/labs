cmake_minimum_required(VERSION 3.10)
project(TableApp VERSION 1.0 LANGUAGES C)

# Устанавливаем стандарт C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Исходные файлы библиотеки (функции таблицы и утилиты)
set(LIB_SOURCES
    "func.c"
    "utils.c"
)

# Заголовочные файлы (не обязательны для компиляции, но указываем для IDE)
set(LIB_HEADERS
    "func.h"
    "structures.h"
    "utils.h"
)

# Создаём статическую библиотеку
add_library(table_lib STATIC ${LIB_SOURCES})

# Указываем директории для поиска заголовков (можно не указывать, если заголовки в той же папке)
target_include_directories(table_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Исходный файл приложения
set(APP_SOURCES
    "lab.c"
)

# Создаём исполняемый файл
add_executable(lab ${APP_SOURCES})

# Линкуем библиотеку
target_link_libraries(lab table_lib)

# Для приложения тоже указываем путь к заголовкам (необязательно)
target_include_directories(lab PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})