#pragma once
#include<vector>
#include<string>
#include<list>
using namespace std;

#include"MyFunc.h"
using namespace MyDef;

#include"Model_HSM.h"

namespace PMX
{
	static const enum WEIGHT_TRANS_METHOD { BDEF1, BDEF2, BDEF4, SDEF };
	static const int PMX_BYTE_ARRAY_INDEX_SIZE_OFFSET = 2;

	struct PMXHeader	//PMX�w�b�_
	{
		unsigned char format[4]; //"PMX "
		float version; //ver (2.0/2.1)
		vector<unsigned char> byteArray;

		/*�o�C�g�� - byte
			[0] - �G���R�[�h���� | 0:UTF16 1 : UTF8
			[1] - �ǉ�UV�� | 0�`4 �ڍׂ͒��_�Q��

			[2] - ���_Index�T�C�Y | 1, 2, 4 �̂����ꂩ
			[3] - �e�N�X�`��Index�T�C�Y | 1, 2, 4 �̂����ꂩ
			[4] - �ގ�Index�T�C�Y | 1, 2, 4 �̂����ꂩ
			[5] - �{�[��Index�T�C�Y | 1, 2, 4 �̂����ꂩ
			[6] - ���[�tIndex�T�C�Y | 1, 2, 4 �̂����ꂩ
			[7] - ����Index�T�C�Y | 1, 2, 4 �̂����ꂩ*/
	};
	struct PMXInfo	//���f�����
	{
		string modelName_JP;
		string modelName_EN;
		string comment_JP;
		string comment_EN;
	};

	struct PMXVertex //���_�f�[�^
	{
		PMXVertex(size_t BoneIndexSize);
		~PMXVertex();
		PMXVertex(PMXVertex&& Tmp);

		float3 pos;
		float3 normal;
		float2 uv;
		vector<float4>additionalUv;	//�ǉ�UV�i�w�b�_�Ŏw�肳���@0 ~ 4�j�@���ł���Ζ����ق����ǂ�

		unsigned char weightTransMethod;		//�E�F�C�g�ό`���� 0:BDEF1 1:BDEF2 2:BDEF4 3:SDEF
		unsigned char* boneIndex[4] = { nullptr };	//�Q�Ƃ���{�[����Index
		float boneWeight[4] = { 0.0f };	//�E�F�C�g
		float3 SDEF_C;
		float3 SDEF_R0;
		float3 SDEF_R1;

		float edgeRate;	//�G�b�W�{��  �ގ��̃G�b�W�T�C�Y�ɑ΂��Ă̔{���l
	/*  BDEF1: int | 4 | �{�[���̂�
		BDEF2 : int, int, float | 4 * 3 | �{�[��2�ƁA�{�[��1�̃E�F�C�g�l(PMD����)
		BDEF4 : int* 4, float* 4 | 4 * 8 | �{�[��4�ƁA���ꂼ��̃E�F�C�g�l�B�E�F�C�g���v��1.0�ł���ۏ�͂��Ȃ�
		SDEF : int, int, float, float3 * 3
		| 4 * 12 | BDEF2�ɉ����ASDEF�p��float3(Vector3)��3�B���ۂ̌v�Z�ł͂���ɕ␳�l�̎Z�o���K�v(�ꉞ���̂܂�BDEF2�Ƃ��Ă��g�p�\)*/
	};

	struct PMXSurface
	{
		unsigned char* vertIndex[3];

		PMXSurface(size_t Size);
		~PMXSurface();
		PMXSurface(PMXSurface&& Tmp);
	};

	struct PMXMaterial
	{
		PMXMaterial(size_t TexIndexSize);
		~PMXMaterial();
		PMXMaterial(PMXMaterial&& Tmp);
		string materialName_JP;
		string materialName_EN;
		float4 diffuse;
		float3 specular;
		float specular_c;	//�X�y�L�����[�W��( c �� coefficient �̓�����)
		float3 ambient;
		unsigned char bitFlag;	//�`��t���O(8bit) - �ebit 0:OFF 1:ON
		/*	0x01:���ʕ`��, 0x02:�n�ʉe, 0x04:�Z���t�V���h�E�}�b�v�ւ̕`��, 0x08:�Z���t�V���h�E�̕`��, 0x10:�G�b�W�`�� */
		float4 edgeColor;
		float edgeSize;

		unsigned char* normalTexIndex;	//�ʏ�e�N�X�`��, �e�N�X�`���e�[�u���̎Q��Index
		unsigned char* sphereTexIndex;	//�X�t�B�A�e�N�X�`��, �e�N�X�`���e�[�u���̎Q��Index  ���e�N�X�`���g���q�̐����Ȃ�

		//�X�t�B�A���[�h 0:���� 1:��Z(sph) 2:���Z(spa) 3:�T�u�e�N�X�`��(�ǉ�UV1��x,y��UV�Q�Ƃ��Ēʏ�e�N�X�`���`����s��)
		unsigned char sphereMode;
		unsigned char shareToonFlag; //���LToon�t���O 0:�p���l�͌�Toon 1:�p���l�͋��LToon

		//���ǂ��炩�����
		//���L�t���O�O�̏ꍇ
		unsigned char* toonTexIndex;	//Toon�e�N�X�`��, �e�N�X�`���e�[�u���̎Q��Index
		//���L�t���O�P�̏ꍇ
		unsigned char* toonTex; //���LToon�e�N�X�`��[0�`9] -> ���ꂼ�� toon01.bmp�`toon10.bmp ��

		string memo; //���� : ���R���^�X�N���v�g�L�q�^�G�t�F�N�g�ւ̃p�����[�^�z�u�Ȃ�

		unsigned int forSurfaceNum; // �ގ��ɑΉ������(���_)�� (�K��3�̔{���ɂȂ�)
	};

	namespace Bone
	{
		struct PMXBoneInfo
		{
		protected:
			PMXBoneInfo() {};

		public:
			virtual ~PMXBoneInfo() {};
		};

		//�ڑ���(PMD�q�{�[���w��)�\�����@ -> 0:���W�I�t�Z�b�g�Ŏw�� 1:�{�[���Ŏw��
		struct Bone_0x0001 : public PMXBoneInfo
		{
		protected:
			Bone_0x0001() {};
		};
		struct PosOffset_0x0001:public Bone_0x0001 //���W�I�t�Z�b�g, �{�[���ʒu����̑��Ε� �F�O
		{
			float3 posOffset;
		};
		
		struct Index_0x0001 :public Bone_0x0001 //�ڑ���{�[���̃{�[��Index�@�F�P
		{
			unsigned char* boneIndex;
			Index_0x0001(size_t BoneIndexSize);
			~Index_0x0001();
			Index_0x0001(Index_0x0001&& Tmp);
		};

		//��]�t�^:1 �܂��� �ړ��t�^:1 �̏ꍇ
		struct Grant : public PMXBoneInfo
		{
			unsigned char* boneIndex; //�ڑ���{�[���̃{�[��Index
			float rate; //�t�^��
			Grant(size_t BoneIndexSize);
			~Grant();
			Grant(Grant&& Tmp);
		};

		//���Œ�:1 �̏ꍇ
		struct AxisVec : public PMXBoneInfo
		{
			float3 vec; //���Œ�:1 �̏ꍇ
		};

		//���[�J����:1 �̏ꍇ
		struct LocalAxisVec : public PMXBoneInfo
		{
			float3 xVec;
			float3 zVec;
		};

		//�O���e�ό`:1 �̏ꍇ
		struct Key : public PMXBoneInfo
		{
			unsigned int key;
		};

		//IK:1 �̏ꍇ IK�f�[�^���i�[
		struct IK : public PMXBoneInfo
		{
			IK(size_t BoneIndexSize);
			~IK();
			IK(IK&& Tmp);
			unsigned char* boneIndex; //IK�^�[�Q�b�g�{�[���̃{�[��Index
			int loopTimes; //IK���[�v�� (PMD�y��MMD���ł�255�񂪍ő�ɂȂ�悤�ł�)
			float limitAngle; // IK���[�v�v�Z����1�񂠂���̐����p�x -> ���W�A���p | PMD��IK�l�Ƃ�4�{�قȂ�̂Œ���
			int linkNum; //IK�����N�� : �㑱�̗v�f��

			struct IKLink
			{
				IKLink(size_t BoneIndexSize);
				~IKLink();
				IKLink(IKLink&& Tmp);
				unsigned char* boneIndex; //�����N�{�[���̃{�[��Index
				unsigned char limitAngleFlag; //�p�x���� 0:OFF 1:ON

				float3 lowerLimit = { 0,0,0 }; //���� (x,y,z) -> ���W�A���p
				float3 upperLimit = { 0,0,0 }; //��� (x,y,z) -> ���W�A���p
			};
			vector<IKLink>IKlinks;
		};
	}
	struct PMXBone
	{
		PMXBone(size_t BoneIndexSize);
		~PMXBone();
		PMXBone(PMXBone&& Tmp);
		string name_JP;
		string name_EN;

		float3 pos;
		unsigned char* boneIndex;	//�e�{�[���̃{�[���C���f�b�N�X
		int transLayer; //�ό`�K�w

		short boneFlag; //�{�[���t���O(16bit) �ebit 0:OFF 1:ON

		list<Bone::PMXBoneInfo*>info;
	};

	namespace Morph
	{
		struct PMXMorphInfo
		{
		protected:
			PMXMorphInfo() {};
		};

		struct Vertex : public PMXMorphInfo
		{
			Vertex(size_t VertexIndexSize);
			~Vertex();
			Vertex(Vertex&& Tmp);
			unsigned char* vertexIndex; //���_Index
			float3 offset; //���W�I�t�Z�b�g��(x,y,z)
		};

		struct UV : public PMXMorphInfo
		{
			UV(size_t VertexIndexSize);
			~UV();
			UV(UV&& Tmp);
			unsigned char* vertexIndex; //���_Index
			float4 offset; //UV�I�t�Z�b�g��(x,y,z,w) // ���ʏ�UV��z,w���s�v���ڂɂȂ邪���[�t�Ƃ��Ẵf�[�^�l�͋L�^���Ă���
		};

		struct Bone : public PMXMorphInfo
		{
			Bone(size_t BoneIndexSize);
			~Bone();
			Bone(Bone&& Tmp);
			unsigned char* boneIndex;	//�{�[��Index
			float3 move; //�ړ���
			float4 quaternion; //��]��
		};

		struct Material : PMXMorphInfo
		{
			Material(size_t MaterialIndexSize);
			~Material();
			Material(Material&& Tmp);
			unsigned char* materialIndex;
			unsigned char offsetFormat;	//�I�t�Z�b�g���Z�`���A�O�F��Z�A�P�F���Z
			float4 duffse;
			float3 specular;
			float specular_c;
			float3 ambient;
			float4 edgeColor;
			float edgeSize;
			float4 tex_c;
			float4 sphere_c;
			float4 toonTex_c;
		};

		struct Group : public PMXMorphInfo
		{
			Group(size_t MorphIndexSize);
			~Group();
			Group(Group&& Tmp);
			unsigned char* morphIndex;
			float rate;	//���[�t���F�O���[�v���[�t�̃��[�t�l * ���[�t�� = �Ώۃ��[�t�̃��[�t�l
		};
	}
	struct PMXMorph
	{
		static const enum TYPE
		{
			GROUP,VERTEX,BONE,UV,
			ADDITIONAL_UV_1, ADDITIONAL_UV_2, ADDITIONAL_UV_3, ADDITIONAL_UV_4,
			MATERIAL,
		};
		string name_JP;
		string name_EN;
		unsigned char controlPanel; // ����p�l�� (PMD:�J�e�S��) 1:��(����) 2:��(����) 3:��(�E��) 4:���̑�(�E��)  | 0:�V�X�e���\��
		unsigned char type; //���[�t��� - 0:�O���[�v, 1:���_, 2:�{�[��, 3:UV, 4:�ǉ�UV1, 5:�ǉ�UV2, 6:�ǉ�UV3, 7:�ǉ�UV4, 8:�ގ�
		unsigned int  offsetNum;
		list<Morph::PMXMorphInfo>data; //���قȂ��ނ̍����͕s��
	};
	//struct PMXDisplayBorder //�\���g
	//{
	//	string name_JP;
	//	string name_EN;
	//	unsigned char specialFlag; // ����g�t���O - 0:�ʏ�g 1:����g
	//	int num; //�g���v�f�� : �㑱�̗v�f��
	//	struct Display //�g���v�f
	//	{
	//		unsigned char target; // �v�f�Ώ� 0:�{�[�� 1:���[�t
	//		unsigned int index;
	//	};
	//	vector<Display>displays;
	//};

	struct PMXRigidBody
	{
		PMXRigidBody(size_t BoneIndexSize);
		~PMXRigidBody();
		PMXRigidBody(PMXRigidBody&& Tmp);
		string name_JP;
		string name_EN;

		unsigned char* boneIndex;

		unsigned char group;
		unsigned short notCollisionGroupFlag; //��Փ˃O���[�v�t���O

		unsigned char shape; //�`�� - 0:�� 1:�� 2:�J�v�Z��
		float3 size;

		float3 pos;
		float3 rotate;

		float mass;	//����
		float moveAttenuation;	//�ړ�����
		float rotateAttenuation;	//��]����
		float rebound; //������
		float friction;	//���C��

		unsigned char physicalOperation; //���̂̕������Z - 0:�{�[���Ǐ](static) 1:�������Z(dynamic) 2:�������Z + Bone�ʒu���킹
	};

	struct PMXJoint
	{
		string name_JP;
		string name_EN;

		unsigned char type; //Joint��� - 0:�X�v�����O6DOF   | PMX2.0�ł� 0 �̂�(�g���p)

		struct JointInfo
		{
			JointInfo(size_t RigidBodyIndexSize);
			~JointInfo();
			JointInfo(JointInfo&& Tmp);
			unsigned char* rigidBodyIndexA; //�֘A����A��Index - �֘A�Ȃ��̏ꍇ��-1
			unsigned char* rigidBodyIndexB; //�֘A����B��Index - �֘A�Ȃ��̏ꍇ��-1
			float3 pos;
			float3 spin;
			float3 moveLimit_Lower;
			float3 moveLimit_Upper;
			float3 spinLimit_Lower;
			float3 spinLimit_Upper;
			float3 springConstant_Move;	//�o�l�萔-�ړ�
			float3 springConstant_Spin;	//�o�l�萔-��]
		};
		JointInfo* info = nullptr;

		~PMXJoint();
	};

	struct PMXModel
	{
		PMX::PMXHeader head;
		PMX::PMXInfo info;

		int vertexNum;	//���_��
		vector<PMXVertex>vertices;

		int surfaceNum;
		vector<PMXSurface>surfaces;

		int texNum;
		vector<string>texPass;

		int materialNum;
		vector<PMXMaterial>materials;

		int boneNum;
		list<PMXBone>bones;

		int morphNum;
		list<PMX::PMXMorph>morphs;

		int rigidBodyNum;
		vector<PMXRigidBody>rigitBodies;

		int jointNum;
		list<PMXJoint>joints;
	};
}

static class PMXLoader
{
	static void ConvertPMX(string Dir, PMX::PMXModel* Pmx, Model_HSM* Hsm);
public:
	static void LoadPMX(string Dir, string ModelName, Model_HSM** Model);
};

