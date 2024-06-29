project(gx2shader-decompiler)

include_directories(".")
include_directories("../../src/libdecaf/src")
include_directories("../../src/libgpu")
include_directories("../../src/libgpu/src")

file(GLOB_RECURSE SOURCE_FILES *.cpp)
file(GLOB_RECURSE HEADER_FILES *.h)

add_executable(gx2shader-decompiler ${SOURCE_FILES} ${HEADER_FILES})
set_target_properties(gx2shader-decompiler PROPERTIES FOLDER tools)

target_link_libraries(gx2shader-decompiler
    libdecaf
    libgpu
    excmd)

install(TARGETS gx2shader-decompiler RUNTIME DESTINATION "${DECAF_INSTALL_BINDIR}")
