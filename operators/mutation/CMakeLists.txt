set(MUTATION_HEADERS
    "GEMutation.hpp"
    # "FloatMutation.hpp"
    )

    target_include_directories(${PROJECT_NAME} PRIVATE CMAKE_CURRENT_SOURCE_DIR)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${MUTATION_HEADERS} DESTINATION include/${PROJECT_NAME}/operators/mutation)
    