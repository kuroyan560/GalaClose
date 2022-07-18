#pragma once
#include<list>
#include<vector>

using namespace std;

#include<DirectXMath.h>
using namespace DirectX;

class DirtyFlag
{
private:
	static list<DirtyFlag*>dirtyFlags;
public:
	static void AllRecord();	//old���X�V�i�P�t���[���̈�ԍŏ��ɍs���j
	static void Init() { dirtyFlags.clear(); }

private:
#pragma region �q�N���X
	template <class T>
	class DirtySet
	{
	private:
		struct Inspector
		{
			T* target;
			T old;		//�O�t���[���̏��

			Inspector(T* Target)
				:target(Target), old(*Target) {};
			void Record() { 
				old = *target;
			}
			bool Dirty() { 
				return (old != *target); 
			}
		};

		vector<Inspector>inspectors;
		void Record();

	public:
		DirtySet() {};
		~DirtySet() {};

		int AddInspector(T* Ptr);
		bool Dirty();		//�����ꂩ��ł��ω��������true��Ԃ�
		void InspectorsRecord();
	};
#pragma endregion

	DirtySet<float> dirty_float;
	DirtySet<bool> dirty_bool;
	DirtySet<int>dirty_int;

public:
	DirtyFlag() {};
	int AddInspector(float* Ptr);
	int AddInspector(bool* Ptr);
	int AddInspector(int* Ptr);

	void Register() { dirtyFlags.push_back(this); }	//�eInit�ŌĂяo��
	void Remove() { dirtyFlags.remove(this); }
	void Record();
	bool Dirty();
};