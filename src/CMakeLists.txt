add_executable(main
	main.cpp
	command_line_options.cpp
)
target_link_libraries(main
	core
	cl_input
)