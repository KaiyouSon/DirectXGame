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
	WorldTransform worldTransform[14];
	// ビュープロジェクション
	ViewProjection viewProjection;

	float moveAngle = 0.05;
	float moveAngle2 = 0;
	bool isChangeRotation = false;
	bool isGreaterThanZeroAngle = false;
	int state = 0;
	float angleSpd = 0.2f;
	int angle2Spd = 3;

  public:
	enum PartID {
		Root,      // 大元
		UpperBody, // 上半身
		Head,      // 頭
		Chest,     // 胸
		ArmL,      // 左腕
		ArmR,      // 右腕
		HandL,     // 左手
		HandR,     // 右手
		LowerBody, // 下半身
		Hip,       // 尻
		LegL,      // 左足
		LegR,      // 右足
		FootL,     // 左足元
		FootR,     // 右足元
	};

	enum State {
		Idle,
		Run,
	};

  private:
	void CharcterInit();
	void CharcterDraw();

	bool CheckmoveAngle2();

	void IdleUpdate();
	void RunUpdate();
	void OtherStatusUpdate();
};
