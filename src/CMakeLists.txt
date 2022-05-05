add_executable(${PROJECT_NAME}
  main.c
  ParseCommand.c
  Command.c
  Daemon.c
  FileSystem.c
  FileInfo.c
  Synchronize.c
  Utils.c
  Log.c
)

target_link_libraries(${PROJECT_NAME} pthread)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_BINARY_DIR}/config/include")