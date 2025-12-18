# Helper function to add a game executable
function(add_game GAME_NAME)
    # Get all source files for this game
    file(GLOB GAME_SOURCES "${GAME_NAME}/*.cpp")

    # Create executable
    add_executable(${GAME_NAME} ${GAME_SOURCES})

    # Link against engine library if it exists
    if(TARGET delphinis_engine)
        target_link_libraries(${GAME_NAME} delphinis_engine)
    else()
        # If no engine library yet, link dependencies directly
        target_link_libraries(${GAME_NAME}
            glfw
            GLEW::GLEW
            ${OPENGL_LIBRARIES}
        )

        # Platform-specific settings
        if(APPLE)
            target_link_libraries(${GAME_NAME}
                "-framework Cocoa"
                "-framework IOKit"
                "-framework CoreVideo"
            )
        endif()
    endif()

    # Compiler warnings
    if(MSVC)
        target_compile_options(${GAME_NAME} PRIVATE /W4)
    else()
        target_compile_options(${GAME_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

# Add games here - one line per game
add_game(hello)
add_game(pong)
