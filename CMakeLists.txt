cmake_minimum_required(VERSION 3.8)

project("HitmanAbsolutionModManager" C CXX)

set(CMAKE_CXX_STANDARD 23)
set(GAME_INSTALL_PATH "" CACHE PATH "Path of Hitman Absolution folder.")
set(VCPKG_BUILD_ROOT "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}")

add_subdirectory(External/imgui-docking)

find_package(RapidJSON REQUIRED)
find_package(libzippp REQUIRED)

file(GLOB_RECURSE SRC_FILES
	CONFIGURE_DEPENDS
	src/*.cpp
	src/*.c
	src/*.hpp
	src/*.h,
)

file(GLOB_RECURSE HEADER_FILES
	CONFIGURE_DEPENDS
	include/*.h
)

add_executable(HitmanAbsolutionModManager
	${SRC_FILES}
	${HEADER_FILES}
)

target_include_directories(HitmanAbsolutionModManager PRIVATE
	${CMAKE_SOURCE_DIR}/External/mINI/src/mini
)

target_include_directories(HitmanAbsolutionModManager PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/External/IconFontCppHeaders
)

target_link_libraries(HitmanAbsolutionModManager PUBLIC
	d3d11
	dxgi
	imgui-docking
	rapidjson
	libzippp::libzippp
)

install(TARGETS HitmanAbsolutionModManager
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

if (EXISTS ${GAME_INSTALL_PATH})
	install(FILES "$<TARGET_FILE:HitmanAbsolutionModManager>" DESTINATION "${GAME_INSTALL_PATH}/ModManager")

	file(TO_CMAKE_PATH "${GAME_INSTALL_PATH}" GAME_INSTALL_PATH_CMAKE)
	install(DIRECTORY "${CMAKE_SOURCE_DIR}/assets/" DESTINATION "${GAME_INSTALL_PATH_CMAKE}/ModManager/assets")
endif()
