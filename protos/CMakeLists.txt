
#遍历匹配目录和子目录
file(GLOB_RECURSE PROTOS ${CMAKE_CURRENT_SOURCE_DIR}/selfmessage.proto)


foreach(PROTOFILE ${PROTOS})
    message(${PROTOFILE})
	get_filename_component(PB_NAME ${PROTOFILE} NAME_WE)
	get_filename_component(PB_PATH ${PROTOFILE} PATH)
	
	set(PB_H ${PB_NAME}.pb.h)
	set(PB_CC ${PB_NAME}.pb.cc)
	
	set(PB_OUT ${CMAKE_CURRENT_SOURCE_DIR}/${PB_H} ${CMAKE_CURRENT_SOURCE_DIR}/${PB_CC})
	set(PB_OUTS ${PB_OUTS} ${PB_OUT})

	add_custom_command(
		OUTPUT ${PB_OUT}
		COMMAND protoc -I ${CMAKE_CURRENT_SOURCE_DIR} --proto_path=${PB_PATH} --cpp_out=${CMAKE_CURRENT_SOURCE_DIR} ${PROTOFILE}
		DEPENDS ${PROTOFILE}
		)
endforeach(PROTOFILE)

# 不使用自描述 
add_library(bowproto STATIC ${PB_OUTS})

# 自描述文件使用
execute_process(COMMAND protoc --cpp_out=${CMAKE_CURRENT_LIST_DIR} --proto_path=${CMAKE_CURRENT_LIST_DIR} --descriptor_set_out=selfdes.set message.proto WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE RM_PB_GEN 
    "message.pb.h" 
    "message.pb.cc")
execute_process(COMMAND rm ${RM_PB_GEN} WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
