#pragma once
#include "Action.h"
/*/
namespace NS_ROOT
{
	namespace NS_OBJECTS
	{
		class ActionMoveBezier : public Action
		{
		public:
			ActionMoveBezier();
			~ActionMoveBezier();

			void Start();
			void Update();

			void SetActionTime(float time) { _actionTime = time; }

			void AddPoint(D3DXVECTOR3 p) { _points.push_back(p); }
			void ClearPoint() { _points.clear(); }
		private:
			std::vector<D3DXVECTOR3> _points;

			D3DXVECTOR3 _prevPosition;

			float _startTime;
			float _currentTime;
			float _actionTime;
		};
	}
}*/