function(combine_archives output_archive)
    # Generate the MRI file for ar to consume.
    # Note that a separate file must be generated for each build configuration.
    set(mri_file ${CMAKE_BINARY_DIR}/$<CONFIG>/${output_archive}.mri)
    set(mri_file_content "create ${CMAKE_BINARY_DIR}/$<CONFIG>/lib${output_archive}.a\n")
    FOREACH(in_target ${ARGN})
        string(APPEND mri_file_content "addlib $<TARGET_FILE:${in_target}>\n")
    ENDFOREACH()
    string(APPEND mri_file_content "save\n")
    string(APPEND mri_file_content "end\n")
    file(GENERATE
            OUTPUT ${mri_file}
            CONTENT ${mri_file_content}
            )

    # Create a dummy file for the combined library
    # This dummy file depends on all the input targets so that the combined library is regenerated if any of them changes.
    set(output_archive_dummy_file ${CMAKE_BINARY_DIR}/${output_archive}.dummy.cpp)
    add_custom_command(OUTPUT ${output_archive_dummy_file}
            COMMAND touch ${output_archive_dummy_file}
            DEPENDS ${ARGN})

    add_library(${output_archive} STATIC ${output_archive_dummy_file})

    target_compile_options(${output_archive} PUBLIC -Wall -Wextra -pedantic -Wno-unused-parameter)

    # Add a custom command to combine the archives after the static library is "built".
    add_custom_command(TARGET ${output_archive}
            POST_BUILD
            COMMAND ar -M < ${mri_file}
            COMMENT "Combining static libraries for ${output_archive}"
            )
    FOREACH(in_target ${ARGN})
        target_link_libraries(${output_archive} PUBLIC ${in_target})
    ENDFOREACH()

    # Clean up the dummy file after the combined library is built.
    file(REMOVE ${CMAKE_BINARY_DIR}/${output_archive}.dummy.cpp)

endfunction(combine_archives)