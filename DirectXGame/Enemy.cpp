#include "Enemy.h"
#include <cassert>

void Enemy::Initialize(
    const Vector3& position, const int& frame, const int& soundType, const int& laneNum,
    Model* model) {

	assert(model);
	model_ = model;

	frame_ = frame;
	soundType_ = soundType;
	laneNum_ = laneNum;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	worldTransform_.UpdateMatrix();
}

void Enemy::Update(const float& speed) {

	Vector3 move = {0.0f, 0.0f, -1.0f};
	
	move *= speed;

	//move = move * Matrix4x4::MakeRotateXYZMatrix(worldTransform_.rotation_);

	worldTransform_.translation_ += move;

	worldTransform_.UpdateMatrix();
	
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}