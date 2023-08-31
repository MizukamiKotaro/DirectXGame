#include "TitleScene.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

#include "Ease.h"
#include "SceneManager.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {
	
}



void TitleScene::FromBlackInitialize() {
	transitionFrameCount_ = 0;
	black_.get()->SetColor({0.0f, 0.0f, 0.0f, 1.0f});
}

void TitleScene::FromBlackUpdate() {
	transitionFrameCount_++;

	float alpha =
	    Ease::UseEase(1.0f, 0.0f, transitionFrameCount_, kTransitionFrame, Ease::EaseInSine, 2);
	black_.get()->SetColor({0.0f, 0.0f, 0.0f, alpha});

	if (transitionFrameCount_ == kTransitionFrame) {
		transitionRequest_ = Transition::kOperation;
	}
}

void TitleScene::ToBlackInitialize() {
	transitionFrameCount_ = 0;
	black_.get()->SetColor({0.0f, 0.0f, 0.0f, 0.0f});
}

void TitleScene::ToBlackUpdate() {
	transitionFrameCount_++;

	float alpha =
	    Ease::UseEase(0.0f, 1.0f, transitionFrameCount_, kTransitionFrame, Ease::EaseInSine, 2);
	black_.get()->SetColor({0.0f, 0.0f, 0.0f, alpha});

	if (transitionFrameCount_ == kTransitionFrame) {
		transitionRequest_ = Transition::kFromBlack;

		sceneManager_->SetSceneRequest(SceneManager::Scene::kSelection);
	}
}

void TitleScene::TitleUpdate() {

	if (countFrame_ == 0) {
		if (isDo_) {
			audio_->PlayWave(soundHandles_[kDoC4]);
		} else {
			audio_->PlayWave(soundHandles_[kFaf3]);
		}
	}

	if (isDo_) {
		neiroWorldTrandform_.rotation_.z = Ease::UseEase(
		    -3.14f / 10.0f, 3.14f / 10.0f, countFrame_, kInterval_, Ease::EaseInOutSine, 3);
	} else {
		neiroWorldTrandform_.rotation_.z = Ease::UseEase(
		    3.14f / 10.0f, -3.14f / 10.0f, countFrame_, kInterval_, Ease::EaseInOutSine, 3);
	}
	neiroWorldTrandform_.UpdateMatrix();

	countFrame_++;

	if (countFrame_ == kInterval_) {
		countFrame_ = 0;
		if (isDo_) {
			isDo_ = false;
		} else {
			isDo_ = true;
		}
	}

	if (input_->TriggerKey(DIK_SPACE)) {
		transitionRequest_ = Transition::kToBlack;
	}
}

void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();

	modelNeiro_.reset(Model::CreateFromOBJ("neiro", true));
	neiroWorldTrandform_.Initialize();

	black_.reset(Sprite::Create(
	    TextureManager::Load("white1x1.png"), {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2},
	    {0.0f, 0.0f, 0.0f, 1.0f}, {0.5f, 0.5f}));
	black_->SetSize({WinApp::kWindowWidth, WinApp::kWindowHeight});

	pianoDo_.reset(Sprite::Create(
	    TextureManager::Load("pianoDo.png"), {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	pianoFa_.reset(Sprite::Create(
	    TextureManager::Load("pianoFa.png"), {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	
	s_.reset(Sprite::Create(
	    TextureManager::Load("space.png"), {WinApp::kWindowWidth / 2, 680},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	uint32_t soundHandle = audio_->LoadWave("doC4.wav");
	soundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("faF3.wav");
	soundHandles_.push_back(soundHandle);
}

void TitleScene::TitleSceneInitialize() {
	countFrame_ = 0;
	isDo_ = false;

	neiroWorldTrandform_.translation_.y = 8.0f;
	neiroWorldTrandform_.scale_ = {15.0f, 15.0f, 15.0f};
	neiroWorldTrandform_.rotation_.z = 3.14f / 10.0f;

	neiroWorldTrandform_.UpdateMatrix();
}

void TitleScene::Update() {
	if (transitionRequest_) {
		transition_ = transitionRequest_.value();

		switch (transition_) {
		case TitleScene::Transition::kFromBlack:
			FromBlackInitialize();
			break;
		case TitleScene::Transition::kOperation:

			break;
		case TitleScene::Transition::kToBlack:
			ToBlackInitialize();
			break;
		default:
			break;
		}

		transitionRequest_ = std::nullopt;
	}

	switch (transition_) {
	case TitleScene::Transition::kFromBlack:
		FromBlackUpdate();
		break;
	case TitleScene::Transition::kOperation:
		TitleUpdate();
		break;
	case TitleScene::Transition::kToBlack:
		ToBlackUpdate();
		break;
	default:
		break;
	}
}

void TitleScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	if (isDo_) {
		pianoDo_->Draw();
	} else {
		pianoFa_->Draw();
	}

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

	modelNeiro_->Draw(neiroWorldTrandform_, viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	s_->Draw();

	if (transition_ == Transition::kFromBlack || transition_ == Transition::kToBlack) {
		black_->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
