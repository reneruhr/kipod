#pragma once

#include <vector>
#include <string>
#include "utils/vec.h"
#include "utils/mat.h"

using namespace std;


class FrustumBox
{

GLuint vao_box;
GLuint vbo_vertices;
GLuint ibo_elements;

 public:
 	FrustumBox();
 	void init();
 	void draw();
};


class Camera {
	bool orthogonal = false;
	mat4 lookatMatrix;
	mat4 perspectiveMatrix;
	mat4 orthogonalMatrix;

	vec3 _eye, _at, _up;
    float _fovy=45.0f,	_aspect = 800.0f/600.0f,	_near = 1.0f, _far=10.0f;
	vec3 u,v,w; // Camera coordinates

    float _proj_right =10.0f, _proj_left = -10.0f, _proj_top = 10.0f, _proj_bottom = -10.0f;


	GLuint vao_camera;
	GLuint vbo_vertices;
	GLuint ibo_elements;

	static FrustumBox _frustum;

public:
	Camera(vec3 eye,vec3 at,vec3 up);
	Camera();
    Camera(const float fovy, const float aspect, const float zNear, const float zFar);
    Camera(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar);



	void setTransformation(const mat4& transform);

	void LookAt(const vec3& eye, const vec3& at, const vec3& up );
	void Ortho( const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar );
	void Ortho();
	void Frustum( const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar );
	void Perspective( const float fovy, const float aspect,
		const float zNear, const float zFar);


	void updateLookAtInternal();
	void updateLookAt(const vec3& eye, const vec3& at, const vec3& up);
	void updateLookAt();
	void updatePerspective(const float& fovy, const float& aspect, const float& near, const float& far);
	void updateOrthogonal( const float left, const float right,const float bottom, const float top,const float zNear, const float zFar);
	void updateFrustum( const float left, const float right,const float bottom, const float top,const float zNear, const float zFar);
	
	void moveEye(const vec3& eye);
	void updateEye(const vec3& eye);
	void updateAt(const vec3& at);

	void tilt(const float x); // x  pitch
	void pan(const float y); // y yaw
	void cant(const float z); // z  roll


	void moveCamera(const vec3& translate);
	void truck(const float x);
	void dolly(const float y);
	void pedestal(const float z);

	vec3 getEye();
	vec3 getAt();
	vec3 getUp();

	float getFOVY();
	float getAspect();
	float getNear();
	float getFar();
	vec4 getLeftRightBottomTop();

	mat4 getcTransform(){ return lookatMatrix; }
	mat4 getProjection(bool perspective=true);


	void init();
	void draw();


	void createFrustum();
    void drawFrustum();

};
