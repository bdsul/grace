set(CROSSOVER_HEADERS
    "GECrossover.hpp"
    # "FloatCrossover.hpp"
    )

    target_include_directories(${PROJECT_NAME} PRIVATE CMAKE_CURRENT_SOURCE_DIR)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${CROSSOVER_HEADERS} DESTINATION include/${PROJECT_NAME}/operators/crossover)
    