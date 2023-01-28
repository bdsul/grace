set(REPLACEMENT_HEADERS
    "FloatReplacement.hpp"
    )

    target_include_directories(${PROJECT_NAME} PRIVATE CMAKE_CURRENT_SOURCE_DIR)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${REPLACEMENT_HEADERS} DESTINATION include/${PROJECT_NAME}/operators/replacement)
    