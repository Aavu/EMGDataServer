add_library(
        ${KEYLOGGER_LIB}
        Src/KeyLogger.cpp
)

target_include_directories(${KEYLOGGER_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)
target_link_libraries(${KEYLOGGER_LIB} ${LOGGER_LIB})
target_precompile_headers(${KEYLOGGER_LIB} REUSE_FROM ${LOGGER_LIB})