#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "MainCamera.h"
#include "Skydome.h"
#include "Ground.h"
#include "Enemy.h"
#include "Score.h"

class SceneManager;
class SelectionScene;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	void GameSceneInitialize();

	void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

	void SetSelectionScene(SelectionScene* selectionScene) { selectionScene_ = selectionScene; }

	Score GetScore() { return score_; }

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	void PlayUpdate();

	void PUpdate();

	void HitTest();

	void LoadEnemyPopData();

	void UpdateEnemyPopCommands();

	void EnemyGeneration(const int& modelType, const int& soundType, const int& laneNum, const int& frame);

	void SakurasakuraSoundLoad();

	void OrarySoundLoad();

	void PlayAudio(int soundType);

	void LaneDraw();

	void UiUpdate();

	void UiDraw();

private:

	void FromBlackInitialize();
	void FromBlackUpdate();
	void ToBlackInitialize();
	void ToBlackUpdate();

	void ExtraInitialize();
	void ExtraUpdate();
	void PoseInitialize();
	void PoseUpdate();
	void FinishInitialize();
	void FinishUpdate();

	enum class Transition {
		kFromBlack,
		kOperation,
		kToBlack,
	};

	enum class Play {
		kExtra,
		kPlay,
		kPose,
		kFinish,
	};

	enum class Pose {
		kPlay,
		kRetry,
		kRetire,
	};

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	SceneManager* sceneManager_ = nullptr;
	SelectionScene* selectionScene_ = nullptr;

	bool isHitLane[4] = {false, false, false, false};
	bool isPushLane[4] = {false, false, false, false};

	Transition transition_ = Transition::kFromBlack;
	std::optional<Transition> transitionRequest_ = std::nullopt;

	int transitionFrameCount_ = 0;
	std::unique_ptr<Sprite> black_;
	const int kTransitionFrame = 30;

	std::unique_ptr<Sprite> sp_[3];
	int finishCount_ = 0;

	Score score_ = {};
	std::unique_ptr<Sprite> scoreNum_[5];

	std::unique_ptr<Sprite> s;
	std::unique_ptr<Sprite> d;
	std::unique_ptr<Sprite> j;
	std::unique_ptr<Sprite> k_;



	const int kNumSize = 32;
	int comCount_ = 0;
	std::unique_ptr<Sprite> comboNum_[3];

	std::unique_ptr<Sprite> hitSp_;
	const int kHitSizeX_ = 220;
	const int kHitSizeY_ = 64;
	int hitCount_ = 0;
	int hitNum_ = 0;

	Play play_ = Play::kExtra;
	std::optional<Play> playRequest_ = std::nullopt;

	Pose pose_ = Pose::kPlay;

	enum HitErrorFrame {
		kPerfect,
		kGreat,
		kGood,
		kMiss,
	};
	const int kHitErrorFrame_[3] = {5, 10, 14};
	//bool TriggerKeyLane[4] = {false, false, false, false};

	//bool isRetry_ = false;


	ViewProjection viewProjection_;

	//std::unique_ptr<MainCamera> mainCamera_;

	std::unique_ptr<Skydome> skydome_;

	std::unique_ptr<Model> modelSkydome_;

	std::unique_ptr<Ground> ground_;

	std::unique_ptr<Model> modelGround_;

	std::list<Enemy*> enemies_;

	std::unique_ptr<Model> modelDo_;
	std::unique_ptr<Model> modelRe_;
	std::unique_ptr<Model> modelMi_;
	std::unique_ptr<Model> modelFa_;
	std::unique_ptr<Model> modelSo_;
	std::unique_ptr<Model> modelRa_;
	std::unique_ptr<Model> modelSi_;

	std::vector<uint32_t> sakurasakuraSoundHandles_;
	std::vector<uint32_t> orarySoundHandles_;

	std::unique_ptr<Model> meyasu_;
	std::unique_ptr<Model> lane_;
	std::unique_ptr<Model> pushLane_;

	WorldTransform meyasuWT_;
	WorldTransform laneWT_[4];
	WorldTransform pushLaneWT_[4];

	int drawCount_[4] = {};

	enum EnemyModelType {
		Do,
		Re,
		Mi,
		Fa,
		So,
		Ra,
		Si,
	};

	float enemySpeed_ = 0.6f;

	int musicEndFrame_ = 0;
	int countFrame_ = 0;

	int subCountFrame_ = 0;

	//bool isPlay = false;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;
	
};
