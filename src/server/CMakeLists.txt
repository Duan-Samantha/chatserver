

#把.指定路径下的所有源文件名字放入变量名SRC_LIST里面
aux_source_directory(. SRC_LIST)
aux_source_directory(./db DB_LIST)
aux_source_directory(./model MODEL_LIST)
aux_source_directory(./redis REDIS_LIST)


#指定生成可执行文件
add_executable(ChatServer ${SRC_LIST} ${DB_LIST} ${MODEL_LIST} ${REDIS_LIST})
#表示chatserver这个目标程序，需要连接muduo_net muduo_base pthread这三个库文件
target_link_libraries(ChatServer muduo_net muduo_base mysqlclient hiredis pthread)

