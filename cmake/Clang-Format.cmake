# additional target to perform clang-format run, requires clang-format - See more at: https://arcanis.me/en/2015/10/17/cppcheck-and-clang-format/#sthash.vPFJkfVz.dpuf
# get all project files
file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h) 
add_custom_target(
 clang-format
 COMMAND clang-format
 -style=file -i
 ${ALL_SOURCE_FILES}
 )
