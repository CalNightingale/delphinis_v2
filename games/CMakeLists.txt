# Helper function to add a game executable
function(add_game GAME_NAME)
    # Get all source files for this game (including subdirectories)
    file(GLOB_RECURSE GAME_SOURCES "${GAME_NAME}/*.cpp")

    # Create executable
    add_executable(${GAME_NAME} ${GAME_SOURCES})

    # Include game-specific directories for headers
    target_include_directories(${GAME_NAME} PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/${GAME_NAME}"
    )

    if(EMSCRIPTEN)
        # Emscripten-specific settings
        target_link_libraries(${GAME_NAME} delphinis_engine)

        # Set output to HTML
        set_target_properties(${GAME_NAME} PROPERTIES SUFFIX ".html")

        # Emscripten linker flags
        target_link_options(${GAME_NAME} PRIVATE
            -sUSE_GLFW=3
            -sFULL_ES3=1
            -sALLOW_MEMORY_GROWTH=1
            -sMAXIMUM_MEMORY=512MB
            --shell-file ${CMAKE_SOURCE_DIR}/web/shell.html
        )

        target_compile_options(${GAME_NAME} PRIVATE
            -sUSE_GLFW=3
        )
    else()
        # Native build
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
    endif()
endfunction()

# Add games here - one line per game
add_game(hello)

# Add preload-file for hello game assets (for web build)
if(EMSCRIPTEN AND TARGET hello)
    target_link_options(hello PRIVATE
        --preload-file ${CMAKE_SOURCE_DIR}/games/hello/assets@/games/hello/assets
    )
endif()

add_game(pong)

# Add preload-file for pong game assets (for web build)
if(EMSCRIPTEN AND TARGET pong)
    target_link_options(pong PRIVATE
        --preload-file ${CMAKE_SOURCE_DIR}/games/pong/assets@/games/pong/assets
    )
endif()
