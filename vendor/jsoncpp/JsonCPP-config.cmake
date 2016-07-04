get_filename_component(SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(${SELF_DIR}/jsoncpp-targets.cmake)
get_filename_component(jsoncpp_INCLUDE_DIRS "${SELF_DIR}/../../inc/json" ABSOLUTE)