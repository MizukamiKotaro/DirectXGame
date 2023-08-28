#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
}

void GameScene::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {

	// 曲のノーツの配置。曲の始めに毎回。

	//// 待機処理
	//if (IsWait_) {
	//	waitTimer_--;
	//	if (waitTimer_ <= 0) {
	//		// 待機完了
	//		IsWait_ = false;
	//	}
	//	return;
	//}

	// 1列分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1列分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行は飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			
			// modelType
			getline(line_stream, word, ',');
			int modelType = (int)std::atoi(word.c_str());

			// soundType
			getline(line_stream, word, ',');
			int soundType = (int)std::atoi(word.c_str());

			// laneNum
			getline(line_stream, word, ',');
			int laneNum = (int)std::atoi(word.c_str());

			// frame
			getline(line_stream, word, ',');
			int frame = (int)std::atoi(word.c_str());

			// 敵を発生させる
			EnemyGeneration(modelType, soundType, laneNum, frame);

		}
		// WAITコマンド
		else if (word.find("END") == 0) {
			getline(line_stream, word, ',');

			// 曲の終了フレーム
			musicEndFrame_ = atoi(word.c_str());

			break;
		}
	}
}

void GameScene::EnemyGeneration(
    const int& modelType, const int& soundType, const int& laneNum, const int& frame) {
	Enemy* enemy_ = new Enemy();

	Vector3 pos = {};
	pos.x = float(laneNum) - 1.5f;
	pos.y = 1.0f;
	pos.z = enemySpeed_ * frame;
	if (modelType == EnemyModelType::Do) {
		enemy_->Initialize(pos, frame, soundType, modelDo_.get());
	} else if (modelType == EnemyModelType::Re) {
		enemy_->Initialize(pos, frame, soundType, modelRe_.get());
	} else if (modelType == EnemyModelType::Mi) {
		enemy_->Initialize(pos, frame, soundType, modelMi_.get());
	} else if (modelType == EnemyModelType::Fa) {
		enemy_->Initialize(pos, frame, soundType, modelFa_.get());
	} else if (modelType == EnemyModelType::So) {
		enemy_->Initialize(pos, frame, soundType, modelSo_.get());
	} else if (modelType == EnemyModelType::Ra) {
		enemy_->Initialize(pos, frame, soundType, modelRa_.get());
	} else if (modelType == EnemyModelType::Si) {
		enemy_->Initialize(pos, frame, soundType, modelSi_.get());
	} else {
		enemy_->Initialize(pos, frame, soundType, modelDo_.get());
	}
	enemies_.push_back(enemy_);
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();

	mainCamera_ = std::make_unique<MainCamera>();
	mainCamera_->Initialize();

	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_.get());

	modelGround_.reset(Model::CreateFromOBJ("ground", true));
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(modelGround_.get());

	modelDo_.reset(Model::CreateFromOBJ("do", true));
	modelRe_.reset(Model::CreateFromOBJ("re", true));
	modelMi_.reset(Model::CreateFromOBJ("mi", true));
	modelFa_.reset(Model::CreateFromOBJ("fa", true));
	modelSo_.reset(Model::CreateFromOBJ("so", true));
	modelRa_.reset(Model::CreateFromOBJ("ra", true));
	modelSi_.reset(Model::CreateFromOBJ("si", true));

	LoadEnemyPopData();
	UpdateEnemyPopCommands();

	uint32_t soundHandle = audio_->LoadWave("doC4.wav");
	soundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("reD4.wav");
	soundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("miE4.wav");
	soundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("faF4.wav");
	soundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("soG4.wav");
	soundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("raA4.wav");
	soundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("siB4.wav");
	soundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("doC5.wav");
	soundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("siB3.wav");
	soundHandles_.push_back(soundHandle);

}

void GameScene::Update() {
	//撮影用
	if (input_->TriggerKey(DIK_SPACE)) {
		isPlay = true;
	}
	//UpdateEnemyPopCommands();
	if (isPlay) {
		countFrame_++;
		for (Enemy* enemy : enemies_) {
			enemy->Update(enemySpeed_);
			if (countFrame_ == enemy->GetFrame()) {
				enemy->Dead();
				audio_->PlayWave(enemy->GetSoundType());
			}
		}

		enemies_.remove_if([](Enemy* enemy) {
			if (enemy->IsDead()) {
				delete enemy;
				return true;
			}
			return false;
		});
	}
	
	//enemies_.remove_if([](Enemy* enemy) {
	//	/*if (enemy->IsDead()) {
	//		delete enemy;
	//		return true;
	//	}*/
	//	return false;
	//});

	const ViewProjection& tmp = mainCamera_->GetViewProjection();
	viewProjection_.matView = tmp.matView;
	viewProjection_.matProjection = tmp.matProjection;
	viewProjection_.TransferMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	skydome_->Draw(viewProjection_);

	ground_->Draw(viewProjection_);

	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
