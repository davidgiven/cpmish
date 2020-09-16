cmake_minimum_required (VERSION 2.8)
project (z80ex)

set (API_REVISION 1)
set (VERSION_MAJOR 1)
set (VERSION_MINOR 21)
set (RELEASE_TYPE "")
set (VERSION_STR "${API_REVISION}.${VERSION_MAJOR}.${VERSION_MINOR}${RELEASE_TYPE}")

option (OPSTEP_FAST_AND_ROUGH "Fast and rough opcode step emulation mode" Off)

#ALL_CFLAGS := -fPIC -fno-common -ansi -pedantic -Wall -pipe -O2 -I. -I./include
if (CMAKE_COMPILER_IS_GNUCC)
    set (CMAKE_C_FLAGS "-fPIC -fno-common -ansi -pedantic -Wall -pipe -O2")
endif ()

include_directories(BEFORE . include)

include (TestBigEndian)
test_big_endian(BIG_ENDIAN)
#endianness (one of: WORDS_LITTLE_ENDIAN, WORDS_BIG_ENDIAN)
if (BIG_ENDIAN)
    set (ENDIANNESS WORDS_BIG_ENDIAN)
else ()
    set (ENDIANNESS WORDS_LITTLE_ENDIAN)
endif ()

add_definitions (-D${ENDIANNESS} -DZ80EX_VERSION_STR=${VERSION_STR} -DZ80EX_API_REVISION=${API_REVISION} -DZ80EX_VERSION_MAJOR=${VERSION_MAJOR} -DZ80EX_VERSION_MINOR=${VERSION_MINOR} -DZ80EX_RELEASE_TYPE=${RELEASE_TYPE})

if (OPSTEP_FAST_AND_ROUGH)
    add_definitions (-DZ80EX_OPSTEP_FAST_AND_ROUGH)
endif ()

set (z80ex_sources z80ex.c)
add_library (z80ex-static STATIC ${z80ex_sources})
set_target_properties (z80ex-static PROPERTIES OUTPUT_NAME z80ex)
if (NOT DEFINED Z80EX_STATIC_ONLY)
    add_library (z80ex SHARED ${z80ex_sources})
# Affects Win32 only: avoid dynamic/static *.lib files naming conflict 
    set_target_properties (z80ex-static PROPERTIES PREFIX "lib")
endif ()

set (z80ex_dasm_sources z80ex_dasm.c)
add_library (z80ex_dasm-static STATIC ${z80ex_dasm_sources})
set_target_properties (z80ex_dasm-static PROPERTIES OUTPUT_NAME z80ex_dasm)
if (NOT DEFINED Z80EX_STATIC_ONLY)
    add_library (z80ex_dasm SHARED ${z80ex_dasm_sources})
# Affects Win32 only: avoid dynamic/static *.lib files naming conflict 
    set_target_properties (z80ex_dasm-static PROPERTIES PREFIX "lib")
endif ()

if (NOT DEFINED Z80EX_STATIC_ONLY)
    set_target_properties(z80ex z80ex_dasm
        PROPERTIES VERSION ${VERSION_STR} SOVERSION ${API_REVISION}
    )
endif ()
set_target_properties(z80ex-static z80ex_dasm-static
    PROPERTIES VERSION ${VERSION_STR} SOVERSION ${API_REVISION}
)

if ("${CMAKE_C_IMPLICIT_LINK_DIRECTORIES}" MATCHES "lib64")
    set (LIB_DIR "lib64")
else ()
    set (LIB_DIR "lib")
endif ()
if (NOT DEFINED Z80EX_STATIC_ONLY)
    install (TARGETS z80ex z80ex_dasm LIBRARY DESTINATION ${LIB_DIR} ARCHIVE DESTINATION ${LIB_DIR})
endif ()
install (TARGETS z80ex-static z80ex_dasm-static LIBRARY DESTINATION ${LIB_DIR} ARCHIVE DESTINATION ${LIB_DIR})
install (DIRECTORY include/ DESTINATION include/z80ex PATTERN "*.h" )
