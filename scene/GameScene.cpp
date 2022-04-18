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

	worldTransform[0].translation_ = {0.0f, 5.0f, 0.0f};
	worldTransform[1].translation_ = {cos(XMConvertToRadians(225)) * 6.0f, -2.5f, 0.0f};
	worldTransform[2].translation_ = {cos(XMConvertToRadians(315)) * 6.0f, -2.5f, 0.0f};
	// X, Y, Z 軸周りの平行移動を設定
	for (size_t i = 0; i < _countof(worldTransform); i++) {

		// X, Y, Z 軸周りの回転角を設定
		worldTransform[i].rotation_ = {0.0f, 0.0f, 0.0f};

		// X, Y, Z 方向のスケーリングを設定
		worldTransform[i].scale_ = {1.0f, 1.0f, 1.0f};

		// ワールドトランスフォームの初期化
		worldTransform[i].Initialize();
	}

	// カメラ視点座標を設定
	viewProjection.eye = {viewPos.x, viewPos.y, viewPos.z};

	// カメラの注視点座標を設定
	viewProjection.target = {0.0f, 0.0f, 0.0f};

	// カメラ上方向ベクトルを設定
	viewProjection.up = {0.0f, 1.0f, 0.0f};

	// ビュープロジェクションの初期化
	viewProjection.Initialize();
}

void GameScene::Update() {

	switch (objNum) {
	case 0:
		viewProjection.target = {
		  worldTransform[0].translation_.x, worldTransform[0].translation_.y,
		  worldTransform[0].translation_.z};
		if (input_->TriggerKey(DIK_SPACE))
			objNum = 1;
		break;
	case 1:
		viewProjection.target = {
		  worldTransform[1].translation_.x, worldTransform[1].translation_.y,
		  worldTransform[1].translation_.z};
		if (input_->TriggerKey(DIK_SPACE))
			objNum = 2;
		break;
	case 2:
		viewProjection.target = {
		  worldTransform[2].translation_.x, worldTransform[2].translation_.y,
		  worldTransform[2].translation_.z};
		if (input_->TriggerKey(DIK_SPACE))
			objNum = 0;
		break;
	default:
		break;
	}

	viewProjection.UpdateMatrix();

	// デバッグ用表示
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

	for (size_t i = 0; i < _countof(worldTransform); i++) {

		model->Draw(worldTransform[i], viewProjection, textureHandle);
	}

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
