set(POPULATION_HEADERS
    "GEPopulation.hpp"
    "FloatPopulation.hpp"
    )

    target_include_directories(${PROJECT_NAME} PRIVATE CMAKE_CURRENT_SOURCE_DIR)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${POPULATION_HEADERS} DESTINATION include/${PROJECT_NAME}/population)
