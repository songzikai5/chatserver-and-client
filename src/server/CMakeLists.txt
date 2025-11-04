#定义SRC_LIST,包含该目录下所有的源文件
aux_source_directory(. SRC_LIST)
aux_source_directory(./commonconnectionpool SRC_DB_LIST)
aux_source_directory(./model SRC_MODEL_LIST)
aux_source_directory(./redis SRC_REDIS_LIST)
#指定生成可执行文件
add_executable(ChatServer ${SRC_LIST} ${SRC_DB_LIST} ${SRC_MODEL_LIST} ${SRC_REDIS_LIST})

# 为 chatserver 目标设置库搜索路径（指向根目录的 lib 文件夹）
target_link_directories(ChatServer
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/lib  # 根目录下的 lib 目录
)

#指定可执行文件依赖的库文件
target_link_libraries(ChatServer muduo_net muduo_base pthread mysqlclient hiredis)


