#include "GameScene.h"
#include "TextureManager.h"
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

	for (size_t i = 0; i < _countof(worldTransform); i++) {
		for (size_t j = 0; j < _countof(worldTransform); j++) {
			// X, Y, Z 軸周りの平行移動を設定
			worldTransform[i][j].translation_ = {-15 + (float)i * 3.5f, 15 + (float)j * -3.5f, 0};
			// X, Y, Z 軸周りの回転角を設定
			worldTransform[i][j].rotation_ = {0, 0, 0};
			// X, Y, Z 方向のスケーリングを設定
			worldTransform[i][j].scale_ = {1, 1, 1};

			// ワールドトランスフォームの初期化
			worldTransform[i][j].Initialize();
		}
	}

	// ビュープロジェクションの初期化
	viewProjection.Initialize();
}

void GameScene::Update() {

	// 変数の値をインクリメント
	// debugText_->SetPos(50, 220);
	// debugText_->Printf(
	//  "translation:(%f,%f,%f)", worldTransform[12].translation_.x,
	//  worldTransform[12].translation_.y, worldTransform[12].translation_.z);

	// debugText_->SetPos(50, 200);
	// debugText_->Printf("tmp%d", _countof(worldTransform));
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
			if (!(i % 2 == 1 && j % 2 == 1)) {
				model->Draw(worldTransform[i][j], viewProjection, textureHandle);
			}
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
