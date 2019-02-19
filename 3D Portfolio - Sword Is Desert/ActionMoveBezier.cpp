#include "stdafx.h"
#include "ActionMoveBezier.h"
/*/
namespace NS_ROOT
{
	namespace NS_OBJECTS
	{
		ActionMoveBezier::ActionMoveBezier()
		{
		}


		ActionMoveBezier::~ActionMoveBezier()
		{
		}

		void ActionMoveBezier::Start()
		{
			_startTime = TIME_MGR->GetLastTime();
			_currentTime = 0.0f;

			if (_object)
			{
				_object->SetWorldPosition(_points[0]);
			}
		}

		namespace Bezier
		{
			D3DXVECTOR3 GetBezierVector3(std::vector<D3DXVECTOR3> &p, float t)

			{
				if (p.size() == 1) return p[0];

				std::vector<D3DXVECTOR3> temp;

				for (int i = 0; i < p.size() - 1; i++)
				{
					//베지어 포인트들이 10개 이상이면 10개만 짤라 계산한다.
					if (i > 9) break;

					temp.push_back((1.0f - t) * p[i] + t * p[i + 1]);
				}

				return GetBezierVector3(temp, t);
			}

			int GetMaterial(int num)
			{
				if (num == 0)
					return 0;
				else if (num == 1)
					return 1;
				else
					return GetMaterial(num - 1) * num;
			}
		}

		void ActionMoveBezier::Update()
		{
			_currentTime = (TIME_MGR->GetLastTime() - _startTime);

			if (_points.empty())
			{
				if (_deligate)
					_deligate->OnActionFinish();
			}

			if (_currentTime > _actionTime)
			{
				_object->SetWorldPosition(_points[_points.size() - 1]);
				if (_deligate)
					_deligate->OnActionFinish();
			}
			else
			{
				float t = _currentTime / _actionTime;

				D3DXVECTOR3 p = { 0, 0, 0 }, temp;
				float f = 1, m, b;

				/*
				//Facto(row) / ((Facto(row - col)) * Facto(col))
				for (int k = 0, n = _points.size(); k < n; k++)
				{
					if ((GetTotalSum(k) * GetTotalSum(n - k)) == 0) 
						f = 1;
					else
						f = GetTotalSum(n) / (GetTotalSum(n - k) * GetTotalSum(k));

					m = pow(t, k);
					b = pow((1 - t), n - k);

					temp = f * m * b * _points[k];
					p += temp;
				}
				*

				p = Bezier::GetBezierVector3(_points, t);
				
				//p = (1.0f - t) * _points[0] + t * _points[1];

				_object->SetWorldPosition(p);
				D3DXVECTOR3 vDir = _prevPosition - p;
				D3DXVec3Normalize(&vDir, &vDir);
				_object->LookDirection(vDir);
				_prevPosition = p;
			}
		}
	}
}*/