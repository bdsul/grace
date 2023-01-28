set(TERMINATION_HEADERS
    "GETermination.hpp"
    # "FloatTermination.hpp"
    )

    target_include_directories(${PROJECT_NAME} PRIVATE CMAKE_CURRENT_SOURCE_DIR)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${TERMINATION_HEADERS} DESTINATION include/${PROJECT_NAME}/operators/termination)
    