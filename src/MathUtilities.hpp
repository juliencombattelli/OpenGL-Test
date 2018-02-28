#ifndef MATHUTILITIES_HPP_
#define MATHUTILITIES_HPP_

#include <glm/glm.hpp>
#include <assimp/types.h>

namespace math
{

template<typename T>
glm::tvec2<T, glm::highp> to_glmvec2(const aiVector2t<T>& vect) {return {vect.x, vect.y};}

template<typename T>
glm::tvec2<T, glm::highp> to_glmvec2(const aiVector3t<T>& vect) {return {vect.x, vect.y};}

template<typename T>
glm::tvec3<T, glm::highp> to_glmvec3(const aiVector3t<T>& vect) {return {vect.x, vect.y, vect.z};}

glm::vec3 to_glmvec3(const aiColor3D& color) {return {color.r, color.g, color.b};}


}  // namespace math

#endif /* MATHUTILITIES_HPP_ */
