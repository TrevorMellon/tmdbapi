find_file(VERAPP NAMES vera++ vera++.exe)

if (${VERAPP} MATCHES VERAPP-NOTFOUND)
	message("-- vera++ Not Found, test disabled")
else(${VERAPP} MATCHES VERAPP-NOTFOUND)
	file(GLOB_RECURSE verasrcs
		${CMAKE_SOURCE_DIR}/src/*.cpp
		${CMAKE_SOURCE_DIR}/include/*.h
		)
		
		
	message("-- vera++ - Found")
	message("-- vera++: ${VERAPP}")	
	
	set(VERARULES -R F001 -R L001 -R L003 -R L004 -R L005 -R L006 -R T001 -R T002 -R T003 -R T004 -R T005 -R T006 -R T007 -R T008 -R T009 -R T010 -R T011 -R T013 -R T015 -R T016 -R T017 -R T018 -R T019)
	
        add_custom_target(verastyle  ${VERAPP} ${VERARULES}  -c ${PROJECT_SOURCE_DIR}/sonar/vera++-report.xml --warning ${verasrcs})
	#message("${VERAPP}  -x ${PROJECT_SOURCE_DIR}/vera++-report.xml --warning ${verasrcs_STR}")
endif(${VERAPP} MATCHES VERAPP-NOTFOUND)


find_file(CPPCHECK NAMES cppcheck cppcheck.exe)

if (${CPPCHECK} MATCHES CPPCHECK-NOTFOUND)
	message("-- cppcheck Not Found, test disabled")
else(${CPPCHECK} MATCHES CPPCHECK-NOTFOUND)
	file(GLOB_RECURSE cpcsrcs
		${CMAKE_SOURCE_DIR}/src/*.cpp
		${CMAKE_SOURCE_DIR}/include/*.h
		)
		
	SET (CPPC_INC -I${Boost_INCLUDE_DIRS} -I${ICU_INCLUDE_DIRS})
	
        if( RAPIDJSON_FOUND)
		SET (CPPC_INC ${CPPC_INC} -I${RAPIDJSON_INCLUDE_DIRS})
	endif()
	
	if(SQLITE3_FOUND)
		SET (CPPC_INC ${CPPC_INC} -I${SQLITE3_INCLUDE_DIR})
	endif(SQLITE3_FOUND)
	
	if(SOCI_FOUND)
		SET (CPPC_INC ${CPPC_INC} -I${SOCI_INCLUDE_DIRS})
	endif(SOCI_FOUND)
	
	message("-- cppcheck - Found")
	message("-- cppcheck: ${CPPCHECK}")
        if(WIN32)
            SET ( VCINC "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\include")
            SET ( CPPC_DEF -D_WIN32 -DWINDOWS -D_NDEBUG -DNDEBUG -D_MSC_VER -DBOOST-ALL-NO-LIB)
            SET ( CPPC_INC ${CPPC_INC} -I${VCINC} )
        endif(WIN32)

        SET ( CPPC_INC -I${PROJECT_SOURCE_DIR}/include)
	
	SET(CPPCHK_FILE ${PROJECT_SOURCE_DIR}/sonar/cppcheck-results.xml)
	SET(CPPCHK_OPTS --force ${CPPC_INC}  --xml --library=windows --platform=win32A --std=c++11 --enable=all ${cpcsrcs})	
	
        add_custom_target(cppcheck COMMAND ${CPPCHECK} ${CPPCHK_OPTS} "${PROJECT_SOURCE_DIR}/src" "${CPPC_INC}" 2> ${CPPCHK_FILE} )

endif(${CPPCHECK} MATCHES CPPCHECK-NOTFOUND)

SET (VSLOG "${PROJECT_SOURCE_DIR}/cmake/vs.log")
