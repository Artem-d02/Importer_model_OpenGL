#pragma once

#include <glad/glad.h> // содержит все объявления OpenGL-типов

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shaders.h"

#include <string>
#include <vector>

namespace msh
{

	struct Vertex {

		// Позиция
		glm::vec3 Position;

		// Нормаль
		glm::vec3 Normal;

		// Текстурные координаты
		glm::vec2 TexCoords;

		// Касательный вектор
		glm::vec3 Tangent;

		// Вектор бинормали (вектор, перпендикулярный касательному вектору и вектору нормали)
		glm::vec3 Bitangent;
	};

	struct Texture {
		unsigned int id;
		std::string type;
		std::string path;
	};

	class Mesh {
	public:
		// Данные меша
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		unsigned int VAO;

		// Конструктор
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

		// Рендеринг меша
		void Draw(shdr::Shader& shader);

	private:
		// Данные для рендеринга 
		unsigned int VBO, EBO;

		// Инициализируем все буферные объекты/массивы
		void setupMesh();
	};

}	//	namespace msh