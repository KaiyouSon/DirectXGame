#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { delete model; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle = TextureManager::Load("mario.jpg");
	model = Model::Create();

	CharcterInit();

	viewProjection.eye = {0.0f, 3.0f, -30.0f};
	viewProjection.target = {0.0f, 0.0f, 0.0f};
	viewProjection.up = {0.0f, 1.0f, 0.0f};
	viewProjection.fovAngleY = XMConvertToRadians(45);
	viewProjection.Initialize();
}

void GameScene::Update() {

	void (GameScene::*Functbl[])() = {
	  &GameScene::IdleUpdate,
	  &GameScene::RunUpdate,
	};

	(this->*Functbl[state])();
	OtherStatusUpdate();

	for (size_t i = 0; i < _countof(worldTransform); i++) {

		worldTransform[i].UpdateMatrix();
	}
	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "eye:(%f,%f,%f)", viewProjection.eye.x, viewProjection.eye.y, viewProjection.eye.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "target:(%f,%f,%f)", viewProjection.target.x, viewProjection.target.y,
	  viewProjection.target.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "up:(%f,%f,%f)", viewProjection.up.x, viewProjection.up.y, viewProjection.up.z);

	debugText_->SetPos(50, 110);
	debugText_->Printf("fovAngleY(Degree):%f", XMConvertToDegrees(viewProjection.fovAngleY));

	debugText_->SetPos(50, 130);
	debugText_->Printf("nearZ:%f", viewProjection.nearZ);

	debugText_->SetPos(50, 150);
	debugText_->Printf(
	  "Root:(%f,%f,%f)", worldTransform[PartID::Root].translation_.x,
	  worldTransform[PartID::Root].translation_.y, worldTransform[PartID::Root].translation_.z);
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

	CharcterDraw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CharcterInit() {

	// 大元の初期化
	worldTransform[PartID::Root].translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform[PartID::Root].rotation_ = {0.0f, XMConvertToRadians(30), 0.0f};
	worldTransform[PartID::Root].scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform[PartID::Root].Initialize();

	// 上半身の初期化
	worldTransform[PartID::UpperBody].parent_ = &worldTransform[PartID::Root];
	worldTransform[PartID::UpperBody].translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform[PartID::UpperBody].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform[PartID::UpperBody].scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform[PartID::UpperBody].Initialize();

	// 頭の初期化
	worldTransform[PartID::Head].parent_ = &worldTransform[PartID::UpperBody];
	worldTransform[PartID::Head].translation_ = {0.0f, 2.5f, 0.0f};
	worldTransform[PartID::Head].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform[PartID::Head].scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform[PartID::Head].Initialize();

	// 胸の初期化
	worldTransform[PartID::Chest].parent_ = &worldTransform[PartID::UpperBody];
	worldTransform[PartID::Chest].translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform[PartID::Chest].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform[PartID::Chest].scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform[PartID::Chest].Initialize();

	// 左腕の初期化
	worldTransform[PartID::ArmL].parent_ = &worldTransform[PartID::UpperBody];
	worldTransform[PartID::ArmL].translation_ = {2.5f, 0.0f, 0.0f};
	worldTransform[PartID::ArmL].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform[PartID::ArmL].scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform[PartID::ArmL].Initialize();

	// 右腕の初期化
	worldTransform[PartID::ArmR].parent_ = &worldTransform[PartID::UpperBody];
	worldTransform[PartID::ArmR].translation_ = {-2.5f, 0.0f, 0.0f};
	worldTransform[PartID::ArmR].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform[PartID::ArmR].scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform[PartID::ArmR].Initialize();

	// 左手の初期化
	worldTransform[PartID::HandL].parent_ = &worldTransform[PartID::ArmL];
	worldTransform[PartID::HandL].translation_ = {0.0f, -2.0f, 0.0f};
	worldTransform[PartID::HandL].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform[PartID::HandL].scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform[PartID::HandL].Initialize();

	// 右手の初期化
	worldTransform[PartID::HandR].parent_ = &worldTransform[PartID::ArmR];
	worldTransform[PartID::HandR].translation_ = {0.0f, -2.0f, 0.0f};
	worldTransform[PartID::HandR].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform[PartID::HandR].scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform[PartID::HandR].Initialize();

	// 下半身の初期化
	worldTransform[PartID::LowerBody].parent_ = &worldTransform[PartID::Root];
	worldTransform[PartID::LowerBody].translation_ = {0.0f, -2.5f, 0.0f};
	worldTransform[PartID::LowerBody].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform[PartID::LowerBody].scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform[PartID::LowerBody].Initialize();

	// 尻の初期化
	worldTransform[PartID::Hip].parent_ = &worldTransform[PartID::LowerBody];
	worldTransform[PartID::Hip].translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform[PartID::Hip].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform[PartID::Hip].scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform[PartID::Hip].Initialize();

	// 左足の初期化
	worldTransform[PartID::LegL].parent_ = &worldTransform[PartID::LowerBody];
	worldTransform[PartID::LegL].translation_ = {2.5f, -2.5f, 0.0f};
	worldTransform[PartID::LegL].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform[PartID::LegL].scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform[PartID::LegL].Initialize();

	// 右足の初期化
	worldTransform[PartID::LegR].parent_ = &worldTransform[PartID::LowerBody];
	worldTransform[PartID::LegR].translation_ = {-2.5f, -2.5f, 0.0f};
	worldTransform[PartID::LegR].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform[PartID::LegR].scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform[PartID::LegR].Initialize();

	// 左足元の初期化
	worldTransform[PartID::FootL].parent_ = &worldTransform[PartID::LegL];
	worldTransform[PartID::FootL].translation_ = {0.0f, -2.0f, 0.0f};
	worldTransform[PartID::FootL].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform[PartID::FootL].scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform[PartID::FootL].Initialize();

	// 右足元の初期化
	worldTransform[PartID::FootR].parent_ = &worldTransform[PartID::LegR];
	worldTransform[PartID::FootR].translation_ = {0.0f, -2.0f, 0.0f};
	worldTransform[PartID::FootR].rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform[PartID::FootR].scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform[PartID::FootR].Initialize();
}

void GameScene::CharcterDraw() {
	model->Draw(worldTransform[PartID::Head], viewProjection, textureHandle);
	model->Draw(worldTransform[PartID::Chest], viewProjection, textureHandle);
	model->Draw(worldTransform[PartID::ArmL], viewProjection, textureHandle);
	model->Draw(worldTransform[PartID::ArmR], viewProjection, textureHandle);
	model->Draw(worldTransform[PartID::HandL], viewProjection, textureHandle);
	model->Draw(worldTransform[PartID::HandR], viewProjection, textureHandle);
	model->Draw(worldTransform[PartID::Hip], viewProjection, textureHandle);
	model->Draw(worldTransform[PartID::LegL], viewProjection, textureHandle);
	model->Draw(worldTransform[PartID::LegR], viewProjection, textureHandle);
	model->Draw(worldTransform[PartID::FootL], viewProjection, textureHandle);
	model->Draw(worldTransform[PartID::FootR], viewProjection, textureHandle);
}

bool GameScene::CheckmoveAngle2() {
	if (moveAngle2 > 0)
		return true;
	if (moveAngle2 < 0)
		return false;
}

void GameScene::IdleUpdate() {
	if (CheckmoveAngle2() == true) {
		moveAngle2 -= angle2Spd;
		if (moveAngle2 <= 0)
			moveAngle2 = 0;
	}
	if (CheckmoveAngle2() == false) {
		moveAngle2 += angle2Spd;
		if (moveAngle2 >= 0)
			moveAngle2 = 0;
	}

	if (input_->PushKey(DIK_W))
		state = Run;
}

void GameScene::RunUpdate() {
	if (moveAngle2 <= -45)
		isChangeRotation = true;
	if (moveAngle2 >= 45)
		isChangeRotation = false;

	if (isChangeRotation == true)
		moveAngle2 += angle2Spd;
	if (isChangeRotation == false)
		moveAngle2 -= angle2Spd;

	if (input_->PushKey(DIK_W) == false)
		state = Idle;
}

void GameScene::OtherStatusUpdate() {

	if (input_->PushKey(DIK_LEFT))
		worldTransform[PartID::Root].translation_.x += angleSpd;
	if (input_->PushKey(DIK_RIGHT))
		worldTransform[PartID::Root].translation_.x -= angleSpd;

	if (input_->PushKey(DIK_D))
		worldTransform[PartID::Root].rotation_.y += moveAngle;
	if (input_->PushKey(DIK_A))
		worldTransform[PartID::Root].rotation_.y -= moveAngle;

	worldTransform[PartID::ArmL].rotation_.x = +XMConvertToRadians(moveAngle2);
	worldTransform[PartID::ArmR].rotation_.x = -XMConvertToRadians(moveAngle2);
	worldTransform[PartID::LegL].rotation_.x = -XMConvertToRadians(moveAngle2);
	worldTransform[PartID::LegR].rotation_.x = +XMConvertToRadians(moveAngle2);
}
