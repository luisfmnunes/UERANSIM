file(GLOB_RECURSE HDR_FILES *.hpp)
file(GLOB_RECURSE SRC_FILES *.cpp)

add_library(onppgnb ${HDR_FILES} ${SRC_FILES})

target_compile_options(onppgnb PRIVATE -Wall -Wextra -pedantic -Wno-unused-parameter)

target_link_libraries(onppgnb gnb)
target_link_libraries(onppgnb asn-ngap)
target_link_libraries(onppgnb asn-rrc)
target_link_libraries(onppgnb common-lib) 

# Export Copy of Header Files to Build INCLUDE_DIR for External Projects
copy_headers("${HDR_FILES}")