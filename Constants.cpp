#include"Constants.h"

//�f�o�b�O�p
//const bool ENEMY_CAN_MOVE = false;

const float PI = 3.14159265359f;

//��ʃ��[�h
const bool FULL_SCREEN = true;
const int WINDOW_WIDTH = 1280;	//�E�B���h�E���[�h���̃T�C�Y
const int WINDOW_HEIGHT = 720;

//�F�n
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

//�v���C���[�֘A
const float PLAYER_ARROW_RADIUS_OFFSET = 33.0f;
const int PLAYER_ARROW_JUMP_RADIUS_OFFSET = 4.0f;		//�A���������ݎ��̖�󂪃W�����v���鍂���̃I�t�Z�b�g
const float PLAYER_ARROW_SCALE_OFFSET = 0.05f;
const float PLAYER_ARROW_ANGLE_OFFSET = 360;	//�x

//����֘A
const int PUSH_INPUT_WAIT_FRAME = 20;	//�A���������ݓ��͂̋��e�t���[��
const int ARROW_PUSH_LAYER_FRAME = 10;	//���͌�A�������݂ɂ����鎞��

//�J�����֘A
const XMFLOAT3 DEFAULT_CAM_POS = { 0,0,-35 };
const XMFLOAT3 DEFAULT_CAM_TARGET = { 0,0,0 };
const float CAMERA_ZOOM_OUT_OFFSET = 16.0f;	//�J�������w�̐��ɂ���ăY�[���A�E�g����Ƃ���Z�I�t�Z�b�g�l

const float SHAKE_POWER_MAX = 0.8f;	//�J�����V�F�C�N�̍ő勭��
const float SHAKE_POWER_MIN = 0.12f;	//�J�����V�F�C�N�̍ŏ�����
const int SHAKE_SPAN = 20;	//�V�F�C�N���s���t���[����

const int ZOOM_CAM_MOTION_TOTAL_TIME = 100;	//�G���S���̃J�������[�V�����ɂ����鍇�v����
const float ZOOM_CAM_MOTION_Z = -7.0f;	//�G���S���̃J�������[�V������Z�l

const int CAM_RETURN_WAIT_TIME_AFTER_ENEMY_KILL = 12;	//�G���S���[�V�����Đ��I����A���t���[���ŃJ��������ʒu�ɖ߂邩
const int RETURN_DEFAULT_CAM_MOTION_TOTAL_TIME = 60;	//��ʒu�ɖ߂郂�[�V�����ɂ����鍇�v����

//�p�l���֘A
const int PANEL_NUM_MIN = 4;
const int PANEL_NUM_MAX = 8;
const int LAYER_NUM_MIN = 2;
const int LAYER_NUM_MAX = 4;
const float INNER_SPACE_RADIUS = 7.2f;	//�����̒��i�H�j���a
const int LAYER_NUM_MAX_LIMIT = 6;	//���C���[���ő吧���i���̐����傫���Ȃ�Ȃ��j
//const int OUTER_CIRCLE_VERTEX_NUM = 8;	//�O�~�̒��_���i�~�̊��炩���j
const int OUTER_CIRCLE_VERTEX_NUM = 4;	//�O�~�̒��_���i�~�̊��炩���j
const float PANEL_HALF_HEIGHT = 2.0f;
const float LAYER_OFFSET_RADIUS = 8.0f;	//���C���[���a�i���j

const int PUSH_LAYER_OFFSET_TOTAL_TIME = 6;	//�����������݂��ꂽ�Ƃ��A�P�K�w��������ł��玟�̊K�w���������ނ܂ł̃I�t�Z�b�g����

//�������݊֘A
//const int PUSH_TOTAL_TIME = 30;	//�������݂ɂ����鎞��
const int PUSH_TOTAL_TIME = 5;	//�������݂ɂ����鎞��
const int ALL_PUSHED_UNTIL_RESET_TOTAL_TIME = 15;	//�S�������݂���Ă��牟�����݃��Z�b�g�����܂ł̎���

//�G�֘A
const int ENEMY_MAX = 256;	//�G�̍ő吔�i����ȏ�̃C���X�^���X�͐������Ȃ��j
//const float ENEMY_OFFSET_LENGTH = 100.0f;
const float ENEMY_OFFSET_LENGTH = LAYER_OFFSET_RADIUS;
const int ENEMY_KILL_SPAN = 13;	//�G���S���o�ŁA��̂��|���Ԋu
const int ENEMY_ALL_KILL_SPAN = 12;		//�K�w�̓G�S���|������̉������݂̊Ԋu
const int ENEMY_BODY_EMIT_SPAN = 20;	//�G�̐g�̃p�[�e�B�N���̃G�~�b�g�Ԋu

//�P�G
const int QUEEN_MAX = 256;

//�G�X�e�[�^�X��l
const float ENEMY_BASE_SPEED_STANDARD = PI / 180.0f;
const int ENEMY_LIFE_SPAN_STANDARD = 1200;
const int ENEMY_KILL_SCORE_STANDARD = 100;

//�G��ʂ��Ƃ̃X�e�[�^�X�䗦
const float ENEMY_BASE_SPEED[ENEMY_TYPE_NUM] = { 1,0,2,1 };
const float ENEMY_LIFE_SPAN[ENEMY_TYPE_NUM] = { 1,2,1,0.5f };	//�����܂ł̎��ԁi�����j
const float ENEMY_KILL_SCORE[ENEMY_TYPE_NUM] = { 1,0.75f,0.5f,1.5f };
const XMFLOAT3 ENEMY_COLOR[ENEMY_TYPE_NUM] = {
	{1.0f,0.396f,0.207f},
	{1.0f,0.011f,0.0f},
	{0.180f,0.329f,0.972f},
	{0.780f,0.117f,0.972f}
};

//�G�g�̃p�[�e�B�N���G�~�b�^�[
const int ENEMY_BODY_EMIT_NUM_MIN = 1;	//�G�̐g�̃p�[�e�B�N���̍ŏ��G�~�b�g��
const int ENEMY_BODY_EMIT_NUM_MAX = 2;	//�G�̐g�̃p�[�e�B�N���̍ő�G�~�b�g��
const float ENEMY_BODY_EMIT_RANGE_RADIUS = 0.3f;	//�G�̐g�̃p�[�e�B�N���G�~�b�g�͈̔�
const float ENEMY_BODY_EMIT_RADIUS_MIN = 0.3f;	//�G�̐g�̃p�[�e�B�N���G�~�b�g���̍ŏ��T�C�Y
const float ENEMY_BODY_EMIT_RADIUS_MAX = 0.6f;	//�G�̐g�̃p�[�e�B�N���G�~�b�g���̍ő�T�C�Y
//const float ENEMY_BODY_RADIUS_EXPAND_SPEED = 0.1f;	//�G�̐g�̃p�[�e�B�N���̊g��X�s�[�h
const float ENEMY_BODY_SPIN_SPEED = 2; //�G�̐g�̃p�[�e�B�N���̉�]�X�s�[�h
const int ENEMY_BODY_LIFE_SPAN = 40;	//�G�̐g�̃p�[�e�B�N���̎���
const float ENEMY_BODY_MOVE_SPEED = 0.1f;	//�G�̐g�̃p�[�e�B�N���̔�юU��X�s�[�h

//�G�t�F�N�g�֘A
const int ENEMY_DEAD_PRIMITIVE_PARTICLE_SPEED_RAND_MIN = 3;
const int ENEMY_DEAD_PRIMITIVE_PARTICLE_SPEED_RAND_MAX = 6;
const int ENEMY_DEAD_PRIMITIVE_PARTICLE_SPIN_RAND_MIN = 90;
const int ENEMY_DEAD_PRIMITIVE_PARTICLE_SPIN_RAND_MAX = 220;
const float ENEMY_DEAD_PRIMITIVE_PARTICLE_RADIUS_RAND_MIN = 0.5f;
const float ENEMY_DEAD_PRIMITIVE_PARTICLE_RADIUS_RAND_MAX = 1.5f;
const float ENEMY_DEAD_PRIMITIVE_PARTICLE_SPEED_ATTENUATION_RATE = 0.95f;
const int ENEMY_DEAD_PRIMITIVE_PARTICLE_LIFE_SPAN = 90;

//�X�R�A�֘A
const int SCORE_MAX_DIGIT = 10;	//�X�R�A�̍ő包��
const int KILL_GET_SCORE = 100;	//�P�L���ɂ�������X�R�A
//const float COMBO_ADD_RATE = 0.1f;	//�R���{�ɂ��{��������
const float COMBO_ADD_RATE = 0.1f;	//�R���{�ɂ��{��������

//�X�R�A�A�^�b�N�֘A
const int ADD_ENEMY_NUM_BY_ADD_LAYER_MIN = 2;
const int ADD_ENEMY_NUM_BY_ADD_LAYER_MAX = 8;
const int ADD_QUEEN_NUM_BY_ADD_LAYER = 3;