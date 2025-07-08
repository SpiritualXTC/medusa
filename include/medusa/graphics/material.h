#pragma once

#include <medusa/medusa.h>
#include <medusa/math.h>

namespace medusa
{
	class Material
	{
	public:
		Material() {}
		virtual ~Material() {}


		inline const glm::vec4& ambient() const { return _ambient; }
		inline const glm::vec4& diffuse() const { return _diffuse; }
		inline const glm::vec4& specular() const { return _specular; }
		inline const glm::vec4& emissive() const { return _emissive; }

		inline Material& ambient(const glm::vec4& ambient) { _ambient = ambient; return *this; }
		inline Material& ambient(float r, float g, float b, float a = 1.0) { return ambient({ r, g, b, a }); }

		inline Material& diffuse(const glm::vec4& diffuse) { _diffuse = diffuse; return *this; }
		inline Material& diffuse(float r, float g, float b, float a = 1.0) { return diffuse({ r, g, b, a }); }
		inline Material& specular(const glm::vec4& specular) { _specular = specular; return *this; }
		inline Material& specular(float r, float g, float b, float a = 1.0) { return specular({ r, g, b, a }); }

		inline Material& emissive(const glm::vec4& emissive) { _emissive = emissive; return *this; }
		inline Material& emissive(float r, float g, float b, float a = 1.0) { return emissive({ r, g, b, a }); }



	private:

		glm::vec4 _ambient;
		glm::vec4 _diffuse;
		glm::vec4 _specular;
		glm::vec4 _emissive;
	};
}
