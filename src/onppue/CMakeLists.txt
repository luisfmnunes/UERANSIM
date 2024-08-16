file(GLOB_RECURSE HDR_FILES *.hpp)
file(GLOB_RECURSE SRC_FILES *.cpp)

add_library(onppue ${HDR_FILES} ${SRC_FILES})

target_compile_options(onppue PRIVATE -Wall -Wextra -pedantic -Wno-unused-parameter)

target_link_libraries(onppue gnb)
target_link_libraries(onppue asn-ngap)
target_link_libraries(onppue asn-rrc)
target_link_libraries(onppue common-lib) 

# Export Copy of Header Files to Build INCLUDE_DIR for External Projects
copy_headers("${HDR_FILES}")