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
	// 乱数範囲（座標用）
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	// X, Y, Z 軸周りの平行移動を設定
	worldTransform.translation_ = {0.0f, 0.0f, 0.0f};

	// X, Y, Z 軸周りの回転角を設定
	worldTransform.rotation_ = {0.0f, 0.0f, 0.0f};

	// X, Y, Z 方向のスケーリングを設定
	worldTransform.scale_ = {1.0f, 1.0f, 1.0f};

	// ワールドトランスフォームの初期化
	worldTransform.Initialize();

	for (size_t i = 0; i < _countof(viewProjection); i++) {

		// カメラ視点座標を設定
		viewProjection[i].eye = {posDist(engine), posDist(engine), posDist(engine)};

		// カメラの注視点座標を設定
		viewProjection[i].target = {0.0f, 0.0f, 0.0f};

		// カメラ上方向ベクトルを設定
		viewProjection[i].up = {0.0f, 1.0f, 0.0f};

		// ビュープロジェクションの初期化
		viewProjection[i].Initialize();
	}
}

void GameScene::Update() {

	// カメラを変更する処理
	if (input_->TriggerKey(DIK_SPACE)) {
		switch (viewNum) {
		case 0:
			viewNum = 1;
			break;
		case 1:
			viewNum = 2;
			break;
		case 2:
			viewNum = 0;
			break;
		default:
			viewNum = 0;
			break;
		}
	}

	// デバッグ用表示
	for (size_t i = 0; i < _countof(viewProjection); i++) {

		// カメラの番号
		debugText_->SetPos(50, 50 + i * 100);
		debugText_->Printf("Camera%d", i + 1);

		// カメラ視点座標
		debugText_->SetPos(50, 70 + i * 100);
		debugText_->Printf(
		  "eye:(%f,%f,%f)", viewProjection[i].eye.x, viewProjection[i].eye.y,
		  viewProjection[i].eye.z);

		// カメラの注視点座標
		debugText_->SetPos(50, 90 + i * 100);
		debugText_->Printf(
		  "target:(%f,%f,%f)", viewProjection[i].target.x, viewProjection[i].target.y,
		  viewProjection[i].target.z);

		// カメラ上方向ベクトル
		debugText_->SetPos(50, 110 + i * 100);
		debugText_->Printf(
		  "up:(%f,%f,%f)", viewProjection[i].up.x, viewProjection[i].up.y, viewProjection[i].up.z);
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

	model->Draw(worldTransform, viewProjection[viewNum], textureHandle);

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
