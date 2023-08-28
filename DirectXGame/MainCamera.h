#pragma once

#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"

class MainCamera {
public:
	void Initialize();

	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; }

private:
	const WorldTransform* target_ = nullptr;

	ViewProjection viewProjection_;

	Input* input_ = nullptr;
};
