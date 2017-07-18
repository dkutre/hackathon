#find subdir in current dir
macro(SUBDIRLIST result curent_dir)
    FILE(GLOB dirs RELATIVE ${curent_dir} ${curent_dir}/*)
    SET(dirlist "")
    foreach(dir ${dirs})
        if(IS_DIRECTORY ${curent_dir}/${dir})
            LIST(APPEND dirlist ${dir})
        endif()
    endforeach()
    SET(${result} ${dirlist})
endmacro()

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

#added all sub_dir
foreach(subdir ${SUBDIRS})
    add_subdirectory(${subdir})
    message("added ${subdir}")
endforeach()