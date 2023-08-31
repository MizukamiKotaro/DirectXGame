#pragma once

#include "GameScene.h"
#include "TitleScene.h"
#include "SelectionScene.h"
#include "ResultScene.h"

class SceneManager {
public:

	SceneManager();

	~SceneManager();

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

public:
	
	enum class Scene {
		kTitle,
		kSelection,
		kPlay,
		kResult,
	};

	void SetSceneRequest(const Scene& scene) { sceneRequest_ = scene; }

private:
	

private:


	Scene scene_ = Scene::kTitle;
	std::optional<Scene> sceneRequest_ = std::nullopt;

	std::unique_ptr<TitleScene> titleScene_;
	std::unique_ptr<GameScene> gameScene_;
	std::unique_ptr<SelectionScene> selectionScene_;
	std::unique_ptr<ResultScene> resultScene_;



};
