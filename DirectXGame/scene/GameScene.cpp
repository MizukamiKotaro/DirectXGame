#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

#include "Ease.h"
#include "SceneManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();

	viewProjection_.rotation_.x = 0.3f;
	viewProjection_.translation_.y = 2.0f;
	viewProjection_.translation_.z = -5.0f;
	viewProjection_.UpdateMatrix();

	/*mainCamera_ = std::make_unique<MainCamera>();
	mainCamera_->Initialize();*/

	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_.get());

	modelGround_.reset(Model::CreateFromOBJ("ground", true));
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(modelGround_.get());

	black_.reset(Sprite::Create(
	    TextureManager::Load("white1x1.png"), {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2},
	    {0.0f, 0.0f, 0.0f, 1.0f}, {0.5f, 0.5f}));
	black_->SetSize({WinApp::kWindowWidth, WinApp::kWindowHeight});

	modelDo_.reset(Model::CreateFromOBJ("do", true));
	modelRe_.reset(Model::CreateFromOBJ("re", true));
	modelMi_.reset(Model::CreateFromOBJ("mi", true));
	modelFa_.reset(Model::CreateFromOBJ("fa", true));
	modelSo_.reset(Model::CreateFromOBJ("so", true));
	modelRa_.reset(Model::CreateFromOBJ("ra", true));
	modelSi_.reset(Model::CreateFromOBJ("si", true));

	meyasu_.reset(Model::CreateFromOBJ("meyasu", true));
	lane_.reset(Model::CreateFromOBJ("lane", true));
	pushLane_.reset(Model::CreateFromOBJ("pushLane", true));

	meyasuWT_.Initialize();
	meyasuWT_.translation_.y = 1.0f;
	meyasuWT_.scale_.x = 2.0f;
	meyasuWT_.scale_.y = 0.3f;
	meyasuWT_.UpdateMatrix();

	for (int i = 0; i < 4; i++) {
		laneWT_[i].Initialize();
		laneWT_[i].translation_.y = 0.2f;
		laneWT_[i].translation_.x = i - 1.5f;
		laneWT_[i].scale_.x = 0.8f;
		laneWT_[i].UpdateMatrix();

		pushLaneWT_[i].Initialize();
		pushLaneWT_[i].translation_.y = 0.20001f;
		pushLaneWT_[i].translation_.x = i - 1.5f;
		pushLaneWT_[i].scale_.x = 0.8f;
		pushLaneWT_[i].UpdateMatrix();
	}

	for (int i = 0; i < 5; i++) {
		scoreNum_[i].reset(Sprite::Create(
		    TextureManager::Load("scoreNum.png"), {float(i * 64 + 64), 120},
		    {1.0f, 1.0f, 0.0f, 1.0f},
		    {0.5f, 0.5f}));

		scoreNum_[i]->SetSize({64, 64});
	}

	for (int i = 0; i < 3; i++) {
		comboNum_[i].reset(Sprite::Create(
		    TextureManager::Load("scoreNum.png"), {float(i * 64 + 1100), 200},
		    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

		comboNum_[i]->SetSize({64, 64});

		sp_[i].reset(Sprite::Create(
		    TextureManager::Load("gameScene.png"), {},
		    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

		sp_[i]->SetSize({400,128});
		sp_[i]->SetTextureRect({i * 200.0f, 0}, {200, 64});
	}

	sp_[0]->SetPosition({160, 50});
	sp_[1]->SetPosition({1100, 100});
	sp_[2]->SetPosition({640, 300});

	hitSp_.reset(Sprite::Create(
	    TextureManager::Load("hit.png"), {110, 300}, {1.0f, 1.0f, 1.0f, 1.0f},
	    {0.5f, 0.5f}));
	hitSp_->SetSize({220, 64});

	s.reset(Sprite::Create(
	    TextureManager::Load("s.png"), {200, 680}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	d.reset(Sprite::Create(
	    TextureManager::Load("d.png"), {500, 680}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	j.reset(Sprite::Create(
	        TextureManager::Load("j.png"), {800, 680}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	k_.reset(Sprite::Create(
	        TextureManager::Load("k.png"), {1100, 680}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	/*LoadEnemyPopData();
	UpdateEnemyPopCommands();*/

	SakurasakuraSoundLoad();
	OrarySoundLoad();
}

void GameScene::SakurasakuraSoundLoad() {

	uint32_t soundHandle = audio_->LoadWave("doC4.wav");
	sakurasakuraSoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("reD4.wav");
	sakurasakuraSoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("miE4.wav");
	sakurasakuraSoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("faF4.wav");
	sakurasakuraSoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("soG4.wav");
	sakurasakuraSoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("raA4.wav");
	sakurasakuraSoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("siB4.wav");
	sakurasakuraSoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("doC5.wav");
	sakurasakuraSoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("siB3.wav");
	sakurasakuraSoundHandles_.push_back(soundHandle);
}

void GameScene::OrarySoundLoad() { 
	uint32_t soundHandle = audio_->LoadWave("doC6.wav");
	orarySoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("reD6.wav");
	orarySoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("miE6.wav");
	orarySoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("faF6.wav");
	orarySoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("soG6.wav");
	orarySoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("raA6.wav");
	orarySoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("siB6.wav");
	orarySoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("doC7.wav");
	orarySoundHandles_.push_back(soundHandle);

	soundHandle = audio_->LoadWave("soG4.wav");
	orarySoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("siB4.wav");
	orarySoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("raA4.wav");
	orarySoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("reD5.wav");
	orarySoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("faF4.wav");
	orarySoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("miE4.wav");
	orarySoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("reD4.wav");
	orarySoundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("doC4.wav");
	orarySoundHandles_.push_back(soundHandle);
	
}

void GameScene::GameSceneInitialize() {

	for (Enemy* enemy : enemies_) {
		enemy->Dead();
	}
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	playRequest_ = Play::kExtra;

	countFrame_ = 0;

	enemyPopCommands.clear(std::stringstream::goodbit);
	LoadEnemyPopData();
	UpdateEnemyPopCommands();

	for (int i = 0; i < 4; i++) {
		isPushLane[i] = false;
		isHitLane[i] = false;
	}

	score_.Reset();
	hitCount_ = 0;

	play_ = Play::kExtra;
}

void GameScene::LoadEnemyPopData() {

	// ファイルを開く
	std::ifstream file;

	switch (selectionScene_->GetStage()) {
	case SelectionScene::Stage::kSakuraSakura:

		file.open("./Resources/sakurasakura.csv");
		break;

	case SelectionScene::Stage::kOrary:

		file.open("./Resources/orari.csv");
		break;
	case SelectionScene::Stage::kMi0:

		

		break;
	case SelectionScene::Stage::kFa0:

		

		break;
	case SelectionScene::Stage::kSo0:

		

		break;
	case SelectionScene::Stage::kRa0:

		
		break;
	case SelectionScene::Stage::kSi0:

		break;
	case SelectionScene::Stage::kDo1:

		break;
	default:
		break;
	}

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

			frame = frame / 3 * 2;
			
			// 敵を発生させる
			EnemyGeneration(modelType, soundType, laneNum, frame);

		}
		// WAITコマンド
		else if (word.find("END") == 0) {
			getline(line_stream, word, ',');

			// 曲の終了フレーム
			musicEndFrame_ = atoi(word.c_str());
			musicEndFrame_ = musicEndFrame_ / 3 * 2;
			

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
		enemy_->Initialize(pos, frame, soundType, laneNum, modelDo_.get());
	} else if (modelType == EnemyModelType::Re) {
		enemy_->Initialize(pos, frame, soundType, laneNum, modelRe_.get());
	} else if (modelType == EnemyModelType::Mi) {
		enemy_->Initialize(pos, frame, soundType, laneNum, modelMi_.get());
	} else if (modelType == EnemyModelType::Fa) {
		enemy_->Initialize(pos, frame, soundType, laneNum, modelFa_.get());
	} else if (modelType == EnemyModelType::So) {
		enemy_->Initialize(pos, frame, soundType, laneNum, modelSo_.get());
	} else if (modelType == EnemyModelType::Ra) {
		enemy_->Initialize(pos, frame, soundType, laneNum, modelRa_.get());
	} else if (modelType == EnemyModelType::Si) {
		enemy_->Initialize(pos, frame, soundType, laneNum, modelSi_.get());
	} else {
		enemy_->Initialize(pos, frame, soundType, laneNum, modelDo_.get());
	}
	enemies_.push_back(enemy_);
}

void GameScene::HitTest() {

	for (int i = 0; i < 4; i++) {
		isPushLane[i] = false;
		isHitLane[i] = false;
	}

	if (input_->TriggerKey(DIK_S)) {
		isPushLane[0] = true;
	}
	if (input_->TriggerKey(DIK_D)) {
		isPushLane[1] = true;
	}
	if (input_->TriggerKey(DIK_J)) {
		isPushLane[2] = true;
	}
	if (input_->TriggerKey(DIK_K)) {
		isPushLane[3] = true;
	}

	for (Enemy* enemy : enemies_) {
		if ((input_->TriggerKey(DIK_S) && 0 == enemy->GetLaneNum()) ||
		    (input_->TriggerKey(DIK_D) && 1 == enemy->GetLaneNum()) ||
		    (input_->TriggerKey(DIK_J) && 2 == enemy->GetLaneNum()) ||
		    (input_->TriggerKey(DIK_K) && 3 == enemy->GetLaneNum())) {
			int errorFrame = std::abs(countFrame_ - enemy->GetFrame());

			if (isHitLane[enemy->GetLaneNum()] == false) {
				if (errorFrame <= kHitErrorFrame_[kPerfect]) {
					enemy->Dead();
					isHitLane[enemy->GetLaneNum()] = true;
					PlayAudio(enemy->GetSoundType());
					// audio_->PlayWave(enemy->GetSoundType());

					score_.AddPer();

					hitCount_ = 1;
					hitNum_ = kPerfect;

				} else if (errorFrame <= kHitErrorFrame_[kGreat]) {
					enemy->Dead();
					isHitLane[enemy->GetLaneNum()] = true;
					PlayAudio(enemy->GetSoundType());
					// audio_->PlayWave(enemy->GetSoundType());

					score_.AddGre();
					hitCount_ = 1;
					hitNum_ = kGreat;

				} else if (errorFrame <= kHitErrorFrame_[kGood]) {
					enemy->Dead();
					isHitLane[enemy->GetLaneNum()] = true;
					PlayAudio(enemy->GetSoundType());
					// audio_->PlayWave(enemy->GetSoundType());

					score_.AddGood();

					hitCount_ = 1;
					hitNum_ = kGood;
				}
			}
		}
	}
}

void GameScene::PlayAudio(int soundType) {
	switch (selectionScene_->GetStage()) {
	case SelectionScene::Stage::kSakuraSakura:

		audio_->PlayWave(sakurasakuraSoundHandles_[soundType]);
		break;

	case SelectionScene::Stage::kOrary:

		audio_->PlayWave(orarySoundHandles_[soundType]);
		break;
	case SelectionScene::Stage::kMi0:

		break;
	case SelectionScene::Stage::kFa0:

		break;
	case SelectionScene::Stage::kSo0:

		break;
	case SelectionScene::Stage::kRa0:

		break;
	case SelectionScene::Stage::kSi0:

		break;
	case SelectionScene::Stage::kDo1:

		break;
	default:
		break;
	}
}

void GameScene::FromBlackInitialize() {
	transitionFrameCount_ = 0;
	black_.get()->SetColor({0.0f, 0.0f, 0.0f, 1.0f});
}

void GameScene::FromBlackUpdate() {
	transitionFrameCount_++;

	float alpha =
	    Ease::UseEase(1.0f, 0.0f, transitionFrameCount_, kTransitionFrame, Ease::EaseInSine, 2);
	black_.get()->SetColor({0.0f, 0.0f, 0.0f, alpha});

	if (transitionFrameCount_ == kTransitionFrame) {
		transitionRequest_ = Transition::kOperation;
	}
}

void GameScene::ToBlackInitialize() {
	transitionFrameCount_ = 0;
	black_.get()->SetColor({0.0f, 0.0f, 0.0f, 0.0f});
}

void GameScene::ToBlackUpdate() {
	transitionFrameCount_++;

	float alpha =
	    Ease::UseEase(0.0f, 1.0f, transitionFrameCount_, kTransitionFrame, Ease::EaseInSine, 2);
	black_.get()->SetColor({0.0f, 0.0f, 0.0f, alpha});

	if (transitionFrameCount_ == kTransitionFrame) {
		transitionRequest_ = Transition::kFromBlack;

		if (play_ == Play::kFinish) {
			sceneManager_->SetSceneRequest(SceneManager::Scene::kResult);
		}else if (pose_ == Pose::kRetry) {
			sceneManager_->SetSceneRequest(SceneManager::Scene::kPlay);
		}else if (pose_ == Pose::kRetire) {
			sceneManager_->SetSceneRequest(SceneManager::Scene::kSelection);
		}
	}
}

void GameScene::ExtraInitialize() {


}

void GameScene::ExtraUpdate() {

	// カウントダウン

	countFrame_++;

	if (countFrame_ == 80) {
		playRequest_ = Play::kPlay;
		countFrame_ = 0;
	}
}

void GameScene::PoseInitialize() { pose_ = Pose::kPlay; }

void GameScene::PoseUpdate() {

	switch (pose_) {
	case GameScene::Pose::kPlay:

		if (input_->TriggerKey(DIK_SPACE)) {
			playRequest_ = Play::kExtra;
		} else if (input_->TriggerKey(DIK_RIGHT) || input_->TriggerKey(DIK_D)) {
			pose_ = Pose::kRetry;
		}

		break;
	case GameScene::Pose::kRetry:

		if (input_->TriggerKey(DIK_SPACE)) {
			transitionRequest_ = Transition::kToBlack;
			
		} else if (input_->TriggerKey(DIK_RIGHT) || input_->TriggerKey(DIK_D)) {
			pose_ = Pose::kRetire;
		} else if (input_->TriggerKey(DIK_LEFT) || input_->TriggerKey(DIK_A)) {
			pose_ = Pose::kPlay;
		}

		break;
	case GameScene::Pose::kRetire:

		if (input_->TriggerKey(DIK_SPACE)) {
			transitionRequest_ = Transition::kToBlack;
		} else if (input_->TriggerKey(DIK_LEFT) || input_->TriggerKey(DIK_A)) {
			pose_ = Pose::kRetry;
		}
		break;
	default:
		break;
	}

}

void GameScene::FinishInitialize() { 
	finishCount_ = 0; }

void GameScene::FinishUpdate() { 
	
	finishCount_++;

	if (finishCount_ == 45) {
		transitionRequest_ = Transition::kToBlack;
	}
}

void GameScene::PUpdate() {

	countFrame_++;
	for (Enemy* enemy : enemies_) {
		enemy->Update(enemySpeed_);

		if (countFrame_ >= enemy->GetFrame() + kHitErrorFrame_[kGood]) {
			enemy->Dead();
			score_.AddMiss();

			hitCount_ = 1;
			hitNum_ = kMiss;
		}
	}

	HitTest();

	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	if (input_->TriggerKey(DIK_P)) {
		playRequest_ = Play::kPose;
	}

	if (countFrame_ == musicEndFrame_) {
		playRequest_ = Play::kFinish;
	}
}

void GameScene::PlayUpdate() {

	if (playRequest_) {
		play_ = playRequest_.value();

		switch (play_) {
		case GameScene::Play::kExtra:
			ExtraInitialize();
			break;
		case GameScene::Play::kPlay:
			break;
		case GameScene::Play::kPose:
			PoseInitialize();
			break;
		case GameScene::Play::kFinish:
			FinishInitialize();
			break;
		default:
			break;
		}
		
		playRequest_ = std::nullopt;
	}

	switch (play_) {
	case GameScene::Play::kExtra:
		ExtraUpdate();
		break;
	case GameScene::Play::kPlay:

		PUpdate();
		break;
	case GameScene::Play::kPose:
		PoseUpdate();
		break;
	case GameScene::Play::kFinish:
		FinishUpdate();
		break;
	default:
		break;
	}

}

void GameScene::Update() {

	if (transitionRequest_) {
		transition_ = transitionRequest_.value();

		switch (transition_) {
		case GameScene::Transition::kFromBlack:
			FromBlackInitialize();
			break;
		case GameScene::Transition::kOperation:
			
			break;
		case GameScene::Transition::kToBlack:
			ToBlackInitialize();
			break;
		default:
			break;
		}

		transitionRequest_ = std::nullopt;
	}

	switch (transition_) {
	case GameScene::Transition::kFromBlack:
		FromBlackUpdate();
		break;
	case GameScene::Transition::kOperation:
		PlayUpdate();
		break;
	case GameScene::Transition::kToBlack:
		ToBlackUpdate();
		break;
	default:
		break;
	}
}

void GameScene::LaneDraw() {

	meyasu_->Draw(meyasuWT_, viewProjection_);
	for (int i = 0; i < 4; i++) {
		//lane_->Draw(laneWT_[i], viewProjection_);

		if (isPushLane[i]) {
			drawCount_[i] = 10;
		}

		if (drawCount_[i] != 0) {
			drawCount_[i]--;
			pushLane_->Draw(laneWT_[i], viewProjection_);
		} else {
			lane_->Draw(laneWT_[i], viewProjection_);
		}
	}
}

void GameScene::UiDraw() {

	int score = score_.GetScore();

	for (int i = 0; i < 5; i++) {
		int num = score * int(pow(10, i)) / 10000;
		score = score % (10000 / int(pow(10, i)));

		if (num > 9) {
			num = 0;
		}
		
		scoreNum_[i]->SetTextureRect({float(num * kNumSize), 0}, {float(kNumSize), float(kNumSize)});

		scoreNum_[i]->Draw();
	}

	sp_[0]->Draw();

	score = score_.GetCom();
	int k = 0;

	for (int i = 0; i < 3; i++) {
		int num = score * int(pow(10, i)) / 100;
		score = score % (100 / int(pow(10, i)));

		if (num > 9) {
			num = 0;
		}

		if (num != 0) {
			k++;
		}

		if (k != 0) {
			comboNum_[i]->SetTextureRect(
			    {float(num * kNumSize), 0}, {float(kNumSize), float(kNumSize)});
			comboNum_[i]->Draw();
		}
	}

	sp_[1]->Draw();

	if (hitCount_ != 0) {
		


		hitSp_->SetTextureRect({float(hitNum_ * kHitSizeX_), 0}, {float(kHitSizeX_), float(kHitSizeY_)});

		hitSp_->Draw();

		hitCount_++;
	}


	if (play_ == Play::kFinish) {
		sp_[2]->Draw();
	}
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

	LaneDraw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	
	UiDraw();

	s->Draw();

	d->Draw();

	j->Draw();

	k_->Draw();

	if (transition_ == Transition::kFromBlack || transition_ == Transition::kToBlack) {
		black_->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
