qt_standard_project_setup()

qt_add_executable(${PROJECT_UI_NAME}
    main.cpp
    majorUI.cpp majorUI.h majorUI.ui
)

set_target_properties(${PROJECT_UI_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(${PROJECT_UI_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

if(TARGET Qt6::PrintSupport)
  target_link_libraries(${PROJECT_UI_NAME} PRIVATE Qt6::PrintSupport)
endif()

target_include_directories(${PROJECT_UI_NAME} PRIVATE ..)
target_link_libraries(${PROJECT_UI_NAME} PRIVATE majorms_lib)

# Resources:
set(my_resource_files
    "images/bold.png"
    "images/copy.png"
    "images/create.png"
    "images/cut.png"
    "images/edit_redo.png"
    "images/edit_undo.png"
    "images/exit.png"
    "images/font.png"
    "images/info.png"
    "images/italic.png"
    "images/new.png"
    "images/open.png"
    "images/paste.png"
    "images/pencil.png"
    "images/print.png"
    "images/save.png"
    "images/save_as.png"
    "images/underline.png"
)

qt_add_resources(${PROJECT_UI_NAME} "majorUI"
    PREFIX
        "/"
    FILES
        ${my_resource_files}
)

install(TARGETS ${PROJECT_UI_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ${PROJECT_UI_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
