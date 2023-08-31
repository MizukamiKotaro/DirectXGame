#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Enemy.h"
#include "Ground.h"
#include "Skydome.h"

class SceneManager;

class TitleScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	void TitleSceneInitialize();

	void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

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

	enum Sound {
		kDoC4,
		kFaf3,

	};

	void TitleUpdate();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	SceneManager* sceneManager_ = nullptr;

	Transition transition_ = Transition::kFromBlack;
	std::optional<Transition> transitionRequest_ = std::nullopt;
	int transitionFrameCount_ = 0;

	std::unique_ptr<Sprite> black_;
	const int kTransitionFrame = 30;

	std::unique_ptr<Sprite> pianoFa_;
	std::unique_ptr<Sprite> pianoDo_;
	std::unique_ptr<Sprite> s_;

	int countFrame_ = 0;
	int kInterval_ = 40;
	bool isDo_ = false;



	std::vector<uint32_t> soundHandles_;

	ViewProjection viewProjection_;

	std::unique_ptr<Model> modelNeiro_;
	WorldTransform neiroWorldTrandform_;

	
};
