file(GLOB_RECURSE sources *.cpp)

add_executable(bnksinf ${sources})
target_include_directories(bnksinf PUBLIC "../include")
target_compile_features(bnksinf PUBLIC cxx_std_23)

set_target_properties(bnksinf
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

add_custom_command(
	TARGET bnksinf POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_BINARY_DIR}/../existing_banks.banksinfo"
		"${CMAKE_BINARY_DIR}"
	COMMENT "Copying existing_banks.banksinfo"
)
