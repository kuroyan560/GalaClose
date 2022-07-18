#include "DirtyFlag.h"

#pragma region DirtySet
template <class T>
void DirtyFlag::DirtySet<T>::Record()
{
	for (auto itr = inspectors.begin(); itr != inspectors.end(); itr++)
	{
		itr->Record();
	}
}

template <class T>
bool DirtyFlag::DirtySet<T>::Dirty()
{
	for (int i = 0; i < inspectors.size(); ++i)
	{
		if (inspectors[i].Dirty())
		{
			return true;
		}
	}
	//for (auto itr = inspectors.begin(); itr != inspectors.end(); itr++)
	//{
	//	if (itr->Dirty())
	//	{
	//		return true;
	//	}
	//}

	return false;
}

template <class T>
int DirtyFlag::DirtySet<T>::AddInspector(T* Ptr)
{
	if (Ptr == nullptr)return -1;
	inspectors.push_back(Inspector(Ptr));
	return 0;
}

template<class T>
void DirtyFlag::DirtySet<T>::InspectorsRecord()
{
	if (!inspectors.empty())
	{
		for (auto itr = inspectors.begin(); itr != inspectors.end(); itr++)
		{
			itr->Record();
		}
	}
}

#pragma endregion

std::list<DirtyFlag*>DirtyFlag::dirtyFlags;

void DirtyFlag::AllRecord()
{
	if (!dirtyFlags.empty())
	{
		for (auto& dirty : dirtyFlags)
		{
			dirty->Record();
		}
	}
}

void DirtyFlag::Record()
{
	dirty_bool.InspectorsRecord();
	dirty_float.InspectorsRecord();
	dirty_int.InspectorsRecord();
}

int DirtyFlag::AddInspector(float* Ptr)
{
	return dirty_float.AddInspector(Ptr);
}

int DirtyFlag::AddInspector(bool* Ptr)
{
	return dirty_bool.AddInspector(Ptr);
}

int DirtyFlag::AddInspector(int* Ptr)
{
	return dirty_int.AddInspector(Ptr);
}

bool DirtyFlag::Dirty()
{
	return (dirty_bool.Dirty() || dirty_float.Dirty() || dirty_int.Dirty());
}
