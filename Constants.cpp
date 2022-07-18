#include"Constants.h"

//デバッグ用
//const bool ENEMY_CAN_MOVE = false;

const float PI = 3.14159265359f;

//画面モード
const bool FULL_SCREEN = true;
const int WINDOW_WIDTH = 1280;	//ウィンドウモード時のサイズ
const int WINDOW_HEIGHT = 720;

//色系
//const float BACK_GROUND_COLOR[3] = { 0.1254f,0.098f,0.349f };
//const float BACK_GROUND_COLOR[3] = { 0.203f,0.733f,0.949f };
const float BACK_GROUND_COLOR[3] = { 0.99f,0.99f,0.99f };
const XMFLOAT3 GLAY_COLORS[3] =
{
	{0.768f,0.803f,0.874f},
	{0.811f,0.831f,0.870f},
	{0.890f,0.909f,0.949f}
};
const XMFLOAT3 RED_COLOR = { 0.949f,0.203f,0.380f };

//プレイヤー関連
const float PLAYER_ARROW_RADIUS_OFFSET = 33.0f;
const int PLAYER_ARROW_JUMP_RADIUS_OFFSET = 4.0f;		//連続押し込み時の矢印がジャンプする高さのオフセット
const float PLAYER_ARROW_SCALE_OFFSET = 0.05f;
const float PLAYER_ARROW_ANGLE_OFFSET = 360;	//度

//操作関連
const int PUSH_INPUT_WAIT_FRAME = 20;	//連続押し込み入力の許容フレーム
const int ARROW_PUSH_LAYER_FRAME = 10;	//入力後、押し込みにかかる時間

//カメラ関連
const XMFLOAT3 DEFAULT_CAM_POS = { 0,0,-35 };
const XMFLOAT3 DEFAULT_CAM_TARGET = { 0,0,0 };
const float CAMERA_ZOOM_OUT_OFFSET = 16.0f;	//カメラが層の数によってズームアウトするときのZオフセット値

const float SHAKE_POWER_MAX = 0.8f;	//カメラシェイクの最大強さ
const float SHAKE_POWER_MIN = 0.12f;	//カメラシェイクの最小強さ
const int SHAKE_SPAN = 20;	//シェイクを行うフレーム数

const int ZOOM_CAM_MOTION_TOTAL_TIME = 100;	//敵死亡時のカメラモーションにかかる合計時間
const float ZOOM_CAM_MOTION_Z = -7.0f;	//敵死亡時のカメラモーションのZ値

const int CAM_RETURN_WAIT_TIME_AFTER_ENEMY_KILL = 12;	//敵死亡モーション再生終了後、何フレームでカメラが定位置に戻るか
const int RETURN_DEFAULT_CAM_MOTION_TOTAL_TIME = 60;	//定位置に戻るモーションにかかる合計時間

//パネル関連
const int PANEL_NUM_MIN = 4;
const int PANEL_NUM_MAX = 8;
const int LAYER_NUM_MIN = 2;
const int LAYER_NUM_MAX = 4;
const float INNER_SPACE_RADIUS = 7.2f;	//中央の柱（？）半径
const int LAYER_NUM_MAX_LIMIT = 6;	//レイヤー数最大制限（この数より大きくならない）
//const int OUTER_CIRCLE_VERTEX_NUM = 8;	//外円の頂点数（円の滑らかさ）
const int OUTER_CIRCLE_VERTEX_NUM = 4;	//外円の頂点数（円の滑らかさ）
const float PANEL_HALF_HEIGHT = 2.0f;
const float LAYER_OFFSET_RADIUS = 8.0f;	//レイヤー半径（幅）

const int PUSH_LAYER_OFFSET_TOTAL_TIME = 6;	//複数押し込みされたとき、１階層押し込んでから次の階層を押し込むまでのオフセット時間

//押し込み関連
//const int PUSH_TOTAL_TIME = 30;	//押し込みにかかる時間
const int PUSH_TOTAL_TIME = 5;	//押し込みにかかる時間
const int ALL_PUSHED_UNTIL_RESET_TOTAL_TIME = 15;	//全押し込みされてから押し込みリセットされるまでの時間

//敵関連
const int ENEMY_MAX = 256;	//敵の最大数（これ以上のインスタンスは生成しない）
//const float ENEMY_OFFSET_LENGTH = 100.0f;
const float ENEMY_OFFSET_LENGTH = LAYER_OFFSET_RADIUS;
const int ENEMY_KILL_SPAN = 13;	//敵死亡演出で、一体ずつ倒す間隔
const int ENEMY_ALL_KILL_SPAN = 12;		//階層の敵全員倒した後の押し込みの間隔
const int ENEMY_BODY_EMIT_SPAN = 20;	//敵の身体パーティクルのエミット間隔

//姫敵
const int QUEEN_MAX = 256;

//敵ステータス基準値
const float ENEMY_BASE_SPEED_STANDARD = PI / 180.0f;
const int ENEMY_LIFE_SPAN_STANDARD = 1200;
const int ENEMY_KILL_SCORE_STANDARD = 100;

//敵種別ごとのステータス比率
const float ENEMY_BASE_SPEED[ENEMY_TYPE_NUM] = { 1,0,2,1 };
const float ENEMY_LIFE_SPAN[ENEMY_TYPE_NUM] = { 1,2,1,0.5f };	//爆発までの時間（寿命）
const float ENEMY_KILL_SCORE[ENEMY_TYPE_NUM] = { 1,0.75f,0.5f,1.5f };
const XMFLOAT3 ENEMY_COLOR[ENEMY_TYPE_NUM] = {
	{1.0f,0.396f,0.207f},
	{1.0f,0.011f,0.0f},
	{0.180f,0.329f,0.972f},
	{0.780f,0.117f,0.972f}
};

//敵身体パーティクルエミッター
const int ENEMY_BODY_EMIT_NUM_MIN = 1;	//敵の身体パーティクルの最小エミット数
const int ENEMY_BODY_EMIT_NUM_MAX = 2;	//敵の身体パーティクルの最大エミット数
const float ENEMY_BODY_EMIT_RANGE_RADIUS = 0.3f;	//敵の身体パーティクルエミットの範囲
const float ENEMY_BODY_EMIT_RADIUS_MIN = 0.3f;	//敵の身体パーティクルエミット時の最小サイズ
const float ENEMY_BODY_EMIT_RADIUS_MAX = 0.6f;	//敵の身体パーティクルエミット時の最大サイズ
//const float ENEMY_BODY_RADIUS_EXPAND_SPEED = 0.1f;	//敵の身体パーティクルの拡大スピード
const float ENEMY_BODY_SPIN_SPEED = 2; //敵の身体パーティクルの回転スピード
const int ENEMY_BODY_LIFE_SPAN = 40;	//敵の身体パーティクルの寿命
const float ENEMY_BODY_MOVE_SPEED = 0.1f;	//敵の身体パーティクルの飛び散りスピード

//エフェクト関連
const int ENEMY_DEAD_PRIMITIVE_PARTICLE_SPEED_RAND_MIN = 3;
const int ENEMY_DEAD_PRIMITIVE_PARTICLE_SPEED_RAND_MAX = 6;
const int ENEMY_DEAD_PRIMITIVE_PARTICLE_SPIN_RAND_MIN = 90;
const int ENEMY_DEAD_PRIMITIVE_PARTICLE_SPIN_RAND_MAX = 220;
const float ENEMY_DEAD_PRIMITIVE_PARTICLE_RADIUS_RAND_MIN = 0.5f;
const float ENEMY_DEAD_PRIMITIVE_PARTICLE_RADIUS_RAND_MAX = 1.5f;
const float ENEMY_DEAD_PRIMITIVE_PARTICLE_SPEED_ATTENUATION_RATE = 0.95f;
const int ENEMY_DEAD_PRIMITIVE_PARTICLE_LIFE_SPAN = 90;

//スコア関連
const int SCORE_MAX_DIGIT = 10;	//スコアの最大桁数
const int KILL_GET_SCORE = 100;	//１キルにつき得られるスコア
//const float COMBO_ADD_RATE = 0.1f;	//コンボによる倍率増加量
const float COMBO_ADD_RATE = 0.1f;	//コンボによる倍率増加量

//スコアアタック関連
const int ADD_ENEMY_NUM_BY_ADD_LAYER_MIN = 2;
const int ADD_ENEMY_NUM_BY_ADD_LAYER_MAX = 8;
const int ADD_QUEEN_NUM_BY_ADD_LAYER = 3;