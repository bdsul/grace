set(SELECTION_HEADERS
    "FloatSelection.hpp"
    )

    target_include_directories(${PROJECT_NAME} PRIVATE CMAKE_CURRENT_SOURCE_DIR)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${SELECTION_HEADERS} DESTINATION include/${PROJECT_NAME}/operators/selection)
    