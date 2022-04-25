#include "GameScene.h"
#include "TextureManager.h"
#include "Vec3.h"
#include <cassert>
#include <random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model;
	delete reticleSprite;
	delete scopeSprite;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle = TextureManager::Load("mario.jpg");
	model = Model::Create();
	reticle = TextureManager::Load("reticle.png");
	reticleSprite = Sprite::Create(reticle, {0, 0});
	scope = TextureManager::Load("scope.png");
	scopeSprite = Sprite::Create(scope, {0, 0});

	// 乱数シード生成器
	std::random_device seed_gen;
	// メルセンヌ・ツイスターab
	std::mt19937_64 engine(seed_gen());
	// 乱数範囲（回転角用）
	std::uniform_real_distribution<float> rotDist(0.0f, XM_PI);
	// 乱数範囲（座標用）
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	for (size_t i = 0; i < _countof(worldTransform); i++) {

		// X, Y, Z 軸周りの平行移動を設定
		worldTransform[i].translation_ = {posDist(engine), posDist(engine), posDist(engine)};

		// X, Y, Z 軸周りの回転角を設定
		worldTransform[i].rotation_ = {rotDist(engine), rotDist(engine), rotDist(engine)};

		// X, Y, Z 方向のスケーリングを設定
		worldTransform[i].scale_ = {1.0f, 1.0f, 1.0f};

		// ワールドトランスフォームの初期化
		worldTransform[i].Initialize();
	}

	viewProjection.eye = {0, 0, 50};
	viewProjection.target = {0, 0, 0};
	viewProjection.up = {0, 1, 0};

	// カメラ垂直方向視野角を設定
	viewProjection.fovAngleY = XMConvertToRadians(addViewFovAngleY);

	// ビュープロジェクションの初期化
	viewProjection.Initialize();
}

void GameScene::Update() {

	// FoVの変更処理
	if (input_->TriggerKey(DIK_SPACE)) {
		if (isExpansion == true)
			isExpansion = false;
		else
			isExpansion = true;
	}

	if (isExpansion == true) {

		if (input_->TriggerKey(DIK_W)) {
			isChangeRate = true;
		} else if (input_->TriggerKey(DIK_S)) {
			isChangeRate = false;
		}

		if (isChangeRate == true) {
			if (addViewFovAngleY > 17)
				addViewFovAngleY -= 2;
		} else {
			if (addViewFovAngleY < 34)
				addViewFovAngleY += 2;
			else
				addViewFovAngleY = 34;
		}

	} else {

		addViewFovAngleY = 90;
	}

	viewProjection.fovAngleY = XMConvertToRadians(addViewFovAngleY);

	// 注視点の移動処理
	const float viewMoveSpd = 0.2f;

	if (input_->PushKey(DIK_UP)) {
		viewProjection.target.y += viewMoveSpd;
	}
	if (input_->PushKey(DIK_DOWN)) {
		viewProjection.target.y -= viewMoveSpd;
	}
	if (input_->PushKey(DIK_LEFT)) {
		viewProjection.target.x += viewMoveSpd;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		viewProjection.target.x -= viewMoveSpd;
	}

	// 行列の再計算
	viewProjection.UpdateMatrix();

	Vec3 WinSize(1280, 720, 0);
	Vec3 reticlePos(WinSize.x / 2, WinSize.y / 2, 0);
	int reticleSize = 64;
	Vec3 scopePos(WinSize.x / 2, WinSize.y / 2, 0);
	Vec3 scopeSize(640, 360, 0);

	reticleSprite->SetPosition({reticlePos.x - reticleSize, reticlePos.y - reticleSize});
	scopeSprite->SetPosition({scopePos.x - scopeSize.x, scopePos.y - scopeSize.y});

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

	if (isExpansion == true) {
		if (isChangeRate == true) {
			debugText_->SetPos(1000, 90);
			debugText_->Printf("x8");
		} else {
			debugText_->SetPos(1000, 90);
			debugText_->Printf("x4");
		}
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
	if (isExpansion == true) {
		reticleSprite->Draw();
		scopeSprite->Draw();
	}

	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
