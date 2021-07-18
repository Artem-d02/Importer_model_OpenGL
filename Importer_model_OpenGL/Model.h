#pragma once
#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "Shaders.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

namespace mdl
{

	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

	class Model
	{
	public:
		// ������ ������ 
		std::vector<msh::Texture> textures_loaded; // (�����������) ��������� ��� ����������� ��������, ����� ���������, ��� ��� �� ��������� ����� ������ ����
		std::vector<msh::Mesh> meshes;
		std::string directory;
		bool gammaCorrection;

		// ����������� � �������� ��������� ���������� ���� � 3D-������
		Model(std::string const& path, bool gamma = false);

		// ������������ ������, � ������ � ��� � ����
		void Draw(shdr::Shader shader);

	private:
		// ��������� ������ � ������� Assimp � ��������� ���������� ���� � ������� meshes
		void loadModel(std::string const& path);

		// ����������� ��������� ����. ������������ ������ ��������� ���, ������������� � ����, � ��������� ���� ������� ��� ����� �������� ����� (���� ������ ������ �������)
		void processNode(aiNode* node, const aiScene* scene);

		msh::Mesh processMesh(aiMesh* mesh, const aiScene* scene);

		// ��������� ��� �������� ���������� ��������� ���� � �������� ��������, ���� ��� ��� �� ���� ���������.
		// ����������� ���������� ������������ � ���� ��������� Texture
		std::vector<msh::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};

}	//	namespace mdl