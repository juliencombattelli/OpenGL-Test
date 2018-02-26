set(GLM_VERSION "0.9.7")
set(GLM_INCLUDE_DIRS "/tmp/mozilla_jucom0/glm-0.9.7.2")

if (NOT CMAKE_VERSION VERSION_LESS "3.0")
    include("${CMAKE_CURRENT_LIST_DIR}/glmTargets.cmake")
endif()
