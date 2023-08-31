#include "TextureManager.h"
#include "SelectionScene.h"
#include <cassert>
#include <fstream>

#include "Ease.h"
#include "SceneManager.h"

SelectionScene::SelectionScene() {}

SelectionScene::~SelectionScene() {
	
}

void SelectionScene::FromBlackInitialize() {
	transitionFrameCount_ = 0;
	black_.get()->SetColor({0.0f, 0.0f, 0.0f, 1.0f});
}

void SelectionScene::FromBlackUpdate() {
	transitionFrameCount_++;

	float alpha =
	    Ease::UseEase(1.0f, 0.0f, transitionFrameCount_, kTransitionFrame, Ease::EaseInSine, 2);
	black_.get()->SetColor({0.0f, 0.0f, 0.0f, alpha});

	if (transitionFrameCount_ == kTransitionFrame) {
		transitionRequest_ = Transition::kOperation;
	}
}

void SelectionScene::ToBlackInitialize() {
	transitionFrameCount_ = 0;
	black_.get()->SetColor({0.0f, 0.0f, 0.0f, 0.0f});
}

void SelectionScene::ToBlackUpdate() {
	transitionFrameCount_++;

	float alpha =
	    Ease::UseEase(0.0f, 1.0f, transitionFrameCount_, kTransitionFrame, Ease::EaseInSine, 2);
	black_.get()->SetColor({0.0f, 0.0f, 0.0f, alpha});

	if (transitionFrameCount_ == kTransitionFrame) {
		transitionRequest_ = Transition::kFromBlack;

		sceneManager_->SetSceneRequest(SceneManager::Scene::kPlay);
	}
}

void SelectionScene::SelectUpDate() {

	switch (stage_) {
	case SelectionScene::Stage::kSakuraSakura:

		if (input_->TriggerKey(DIK_SPACE)) {
			state_ = State::kSet;
			setSelect_ = SetSelect::kPlay;

			audio_->PlayWave(soundHandles_[0]);
		} else if (input_->TriggerKey(DIK_RIGHT) || input_->TriggerKey(DIK_D)) {
			stage_ = Stage::kOrary;

			audio_->PlayWave(soundHandles_[0]);
		}
		break;
	case SelectionScene::Stage::kOrary:

		if (input_->TriggerKey(DIK_SPACE)) {
			state_ = State::kSet;
			setSelect_ = SetSelect::kPlay;
			audio_->PlayWave(soundHandles_[0]);
		} 
		/*else if (input_->TriggerKey(DIK_RIGHT) || input_->TriggerKey(DIK_D)) {
			stage_ = Stage::kMi0;
		}*/
		else if (input_->TriggerKey(DIK_LEFT) || input_->TriggerKey(DIK_A)) {
			stage_ = Stage::kSakuraSakura;
			audio_->PlayWave(soundHandles_[1]);
		}

		break;
	case SelectionScene::Stage::kMi0:

		if (input_->TriggerKey(DIK_SPACE)) {
			state_ = State::kSet;
			setSelect_ = SetSelect::kPlay;
		} else if (input_->TriggerKey(DIK_RIGHT) || input_->TriggerKey(DIK_D)) {
			stage_ = Stage::kFa0;
		} else if (input_->TriggerKey(DIK_LEFT) || input_->TriggerKey(DIK_A)) {
			stage_ = Stage::kOrary;
		}

		break;
	case SelectionScene::Stage::kFa0:

		if (input_->TriggerKey(DIK_SPACE)) {
			state_ = State::kSet;
			setSelect_ = SetSelect::kPlay;
		} else if (input_->TriggerKey(DIK_RIGHT) || input_->TriggerKey(DIK_D)) {
			stage_ = Stage::kSo0;
		} else if (input_->TriggerKey(DIK_LEFT) || input_->TriggerKey(DIK_A)) {
			stage_ = Stage::kMi0;
		}

		break;
	case SelectionScene::Stage::kSo0:

		if (input_->TriggerKey(DIK_SPACE)) {
			state_ = State::kSet;
			setSelect_ = SetSelect::kPlay;
		} else if (input_->TriggerKey(DIK_RIGHT) || input_->TriggerKey(DIK_D)) {
			stage_ = Stage::kRa0;
		} else if (input_->TriggerKey(DIK_LEFT) || input_->TriggerKey(DIK_A)) {
			stage_ = Stage::kFa0;
		}

		break;
	case SelectionScene::Stage::kRa0:

		if (input_->TriggerKey(DIK_SPACE)) {
			state_ = State::kSet;
			setSelect_ = SetSelect::kPlay;
		} else if (input_->TriggerKey(DIK_RIGHT) || input_->TriggerKey(DIK_D)) {
			stage_ = Stage::kSi0;
		} else if (input_->TriggerKey(DIK_LEFT) || input_->TriggerKey(DIK_A)) {
			stage_ = Stage::kSo0;
		}

		break;
	case SelectionScene::Stage::kSi0:

		if (input_->TriggerKey(DIK_SPACE)) {
			state_ = State::kSet;
			setSelect_ = SetSelect::kPlay;
		} else if (input_->TriggerKey(DIK_RIGHT) || input_->TriggerKey(DIK_D)) {
			stage_ = Stage::kDo1;
		} else if (input_->TriggerKey(DIK_LEFT) || input_->TriggerKey(DIK_A)) {
			stage_ = Stage::kRa0;
		}

		break;
	case SelectionScene::Stage::kDo1:

		if (input_->TriggerKey(DIK_SPACE)) {
			state_ = State::kSet;
			setSelect_ = SetSelect::kPlay;
		} else if (input_->TriggerKey(DIK_LEFT) || input_->TriggerKey(DIK_A)) {
			stage_ = Stage::kSi0;
		}

		break;
	default:
		break;
	}

}

void SelectionScene::SelectionUpdate() {

	switch (state_) {
	case SelectionScene::State::kSelect:

		SelectUpDate();
		break;
	case SelectionScene::State::kSet:

		if (input_->TriggerKey(DIK_Z)) {
			state_ = State::kSelect;
			
		}
		else if (setSelect_ == SetSelect::kPlay) {
			
			if (input_->TriggerKey(DIK_SPACE)) {
				transitionRequest_ = Transition::kToBlack;
				audio_->PlayWave(soundHandles_[0]);

			}else if (input_->TriggerKey(DIK_LEFT) || input_->TriggerKey(DIK_A)) {
				setSelect_ = SetSelect::kEzit;
				audio_->PlayWave(soundHandles_[1]);
			}
		}else {
			if (input_->TriggerKey(DIK_SPACE)) {
				state_ = State::kSelect;
				setSelect_ = SetSelect::kPlay;
				audio_->PlayWave(soundHandles_[0]);
			} else if (input_->TriggerKey(DIK_RIGHT) || input_->TriggerKey(DIK_D)) {
				setSelect_ = SetSelect::kPlay;
				audio_->PlayWave(soundHandles_[0]);
			}
		}
		

		break;
	case SelectionScene::State::kSpeedSet:

		if (input_->TriggerKey(DIK_SPACE)) {
			state_ = State::kSet;
			setSelect_ = SetSelect::kPlay;
			audio_->PlayWave(soundHandles_[0]);
		} else if (input_->TriggerKey(DIK_RIGHT) || input_->TriggerKey(DIK_D)) {
			if (speedNum != 9) {
				speedNum++;
			}
		} else if (input_->TriggerKey(DIK_LEFT) || input_->TriggerKey(DIK_A)) {
			if (speedNum != 1) {
				speedNum--;
			}
		}

		break;
	default:
		break;
	}
	
}

void SelectionScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();

	black_.reset(Sprite::Create(
	    TextureManager::Load("white1x1.png"), {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2},
	    {0.0f, 0.0f, 0.0f, 1.0f}, {0.5f, 0.5f}));
	black_->SetSize({WinApp::kWindowWidth, WinApp::kWindowHeight});

	stageSelectDo_.reset(Sprite::Create(
	    TextureManager::Load("stageSelectDo.png"), {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	
	stageSelectRe_.reset(Sprite::Create(
	    TextureManager::Load("stageSelectRe.png"),
	    {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2}, {1.0f, 1.0f, 1.0f, 1.0f},
	    {0.5f, 0.5f}));

	stageSelectMi_.reset(Sprite::Create(
	    TextureManager::Load("stageSelectMi.png"),
	    {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2}, {1.0f, 1.0f, 1.0f, 1.0f},
	    {0.5f, 0.5f}));

	sakurasakura_.reset(Sprite::Create(
	    TextureManager::Load("sakurasakura.png"),
	    {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2 - 150}, {1.0f, 1.0f, 1.0f, 1.0f},
	    {0.5f, 0.5f}));

	orary_.reset(Sprite::Create(
	    TextureManager::Load("orary.png"),
	    {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2 - 150}, {1.0f, 1.0f, 1.0f, 1.0f},
	    {0.5f, 0.5f}));

	start_.reset(Sprite::Create(
	    TextureManager::Load("start.png"),
	    {WinApp::kWindowWidth / 2 , WinApp::kWindowHeight / 2 + 200}, {1.0f, 1.0f, 1.0f, 1.0f},
	    {0.5f, 0.5f}));

	exit_.reset(Sprite::Create(
	    TextureManager::Load("exit.png"),
	    {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2 + 200}, {1.0f, 1.0f, 1.0f, 1.0f},
	    {0.5f, 0.5f}));

	/*speed_.reset(Sprite::Create(
	    TextureManager::Load("speed.png"),
	    {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2 + 200}, {1.0f, 1.0f, 1.0f, 1.0f},
	    {0.5f, 0.5f}));*/

	uint32_t soundHandle = audio_->LoadWave("doC4.wav");
	soundHandles_.push_back(soundHandle);
	soundHandle = audio_->LoadWave("faF3.wav");
	soundHandles_.push_back(soundHandle);

}

void SelectionScene::SelectionSceneInitialize() {

	state_ = State::kSelect;

}

void SelectionScene::Update() {
	if (transitionRequest_) {
		transition_ = transitionRequest_.value();

		switch (transition_) {
		case SelectionScene::Transition::kFromBlack:
			FromBlackInitialize();
			break;
		case SelectionScene::Transition::kOperation:

			break;
		case SelectionScene::Transition::kToBlack:
			ToBlackInitialize();
			break;
		default:
			break;
		}

		transitionRequest_ = std::nullopt;
	}

	switch (transition_) {
	case SelectionScene::Transition::kFromBlack:
		FromBlackUpdate();
		break;
	case SelectionScene::Transition::kOperation:
		SelectionUpdate();
		break;
	case SelectionScene::Transition::kToBlack:
		ToBlackUpdate();
		break;
	default:
		break;
	}
}

void SelectionScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	switch (stage_) {
	case SelectionScene::Stage::kSakuraSakura:
		stageSelectDo_->Draw();
		sakurasakura_->Draw();

		break;
	case SelectionScene::Stage::kOrary:

		stageSelectRe_->Draw();
		orary_->Draw();
		break;
	case SelectionScene::Stage::kMi0:

		stageSelectMi_->Draw();
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

	if (state_ != State::kSelect) {
		switch (setSelect_) {
		case SelectionScene::SetSelect::kPlay:

			start_->Draw();
			break;
		case SelectionScene::SetSelect::kSet:

			//speed_->Draw();
			break;
		case SelectionScene::SetSelect::kEzit:

			exit_->Draw();
			break;
		default:
			break;
		}
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

	

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	if (transition_ == Transition::kFromBlack || transition_ == Transition::kToBlack) {
		black_->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
