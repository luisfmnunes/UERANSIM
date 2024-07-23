macro(copy_headers H_FILES)
    foreach(H_FILE ${H_FILES})
        
        # Get Relative Path of Header File
        file(RELATIVE_PATH H_REL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/.. ${H_FILE})
        
        # Set Output Path of Header File
        set(DEST_PATH "${COMPILED_INCLUDE_DIR}/${H_REL_DIR}")

        # Get Directory of Build Include Path
        get_filename_component(I_DIR ${DEST_PATH} PATH)

        # Create the Directory in the Build Destination
        file(MAKE_DIRECTORY ${I_DIR})

        # Copy the Header File to the Build Destination
        file(COPY ${H_FILE} DESTINATION ${I_DIR})

    endforeach()
endmacro()