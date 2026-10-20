cmake_minimum_required(VERSION 3.24)
project(c12_port LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Python3 REQUIRED COMPONENTS Interpreter)
option(C12_BUILD_RUNTIME "Build the C-12 product" ON)

if(C12_BUILD_RUNTIME)
  set(PSXPORT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/psxport" CACHE PATH "Shared psxport checkout")
  if(NOT EXISTS "${PSXPORT_DIR}/runtime/psx/cd/stock_libcd.h")
    message(FATAL_ERROR "C-12 requires psxport with the stock library classes at ${PSXPORT_DIR}")
  endif()
  add_subdirectory("${PSXPORT_DIR}" framework EXCLUDE_FROM_ALL)

  file(READ "${CMAKE_CURRENT_SOURCE_DIR}/title.json" C12_IDENTITY_JSON)
  set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/title.json")
  string(JSON C12_EXECUTABLE GET "${C12_IDENTITY_JSON}" executable)
  string(JSON C12_SIZE GET "${C12_IDENTITY_JSON}" size)
  string(JSON C12_SHA256 GET "${C12_IDENTITY_JSON}" sha256)
  string(JSON C12_DISC_EXECUTABLE GET "${C12_IDENTITY_JSON}" disc_executable)
  string(JSON C12_EXECUTABLE_ENTRY GET "${C12_IDENTITY_JSON}" header entry)
  string(JSON C12_EXECUTABLE_GP GET "${C12_IDENTITY_JSON}" header gp)
  string(JSON C12_EXECUTABLE_TEXT GET "${C12_IDENTITY_JSON}" header text_address)
  string(JSON C12_EXECUTABLE_TEXT_SIZE GET "${C12_IDENTITY_JSON}" header text_size)
  string(JSON C12_EXECUTABLE_STACK GET "${C12_IDENTITY_JSON}" header stack_address)
  string(JSON C12_EXECUTABLE_STACK_OFFSET GET "${C12_IDENTITY_JSON}" header stack_offset)
  configure_file(game/facts/executable_identity.h.in metadata/executable_identity.h @ONLY)

  add_library(c12_runtime STATIC
    game/cd/cd_command_completion.cpp
    game/frame/frame_driver.cpp
    game/frame/vsync_continuation.cpp
    game/render/cell_collector.cpp
    game/render/cell_output.cpp
    game/render/draw_distance.cpp
    game/render/fog_table.cpp
    game/render/view_frustum.cpp
    game/render/world_mesh_pass.cpp
    game/runtime/runtime.cpp
    game/runtime/session.cpp
    game/title/enhancements.cpp
    game/widescreen/widescreen_policy.cpp
  )
  target_link_libraries(c12_runtime PUBLIC psxport)
  target_include_directories(c12_runtime PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/game"
    "${CMAKE_CURRENT_BINARY_DIR}/metadata")
  psxport_add_product(
    c12_port
    CATALOG c12::createCatalog
    HEADER entry/catalog.h
    SOURCES game/entry/catalog.cpp
    LIBRARIES c12_runtime)
endif()
