set(GLM_VERSION "0.9.8")
set(GLM_INCLUDE_DIRS "../external/glm-0.9.8.5")

if (NOT CMAKE_VERSION VERSION_LESS "3.0")
    include("${CMAKE_CURRENT_LIST_DIR}/glmTargets.cmake")
endif()
