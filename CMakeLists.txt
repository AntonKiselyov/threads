cmake_minimum_required(VERSION 3.3)
project(threads)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES stack.h MyTests.h avl.h treethreads.cpp)
add_executable(threads ${SOURCE_FILES})



SET(CXXTEST_EXECUTABLE ${PROJECT_SOURCE_DIR}/cxxtest-4.3/cxxtest/cxxtestgen.pl)

MACRO(unit_test NAME CXX_FILE FILES)
    SET(PATH_FILES "")
    # Мне это не нужно, но если файлы расположены в
    # другом каталоге то может понадобиться
    #FOREACH(part ${FILES})
    #    SET(PATH_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${part}" ${PATH_FILES})
    #ENDFOREACH(part ${FILES})
    SET(PATH_FILES ${FILES})

    SET(CXX_FILE_REAL "${CMAKE_CURRENT_SOURCE_DIR}/${CXX_FILE}")
    SET(CXXTEST_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${NAME}.cxx")
    ADD_CUSTOM_COMMAND(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${NAME}.cxx"
            COMMAND ${CXXTEST_EXECUTABLE} --error-printer -o "${CXXTEST_OUTPUT}" ${CXX_FILE_REAL}
            DEPENDS "${FILE}")
    SET_SOURCE_FILES_PROPERTIES(${CXXTEST_OUTPUT} PROPERTIES GENERATED true)

    ADD_EXECUTABLE("${NAME}" "${CXXTEST_OUTPUT}" ${PATH_FILES})
    TARGET_LINK_LIBRARIES("${NAME}" ${CXXTEST_LINK_LIBS})
    ADD_TEST("${NAME}" "${EXECUTABLE_OUTPUT_PATH}/${NAME}")
ENDMACRO(unit_test)