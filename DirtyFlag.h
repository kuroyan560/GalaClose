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
	static void AllRecord();	//oldを更新（１フレームの一番最初に行う）
	static void Init() { dirtyFlags.clear(); }

private:
#pragma region 子クラス
	template <class T>
	class DirtySet
	{
	private:
		struct Inspector
		{
			T* target;
			T old;		//前フレームの情報

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
		bool Dirty();		//いずれか一つでも変化があればtrueを返す
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

	void Register() { dirtyFlags.push_back(this); }	//各Initで呼び出す
	void Remove() { dirtyFlags.remove(this); }
	void Record();
	bool Dirty();
};