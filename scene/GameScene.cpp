#include "GameScene.h"
#include "TextureManager.h"
#include "Vec3.h"
#include <cassert>

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
	// ワールドトランスフォームの初期化
	worldTransform.Initialize();
	// ビュープロジェクションの初期化
	viewProjection.Initialize();
}

Vec3 pos = {10, 10, 10};
Vec3 angle = {XM_PI / 4, XM_PI / 4, 0};
Vec3 scale = {5, 5, 5};

void GameScene::Update() {

	// X, Y, Z 軸周りの平行移動を設定
	worldTransform.translation_ = {pos.x, pos.y, pos.z};
	// X, Y, Z 軸周りの回転角を設定
	worldTransform.rotation_ = {angle.x, angle.y, angle.z};
	// X, Y, Z 方向のスケーリングを設定
	worldTransform.scale_ = {scale.x, scale.y, scale.z};

	worldTransform.UpdateMatrix();

	// 変数の値をインクリメント
	debugText_->SetPos(50, 50);
	debugText_->Printf("translation:(%f,%f,%f)", pos.x, pos.y, pos.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf("rotation:(%f,%f,%f)", angle.x, angle.y, angle.z);
	debugText_->SetPos(50, 90);
	debugText_->Printf("scale:(%f,%f,%f)", scale.x, scale.y, scale.z);
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
