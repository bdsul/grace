set(MAPPER_HEADERS
    "GEMapper.hpp"
    # "FloatMapper.hpp"
    )

    target_include_directories(${PROJECT_NAME} PRIVATE CMAKE_CURRENT_SOURCE_DIR)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${MAPPER_HEADERS} DESTINATION include/${PROJECT_NAME}/operators/mapper)
    