project(CS6_conn_test_CSTP)

# Targets  
add_executable(CS6_conn_test_CSTP
    src/main_CS6_conn_test_CSTP.cpp
)

# definitions     
if(CARLSIM_NO_CUDA)
    target_compile_definitions(CS6_conn_test_CSTP PUBLIC
        -D__NO_CUDA__ )
endif()

# Linking    
target_link_libraries(CS6_conn_test_CSTP PRIVATE
    carlsim
    carlsim-stopwatch
)
target_include_directories(CS6_conn_test_CSTP PRIVATE
    ../../tools/stopwatch
)
   
        
# Installation   
install(TARGETS CS6_conn_test_CSTP
    EXPORT carlsim
    RUNTIME 
        DESTINATION $<$<CONFIG:Debug>:debug/>samples 
        COMPONENT CS6_conn_test_CSTP
        )

#install(FILES run_CS6_conn_test_CSTP DESTINATION $<$<CONFIG:Debug>:debug/>samples)          