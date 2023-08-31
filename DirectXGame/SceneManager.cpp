#include "SceneManager.h"

SceneManager::SceneManager() {}

SceneManager::~SceneManager() {}

void SceneManager::Initialize() {

	titleScene_ = std::make_unique<TitleScene>();
	selectionScene_ = std::make_unique<SelectionScene>();
	gameScene_ = std::make_unique<GameScene>();
	resultScene_ = std::make_unique<ResultScene>();

	titleScene_->Initialize();
	titleScene_->SetSceneManager(this);

	selectionScene_->Initialize();
	selectionScene_->SetSceneManager(this);

	gameScene_->Initialize();
	gameScene_->SetSceneManager(this);

	gameScene_->SetSelectionScene(selectionScene_.get());

	resultScene_->Initialize();
	resultScene_->SetSceneManager(this);

	resultScene_->SetGameScene(gameScene_.get());

	sceneRequest_ = Scene::kTitle;
}

void SceneManager::Update() {

	if (sceneRequest_) {

		scene_ = sceneRequest_.value();

		switch (scene_) {
		case SceneManager::Scene::kTitle:

			titleScene_->TitleSceneInitialize();
			break;
		case SceneManager::Scene::kSelection:

			selectionScene_->SelectionSceneInitialize();
			break;
		case SceneManager::Scene::kPlay:

			gameScene_->GameSceneInitialize();
			break;
		case SceneManager::Scene::kResult:

			resultScene_->ResultSceneInitialize();
			break;
		default:
			break;
		}

		sceneRequest_ = std::nullopt;

	}

	switch (scene_) {
	case SceneManager::Scene::kTitle:

		titleScene_->Update();
		break;
	case SceneManager::Scene::kSelection:

		selectionScene_->Update();
		break;
	case SceneManager::Scene::kPlay:

		gameScene_->Update();
		break;
	case SceneManager::Scene::kResult:

		resultScene_->Update();
		break;
	default:
		break;
	}

}

void SceneManager::Draw() {

	switch (scene_) {
	case SceneManager::Scene::kTitle:

		titleScene_->Draw();
		break;
	case SceneManager::Scene::kSelection:

		selectionScene_->Draw();
		break;
	case SceneManager::Scene::kPlay:

		gameScene_->Draw();
		break;
	case SceneManager::Scene::kResult:

		resultScene_->Draw();
		break;
	default:
		break;
	}
}
