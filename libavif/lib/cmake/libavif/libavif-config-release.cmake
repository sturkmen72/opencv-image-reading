#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "avif" for configuration "Release"
set_property(TARGET avif APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(avif PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/avif.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/avif.dll"
  )

list(APPEND _cmake_import_check_targets avif )
list(APPEND _cmake_import_check_files_for_avif "${_IMPORT_PREFIX}/lib/avif.lib" "${_IMPORT_PREFIX}/bin/avif.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
