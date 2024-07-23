file(GLOB HDR_FILES *.h)
file(GLOB SRC_FILES *.c)

add_library(asn-rrc ${HDR_FILES} ${SRC_FILES})
target_link_libraries(asn-rrc asn-asn1c)

# Export Copy of Header Files to Build INCLUDE_DIR for External Projects
copy_headers("${HDR_FILES}")