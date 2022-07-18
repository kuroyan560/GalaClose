#pragma once
#include<DirectXMath.h>
using namespace DirectX;

//�f�o�b�O�p
//extern const bool ENEMY_CAN_MOVE;

extern const float PI;

//��ʃ��[�h
extern const bool FULL_SCREEN;
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

//�F�n
extern const float BACK_GROUND_COLOR[3];
extern const XMFLOAT3 GLAY_COLORS[3];
extern const XMFLOAT3 RED_COLOR;

//�v���C���[�i���j�֘A
extern const float PLAYER_ARROW_RADIUS_OFFSET;
extern const int PLAYER_ARROW_JUMP_RADIUS_OFFSET;	//�A���������ݎ��̖�󂪃W�����v���鍂���̃I�t�Z�b�g
extern const float PLAYER_ARROW_SCALE_OFFSET;
extern const float PLAYER_ARROW_ANGLE_OFFSET;

//����֘A
extern const int PUSH_INPUT_WAIT_FRAME;	//�A���������ݓ��͂̋��e�t���[��
extern const int ARROW_PUSH_LAYER_FRAME;	//���͌�A�������݂ɂ����鎞��

//�J�����֘A
extern const XMFLOAT3 DEFAULT_CAM_POS;	//�f�t�H���g�̎��_
extern const XMFLOAT3 DEFAULT_CAM_TARGET;	//�f�t�H���g�̒����_
extern const float CAMERA_ZOOM_OUT_OFFSET;	//�J�������w�̐��ɂ���ăY�[���A�E�g����Ƃ���Z�I�t�Z�b�g�l

extern const float SHAKE_POWER_MAX;	//�J�����V�F�C�N�̍ő勭��
extern const float SHAKE_POWER_MIN;	//�J�����V�F�C�N�̍ŏ�����
extern const int SHAKE_SPAN;	//�V�F�C�N���s���t���[����

extern const int ZOOM_CAM_MOTION_TOTAL_TIME;	//�G���S���̃J�������[�V�����ɂ����鍇�v����
extern const float ZOOM_CAM_MOTION_Z;	//�G���S���̃J�������[�V������Z�l

extern const int CAM_RETURN_WAIT_TIME_AFTER_ENEMY_KILL;	//�G���S���[�V�����Đ��I����A���t���[���ŃJ��������ʒu�ɖ߂邩
extern const int RETURN_DEFAULT_CAM_MOTION_TOTAL_TIME;	//��ʒu�ɖ߂郂�[�V�����ɂ����鍇�v����

//�p�l���֘A
extern const int PANEL_NUM_MIN;
extern const int PANEL_NUM_MAX;
extern const int LAYER_NUM_MIN;
extern const int LAYER_NUM_MAX;
extern const float INNER_SPACE_RADIUS;	//�����̒��i�H�j���a
extern const int LAYER_NUM_MAX_LIMIT;	//���C���[���ő吧���i���̐����傫���Ȃ�Ȃ��j
extern const int OUTER_CIRCLE_VERTEX_NUM;	//�O�~�̒��_���i�~�̊��炩���j
extern const float PANEL_HALF_HEIGHT;
extern const float LAYER_OFFSET_RADIUS;	//���C���[���a�i���j

extern const int PUSH_LAYER_OFFSET_TOTAL_TIME;	//�����������݂��ꂽ�Ƃ��A�P�K�w��������ł��玟�̊K�w���������ނ܂ł̃I�t�Z�b�g����

//�������݊֘A
extern const int PUSH_TOTAL_TIME;	//�������݂ɂ����鎞��
extern const int ALL_PUSHED_UNTIL_RESET_TOTAL_TIME;	//�S�������݂���Ă��牟�����݃��Z�b�g�����܂ł̎���

//�G�֘A
extern const int ENEMY_MAX;	//�G�̍ő吔�i����ȏ�̃C���X�^���X�͐������Ȃ��j
//extern const float ENEMY_OFFSET_LENGTH = 100.0f;
extern const float ENEMY_OFFSET_LENGTH;
extern const int ENEMY_KILL_SPAN;	//�G���S���o�ŁA��̂��|���Ԋu
extern const int ENEMY_ALL_KILL_SPAN;		//�K�w�̓G�S���|������̉������݂̊Ԋu
extern const int ENEMY_BODY_EMIT_SPAN;	//�G�̐g�̃p�[�e�B�N���̃G�~�b�g�Ԋu

//�P�G
extern const int QUEEN_MAX;

//�G�X�e�[�^�X��l
extern const float ENEMY_BASE_SPEED_STANDARD;
extern const int ENEMY_LIFE_SPAN_STANDARD;
extern const int ENEMY_KILL_SCORE_STANDARD;

//�G��ʂ��Ƃ̃X�e�[�^�X�䗦
const enum ENEMY_TYPE { PORN, QUEEN, KING, JACK, ENEMY_TYPE_NUM };
extern const float ENEMY_BASE_SPEED[ENEMY_TYPE_NUM];
extern const float ENEMY_LIFE_SPAN[ENEMY_TYPE_NUM];	//�����܂ł̎��ԁi�����j
extern const float ENEMY_KILL_SCORE[ENEMY_TYPE_NUM];
extern const XMFLOAT3 ENEMY_COLOR[ENEMY_TYPE_NUM];

//�G�g�̃p�[�e�B�N���G�~�b�^�[
extern const int ENEMY_BODY_EMIT_NUM_MIN;	//�G�̐g�̃p�[�e�B�N���̍ŏ��G�~�b�g��
extern const int ENEMY_BODY_EMIT_NUM_MAX;	//�G�̐g�̃p�[�e�B�N���̍ő�G�~�b�g��
extern const float ENEMY_BODY_EMIT_RANGE_RADIUS;	//�G�̐g�̃p�[�e�B�N���G�~�b�g�͈̔�
extern const float ENEMY_BODY_EMIT_RADIUS_MIN;	//�G�̐g�̃p�[�e�B�N���G�~�b�g���̍ŏ��T�C�Y
extern const float ENEMY_BODY_EMIT_RADIUS_MAX;	//�G�̐g�̃p�[�e�B�N���G�~�b�g���̍ő�T�C�Y
//extern const float ENEMY_BODY_RADIUS_EXPAND_SPEED;	//�G�̐g�̃p�[�e�B�N���̊g��X�s�[�h
extern const float ENEMY_BODY_SPIN_SPEED; //�G�̐g�̃p�[�e�B�N���̉�]�X�s�[�h
extern const int ENEMY_BODY_LIFE_SPAN;	//�G�̐g�̃p�[�e�B�N���̎���
extern const float ENEMY_BODY_MOVE_SPEED;	//�G�̐g�̃p�[�e�B�N���̔�юU��X�s�[�h

//�G�t�F�N�g�֘A
extern const int ENEMY_DEAD_PRIMITIVE_PARTICLE_SPEED_RAND_MIN;
extern const int ENEMY_DEAD_PRIMITIVE_PARTICLE_SPEED_RAND_MAX;
extern const int ENEMY_DEAD_PRIMITIVE_PARTICLE_SPIN_RAND_MIN;
extern const int ENEMY_DEAD_PRIMITIVE_PARTICLE_SPIN_RAND_MAX;
extern const float ENEMY_DEAD_PRIMITIVE_PARTICLE_RADIUS_RAND_MIN;
extern const float ENEMY_DEAD_PRIMITIVE_PARTICLE_RADIUS_RAND_MAX;
extern const float ENEMY_DEAD_PRIMITIVE_PARTICLE_SPEED_ATTENUATION_RATE;
extern const int ENEMY_DEAD_PRIMITIVE_PARTICLE_LIFE_SPAN;

//�X�R�A�֘A
extern const int SCORE_MAX_DIGIT;	//�X�R�A�̍ő包��
extern const int KILL_GET_SCORE;	//�P�L���ɂ�������X�R�A
extern const float COMBO_ADD_RATE;	//�R���{�ɂ��{��������

//�X�R�A�A�^�b�N�֘A
extern const int ADD_ENEMY_NUM_BY_ADD_LAYER_MIN;
extern const int ADD_ENEMY_NUM_BY_ADD_LAYER_MAX;
extern const int ADD_QUEEN_NUM_BY_ADD_LAYER;