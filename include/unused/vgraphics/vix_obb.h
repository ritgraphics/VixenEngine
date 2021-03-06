#ifndef VIX_OBB_H
#define VIX_OBB_H

#include <vix_platform.h>
#include <vix_color.h>
#include <vix_primitive_cube.h>

namespace Vixen {

	class Model;

	class VIX_API OBB
	{
	public:
		OBB(Model* parent);

		~OBB();

		/*GETTERS*/
		const Vec3& Min() const;
		const Vec3& Max() const;
		const Vec3& Centroid() const;
		Vec3 TransformedCentroid();
		const Vec3& Size() const;
		const Mat4& World() const;

		/*SETTERS*/

		void SetHighlight(const Color& color);

		void Update();

		void Render(GLCamera3D* camera);

		static bool CheckCollision(OBB* a, OBB* b);

	private:
		void UpdateTransformed();

	private:
		Model*			m_parent;
		PrimitiveCube*	m_box;
		Color           m_highlight;
		Vec3			m_position;
		Vec3			m_centroid;
		Vec3			m_size;
		Vec3            m_min;
		Vec3            m_max;
		Mat4            m_world;
	};

}

#endif