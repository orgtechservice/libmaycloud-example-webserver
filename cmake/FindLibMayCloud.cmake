# Ищем заголовочный файл конфигурации libmaycloud
find_path(LibMayCloud_INCLUDE_DIR NAMES maycloud/config.h)
mark_as_advanced(LibMayCloud_INCLUDE_DIR)

# Ищем библиотеку libmaycloud
find_library(LibMayCloud_LIBRARY NAMES maycloud)
mark_as_advanced(LibMayCloud_LIBRARY)

# Получаем информацию о версии libmaycloud
if(LibMayCloud_INCLUDE_DIR)
	file(STRINGS ${LibMayCloud_INCLUDE_DIR}/maycloud/config.h _ver_line REGEX "^#define +LIBMAYCLOUD_VERSION +\"[^\"]+\"" LIMIT_COUNT 1)
	string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+(\-[a-z]+)?" LibMayCloud_VERSION("${_ver_line}"))
	unset(_ver_line)

	file(STRINGS ${LibMayCloud_INCLUDE_DIR}/maycloud/config.h _mysql_support REGEX "^#define +LIBMAYCLOUD_MYSQL" LIMIT_COUNT 1)
	if(_mysql_support)
		set(LibMayCloud_MYSQL "yes")
	endif()
	unset(_mysql_line)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibMayCloud REQUIRED_VARS LibMayCloud_LIBRARY LibMayCloud_INCLUDE_DIR VERSION_VAR LibMayCloud_VERSION)

# Отобразим информацию и добавим библиотеку как импортированную
if(LibMayCloud_FOUND)
	set(LibMayCloud_INCLUDE_DIRS ${LibMayCloud_INCLUDE_DIR})
	set(LibMayCloud_LIBRARIES ${LibMayCloud_LIBRARY})
	if(NOT TARGET LibMayCloud::LibMayCloud)
		add_library(LibMayCloud::LibMayCloud UNKNOWN IMPORTED)
		set_target_properties(LibMayCloud::LibMayCloud PROPERTIES
			IMPORTED_LOCATION "${LibMayCloud_LIBRARY}"
			INTERFACE_INCLUDE_DIRECTORIES "${LibMayCloud_INCLUDE_DIR}")
	endif()
endif()
