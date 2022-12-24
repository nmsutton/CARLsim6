project(NoisyCurrent)

# Targets  
add_executable(NoisyCurrent
    src/main_NoisyCurrent.cpp
)

# definitions     
if(CARLSIM_NO_CUDA)
    target_compile_definitions(NoisyCurrent PUBLIC
        -D__NO_CUDA__ )
endif()

# Linking    
target_link_libraries(NoisyCurrent PRIVATE
    carlsim
    carlsim-stopwatch
)
target_include_directories(NoisyCurrent PRIVATE
    ../../tools/stopwatch
)
   
        
# Installation   
install(TARGETS NoisyCurrent
    EXPORT carlsim
    RUNTIME 
        DESTINATION $<$<CONFIG:Debug>:debug/>samples 
        COMPONENT NoisyCurrent
        )

#install(FILES run_NoisyCurrent DESTINATION $<$<CONFIG:Debug>:debug/>samples)          