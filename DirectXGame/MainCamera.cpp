#include "MainCamera.h"
#include "Matrix4x4.h"
#include "ImGuiManager.h"

void MainCamera::Initialize() {

	viewProjection_.Initialize();

	viewProjection_.rotation_.x = 0.3f;
	viewProjection_.translation_.y = 2.0f;
	viewProjection_.translation_.z = -10.0f;
	viewProjection_.UpdateMatrix();
}

void MainCamera::Update() {

	
}