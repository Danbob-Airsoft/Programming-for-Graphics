#include "Camera.h"

void Camera::UpdateVectors()
{

}

mat4 Camera::CalculateViewMatrix()
{
	ForwardVec = normalize(vec3(0) - CameraTransform->GetPosition());
	RightVec = normalize(cross(vec3(0, 1, 0), ForwardVec));
	UpVec = cross(ForwardVec, RightVec);

	return glm::lookAt(CameraTransform->GetPosition(), vec3(0), UpVec);
}
