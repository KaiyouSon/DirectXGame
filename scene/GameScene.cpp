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

	// X, Y, Z 軸周りの平行移動を設定
	worldTransform.translation_ = {0.0f, 0.0f, 0.0f};

	// X, Y, Z 軸周りの回転角を設定
	worldTransform.rotation_ = {0.0f, 0.0f, 0.0f};

	// X, Y, Z 方向のスケーリングを設定
	worldTransform.scale_ = {1.0f, 1.0f, 1.0f};

	// ワールドトランスフォームの初期化
	worldTransform.Initialize();

	// カメラ視点座標を設定
	viewProjection.eye = {0.0f, 0.0f, -10.0f};

	// カメラの注視点座標を設定
	viewProjection.target = {0.0f, 0.0f, 0.0f};

	// カメラ上方向ベクトルを設定
	viewProjection.up = {0.0f, 0.0f, 1.0f};

	// ビュープロジェクションの初期化
	viewProjection.Initialize();

	objPos = {0.0f, 0.0f, 0.0f};
	objVec = {0.0f, 0.0f, 1.0f};
	objSpeed = {0.1f, 0.1f, 0.1f};
	angle = 0.0f;
}

void GameScene::Update() {

	// デバッグ用表示

	worldTransform.translation_ = {objPos.x, objPos.y, objPos.z};
	worldTransform.rotation_ = {0.0f, XMConvertToRadians(angle), 0.0f};

	if (input_->PushKey(DIK_UP)) {

		objPos += objVec * objSpeed;
	}
	if (input_->PushKey(DIK_DOWN)) {

		objPos -= objVec * objSpeed;
	}
	if (input_->PushKey(DIK_LEFT)) {
		angle -= 2;
		if (angle < 0)
			angle = 360;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		angle += 2;
		if (angle > 360)
			angle = 0;
	}

	objVec = {sin(XMConvertToRadians(angle)), 0.0f, cos(XMConvertToRadians(angle))};

	worldTransform.UpdateMatrix();

	viewProjection.eye = {0.0f, 10.0f, 0.0f};

	viewProjection.UpdateMatrix();

	// カメラ視点座標
	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "eye:(%f,%f,%f)", viewProjection.eye.x, viewProjection.eye.y, viewProjection.eye.z);

	// カメラの注視点座標
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "target:(%f,%f,%f)", viewProjection.target.x, viewProjection.target.y,
	  viewProjection.target.z);

	// カメラ上方向ベクトル
	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "up:(%f,%f,%f)", viewProjection.up.x, viewProjection.up.y, viewProjection.up.z);
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

	model->Draw(worldTransform, viewProjection, textureHandle);

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
