set(ALGORITHM_HEADERS
    "GeneticAlgorithm.hpp"
    )

target_include_directories(${PROJECT_NAME} PRIVATE CMAKE_CURRENT_SOURCE_DIR)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${ALGORITHM_HEADERS} DESTINATION include/${PROJECT_NAME}/algorithm)