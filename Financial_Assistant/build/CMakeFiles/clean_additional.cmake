# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "CMakeFiles/Financial_Assistant_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Financial_Assistant_autogen.dir/ParseCache.txt"
  "Financial_Assistant_autogen"
  )
endif()
