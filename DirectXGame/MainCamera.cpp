#include "MainCamera.h"
#include "Matrix4x4.h"
#include "ImGuiManager.h"

void MainCamera::Initialize() {

	viewProjection_.Initialize();

	//viewProjection_.rotation_.x = 1.2f;
	viewProjection_.translation_.y = 3.0f;
	viewProjection_.translation_.z = -10.0f;
	viewProjection_.UpdateMatrix();
}

void MainCamera::Update() {

	
}