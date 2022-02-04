#pragma once
#include <array>
#include<DirectXMath.h>
#include"Vector.h"

struct Matrix4
{
public:

	std::array<std::array<float,4>,4> m = 
	{
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1,
	};
	
	Matrix4();
	Matrix4(const float val);
	Matrix4(const DirectX::XMMATRIX& mat);
	Matrix4(const float m00, const float m01, const float m02, const float m03,
		const float m10, const float m11, const float m12, const float m13,
		const float m20, const float m21, const float m22, const float m23,
		const float m30, const float m31, const float m32, const float m33);

	DirectX::XMMATRIX convertToXMMATRIX();
	
	//Matrix4(const float val[4][4]);
	//�P���s������߂�
	static Matrix4 identity();

	//static Matrix4 convertToMatrix4(const DirectX::XMMATRIX& mat);

	static DirectX::XMMATRIX convertToXMMATRIX(const Matrix4& mat);

	//�g��k���̐ݒ�
	static Matrix4 scale(const Vector3& s);

	//��]�s��̐ݒ�
	static Matrix4 rotateX(float angle);
	static Matrix4 rotateY(float angle);
	static Matrix4 rotateZ(float angle);

	//���s�ړ��s��̍쐬
	static Matrix4 translate(const Vector3& t);

	//���W�ϊ�(�x�N�g���ƍs��̊|���Z������)
	static Vector3 transform(const Vector3& v, const Matrix4& m);
};

//2�����Z�q�I�[�o�[���[�h
const Matrix4 operator * (const Matrix4& m1, const Matrix4& m2);
const Vector3 operator * (const Vector3& v, const Matrix4& m);

//������Z�q�I�[�o�[���[�h
const Matrix4& operator*= (const Matrix4& m1, const Matrix4& m2);


const bool operator== (const Matrix4& m1, const Matrix4& m2);
