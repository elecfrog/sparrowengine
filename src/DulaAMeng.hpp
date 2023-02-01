#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

inline glm::vec3 ToGLMVec3(float _v[3])
{
	return glm::vec3(_v[0], _v[1], _v[2]);
}

inline float* ToFArray3(glm::vec3& _v)
{
	float r[3] = { _v[0], _v[1], _v[2] };
	
	return r;
}