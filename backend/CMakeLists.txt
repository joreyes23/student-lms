cmake_minimum_required(VERSION 3.16)
project(backend CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Drogon CONFIG REQUIRED)

aux_source_directory(controllers CTL_SRC)
aux_source_directory(filters FILTER_SRC)
aux_source_directory(plugins PLUGIN_SRC)
aux_source_directory(models MODEL_SRC)
aux_source_directory(services SERVICE_SRC)

add_executable(backend
    main.cc
    ${CTL_SRC}
    ${FILTER_SRC}
    ${PLUGIN_SRC}
    ${MODEL_SRC}
    ${SERVICE_SRC}
    db/Database.cc
)

target_link_libraries(backend
    PRIVATE
        Drogon::Drogon
        mysqlclient
)

target_include_directories(backend
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/models
)

add_subdirectory(test)
