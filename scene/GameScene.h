#pragma once

#include "Audio.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "Vec3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>

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

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	uint32_t textureHandle = 0;
	Model* model = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform[10];
	// ビュープロジェクション
	ViewProjection viewProjection;

  public:
	enum PartID {
		Root, // 大元
		UpperBody, // 上半身
		Head, // 頭
		Chest,// 胸
		ArmL, // 左腕
		ArmR, // 右腕
		LowerBody, // 下半身
		Hip,  // 尻
		LegL, // 左足
		LegR, // 右足
	};

  private:
	void CharcterInit();
	void CharcterDraw();
};
