#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Enemy {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position, const int& frame, const int& soundType, Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const float& speed);

	// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(const ViewProjection& viewProjection);

	int GetFrame() { return frame_; }

	int GetSoundType() { return soundType_; }

	void Dead() { isDead_ = true; }

	bool IsDead() { return isDead_; }

private:
	

private:
	
	// モデル
	Model* model_ = nullptr;

	WorldTransform worldTransform_;
	
	int frame_ = 0;

	int soundType_ = 0;

	bool isDead_ = false;
	
};
