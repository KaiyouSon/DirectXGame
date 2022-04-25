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

	for (size_t i = 0; i < _countof(worldTransform); i++) {
		for (size_t j = 0; j < _countof(worldTransform); j++) {
			// X, Y, Z 軸周りの平行移動を設定
			worldTransform[i][j].translation_ = {-14 + (float)i * 3.5f, 14 + (float)j * -3.5f, 0};
			// X, Y, Z 軸周りの回転角を設定
			worldTransform[i][j].rotation_ = {0, 0, 0};
			// X, Y, Z 方向のスケーリングを設定
			worldTransform[i][j].scale_ = {1, 1, 1};

			// ワールドトランスフォームの初期化
			worldTransform[i][j].Initialize();
		}
	}

	viewProjection.eye = {0, 0, -50};
	viewProjection.target = {0, 0, 0};
	viewProjection.up = {0, 1, 0};

	// カメラ垂直方向視野角を設定
	viewProjection.fovAngleY = XMConvertToRadians(45);

	// ビュープロジェクションの初期化
	viewProjection.Initialize();
}

void GameScene::Update() {

	// FoVの変更処理
	// 上キーで視野角が広がる
	if (input_->PushKey(DIK_UP)) {
		viewProjection.fovAngleY -= 0.01f;
		viewProjection.fovAngleY = max(viewProjection.fovAngleY, 0.01f);
	}
	// 上キーで視野角が狭まる
	else if (input_->PushKey(DIK_DOWN)) {
		viewProjection.fovAngleY += 0.01f;
		viewProjection.fovAngleY = min(viewProjection.fovAngleY, XM_PI);
	}

	// 注視点の移動処理

	const float viewMoveSpd = 0.2f;

	if (input_->PushKey(DIK_W)) {
		viewProjection.target.y += viewMoveSpd;
	}
	if (input_->PushKey(DIK_S)) {
		viewProjection.target.y -= viewMoveSpd;
	}
	if (input_->PushKey(DIK_D)) {
		viewProjection.target.x += viewMoveSpd;
	}
	if (input_->PushKey(DIK_A)) {
		viewProjection.target.x -= viewMoveSpd;
	}

	// 行列の再計算
	viewProjection.UpdateMatrix();

	// デバッグ用表示
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
		for (size_t j = 0; j < _countof(worldTransform); j++) {

			model->Draw(worldTransform[i][j], viewProjection, textureHandle);
		}
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
