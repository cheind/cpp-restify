get_filename_component(SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(${SELF_DIR}/cpp-restify-targets.cmake)
get_filename_component(CPPRestify_INCLUDE_DIRS "${SELF_DIR}/../../inc/" ABSOLUTE)