set(GRAMMAR_HEADERS
    "Symbol.hpp"
    "Choice.hpp"
    "Rule.hpp"
    "CFRule.hpp"
    "Grammar.hpp"
    "CFGrammar.hpp"
    )

set(GRAMMAR_SOURCES
    "Symbol.cpp"
    "Choice.cpp"
    "CFRule.cpp"
    "CFGrammar.cpp"
    )

target_sources(${PROJECT_NAME} PRIVATE ${GRAMMAR_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE CMAKE_CURRENT_SOURCE_DIR)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${GRAMMAR_HEADERS} DESTINATION include/${PROJECT_NAME}/grammar)
