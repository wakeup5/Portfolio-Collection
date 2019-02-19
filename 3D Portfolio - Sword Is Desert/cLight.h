#pragma once

class Transform;

class cLight
{
public:
	Transform*			pTransform;			//����Ʈ Ʈ������
	D3DXCOLOR			Color;				//����Ʈ �÷�
	float				Intensity;			//����Ʈ ����


public:
	cLight(void);
	~cLight(void);


	// ����Ʈ ���
	// Pos.x,         Pos.y,          Pos.z,         lightType ( 0 Direction, 1 Point, 2 Spot )
	// Dir.x,         Dir.y,          Dir.z,         DistancePow;
	// Color.r,       Color.g,        Color.b,       strength
	// MaxRange,      MinRange,       InAngle        OutAngle 

	//����Ʈ ����� ��´�.
	virtual D3DXMATRIXA16 GetLightMatrix() = 0;

	//����Ʈ ����� �׸���.
	virtual void RenderGizmo() = 0;

};
