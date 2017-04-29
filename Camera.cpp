#include "Camera.h"
#include <glm/ext.hpp>
#include <cmath>
#include <iostream>
#define PI 3.141592f

const glm::vec3 Camera::center_position() const
{
  return  glm::vec3(position_.x + front_dir_.x,
                 position_.y + front_dir_.y, 
                 position_.z + front_dir_.z);
}

// TODO: fill up the following functions properly 
void Camera::move_forward(float delta)
{	
	position_.z -= delta*front_dir_.z;
	position_.x -= delta*front_dir_.x;
}

void Camera::move_backward(float delta)
{	
	position_.z += delta*front_dir_.z;
	position_.x += delta*front_dir_.x;
}

void Camera::move_left(float delta)
{	
	position_.x += delta*right_dir_.x;
	position_.z += delta*right_dir_.z;
}

void Camera::move_right(float delta)
{
	position_.x -= delta*right_dir_.x;
	position_.z -= delta*right_dir_.z;
}

void Camera::rotate_left(float delta)
{
	front_dir_ = glm::rotate(front_dir_, delta, glm::vec3(0.0f,1.0f,0.0f));
	right_dir_ = glm::cross(front_dir_, up_dir_);
}
void Camera::rotate_right(float delta)
{
	front_dir_ = glm::rotate(front_dir_, delta, glm::vec3(0.0f,1.0f,0.0f));
	right_dir_ = glm::cross(front_dir_, up_dir_);
}