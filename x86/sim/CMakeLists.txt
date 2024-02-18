set(SRCS ./src/main.c ./src/cli.c ./src/app.c ./src/image_buffer.c)

add_executable(${PROJECT_NAME}_sim ${SRCS})
target_include_directories(${PROJECT_NAME}_sim PRIVATE inc)
