cmake_minimum_required(VERSION 3.14)
project(RadialGM)

# Uncomment to give priority to the local CMake modules
# set(CMAKE_PREFIX_PATH "/usr/local/lib")

include(CMakeDependentOption)

option(RGM_BUILD_EMAKE "Build Emake and the compiler." ON)

# FIXME: MSVC dynamic linking requires US TO DLLEXPORT our funcs
# since we currently don't, I'm force disabling the option on MSVC
cmake_dependent_option(RGM_BUILD_STATIC "Build static libs." ON "MSVC" OFF)

# Check https://stackoverflow.com/q/33062728/14629018 for more information.
# if(MSVC)
#     set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
# endif()

if (RGM_BUILD_STATIC)
 set(LIB_TYPE STATIC CACHE STRING "Static Library type")
else()
 set(LIB_TYPE SHARED CACHE STRING "Shared Library type")
endif()

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Build type not set - defaulting to Debug")
  set(
    CMAKE_BUILD_TYPE "Debug"
    CACHE
      STRING
      "Choose the type of build from:  Debug Release RelWithDebInfo MinSizeRel."
    FORCE)
endif()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
  set(EXE "RadialGM-Debug" CACHE STRING "RGM Executable name")
  add_definitions(-DRGM_DEBUG)
  set(CMAKE_DEBUG_POSTFIX d)
else()
  set(EXE "RadialGM" CACHE STRING "RGM Executable name")
endif()

set(EXE_DESCRIPTION "ENIGMA IDE")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
SET(CMAKE_SKIP_BUILD_RPATH ON)
set(CMAKE_INSTALL_RPATH $ORIGIN)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
# Create code from Qt rc files
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/" "${CMAKE_CURRENT_SOURCE_DIR}/Dialogs" "${CMAKE_CURRENT_SOURCE_DIR}/Editors")

# Uncomment to be able to use local grpc_cpp_plugin
# set(GRPC_EXE "/usr/local/bin/grpc_cpp_plugin")

# Include ENIGMA things
set(ENIGMA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Submodules/enigma-dev CACHE PATH "ENIGMA directory")
include_directories("${CMAKE_BINARY_DIR}/Submodules/enigma-dev/shared/protos/" "${ENIGMA_DIR}/CommandLine/libEGM" "${ENIGMA_DIR}/shared")

# Populate a CMake variable with the sources
set(RGM_SOURCES
  Models/RepeatedMessageModel.cpp
  Models/TreeSortFilterProxyModel.cpp
  Models/PrimitiveModel.cpp
  Models/EventsListModel.cpp
  Models/ModelMapper.cpp
  Models/RepeatedModel.cpp
  Models/MessageModel.cpp
  Models/TreeModel.cpp
  Models/EventTypesListModel.cpp
  Models/ResourceModelMap.cpp
  Models/ImmediateMapper.cpp
  Models/ProtoModel.cpp
  Models/EventTypesListSortFilterProxyModel.cpp
  Models/RepeatedSortFilterProxyModel.cpp
  Components/Utility.cpp
  Components/RecentFiles.cpp
  Components/QMenuView.cpp
  Components/ArtManager.cpp
  Editors/PathEditor.cpp
  Editors/RoomEditor.cpp
  Editors/ObjectEditor.cpp
  Editors/FontEditor.cpp
  Editors/SpriteEditor.cpp
  Editors/BackgroundEditor.cpp
  Editors/SettingsEditor.cpp
  Editors/ShaderEditor.cpp
  Editors/SoundEditor.cpp
  Editors/IncludeEditor.cpp
  Editors/InformationEditor.cpp
  Editors/BaseEditor.cpp
  Editors/ScriptEditor.cpp
  Editors/CodeEditor.cpp
  Editors/TimelineEditor.cpp
  Editors/VisualShaderEditor.cpp
  main.cpp
  Plugins/RGMPlugin.cpp
  Dialogs/EventArgumentsDialog.cpp
  Dialogs/TimelineChangeMoment.cpp
  Dialogs/PreferencesDialog.cpp
  Dialogs/KeyBindingPreferences.cpp
  Utils/ProtoManip.cpp
  Utils/FieldPath.cpp
  MainWindow.cpp
  Widgets/BackgroundView.cpp
  Widgets/ColorPicker.cpp
  Widgets/AssetView.cpp
  Widgets/PathView.cpp
  Widgets/RoomView.cpp
  Widgets/SpriteView.cpp
  Widgets/CodeWidget.cpp
  Widgets/SpriteSubimageListView.cpp
  Widgets/StackedCodeWidget.cpp
  Widgets/AssetScrollAreaBackground.cpp
)

set(RGM_HEADERS
  Models/MessageModel.h
  Models/ProtoModel.h
  Models/TreeModel.h
  Models/PrimitiveModel.h
  Models/RepeatedMessageModel.h
  Models/EventTypesListSortFilterProxyModel.h
  Models/ResourceModelMap.h
  Models/EventTypesListModel.h
  Models/ImmediateMapper.h
  Models/RepeatedModel.h
  Models/EventsListModel.h
  Models/ModelMapper.h
  Models/RepeatedSortFilterProxyModel.h
  Models/TreeSortFilterProxyModel.h
  main.h
  Components/RecentFiles.h
  Components/QMenuView_p.h
  Components/Utility.h
  Components/QMenuView.h
  Components/Logger.h
  Components/ArtManager.h
  Editors/ObjectEditor.h
  Editors/PathEditor.h
  Editors/ScriptEditor.h
  Editors/CodeEditor.h
  Editors/SoundEditor.h
  Editors/IncludeEditor.h
  Editors/InformationEditor.h
  Editors/TimelineEditor.h
  Editors/SettingsEditor.h
  Editors/ShaderEditor.h
  Editors/RoomEditor.h
  Editors/BaseEditor.h
  Editors/FontEditor.h
  Editors/SpriteEditor.h
  Editors/BackgroundEditor.h
  Editors/VisualShaderEditor.h
  Plugins/RGMPlugin.h
  MainWindow.h
  Dialogs/EventArgumentsDialog.h
  Dialogs/PreferencesDialog.h
  Dialogs/PreferencesKeys.h
  Dialogs/TimelineChangeMoment.h
  Dialogs/KeyBindingPreferences.h
  Utils/SafeCasts.h
  Utils/ProtoManip.h
  Utils/FieldPath.h
  Utils/QBoilerplate.h
  Widgets/BackgroundView.h
  Widgets/CodeWidget.h
  Widgets/ResourceSelector.h
  Widgets/ColorPicker.h
  Widgets/AssetScrollArea.h
  Widgets/SpriteView.h
  Widgets/AssetView.h
  Widgets/PathView.h
  Widgets/StackedCodeWidget.h
  Widgets/SpriteSubimageListView.h
  Widgets/AssetScrollAreaBackground.h
  Widgets/RoomView.h
)

set(RGM_UI
    MainWindow.ui
    Dialogs/TimelineChangeMoment.ui
    Dialogs/PreferencesDialog.ui
    Dialogs/AddImageDialog.ui
    Editors/CodeEditor.ui
    Editors/SoundEditor.ui
    Editors/BackgroundEditor.ui
    Editors/SpriteEditor.ui
    Editors/PathEditor.ui
    Editors/SettingsEditor.ui
    Editors/RoomEditor.ui
    Editors/FontEditor.ui
    Editors/TimelineEditor.ui
    Editors/ObjectEditor.ui
    Editors/InformationEditor.ui
    Editors/IncludeEditor.ui
)

set(RGM_RC
    images.qrc
    resources.rc
)

# Check for QScintilla
find_library(LIB_QSCINTILLA NAMES qscintilla2 qscintilla2_qt5 qscintilla2-qt5 qt5scintilla2 libqscintilla2_qt5.so libqscintilla2.so libqscintilla2.a qscintilla2.lib)
if (NOT LIB_QSCINTILLA)
    set(EDITOR_SOURCES Widgets/CodeWidgetPlain.cpp)
else()
    set(EDITOR_SOURCES Widgets/CodeWidgetScintilla.cpp)
endif()

set(RGM_SOURCES ${RGM_SOURCES} Plugins/ServerPlugin.cpp)
set(RGM_HEADERS ${RGM_HEADERS} Plugins/ServerPlugin.h)

# Tell CMake to create the RadialGM executable
add_executable(${EXE} WIN32 ${RGM_UI} ${RGM_HEADERS} ${RGM_SOURCES} ${EDITOR_SOURCES} ${RGM_RC})

# we do this even in release mode for "Editor Diagnostics"
target_compile_definitions(${EXE} PUBLIC QT_MESSAGELOGCONTEXT)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT WIN32)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
  set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
endif()

message(STATUS "Initial build flags:")
set(CompilerFlags
    CMAKE_C_FLAGS_DEBUG
    CMAKE_C_FLAGS_MINSIZEREL
    CMAKE_C_FLAGS_RELEASE
    CMAKE_C_FLAGS_RELWITHDEBINFO
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_MINSIZEREL
    CMAKE_CXX_FLAGS_RELEASE
    CMAKE_CXX_FLAGS_RELWITHDEBINFO
    )

foreach(CompilerFlag ${CompilerFlags})
  message(STATUS "  '${CompilerFlag}': ${${CompilerFlag}}")
endforeach()

if (LIB_QSCINTILLA)
    message(STATUS "Found QScintilla: ${LIB_QSCINTILLA}")
    add_definitions(-DQSCINTILLA_DLL)
    target_link_libraries(${EXE} PRIVATE ${LIB_QSCINTILLA})
endif()

# Find PugiXML
find_library(LIB_PUGIXML NAMES pugixml)
target_link_libraries(${EXE} PRIVATE ${LIB_PUGIXML})

# Find RapidJSON
find_package(RapidJSON REQUIRED)
include_directories(${EXE} PRIVATE ${RAPIDJSON_INCLUDE_DIRS})

# Find yaml-cpp
find_package(yaml-cpp CONFIG REQUIRED)
target_link_libraries(${EXE} PRIVATE yaml-cpp)

# Find gRPC
find_package(gRPC CONFIG REQUIRED)
target_link_libraries(${EXE} PRIVATE gRPC::gpr gRPC::grpc gRPC::grpc++)

# Find Protobuf
find_package(Protobuf CONFIG REQUIRED)
target_link_libraries(${EXE} PRIVATE protobuf::libprotobuf)

# Find OpenSSL
find_package(OpenSSL REQUIRED)
target_link_libraries(${EXE} PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Find Qt
find_package(Qt5 COMPONENTS Core Widgets Gui PrintSupport Multimedia REQUIRED)
target_link_libraries(${EXE} PRIVATE Qt5::Core Qt5::Widgets Qt5::Gui Qt5::PrintSupport Qt5::Multimedia)

# LibProto
# Arrangement of these is important: shared depends on proto and emake depends on all of them
# We need to cache the library names first so we can build on top of them
add_subdirectory(Submodules/enigma-dev/shared/protos)
add_subdirectory(Submodules/enigma-dev/shared)
add_subdirectory(Submodules/enigma-dev/CommandLine/libEGM)
add_dependencies(${EXE} ${LIB_EGM})
target_link_libraries(${EXE} PRIVATE ${LIB_EGM} ${LIB_PROTO} ${SHARED_LIB})

# Find FreeType
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})
target_link_libraries(${EXE} PRIVATE ${FREETYPE_LIBRARIES})

# Find JPEG
find_library(LIB_JPEG NAMES jpeg)
target_link_libraries(${EXE} PRIVATE ${LIB_JPEG})

# Find HarfBuzz
find_library(LIB_HARFBUZZ NAMES harfbuzz)
target_link_libraries(${EXE} PRIVATE ${LIB_HARFBUZZ})

# Find Pcre2
find_library(LIB_PCRE2 NAMES "pcre2-16")
target_link_libraries(${EXE} PRIVATE ${LIB_PCRE2})

# Find double-conversion
find_library(LIB_DOUBLE_CONVERSION NAMES double-conversion)
target_link_libraries(${EXE} PRIVATE ${LIB_DOUBLE_CONVERSION})

# nodeeditor
# FIXME: In order for BUILD_DEBUG_POSTFIX_D and USE_QT6 to be set correctly, you will need to configure the project twice: https://cmake.org/cmake/help/latest/policy/CMP0077.html
if (CMAKE_BUILD_TYPE MATCHES "Debug")
  set(BUILD_DEBUG_POSTFIX_D ON)
endif()
set(USE_QT6 OFF) # We use Qt5
add_subdirectory(Submodules/nodeeditor)

if(WIN32)
  # Windows is a turd
  target_link_libraries(${EXE} PRIVATE Ws2_32 Wtsapi32 Wldap32 Crypt32 Winmm Userenv Netapi32 version Dwmapi Imm32)
endif(WIN32)

if(MSVC)
  # Default MSVC warnings are overkill. Set to Lvl 2
  target_compile_options(${EXE} PRIVATE /W1)
endif()

if (RGM_BUILD_EMAKE)
  add_subdirectory(Submodules/enigma-dev/CompilerSource)
  add_subdirectory(Submodules/enigma-dev/CommandLine/emake)
  add_dependencies(${EXE} ${CLI_TARGET})
endif()

add_custom_command(
    TARGET ${EXE}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/Submodules/enigma-dev/CommandLine/emake/${CLI_TARGET}${CMAKE_EXECUTABLE_SUFFIX}
        ${CMAKE_BINARY_DIR}/Submodules/enigma-dev/CommandLine/libEGM/${CMAKE_SHARED_LIBRARY_PREFIX}${LIB_EGM}${CMAKE_DEBUG_POSTFIX}${CMAKE_SHARED_LIBRARY_SUFFIX}
        ${CMAKE_BINARY_DIR}/Submodules/enigma-dev/CompilerSource/${CMAKE_SHARED_LIBRARY_PREFIX}${COMPILER_LIB}${CMAKE_DEBUG_POSTFIX}${CMAKE_SHARED_LIBRARY_SUFFIX}
        ${CMAKE_BINARY_DIR}/Submodules/enigma-dev/shared/protos/${CMAKE_SHARED_LIBRARY_PREFIX}${LIB_PROTO}${CMAKE_DEBUG_POSTFIX}${CMAKE_SHARED_LIBRARY_SUFFIX}
        ${CMAKE_BINARY_DIR}/Submodules/enigma-dev/shared/${CMAKE_SHARED_LIBRARY_PREFIX}${SHARED_LIB}${CMAKE_DEBUG_POSTFIX}${CMAKE_SHARED_LIBRARY_SUFFIX}
        ${CMAKE_CURRENT_SOURCE_DIR}/Submodules/enigma-dev
    COMMENT "Copying exes to ENIGMA's root directory"
)

install(TARGETS ${EXE} RUNTIME DESTINATION .)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${EXE}.dir/Debug/${EXE}.pdb" DESTINATION . OPTIONAL)

set(RGM_APP "${CMAKE_INSTALL_PREFIX}/${EXE}${CMAKE_EXECUTABLE_SUFFIX}")
set(EMAKE_APP "${CMAKE_INSTALL_PREFIX}/${CLI_TARGET}${CMAKE_EXECUTABLE_SUFFIX}")

if (MSVC)
file(TO_CMAKE_PATH ${VCPKG_ROOT} VCPKG_ROOT)
  if (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(SEARCH_PATHS "${VCPKG_ROOT}/installed/x64-windows/debug/bin/")
  else()
    set(SEARCH_PATHS "${VCPKG_ROOT}/installed/x64-windows/bin/")
  endif()
else()
  set(LIBS "${CMAKE_INSTALL_PREFIX}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIB_EGM}${CMAKE_SHARED_LIBRARY_SUFFIX}"
           "${CMAKE_INSTALL_PREFIX}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIB_PROTO}${CMAKE_SHARED_LIBRARY_SUFFIX}"
           "${CMAKE_INSTALL_PREFIX}/${CMAKE_SHARED_LIBRARY_PREFIX}${SHARED_LIB}${CMAKE_SHARED_LIBRARY_SUFFIX}")
endif()

if (WIN32)

  set(WINDEPLOY_ARGS "--no-translations --no-angle")

  if (MSVC)
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
      set(CMAKE_INSTALL_DEBUG_LIBRARIES_ONLY TRUE)
      set(WINDEPLOY_ARGS "${WINDEPLOY_ARGS} --pdb --no-compiler-runtime")
    else()
      set(WINDEPLOY_ARGS "${WINDEPLOY_ARGS} --no-compiler-runtime")
    endif()
    set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
    include(InstallRequiredSystemLibraries)
    install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION .)
  endif()

  install(CODE "
          include(BundleUtilities)
          fixup_bundle(\"${RGM_APP}\" \"${LIBS}\" \"${SEARCH_PATHS}\")
          fixup_bundle(\"${EMAKE_APP}\" \"${LIBS}\" \"${SEARCH_PATHS}\")
          execute_process(COMMAND windeployqt.exe ${WINDEPLOY_ARGS} ${RGM_APP})
          ")
endif()
