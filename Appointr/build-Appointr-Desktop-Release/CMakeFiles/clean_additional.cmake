# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "Appointr_autogen"
  "CMakeFiles/Appointr_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Appointr_autogen.dir/ParseCache.txt"
  )
endif()
