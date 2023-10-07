set(
    HEADERS
    scan_data_model.h
)

set(
    SOURCES
    scan_data_model.cpp
)

add_library(models STATIC ${SOURCES} ${HEADERS})