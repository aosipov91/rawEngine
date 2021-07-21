#ifndef UNIFORM_H_
#define UNIFORM_H_

#include "src/math/vec2.h"
#include "src/math/vec3.h"
#include "src/math/vec4.h"
#include "src/math/mat4.h"
#include "src/math/quat.h"

#include <vector>

//  how to use? Uniform<mat4>::Set(mShader->GetUniform("model"), model);

namespace renderer {

template <typename T>
class Uniform {
private:
	Uniform();
	Uniform(const Uniform&);
	Uniform& operator=(const Uniform&);
	~Uniform();
public:
	static void Set(unsigned int slot, const T& value);
	static void Set(unsigned int slot, T* inputArray, unsigned int arrayLength);
	static void Set(unsigned int slot, std::vector<T>& inputArray);
};

}

#endif
