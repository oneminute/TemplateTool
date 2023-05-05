# CMakeList.txt : CMake project for TeamplteTool, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("TemplateTool")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets Charts LinguistTools OpenGL Concurrent Multimedia MultimediaWidgets Test CONFIG REQUIRED)

# Add source to this project's executable.
add_executable(${PROJECT_NAME} 
    src/ui/MainWindow.h
    src/ui/MainWindow.cpp
    src/ui/MainWindow.ui
    src/ui/MatchResultItem.h
    src/ui/MatchResultItem.cpp
    src/ui/ImageWidget.h
    src/ui/ImageWidget.cpp
    src/ui/TemplateScene.h
    src/ui/TemplateScene.cpp
    src/util/util.h
    src/util/util.cpp
    src/TemplateTool.cpp
    src/TemplateTool.h
    src/common.h
    src/main.cpp
 "src/ui/TemplateListWidget.h" "src/ui/TemplateListWidget.cpp")

target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt5::Widgets Qt5::Charts Qt5::OpenGL Qt5::Concurrent Qt5::Test Qt5::Multimedia Qt5::MultimediaWidgets
    halconcpp
)

message(STATUS ${WORKING_DIRECTORY}/${PROJECT_NAME}_autogen/include)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${AUTOGEN_BUILD_DIR}/${PROJECT_NAME}_autogen/include
    include
    include/Halcon
    src
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${Qt5_DIR}/../../../bin"
)

INSTALL(TARGETS ${PROJECT_NAME}
    DESTINATION "${CMAKE_INSTALL_PREFIX}" COMPONENT runtime
	CONFIGURATIONS ${CMAKE_BUILD_TYPE})

INSTALL(FILES 
            "${CMAKE_SOURCE_DIR}/bin/halcon.dll"
            "${CMAKE_SOURCE_DIR}/bin/halconcpp.dll"
        DESTINATION ${CMAKE_INSTALL_PREFIX}
		CONFIGURATIONS ${CMAKE_BUILD_TYPE})

if(Qt5_FOUND AND WIN32 AND TARGET Qt5::qmake AND NOT TARGET Qt5::windeployqt)
    get_target_property(_qt5_qmake_location Qt5::qmake IMPORTED_LOCATION)

    execute_process(
        COMMAND "${_qt5_qmake_location}" -query QT_INSTALL_PREFIX
        RESULT_VARIABLE return_code
        OUTPUT_VARIABLE qt5_install_prefix
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    set(imported_location "${qt5_install_prefix}/bin/windeployqt.exe")

    if(EXISTS ${imported_location})
        add_executable(Qt5::windeployqt IMPORTED)

        set_target_properties(Qt5::windeployqt PROPERTIES
            IMPORTED_LOCATION ${imported_location}
        )
    endif()
endif()

set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
include(InstallRequiredSystemLibraries)
install(
    PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
    DESTINATION ${CMAKE_INSTALL_PREFIX}
	CONFIGURATIONS ${CMAKE_BUILD_TYPE})

get_target_property(QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
get_filename_component(QT_BIN_DIR "${QMAKE_EXECUTABLE}" DIRECTORY)
INSTALL(CODE "
	execute_process(
		COMMAND \"${QT_BIN_DIR}/windeployqt.exe\"
			--dir ${CMAKE_INSTALL_PREFIX}
			--verbose 1
			--release
			--no-translations
			${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.exe)"
	CONFIGURATIONS ${CMAKE_BUILD_TYPE})

set(BASE_CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

