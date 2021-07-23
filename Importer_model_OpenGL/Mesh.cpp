#include "Mesh.h"

namespace msh
{

	Mesh::Mesh(std::vector<Vertex> new_vertices, std::vector<unsigned int> new_indices, std::vector<Texture> new_textures)
	{
		vertices = new_vertices;
		indices = new_indices;
		textures = new_textures;

		// ������, ����� � ��� ���� ��� ����������� ������, ������������� ��������� ������ � ��������� ���������
		setupMesh();
	}

	void Mesh::Draw(shdr::Shader& shader)
	{
		// ��������� ��������������� ��������
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // ����� ����������� ���������� ������ ���������� ����

			// �������� ����� �������� (����� N � diffuse_textureN)
			std::string number;
			std::string type = textures[i].type;
			std::string name_first_part = "material_";
			std::string name_last_part;
			if (type == "texture_diffuse")
			{
				number = std::to_string(diffuseNr++);
				name_last_part = ".diffuse";
			}
			else if (type == "texture_specular")
			{
				number = std::to_string(specularNr++); // ������������ unsigned int � ������
				name_last_part = ".specular";
			}
			else if (type == "texture_normal")
			{
				number = std::to_string(normalNr++); // ������������ unsigned int � ������
				name_last_part = ".normal";
			}
			else if (type == "texture_height")
			{
				number = std::to_string(heightNr++); // ������������ unsigned int � ������
				name_last_part = ".height";
			}
			// ������ ������������� ������� �� ������ ���������� ����
			glUniform1i(glGetUniformLocation(shader.ID, (name_first_part + number + name_last_part).c_str()), i);
			shader.setFloat(name_first_part + number + std::string(".shininess"), 32.0f);
			// � ��������� ��������
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}

		// ������������ ���
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// ��������� ������� ��������� ���������� �������� ���������� � �� �������������� ���������
		glActiveTexture(GL_TEXTURE0);
	}

	void Mesh::setupMesh()
	{
		// ������� �������� �������/�������
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		// ��������� ������ � ��������� �����
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// ����� ������������� � ���������� ��, ��� ������������ � ������ �� ���������� ���������� �������� ����������������.
		// ����� ������� ����� � ���, ��� �� ����� ������ �������� ��������� �� ���������, � ��� ��������� ������������� � ������ ������ � ���������� ���� glm::vec3 (��� glm::vec2), ������� ����� ����� ������������ � ������ ������ float, �� � � ����� � � �������� ������
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// ������������� ��������� ��������� ���������

		// ���������� ������
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		// ������� ������
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

		// ���������� ���������� ������
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		// ����������� ������ �������
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

		// ������ ��������� �������
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

		glBindVertexArray(0);
	}

}	//	namespace msh