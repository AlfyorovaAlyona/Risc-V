cmake_minimum_required(VERSION 3.9)
project(risc_v)

set(CMAKE_CXX_STANDARD 17)

if (LIBELF_LIBRARIES AND LIBELF_INCLUDE_DIRS)
    set (LibElf_FIND_QUIETLY TRUE)
endif (LIBELF_LIBRARIES AND LIBELF_INCLUDE_DIRS)

find_path (LIBELF_INCLUDE_DIRS
        NAMES
        libelf.h
        PATHS
        /usr/include
        /usr/include/libelf
        /usr/local/include
        /usr/local/include/libelf
        /opt/local/include
        /opt/local/include/libelf
        /sw/include
        /sw/include/libelf
        ENV CPATH)

find_library (LIBELF_LIBRARIES
        NAMES
        elf
        PATHS
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        /sw/lib
        ENV LIBRARY_PATH
        ENV LD_LIBRARY_PATH)

include (FindPackageHandleStandardArgs)


# handle the QUIETLY and REQUIRED arguments and set LIBELF_FOUND to TRUE if all listed variables are TRUE
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibElf DEFAULT_MSG
        LIBELF_LIBRARIES
        LIBELF_INCLUDE_DIRS)

SET(CMAKE_REQUIRED_LIBRARIES elf)
INCLUDE(CheckCXXSourceCompiles)
CHECK_CXX_SOURCE_COMPILES("#include <libelf.h>
int main() {
  Elf *e = (Elf*)0;
  size_t sz;
  elf_getshdrstrndx(e, &sz);
  return 0;
}" ELF_GETSHDRSTRNDX)

mark_as_advanced(LIBELF_INCLUDE_DIRS LIBELF_LIBRARIES ELF_GETSHDRSTRNDX)

add_executable(risc_v main.c op.h instruction_decoder.h instruction_decoder.c op.c load_elf.c load_elf.h)
target_link_libraries(risc_v ${LIBELF_LIBRARIES})