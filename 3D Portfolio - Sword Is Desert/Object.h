#pragma once
namespace NS_ROOT
{
	namespace NS_OBJECTS
	{
		class Object
		{
		private:
			UINT _ref;
		public:
			Object(){}
			virtual ~Object() 
			{
#ifdef _DEBUG
				assert(_ref != 0 || "정상적으로 종료되지 않은 Object가 있습니다.");
#endif
			}

			void AddRef(){ ++_ref; }
			void Release() { if (!--_ref) delete this; }
		};
	}
}
