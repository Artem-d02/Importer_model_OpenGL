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
		// ƒанные модели 
		std::vector<msh::Texture> textures_loaded; // (оптимизаци€) сохран€ем все загруженные текстуры, чтобы убедитьс€, что они не загружены более одного раза
		std::vector<msh::Mesh> meshes;
		std::string directory;
		bool gammaCorrection;

		//  онструктор в качестве аргумента использует путь к 3D-модели
		Model(std::string const& path, bool gamma = false);

		// ќтрисовываем модель, а значит и все еЄ меши
		void Draw(shdr::Shader shader);

	private:
		// «агружаем модель с помощью Assimp и сохран€ем полученные меши в векторе meshes
		void loadModel(std::string const& path);

		// –екурсивна€ обработка узла. ќбрабатываем каждый отдельный меш, расположенный в узле, и повтор€ем этот процесс дл€ своих дочерних углов (если таковы вообще имеютс€)
		void processNode(aiNode* node, const aiScene* scene);

		msh::Mesh processMesh(aiMesh* mesh, const aiScene* scene);

		// ѕровер€ем все текстуры материалов заданного типа и загружам текстуры, если они еще не были загружены.
		// Ќеобходима€ информаци€ возвращаетс€ в виде структуры Texture
		std::vector<msh::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};

}	//	namespace mdl