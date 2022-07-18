#pragma once
#include<DirectXMath.h>
using namespace DirectX;

//デバッグ用
//extern const bool ENEMY_CAN_MOVE;

extern const float PI;

//画面モード
extern const bool FULL_SCREEN;
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

//色系
extern const float BACK_GROUND_COLOR[3];
extern const XMFLOAT3 GLAY_COLORS[3];
extern const XMFLOAT3 RED_COLOR;

//プレイヤー（矢印）関連
extern const float PLAYER_ARROW_RADIUS_OFFSET;
extern const int PLAYER_ARROW_JUMP_RADIUS_OFFSET;	//連続押し込み時の矢印がジャンプする高さのオフセット
extern const float PLAYER_ARROW_SCALE_OFFSET;
extern const float PLAYER_ARROW_ANGLE_OFFSET;

//操作関連
extern const int PUSH_INPUT_WAIT_FRAME;	//連続押し込み入力の許容フレーム
extern const int ARROW_PUSH_LAYER_FRAME;	//入力後、押し込みにかかる時間

//カメラ関連
extern const XMFLOAT3 DEFAULT_CAM_POS;	//デフォルトの視点
extern const XMFLOAT3 DEFAULT_CAM_TARGET;	//デフォルトの注視点
extern const float CAMERA_ZOOM_OUT_OFFSET;	//カメラが層の数によってズームアウトするときのZオフセット値

extern const float SHAKE_POWER_MAX;	//カメラシェイクの最大強さ
extern const float SHAKE_POWER_MIN;	//カメラシェイクの最小強さ
extern const int SHAKE_SPAN;	//シェイクを行うフレーム数

extern const int ZOOM_CAM_MOTION_TOTAL_TIME;	//敵死亡時のカメラモーションにかかる合計時間
extern const float ZOOM_CAM_MOTION_Z;	//敵死亡時のカメラモーションのZ値

extern const int CAM_RETURN_WAIT_TIME_AFTER_ENEMY_KILL;	//敵死亡モーション再生終了後、何フレームでカメラが定位置に戻るか
extern const int RETURN_DEFAULT_CAM_MOTION_TOTAL_TIME;	//定位置に戻るモーションにかかる合計時間

//パネル関連
extern const int PANEL_NUM_MIN;
extern const int PANEL_NUM_MAX;
extern const int LAYER_NUM_MIN;
extern const int LAYER_NUM_MAX;
extern const float INNER_SPACE_RADIUS;	//中央の柱（？）半径
extern const int LAYER_NUM_MAX_LIMIT;	//レイヤー数最大制限（この数より大きくならない）
extern const int OUTER_CIRCLE_VERTEX_NUM;	//外円の頂点数（円の滑らかさ）
extern const float PANEL_HALF_HEIGHT;
extern const float LAYER_OFFSET_RADIUS;	//レイヤー半径（幅）

extern const int PUSH_LAYER_OFFSET_TOTAL_TIME;	//複数押し込みされたとき、１階層押し込んでから次の階層を押し込むまでのオフセット時間

//押し込み関連
extern const int PUSH_TOTAL_TIME;	//押し込みにかかる時間
extern const int ALL_PUSHED_UNTIL_RESET_TOTAL_TIME;	//全押し込みされてから押し込みリセットされるまでの時間

//敵関連
extern const int ENEMY_MAX;	//敵の最大数（これ以上のインスタンスは生成しない）
//extern const float ENEMY_OFFSET_LENGTH = 100.0f;
extern const float ENEMY_OFFSET_LENGTH;
extern const int ENEMY_KILL_SPAN;	//敵死亡演出で、一体ずつ倒す間隔
extern const int ENEMY_ALL_KILL_SPAN;		//階層の敵全員倒した後の押し込みの間隔
extern const int ENEMY_BODY_EMIT_SPAN;	//敵の身体パーティクルのエミット間隔

//姫敵
extern const int QUEEN_MAX;

//敵ステータス基準値
extern const float ENEMY_BASE_SPEED_STANDARD;
extern const int ENEMY_LIFE_SPAN_STANDARD;
extern const int ENEMY_KILL_SCORE_STANDARD;

//敵種別ごとのステータス比率
const enum ENEMY_TYPE { PORN, QUEEN, KING, JACK, ENEMY_TYPE_NUM };
extern const float ENEMY_BASE_SPEED[ENEMY_TYPE_NUM];
extern const float ENEMY_LIFE_SPAN[ENEMY_TYPE_NUM];	//爆発までの時間（寿命）
extern const float ENEMY_KILL_SCORE[ENEMY_TYPE_NUM];
extern const XMFLOAT3 ENEMY_COLOR[ENEMY_TYPE_NUM];

//敵身体パーティクルエミッター
extern const int ENEMY_BODY_EMIT_NUM_MIN;	//敵の身体パーティクルの最小エミット数
extern const int ENEMY_BODY_EMIT_NUM_MAX;	//敵の身体パーティクルの最大エミット数
extern const float ENEMY_BODY_EMIT_RANGE_RADIUS;	//敵の身体パーティクルエミットの範囲
extern const float ENEMY_BODY_EMIT_RADIUS_MIN;	//敵の身体パーティクルエミット時の最小サイズ
extern const float ENEMY_BODY_EMIT_RADIUS_MAX;	//敵の身体パーティクルエミット時の最大サイズ
//extern const float ENEMY_BODY_RADIUS_EXPAND_SPEED;	//敵の身体パーティクルの拡大スピード
extern const float ENEMY_BODY_SPIN_SPEED; //敵の身体パーティクルの回転スピード
extern const int ENEMY_BODY_LIFE_SPAN;	//敵の身体パーティクルの寿命
extern const float ENEMY_BODY_MOVE_SPEED;	//敵の身体パーティクルの飛び散りスピード

//エフェクト関連
extern const int ENEMY_DEAD_PRIMITIVE_PARTICLE_SPEED_RAND_MIN;
extern const int ENEMY_DEAD_PRIMITIVE_PARTICLE_SPEED_RAND_MAX;
extern const int ENEMY_DEAD_PRIMITIVE_PARTICLE_SPIN_RAND_MIN;
extern const int ENEMY_DEAD_PRIMITIVE_PARTICLE_SPIN_RAND_MAX;
extern const float ENEMY_DEAD_PRIMITIVE_PARTICLE_RADIUS_RAND_MIN;
extern const float ENEMY_DEAD_PRIMITIVE_PARTICLE_RADIUS_RAND_MAX;
extern const float ENEMY_DEAD_PRIMITIVE_PARTICLE_SPEED_ATTENUATION_RATE;
extern const int ENEMY_DEAD_PRIMITIVE_PARTICLE_LIFE_SPAN;

//スコア関連
extern const int SCORE_MAX_DIGIT;	//スコアの最大桁数
extern const int KILL_GET_SCORE;	//１キルにつき得られるスコア
extern const float COMBO_ADD_RATE;	//コンボによる倍率増加量

//スコアアタック関連
extern const int ADD_ENEMY_NUM_BY_ADD_LAYER_MIN;
extern const int ADD_ENEMY_NUM_BY_ADD_LAYER_MAX;
extern const int ADD_QUEEN_NUM_BY_ADD_LAYER;