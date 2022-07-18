#pragma once
#include"Singleton.h"

#include<string>
#include<map>

#include"Pipeline.h"
#include<wrl.h>

using namespace std;

class PipelineMgr :public Singleton<PipelineMgr>
{
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	PipelineMgr() {};
	friend Singleton<PipelineMgr>;

	map<string, Pipeline>pipelines;

	string nowPipeline = "NULL";

public:
	~PipelineMgr();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="Key">�p�C�v���C���̖��O</param>
	/// <param name="VSName">���_�V�F�[�_�̖��O</param>
	/// <param name="PSName">�s�N�Z���V�F�[�_�̖��O</param>
	/// <param name="InputLayout">�C���v�b�g���C�A�E�g</param>
	/// <param name="NumInputLayout">�C���v�b�g���C�A�E�g�̗v�f��</param>
	/// <param name="RootParams">���[�g�p�����[�^</param>
	/// <param name="NumRootParams">���[�g�p�����[�^�̗v�f��</param>
	/// <param name="Topology">�`�掞�̃g�|���W�[�w��</param>
	/// <param name="CallMode">�w�ʃJ�����O���邩</param>
	/// <param name="DepthWriteMask">�f�v�X�̏������݂����邩</param>
	/// <param name="StaticSampler">�T���v���[</param>
	/// <param name="NumStaticSampler">�T���v���[�̗v�f��</param>
	void AddPipeLine(const string Key,
		const wchar_t* VSName, const wchar_t* PSName,
		D3D12_INPUT_ELEMENT_DESC* InputLayout, UINT NumInputLayout,
		D3D12_ROOT_PARAMETER* RootParams, UINT NumRootParams,
		D3D_PRIMITIVE_TOPOLOGY Topology,
		bool CallMode = true, bool DepthWriteMask = true,
		D3D12_STATIC_SAMPLER_DESC* StaticSampler = nullptr, UINT NumStaticSampler = 0,
		bool DepthTest = true);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="Key">�p�C�v���C���̖��O</param>
	/// <param name="VSName">���_�V�F�[�_�̖��O</param>
	/// <param name="PSName">�s�N�Z���V�F�[�_�̖��O</param>
	/// <param name="GSName">�W�I���g���V�F�[�_�̖��O</param>
	/// <param name="InputLayout">�C���v�b�g���C�A�E�g</param>
	/// <param name="NumInputLayout">�C���v�b�g���C�A�E�g�̗v�f��</param>
	/// <param name="RootParams">���[�g�p�����[�^</param>
	/// <param name="NumRootParams">���[�g�p�����[�^�̗v�f��</param>
	/// <param name="Topology">�`�掞�̃g�|���W�[�w��</param>
	/// <param name="CallMode">�w�ʃJ�����O���邩</param>
	/// <param name="DepthWriteMask">�f�v�X�̏������݂����邩</param>
	/// <param name="StaticSampler">�T���v���[</param>
	/// <param name="NumStaticSampler">�T���v���[�̗v�f��</param>
	void AddPipeLine(string Key,
		const wchar_t* VSName, const wchar_t* PSName, const wchar_t* GSName,
		D3D12_INPUT_ELEMENT_DESC* InputLayout, UINT NumInputLayout,
		D3D12_ROOT_PARAMETER* RootParams, UINT NumRootParams,
		D3D_PRIMITIVE_TOPOLOGY Topology,
		bool CallMode = true, bool DepthWriteMask = true,
		D3D12_STATIC_SAMPLER_DESC* StaticSampler = nullptr, UINT NumStaticSampler = 0,
		bool DepthTest = true);
	void AddPipeLine(string Key, ID3D12RootSignature* RootSignature,
		ID3D12PipelineState* PipeLine, D3D_PRIMITIVE_TOPOLOGY Topology);

	void SetPipePine(string Key);

	void Reset() { nowPipeline = "NULL"; }
	bool Exsit(string Key) { return !pipelines.empty() && pipelines.find(Key) != pipelines.end(); }
};