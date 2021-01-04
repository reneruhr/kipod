#include "../include/core.h"
#include "../include/camera.h"
#include "../include/meshmodel.h"

using namespace std;


Camera::Camera(vec3 eye,vec3 at,vec3 up): _eye(eye),	_at(at),	_up(up)
	{
		updateLookAt();
		init();
	}

Camera::Camera(): _eye(vec3(0,0,0)),	_at(vec3(0,0,-1)),	_up(vec3(0,1,0))
	{
		updateLookAt();
		init();
	}

Camera::Camera( const float fovy, const float aspect, const float zNear, const float zFar): Camera(){
    Perspective(fovy, aspect, zNear, zFar);
    Ortho();
}

Camera::Camera( const float left, const float right, const float bottom, const float top, const float zNear, const float zFar ) : Camera(){
    Ortho(left, right, bottom, top, zNear,  zFar);
}


void Camera::LookAt(const vec3& eye, const vec3& at, const vec3& up ){
	// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluLookAt.xml
	assert(length(eye-at)>0.0001);
	w = normalize(at-eye);  // direction
	u = normalize(cross(w,normalize(up))); // orthogonal vector to direction and up
	v = normalize(cross(u, w)); // replacing up to make orthogonal

	updateLookAtInternal();

};
	
void Camera::Perspective( const float fovy, const float aspect, const float zNear, const float zFar){
	// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml
	
	float f = 1.0 / tan(0.5 * fovy);
	float d = 1.0/ (zNear - zFar);
	perspectiveMatrix = mat4( f/aspect , 0, 0, 0,
	  				    		0, f, 0, 0,
	  							0, 0, (zFar+zNear) * d , -1,
	  							0, 0, (2.0*zFar * zNear) * d, 0);
};


void Camera::Ortho( const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar ){
	float mid_x = (left+right)/2;
	float mid_y = (bottom+top)/2;
	float mid_z = -(zNear+zFar)/2;

	float scale_x = 2/(right-left);
	float scale_y = 2/(top-bottom);
	float scale_z = 2/(zFar-zNear);

	orthogonalMatrix = mat4( vec4(scale_x , 0, 0, -mid_x*scale_x),
	  				    	 vec4(0, scale_y, 0, -mid_y*scale_y),
	  						 vec4(0, 0, -scale_z , mid_z*scale_z),
	  						 vec4(0, 0, 0, 1));

}

void Camera::Frustum( const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar ){


	mat4 pers = mat4(-zNear, 0, 0 ,0,
					 0, -zNear, 0, 0,
					 0,0, -zNear+zFar, -1,
					 0,0, zNear*zFar,0);

	Ortho( left, right, bottom, top, zNear, zFar );
	perspectiveMatrix = orthogonalMatrix* pers;

	// Also works:

	// float mid_x = (left+right)/2;
	// float mid_y = (bottom+top)/2;
	// float mid_z = -(zNear+zFar)/2;

	// float scale_x = 2/(right-left);
	// float scale_y = 2/(top-bottom);
	// float scale_z = 2/(zFar-zNear);
	// perspectiveMatrix = mat4(vec4(zNear*scale_x, 0, 					-mid_x*scale_x, 0					),
	//   				    	 vec4(0, 	   		zNear*scale_y, 			-mid_y*scale_y, 0					),
	//   						 vec4(0, 	   		0, 		scale_z*mid_z, -scale_z*zFar*zNear  					),
	//   						 vec4(0, 	   		0, 		-1, 				0									));

}

void Camera::Ortho(){
	Ortho(_proj_left, _proj_right, _proj_bottom, _proj_top, _near, _far);
}


mat4 Camera::getProjection(bool perspective){ 
	
	if(perspective)	return perspectiveMatrix;
	return orthogonalMatrix;
}


void Camera::updateLookAtInternal(){
		lookatMatrix = mat4(vec4(u,-dot(_eye,u)),
						vec4(v,-dot(_eye,v)),
						vec4(w,-dot(_eye,w)),
                        vec4(vec3(0.0f))
		);
}


void Camera::updateLookAt(const vec3& eye, const vec3& at, const vec3& up){ 
	std::cout << "Called upDateLookAt" << std::endl;
	_eye=eye; 
	_at=at; 
	_up=up; 
	LookAt(_eye, _at, _up);
}
void Camera::updateLookAt(){ 
	std::cout << "Called upDateLookAt" << std::endl;
	LookAt(_eye, _at, _up);
}

void Camera::updatePerspective(const float& fovy, const float& aspect, const float& near, const float& far){
	_fovy=fovy;
	_aspect=aspect;
	_near=near;
	_far=far;
	Perspective(_fovy,_aspect,_near,_far);
}

void Camera::updateOrthogonal( const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar){
	_proj_right =right, 
	_proj_left = left, 
	_proj_top = top, 
	_proj_bottom = bottom;
	_near = zNear;
	_far = zFar;

	Ortho(_proj_left, _proj_right, _proj_bottom, _proj_top, _near, _far);
}


void Camera::updateFrustum( const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar){
	_proj_right =right, 
	_proj_left = left, 
	_proj_top = top, 
	_proj_bottom = bottom;
	_near = zNear;
	_far = zFar;

	Frustum(_proj_left, _proj_right, _proj_bottom, _proj_top, _near, _far);
}


void Camera::moveCamera(const vec3& translate){
	_eye+=translate;
	_at+=translate;
	LookAt(_eye, _at, _up);
}

void Camera::moveEye(const vec3& eye){
	_eye+=eye; 
	while(length(_eye-_at)<0.0001)	_eye+=eye;
	LookAt(_eye, _at, _up);
}

void Camera::updateEye(const vec3& eye){
	_eye=eye; 
	LookAt(_eye, _at, _up);
}

void Camera::updateAt(const vec3& at){
	_at=-at; 
	LookAt(_eye, _at, _up);
}


vec3 Camera::getEye(){ return _eye;}
vec3 Camera::getAt(){ return _at;}
vec3 Camera::getUp(){ return _up;}

float Camera::getFOVY(){ return _fovy;}
float Camera::getAspect(){ return _aspect;}
float Camera::getNear(){ return _near;}
float Camera::getFar(){ return _far;}

vec4 Camera::getLeftRightBottomTop(){
	return vec4(_proj_left,_proj_right, _proj_bottom, _proj_top);
}


void Camera::tilt(float x){ // x  pitch
	mat3 k = rotate(u,x);
	v = k*v;
	w = k*w;
	updateLookAtInternal();
}

void Camera::pan(float y){
	mat3 k = rotate(v,y);
	u = k*u;
	w = k*w;
	updateLookAtInternal();
}
void Camera::cant(float z){
	mat3 k = rotate(w,z);
	u = k*u;
	v = k*v;
	updateLookAtInternal();
}


void Camera::truck(const float x){
	//moveCamera(vec3(x,0,0));
	_eye+=x*u;
	updateLookAtInternal();

	}
void Camera::pedestal(const float y){
	//moveCamera(vec3(0,y,0));
	_eye+=y*v;
	updateLookAtInternal();
	}
void Camera::dolly(const float z){
	//moveCamera(vec3(0,0,z));
	_eye+=z*w;
	updateLookAtInternal();
	}



void Camera::init(){
	
GLfloat standardPyramid[] = {
    -0.05, -0.05, 0,
     0.05, -0.05, 0,
     0.05,  0.05, 0,
    -0.05,  0.05, 0,
        0,   0,0.05
  };
GLushort standardPyramidLoopLineOrder[] = {
    0, 1, 2, 3,
    0,1,4,
    1,2,4,
    2,3,4,
    3,0,4

  };


  	glGenVertexArrays(1,&vao_camera);
  	
	glGenBuffers(1, &vbo_vertices);
  	glGenBuffers(1, &ibo_elements);

  	glBindVertexArray(vao_camera);
  	//glEnableVertexAttribArray(0);

  	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
  	glBufferData(GL_ARRAY_BUFFER, sizeof(standardPyramid), standardPyramid, GL_STATIC_DRAW);
  	glBindBuffer(GL_ARRAY_BUFFER, 0);
  	
  	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
  	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(standardPyramidLoopLineOrder), standardPyramidLoopLineOrder, GL_STATIC_DRAW);
  	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  	glBindVertexArray(0);

}

void Camera::draw(){


		glBindVertexArray(vao_camera);
		
 		
 		glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
 		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
		  
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		  
		  glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
		  glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_SHORT, (GLvoid*)(4*sizeof(GLushort)));
		  glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_SHORT, (GLvoid*)(7*sizeof(GLushort)));
		  glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_SHORT, (GLvoid*)(10*sizeof(GLushort)));
		  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  		  //glDisableVertexAttribArray(0);
  		  glBindBuffer(GL_ARRAY_BUFFER, 0);
  		  glBindVertexArray(0);
}



FrustumBox Camera::_frustum;

void Camera::createFrustum(){
	Camera::_frustum.init();
}

void Camera::drawFrustum(){
	Camera::_frustum.draw();
}




FrustumBox::FrustumBox(){}

void FrustumBox::init(){
	
	glGenVertexArrays(1, &vao_box);
	glGenBuffers(1, &vbo_vertices);
	glGenBuffers(1, &ibo_elements);
	glBindVertexArray(vao_box);

	GLfloat standardCube[] = {
	    -1.0, -1.0, -1.0,
	     1.0, -1.0, -1.0,
	     1.0,  1.0, -1.0,
	    -1.0,  1.0, -1.0,
	    -1.0, -1.0,  1.0,
	     1.0, -1.0,  1.0,
	     1.0,  1.0,  1.0,
	    -1.0,  1.0,  1.0,
	  };
	GLushort standardCubeLoopLineOrder[] = {
	    0, 1, 2, 3,
	    4, 5, 6, 7,
	    0, 4, 1, 5, 2, 6, 3, 7
	  };

	  	
	  	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	  	glBufferData(GL_ARRAY_BUFFER, sizeof(standardCube), standardCube, GL_STATIC_DRAW);
	  	glBindBuffer(GL_ARRAY_BUFFER, 0);
	  	
	  	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	  	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(standardCubeLoopLineOrder), standardCubeLoopLineOrder, GL_STATIC_DRAW);
	  	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	  	
		glBindVertexArray(0);

}

void FrustumBox::draw(){
 		  glBindVertexArray(vao_box);
 		  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
		  
		  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		  glEnableVertexAttribArray(0);


		  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
		  glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
		  glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4*sizeof(GLushort)));
		  glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8*sizeof(GLushort)));
		  
		  glBindBuffer(GL_ARRAY_BUFFER, 0);
		  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		  glBindVertexArray(0);
}

