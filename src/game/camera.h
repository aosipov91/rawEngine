#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{
public:
        Camera() {}
		explicit Camera(float aspect);
		~Camera();
		void update();

		glm::vec3 pos;
		glm::vec3 rot;

		glm::mat4 mView;
		glm::mat4 mProj;
		glm::mat4 mViewProj;

    float mAspect;
	


};

#endif
