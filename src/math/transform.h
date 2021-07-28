#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

struct Transform {
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	Transform() :
		position(glm::vec3(0, 0, 0)),
		rotation(glm::quat(0, 0, 0, 1)),
		scale(glm::vec3(1, 1, 1)) {}
	Transform(const glm::vec3& p, const glm::quat& r, const glm::vec3& s) :
		position(p), rotation(r), scale(s) {}
}; // End of transform struct

/*
Transform combine(const Transform& a, const Transform& b);
Transform inverse(const Transform& t);
Transform mix(const Transform& a, const Transform& b, float t);
mat4 transformToMat4(const Transform& t);
Transform mat4ToTransform(const mat4& m);
vec3 transformPoint(const Transform& a, const vec3& b);
vec3 transformVector(const Transform& a, const vec3& b);

bool operator==(const Transform& a, const Transform& b);
bool operator!=(const Transform& a, const Transform& b);
*/
#endif
