#pragma once
#include "Macro.h"
template<typename Type>
class SingleTon
{
private:
	static Type * pInstance;
protected:
	SingleTon()
	{

	}

	virtual ~SingleTon()
	{

	}

public:
	static Type* GetInstance()
	{
		if (pInstance == NULL)
			pInstance = new Type;
		return pInstance;
	};

	static void DestroyInstance()
	{
		if (pInstance)
		{
			SAFE_DELETE(pInstance);
		}
	};
};

template <typename Type> Type * SingleTon<Type>::pInstance = NULL;