get_filename_component(SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(${SELF_DIR}/jsoncpp-targets.cmake)
get_filename_component(JsonCPP_INCLUDE_DIRS "${SELF_DIR}/../../inc/" ABSOLUTE)