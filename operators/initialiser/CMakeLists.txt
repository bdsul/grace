set(INITIALISER_HEADERS
    "GEInitialiser.hpp"
    # "FloatInitialiser.hpp"
    )

    target_include_directories(${PROJECT_NAME} PRIVATE CMAKE_CURRENT_SOURCE_DIR)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${INITIALISER_HEADERS} DESTINATION include/${PROJECT_NAME}/operators/initialiser)
    