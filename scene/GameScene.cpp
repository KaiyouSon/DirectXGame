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

	// X, Y, Z 軸周りの平行移動を設定
	worldTransform.translation_ = {0.0f, 0.0f, 0.0f};

	// X, Y, Z 軸周りの回転角を設定
	worldTransform.rotation_ = {0.0f, 0.0f, 0.0f};

	// X, Y, Z 方向のスケーリングを設定
	worldTransform.scale_ = {1.0f, 1.0f, 1.0f};

	// ワールドトランスフォームの初期化
	worldTransform.Initialize();

	for (size_t i = 0; i < _countof(worldTransform2); i++) {

		// X, Y, Z 軸周りの平行移動を設定
		worldTransform2[i].translation_ = {posDist(engine), posDist(engine), posDist(engine)};

		// X, Y, Z 軸周りの回転角を設定
		worldTransform2[i].rotation_ = {rotDist(engine), rotDist(engine), rotDist(engine)};
		// X, Y, Z 方向のスケーリングを設定
		worldTransform2[i].scale_ = {1.0f, 1.0f, 1.0f};

		// ワールドトランスフォームの初期化
		worldTransform2[i].Initialize();
	}

	// カメラ視点座標を設定
	viewProjection.eye = {0.0f, 5.0f, -10.0f};

	// カメラの注視点座標を設定
	viewProjection.target = {0.0f, 0.0f, 0.0f};

	// カメラ上方向ベクトルを設定
	viewProjection.up = {0.0f, 1.0f, 0.0f};

	// ビュープロジェクションの初期化
	viewProjection.Initialize();

	objPos = {0.0f, 0.0f, 0.0f};
	objVec = {0.0f, 0.0f, 1.0f};
	objSpeed = {0.1f, 0.0f, 0.1f};
	angle = 0.0f;
}

void GameScene::Update() {

	// デバッグ用表示

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
	worldTransform.translation_ = {objPos.x, objPos.y, objPos.z};
	worldTransform.rotation_ = {0.0f, XMConvertToRadians(angle), 0.0f};

	worldTransform.UpdateMatrix();

	viewProjection.eye = {objPos.x + (objVec.x * -10), objPos.y + 5, objPos.z + (objVec.z * -10)};

	viewProjection.target = {objPos.x, objPos.y, objPos.z};

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

	debugText_->SetPos(50, 90);
	debugText_->Printf("objPos:(%f,%f,%f)", objPos.x, objPos.y, objPos.z);
	debugText_->SetPos(50, 110);
	debugText_->Printf("objVec:(%f,%f,%f)", objVec.x, objVec.y, objVec.z);
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

	for (size_t i = 0; i < _countof(worldTransform2); i++) {

		model->Draw(worldTransform2[i], viewProjection, textureHandle);
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
