#pragma once

#include <glad/glad.h> // �������� ��� ���������� OpenGL-�����

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shaders.h"

#include <string>
#include <vector>

namespace msh
{

	struct Vertex {

		// �������
		glm::vec3 Position;

		// �������
		glm::vec3 Normal;

		// ���������� ����������
		glm::vec2 TexCoords;

		// ����������� ������
		glm::vec3 Tangent;

		// ������ ��������� (������, ���������������� ������������ ������� � ������� �������)
		glm::vec3 Bitangent;

		// ����� �������
		glm::vec4 ColorAmbient;

		glm::vec4 ColorDiffuse;

		glm::vec4 ColorSpecular;

		float shininess;

	};

	struct Texture {
		unsigned int id;
		std::string type;
		std::string path;
	};

	class Mesh {
	public:
		// ������ ����
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		unsigned int VAO;

		// �����������
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

		// ��������� ����
		void Draw(shdr::Shader& shader);

	private:
		// ������ ��� ���������� 
		unsigned int VBO, EBO;

		// �������������� ��� �������� �������/�������
		void setupMesh();
	};

}	//	namespace msh