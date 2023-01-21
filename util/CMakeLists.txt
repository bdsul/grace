set(UTIL_HEADERS
    "DerivationTree.hpp"
    )

set(UTIL_SOURCES
    "DerivationTree.cpp"
)

target_sources(${PROJECT_NAME} PRIVATE ${UTIL_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE CMAKE_CURRENT_SOURCE_DIR)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${UTIL_HEADERS} DESTINATION include/${PROJECT_NAME}/util)
