# CMakeList.txt : CMake project for GoodreadsApp, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (GoodreadsApp "GoodreadsApp.cpp" "GoodreadsApp.h" "include/utils/Validator.h" "src/utils/Validator.cpp" "include/models/Book.h" "include/models/Date.h" "include/models/Shelf.h" "include/models/Author.h" "include/models/Publisher.h" "include/utils/Genre.h" "include/services/AuthService.h" "include/services/LibrarySystem.h" "include/services/SocialService.h" "include/services/FileManager.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GoodreadsApp PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
