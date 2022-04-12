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

	// 乱数シード生成器
	std::random_device seed_gen;
	// メルセンヌ・ツイスター
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

	// カメラ視点座標を設定
	viewProjection.eye = {0, 0, -50};

	// カメラの注視点座標を設定
	viewProjection.target = {10, 0, 0};

	// カメラ上方向ベクトルを設定（右上45度指定）
	viewProjection.up = {1, 0, 0};

	// ビュープロジェクションの初期化
	viewProjection.Initialize();
}

void GameScene::Update() {

	// 視点移動処理
	// 視点の移動ベクトル
	XMFLOAT3 move = {0, 0, 0};

	// 視点の移動の速さ
	const float kEyeSpeed = 0.2f;

	if (input_->PushKey(DIK_W)) {
		move = {0, 0, kEyeSpeed};
	} else if (input_->PushKey(DIK_S)) {
		move = {0, 0, -kEyeSpeed};
	}

	// 視点移動（ベクトルの加算）
	viewProjection.eye.x += move.x;
	viewProjection.eye.y += move.y;
	viewProjection.eye.z += move.z;

	// 注視点の移動処理
	// 注視点の移動ベクトル
	XMFLOAT3 viewTargerMove = {0, 0, 0};

	// 注視点の移動の速さ
	const float viewTargerSpeed = 0.2f;

	if (input_->PushKey(DIK_RIGHT)) {
		viewTargerMove = {viewTargerSpeed, 0, 0};
	} else if (input_->PushKey(DIK_LEFT)) {
		viewTargerMove = {-viewTargerSpeed, 0, 0};
	}

	// 注視点移動（ベクトルの加算）
	viewProjection.target.x += viewTargerMove.x;
	viewProjection.target.y += viewTargerMove.y;
	viewProjection.target.z += viewTargerMove.z;

	// 上方向回転処理
	// 上方向の回転速さ［ラジアン/frame］
	const float kUpRotSpeed = 0.05f;

	// 押した方向で移動ベクトル変更
	if (input_->PushKey(DIK_SPACE)) {
		viewAngle += kUpRotSpeed;
		// 2πを超えたら0に戻す
		viewAngle = fmodf(viewAngle, XM_2PI);
	}

	// 上方向ベクトルを計算（半径１の円周上の座標）
	viewProjection.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};

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
