 #pragma once

#include <iostream>
#include <cmath>
#include <array>



namespace math
{
	static const double PI = 3.14159265359L;

	template<typename Type>
	struct vec4
	{
		Type x = 0;
		Type y = 0;
		Type z = 0;
		Type w = 0;

		vec4() = default;
		vec4(Type x, Type y, Type z, Type w) : x(x), y(y), z(z), w(w) { }

		const Type& operator[](int i) const
		{
			switch (i)
			{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			case 3: return w;
			default: throw "Index Error";
			}
		}

		inline Type& operator[](int i) {
			return const_cast<Type&>(static_cast<const vec4&>(*this)[i]);
		}
	};

	template<typename Type>
	struct vec3
	{
		Type x = 0;
		Type y = 0;
		Type z = 0;

		vec3() = default;
		vec3(Type x) : x(x), y(x), z(x) { }
		vec3(Type x, Type y, Type z) : x(x), y(y), z(z) { }

		const Type& operator[](int i) const
		{
			switch (i)
			{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: throw "Index Error";
			}
		}

		inline Type& operator[](int i) {
			return const_cast<Type&>(static_cast<const vec3&>(*this)[i]);
		}
		inline operator vec4<Type>() const { return vec4(x, y, z, static_cast<Type>(0)); }
	};

	template<typename Type>
	struct vec2
	{
		Type x = 0;
		Type y = 0;

		vec2() = default;
		vec2(Type x, Type y) : x(x), y(y) { }

		const Type& operator[](int i) const
		{
			switch (i)
			{
			case 0: return x;
			case 1: return y;
			default: throw "Index Error";
			}
		}

		inline Type& operator[](int i) {
			return const_cast<Type&>(static_cast<const vec2&>(*this)[i]);
		}
		inline operator vec3<Type>() const { return vec3<Type>(x, y, static_cast<Type>(0.0f)); }
		inline void operator+=(const vec2& b) { x += b.x; y += b.y; }
	};

	template<typename Type>
	struct mat4
	{
		mat4() = default;
		mat4(const vec4<Type> _data[4]) { data = { _data[0], _data[1], _data[2], _data[3] }; }

		inline const vec4<Type>& operator[](int i) const { return data[i]; }
		inline vec4<Type>& operator[](int i) {
			return const_cast<vec4<Type>&>(static_cast<const mat4&>(*this)[i]);
		}
		
		vec4<Type> operator * (const vec4<Type> v)
		{
			vec4 result;
			
			result.x = data[0][0] * v.x + data[0][1] * v.y + data[0][2] * v.z + data[0][3] * v.w;
			result.y = data[1][0] * v.x + data[1][1] * v.y + data[1][2] * v.z + data[1][3] * v.w;
			result.z = data[2][0] * v.x + data[2][1] * v.y + data[2][2] * v.z + data[2][3] * v.w;
			result.w = data[3][0] * v.x + data[3][1] * v.y + data[3][2] * v.z + data[3][3] * v.w;
			
			return result;
		}

		template<typename NewType>
		operator mat4<NewType>()
		{
			vec4<NewType> _data[4];
			for (unsigned int y = 0; y < 4; y++)
			{
				vec4<NewType> temp;
				for (unsigned int x = 0; x < 4; x++)
					temp[x] = static_cast<NewType>(data[y][x]);

				_data[y] = temp;
			}
		}

	private:
		std::array<vec4<Type>, 4> data;
	};

	template<typename Type>
	static mat4<Type> IdentityMatrix()
	{
		vec4<Type> _data[] = {
			vec4<Type>(1.0f, 0.0f, 0.0f, 0.0f),
			vec4<Type>(0.0f, 1.0f, 0.0f, 0.0f),
			vec4<Type>(0.0f, 0.0f, 1.0f, 0.0f),
			vec4<Type>(0.0f, 0.0f, 0.0f, 1.0f),
		};

		return mat4<Type>(_data);
	}

	template<typename Type>
	static mat4<Type> Translate(const mat4<Type>& matrix, const vec3<Type>& translationVector)
	{
		mat4 result = matrix;

		result[0][3] += translationVector.x;
		result[1][3] += translationVector.y;
		result[2][3] += translationVector.z;
		
		return result;
	}

	template<typename Type>
	inline static mat4<Type> Translate(const mat4<Type>& matrix, const vec2<Type>& translationVector)
	{
		return Translate(matrix, math::vec3(translationVector.x, translationVector.y, 0.0f));
	}

	template<typename Type>
	static mat4<Type> Scale(const mat4<Type>& matrix, const vec3<Type>& translationVector)
	{
		mat4 result = matrix;

		for (unsigned char row = 0; row < 4; row++)
			for (unsigned char col = 0; col < 3; col++)
				result[row][col] *= translationVector[col];

		return result;
	}

	template<typename Type>
	static mat4<Type> Ortho(float left, float right, float bottom, float top)
	{
		mat4<Type> result = IdentityMatrix<Type>();

		result[0][0] = static_cast<Type>(2) / (right - left);
		result[1][1] = static_cast<Type>(2) / (top - bottom);

		result[0][3] = static_cast<Type>(-1) * (right + left) / (right - left);
		result[1][3] = static_cast<Type>(-1) * (top + bottom) / (top - bottom);

		return result;
	}

	template<typename Type>
	static mat4<Type> Ortho(float left, float right, float bottom, float top, float far, float near)
	{
		mat4<Type> result = IdentityMatrix<Type>();

		result[0][0] = static_cast<Type>(2) / (right - left);
		result[1][1] = static_cast<Type>(2) / (top - bottom);
		result[2][2] = static_cast<Type>(-2) / (far - near);

		result[0][3] = static_cast<Type>(-1) * (right + left) / (right - left);
		result[1][3] = static_cast<Type>(-1) * (top + bottom) / (top - bottom);
		result[2][3] = static_cast<Type>(-1) * (far + near) / (far - near);

		return result;
	}

	template<typename Type>
	static mat4<Type> Rotate(const mat4<Type>& matrix, float angle, const vec3<Type>& rotation_axis)
	{
		mat4 result = matrix;

		double _cos = cos(angle);
		double _sin = sin(angle);

		result[0][0] = static_cast<Type>(_cos + pow(rotation_axis.x, 2) * (1 - _cos));
		result[0][1] = static_cast<Type>(rotation_axis.x * rotation_axis.y * (1 - _cos) - rotation_axis.z * _sin);
		result[0][2] = static_cast<Type>(rotation_axis.x * rotation_axis.z * (1 - _cos) + rotation_axis.y * _sin);

		result[1][0] = static_cast<Type>(rotation_axis.x * rotation_axis.y * (1 - _cos) + rotation_axis.z * _sin);
		result[1][1] = static_cast<Type>(_cos + pow(rotation_axis.y, 2) * (1 - _cos));
		result[1][2] = static_cast<Type>(rotation_axis.y * rotation_axis.z * (1 - _cos) - rotation_axis.y * _sin);

		result[2][0] = static_cast<Type>(rotation_axis.x * rotation_axis.z * (1 - _cos) - rotation_axis.y * _sin);
		result[2][1] = static_cast<Type>(rotation_axis.y * rotation_axis.z * (1 - _cos) + rotation_axis.y * _sin);
		result[2][2] = static_cast<Type>(_cos + pow(rotation_axis.z, 2) * (1 - _cos));

		return result;
	}

	static inline float Radians(float degrees) {
		return (float)(degrees * (PI / 180));
	}

	static inline float Angle(float radians) {
		return (float)(radians * (180 / PI));
	}
}