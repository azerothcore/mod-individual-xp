CU_SET_PATH("CMAKE_INDIVIDUAL_XP_DIR" "${CMAKE_CURRENT_LIST_DIR}")

AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/Individual_XP.cpp")

AC_ADD_SCRIPT_LOADER("Individual_XP" "${CMAKE_CURRENT_LIST_DIR}/src/loader.h")

AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/mod_individualxp.conf.dist")

message("-- Individual-XP CONFIGURED --")
