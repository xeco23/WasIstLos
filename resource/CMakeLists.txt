message(STATUS "Compiling UI resources")
execute_process(
        COMMAND glib-compile-resources --target=${CMAKE_BINARY_DIR}/src/Resources.c --generate-source gresource.xml
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

message(STATUS "Configuring desktop resource files")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/desktop/${WIL_APP_ID}.desktop.in ${CMAKE_CURRENT_BINARY_DIR}/${WIL_APP_ID}.desktop)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/desktop/${WIL_APP_ID}.appdata.xml.in ${CMAKE_CURRENT_BINARY_DIR}/${WIL_APP_ID}.appdata.xml)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${WIL_APP_ID}.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${WIL_APP_ID}.appdata.xml DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/image/icons DESTINATION ${CMAKE_INSTALL_DATADIR})
