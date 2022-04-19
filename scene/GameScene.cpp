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

	// 乱数シード生成器a
	std::random_device seed_gen;
	// メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	// 乱数範囲（回転角用）
	std::uniform_real_distribution<float> rotDist(0.0f, XM_PI);
	// 乱数範囲（座標用）
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	for (size_t i = 0; i < _countof(worldTransform); i++) {

		// X, Y, Z 軸周りの平行移動を設定
		worldTransform[i].translation_ = {
		  cos(XMConvertToRadians(i * 36)) * 10, sin(XMConvertToRadians(i * 36)) * 10, 0.0f};

		// X, Y, Z 軸周りの回転角を設定
		worldTransform[i].rotation_ = {0.0f, 0.0f, 0.0f};

		// X, Y, Z 方向のスケーリングを設定
		worldTransform[i].scale_ = {1.0f, 1.0f, 1.0f};

		// ワールドトランスフォームの初期化
		worldTransform[i].Initialize();
	}

	// カメラ視点座標を設定
	viewProjection.eye = {0.0f, 0.0f, -50.0f};

	// カメラの注視点座標を設定
	viewProjection.target = {0.0f, 0.0f, 0.0f};

	// カメラ上方向ベクトルを設定
	viewProjection.up = {0.0f, 1.0f, 0.0f};

	// ビュープロジェクションの初期化
	viewProjection.Initialize();

	angle = 0.0f;
}

void GameScene::Update() {

	angle += 2;
	if (angle > 360)
		angle = 0;

	// デバッグ用表示
	for (size_t i = 0; i < _countof(worldTransform); i++) {

		worldTransform[i].translation_ = {
		  cos(XMConvertToRadians(angle + i * 40)) * 10,
		  sin(XMConvertToRadians(angle + i * 40)) * 10, 0.0f};

		worldTransform[i].UpdateMatrix();
	}
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
