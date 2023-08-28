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

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	void LoadEnemyPopData();

	void UpdateEnemyPopCommands();

	void EnemyGeneration(const int& modelType, const int& soundType, const int& laneNum, const int& frame);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	ViewProjection viewProjection_;

	std::unique_ptr<MainCamera> mainCamera_;

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

	std::vector<uint32_t> soundHandles_;

	/*enum BaseSounds {
		Do,
		Re,
		Mi,
		Fa,
		So,
		Ra,
		Si,
		Do2,
		Si0 = 10,
	};*/

	/*enum SubSounds {
		Si = 10,
	};*/


	enum EnemyModelType {
		Do,
		Re,
		Mi,
		Fa,
		So,
		Ra,
		Si,
	};

	float enemySpeed_ = 1.0f;

	int musicEndFrame_ = 0;
	int countFrame_ = 0;

	bool isPlay = false;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;
	
};
