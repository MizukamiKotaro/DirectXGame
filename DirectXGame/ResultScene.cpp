#include "TextureManager.h"
#include "ResultScene.h"
#include <cassert>
#include <fstream>

#include "Ease.h"
#include "SceneManager.h"
#include "GameScene.h"

ResultScene::ResultScene() {}

ResultScene::~ResultScene() {
	
}

void ResultScene::FromBlackInitialize() {
	transitionFrameCount_ = 0;
	black_.get()->SetColor({0.0f, 0.0f, 0.0f, 1.0f});
}

void ResultScene::FromBlackUpdate() {
	transitionFrameCount_++;

	float alpha =
	    Ease::UseEase(1.0f, 0.0f, transitionFrameCount_, kTransitionFrame, Ease::EaseInSine, 2);
	black_.get()->SetColor({0.0f, 0.0f, 0.0f, alpha});

	if (transitionFrameCount_ == kTransitionFrame) {
		transitionRequest_ = Transition::kOperation;
	}
}

void ResultScene::ToBlackInitialize() {
	transitionFrameCount_ = 0;
	black_.get()->SetColor({0.0f, 0.0f, 0.0f, 0.0f});
}

void ResultScene::ToBlackUpdate() {
	transitionFrameCount_++;

	float alpha =
	    Ease::UseEase(0.0f, 1.0f, transitionFrameCount_, kTransitionFrame, Ease::EaseInSine, 2);
	black_.get()->SetColor({0.0f, 0.0f, 0.0f, alpha});

	if (transitionFrameCount_ == kTransitionFrame) {
		transitionRequest_ = Transition::kFromBlack;

		sceneManager_->SetSceneRequest(SceneManager::Scene::kSelection);
	}
}

void ResultScene::ResultUpdate() {



	if (input_->TriggerKey(DIK_SPACE)) {
		transitionRequest_ = Transition::kToBlack;
	}
}

void ResultScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();

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

	s_.reset(Sprite::Create(
	    TextureManager::Load("space.png"), {WinApp::kWindowWidth / 2, 600},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	for (int i = 0; i < 5; i++) {



		if (i < 3) {
		
			comboNum_[i].reset(Sprite::Create(
			    TextureManager::Load("scoreNum.png"), {float(i * 64 + 1100), 200},
			    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

			comboNum_[i]->SetSize({64, 64});

			comboNum_[i]->SetPosition({700 + 64 * 2 + 64.0f * i, 160});

			sp_[i].reset(Sprite::Create(
			    TextureManager::Load("gameScene.png"), {}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

			sp_[i]->SetSize({400, 128});
			sp_[i]->SetTextureRect({i * 200.0f, 0}, {200, 64});

			sp_[i]->SetPosition({400, 80.0f * i});

			if (i == 2) {
				sp_[i]->SetTextureRect({3 * 200.0f, 0}, {200, 64});
				sp_[i]->SetPosition({640, 600});
			}

			hitSp_[i].reset(Sprite::Create(
			    TextureManager::Load("hit.png"), {}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

			hitSp_[i]->SetPosition({400, 80.0f * i + 160});
		
			perNum_[i].reset(Sprite::Create(
			    TextureManager::Load("scoreNum.png"), {}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

			perNum_[i]->SetPosition({700 + 64 * 2 + 64.0f * i, 240});

			perNum_[i]->SetSize({64, 64});

			greNum_[i].reset(Sprite::Create(
			    TextureManager::Load("scoreNum.png"), {}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

			greNum_[i]->SetSize({64, 64});

			greNum_[i]->SetPosition({700 + 64 * 2 + 64.0f * i, 320});

			goodNum_[i].reset(Sprite::Create(
			    TextureManager::Load("scoreNum.png"), {}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

			goodNum_[i]->SetSize({64, 64});

			goodNum_[i]->SetPosition({700 + 64 * 2 + 64.0f * i, 400});

			missNum_[i].reset(Sprite::Create(
			    TextureManager::Load("scoreNum.png"), {}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
		
			missNum_[i]->SetSize({64, 64});

			missNum_[i]->SetPosition({700 + 64 * 2 + 64.0f * i, 480});
		}


		scoreNum_[i].reset(Sprite::Create(
		    TextureManager::Load("scoreNum.png"), {float(i * 64 + 64), 120},
		    {1.0f, 1.0f, 0.0f, 1.0f}, {0.5f, 0.5f}));

		scoreNum_[i]->SetSize({64, 64});

		scoreNum_[i]->SetPosition({700.0f + 64 * i, 80});
	}

	
}

void ResultScene::ResultSceneInitialize() {

	count_ = 0; }

void ResultScene::Update() {
	if (transitionRequest_) {
		transition_ = transitionRequest_.value();

		switch (transition_) {
		case ResultScene::Transition::kFromBlack:
			FromBlackInitialize();
			break;
		case ResultScene::Transition::kOperation:

			break;
		case ResultScene::Transition::kToBlack:
			ToBlackInitialize();
			break;
		default:
			break;
		}

		transitionRequest_ = std::nullopt;
	}

	switch (transition_) {
	case ResultScene::Transition::kFromBlack:
		FromBlackUpdate();
		break;
	case ResultScene::Transition::kOperation:
		ResultUpdate();
		break;
	case ResultScene::Transition::kToBlack:
		ToBlackUpdate();
		break;
	default:
		break;
	}
}

void ResultScene::SpDraw() {


	/*int score = score_.GetScore();

	for (int i = 0; i < 5; i++) {
		int num = score * int(pow(10, i)) / 10000;
		score = score % (10000 / int(pow(10, i)));

		if (num > 9) {
			num = 0;
		}

		scoreNum_[i]->SetTextureRect(
		    {float(num * kNumSize), 0}, {float(kNumSize), float(kNumSize)});

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

		hitSp_->SetTextureRect(
		    {float(hitNum_ * kHitSizeX_), 0}, {float(kHitSizeX_), float(kHitSizeY_)});

		hitSp_->Draw();

		hitCount_++;
	}

	if (play_ == Play::kFinish) {
		sp_[2]->Draw();
	}*/
}

void ResultScene::Draw() {

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

	

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	SpDraw();

	s_->Draw();

	if (transition_ == Transition::kFromBlack || transition_ == Transition::kToBlack) {
		black_->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
