#pragma once
#include "FitClasses.h"

class fit::Manipulator {
public:
	Manipulator(CameraWeak c) :camera(c) {}
	virtual ~Manipulator() {}

	void setCamera(CameraWeak c) { camera=c; }
  CameraWeak getCamera() const { return camera;}

	virtual void update(float dt) {}
	virtual void setupCallbacks(fit::BaseApp &app) {}
	
protected:
	CameraWeak camera;
};


class fit::OrbitManipulator: public fit::Manipulator{
public:
	OrbitManipulator(CameraWeak c=CameraShared()) :Manipulator(c) {}
	virtual ~OrbitManipulator() {}

  void update(float dt) override;
  void setupCallbacks(fit::BaseApp &app) override;

	void setZoom(float z) { zoom = z; }
	void setRotationX(float x) { movex = x; }
	void setRotationY(float y) { movey = y; }
protected:
	bool mouseButton[6] = {};
	float movex = 0;
	float movey = 0;
	int dragx = 0;
	int dragy = 0;
	float zoom = 30;
	float rotSpeedX = 1;
	float rotSpeedY = 1;
	float moveSpeed = 0.001f;
	float dragSpeed = 0.001f;
	bool keymap[256] = {};
};