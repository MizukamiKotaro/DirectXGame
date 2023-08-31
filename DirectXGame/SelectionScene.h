#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Score.h"

class SceneManager;

class SelectionScene {

public:
	enum class Stage {
		kSakuraSakura,
		kOrary,
		kMi0,
		kFa0,
		kSo0,
		kRa0,
		kSi0,
		kDo1,
	};

	enum stageName {
		kDo0,
		kRe0,
		kMi0,
		kFa0,
		kSo0,
		kRa0,
		kSi0,
		kDo1,
	};

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	SelectionScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SelectionScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	void SelectionSceneInitialize();

	void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

	Stage GetStage() { return stage_; }

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	void SelectUpDate();

	void FromBlackInitialize();
	void FromBlackUpdate();
	void ToBlackInitialize();
	void ToBlackUpdate();

	enum class Transition {
		kFromBlack,
		kOperation,
		kToBlack,
	};

	enum class State {
		kSelect,
		kSet,
		kSpeedSet,
	};

	enum class SetSelect {
		kPlay,
		kSet,
		kEzit,
	};


	void SelectionUpdate();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	SceneManager* sceneManager_ = nullptr;

	std::vector<Score*> scores_; 

	Stage stage_ = Stage::kSakuraSakura;
	State state_ = State::kSelect;
	SetSelect setSelect_ = SetSelect::kPlay;

	Transition transition_ = Transition::kFromBlack;
	std::optional<Transition> transitionRequest_ = std::nullopt;
	int transitionFrameCount_ = 0;

	int speedNum = 5;

	std::unique_ptr<Sprite> black_;
	const int kTransitionFrame = 30;

	std::unique_ptr<Sprite> stageSelectDo_;
	std::unique_ptr<Sprite> stageSelectRe_;
	std::unique_ptr<Sprite> stageSelectMi_;

	std::unique_ptr<Sprite> start_;
	std::unique_ptr<Sprite> exit_;
	//std::unique_ptr<Sprite> speed_;

	std::unique_ptr<Sprite> b_;
	std::unique_ptr<Sprite> y_;

	std::unique_ptr<Sprite> sakurasakura_;
	std::unique_ptr<Sprite> orary_;
	//std::unique_ptr<Sprite> stageSelectMi_;

	ViewProjection viewProjection_;


};
