#include "camera.h"

Camera::Camera(float aspect)
		: pos()
		, rot()
		, mView()
		, mProj()
		, mViewProj()
		, mAspect(aspect)
{

}

Camera::~Camera()
{}

void Camera::update(float deltaTime)
{
	mView = glm::mat4(1.0f);
	mView = glm::rotate(mView, -rot.x, glm::vec3(1.0, 0.0, 0.0));
	mView = glm::rotate(mView, -rot.y, glm::vec3(0.0, 1.0, 0.0));
	mView = glm::translate(mView, glm::vec3(-pos.x, -pos.y, -pos.z));

	mProj = glm::perspective(90.f, mAspect, 0.1f, 1000.0f);
	mViewProj = mProj * mView;
}