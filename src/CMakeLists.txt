ENABLE_LANGUAGE(ASM)

set(CMAKE_ASM_CREATE_SHARED_LIBRARY "ld <CMAKE_SHARED_LIBRARY_CREATE_ASM_FLAGS> <CMAKE_SHARED_LIBRARY_SONAME_ASM_FLAG><TARGET_SONAME> <LINK_FLAGS> -o <TARGET> <OBJECTS> <LINK_LIBRARIES>")

file(GLOB coroutine ./coroutine/*)

add_library(fiber SHARED ${coroutine})