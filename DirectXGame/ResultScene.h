#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Ground.h"
#include "Skydome.h"

class SceneManager;

class GameScene;

class ResultScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	ResultScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ResultScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	void ResultSceneInitialize();

	void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void SpDraw();

private:
	void FromBlackInitialize();
	void FromBlackUpdate();
	void ToBlackInitialize();
	void ToBlackUpdate();

	enum class Transition {
		kFromBlack,
		kOperation,
		kToBlack,
	};

	void ResultUpdate();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	SceneManager* sceneManager_ = nullptr;
	GameScene* gameScene_ = nullptr;

	Transition transition_ = Transition::kFromBlack;
	std::optional<Transition> transitionRequest_ = std::nullopt;
	int transitionFrameCount_ = 0;

	std::unique_ptr<Sprite> black_;
	const int kTransitionFrame = 30;

	ViewProjection viewProjection_;

	std::unique_ptr<Skydome> skydome_;

	std::unique_ptr<Model> modelSkydome_;

	std::unique_ptr<Ground> ground_;

	std::unique_ptr<Model> modelGround_;

	std::unique_ptr<Sprite> sp_[3];

	std::unique_ptr<Sprite> scoreNum_[5];
	const int kNumSize = 32;

	std::unique_ptr<Sprite> comboNum_[3];

	std::unique_ptr<Sprite> perNum_[3];

	std::unique_ptr<Sprite> greNum_[3];

	std::unique_ptr<Sprite> goodNum_[3];

	std::unique_ptr<Sprite> missNum_[3];

	std::unique_ptr<Sprite> hitSp_[4];

	std::unique_ptr<Sprite> s_;

	int count_ = 0;
};
