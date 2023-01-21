set(GENOME_HEADERS
    "GEGenome.hpp"
    "FloatGenome.hpp"
    )

target_include_directories(${PROJECT_NAME} PRIVATE CMAKE_CURRENT_SOURCE_DIR)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${GENOME_HEADERS} DESTINATION include/${PROJECT_NAME}/genome)
