#include "Model.h"

namespace mdl
{
	Model::Model(std::string const& path)
	{
		loadModel(path);
	}

	void Model::Draw(shdr::Shader shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i].Draw(shader);
	}

	void Model::loadModel(std::string const& path)
	{
		// ������ ����� � ������� Assimp
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		// �������� �� ������
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // ���� �� 0
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}

		// ��������� ���� � �����
		directory = path.substr(0, path.find_last_of('/'));

		// ����������� ��������� ��������� ���� Assimp
		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		// ������������ ������ ��� �������� ����
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// ���� �������� ������ ������� �������� � �����.
			// ����� �� �������� ��� ������; ���� - ��� ���� ������ ����������� ������
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			aiMaterial* mtl = scene->mMaterials[mesh->mMaterialIndex];
			meshes.push_back(processMesh(mesh, scene, mtl));
		}
		// ����� ����, ��� �� ���������� ��� ���� (���� ������� �������), �� �������� ���������� ������������ ������ �� �������� �����
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}

	}

	msh::Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene, const aiMaterial* mtl)
	{
		// ������ ��� ����������
		std::vector<msh::Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<msh::Texture> textures;

		// ���� �� ���� �������� ����
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			msh::Vertex vertex;
			glm::vec4 vector; // �� ��������� ������������� ������, �.�. Assimp ���������� ���� ����������� ��������� �����, ������� �� ������������� �������� � ��� glm::vec3, ������� ������� �� �������� ������ � ���� ������������� ������ ���� glm::vec3

			// ����������
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			// �������
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			// ���������� ����������
			if (mesh->mTextureCoords[0]) // ���� ��� �������� ���������� ����������
			{
				glm::vec2 vec;

				// ������� ����� ��������� �� 8 ��������� ���������� ���������. �� ������������, ��� �� �� ����� ������������ ������,
				// � ������� ������� ����� ��������� ��������� ���������� ���������, ������� �� ������ ����� ������ ����� (0)
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
			{
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
				//std::cout << "Colors is not found" << std::endl;
			}

			if (mesh->mTangents != nullptr)
			{
				// ����������� ������
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.Tangent = vector;
			}
			else
			{
				vertex.Tangent = glm::vec3(0, 0, 0);
			}

			if (mesh->mBitangents != nullptr)
			{
				// ������ ���������
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.Bitangent = vector;
				
			}
			else
			{
				vertex.Bitangent = glm::vec3(0, 0, 0);
			}

			aiColor4D ambient;
			if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
			{
				vector = glm::vec4(ambient.r, ambient.g, ambient.b, ambient.a);
				//std::cout << vector.x << "\t" << vector.y << "\t" << vector.z << "\t" << vector.w << std::endl;
				vertex.ColorAmbient = vector;
			}
			else
			{
				vertex.ColorAmbient = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
			}

			aiColor4D diffuse;
			if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
			{
				vector = glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
				//std::cout << vector.x << "\t" << vector.y << "\t" << vector.z << "\t" << vector.w << std::endl;
				vertex.ColorDiffuse = vector;
			}
			else
			{
				vertex.ColorDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
			}

			aiColor4D specular;
			if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
			{
				vector = glm::vec4(specular.r, specular.g, specular.b, specular.a);
				//std::cout << vector.x << "\t" << vector.y << "\t" << vector.z << "\t" << vector.w << std::endl;
				vertex.ColorSpecular = vector;
			}
			else
			{
				vertex.ColorSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
			}

			unsigned int max = 1;
			aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &vertex.shininess, &max);

			vertices.push_back(vertex);
		}
		// ������ ���������� �� ������ ����� ���� (����� - ��� ����������� ����) � ��������� ��������������� ������� ������
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			// �������� ��� ������� ������ � ��������� �� � ������� indices
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// ������������ ���������
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// �� ������ ���������� �� ������ ��������� � ��������. ������ ��������� �������� ����� ���������� 'texture_diffuseN',
		// ��� N - ���������� ����� �� 1 �� MAX_SAMPLER_NUMBER. 
		// ���� ����� ��������� � � ������ ���������:
		// ��������� - texture_diffuseN
		// ��������� - texture_specularN
		// ������� - texture_normalN

		// 1. ��������� �����
		std::vector<msh::Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// 2. ����� ���������
		std::vector<msh::Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		// 3. ����� ��������
		std::vector<msh::Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		// 4. ����� �����
		std::vector<msh::Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		// Find count of different textures
		/*
		std::cout << "Different textures count:" << std::endl;
		std::cout << "AMBIENT:\t\t" << material->GetTextureCount(aiTextureType_AMBIENT) << std::endl;
		std::cout << "AMBIENT_OCCLUSION:\t" << material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) << std::endl;
		std::cout << "BASE_COLOR:\t\t" << material->GetTextureCount(aiTextureType_BASE_COLOR) << std::endl;
		std::cout << "DIFFUSE:\t\t" << material->GetTextureCount(aiTextureType_DIFFUSE) << std::endl;
		std::cout << "DIFFUSE_ROUGHNESS:\t" << material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) << std::endl;
		std::cout << "DISPLACEMENT:\t\t" << material->GetTextureCount(aiTextureType_DISPLACEMENT) << std::endl;
		std::cout << "EMISSION_COLOR:\t\t" << material->GetTextureCount(aiTextureType_EMISSION_COLOR) << std::endl;
		std::cout << "EMISSIVE:\t\t" << material->GetTextureCount(aiTextureType_EMISSIVE) << std::endl;
		std::cout << "HEIGHT:\t\t\t" << material->GetTextureCount(aiTextureType_HEIGHT) << std::endl;
		std::cout << "LIGHTMAP:\t\t" << material->GetTextureCount(aiTextureType_LIGHTMAP) << std::endl;
		std::cout << "METALNESS:\t\t" << material->GetTextureCount(aiTextureType_METALNESS) << std::endl;
		std::cout << "NORMALS:\t\t" << material->GetTextureCount(aiTextureType_NORMALS) << std::endl;
		std::cout << "NORMAL_CAMERA:\t\t" << material->GetTextureCount(aiTextureType_NORMAL_CAMERA) << std::endl;
		std::cout << "OPACITY:\t\t" << material->GetTextureCount(aiTextureType_OPACITY) << std::endl;
		std::cout << "REFLECTION:\t\t" << material->GetTextureCount(aiTextureType_REFLECTION) << std::endl;
		std::cout << "SHININESS:\t\t" << material->GetTextureCount(aiTextureType_SHININESS) << std::endl;
		std::cout << "SPECULAR:\t\t" << material->GetTextureCount(aiTextureType_SPECULAR) << std::endl;
		std::cout << std::endl;
		*/
		// ���������� ���-������, ��������� �� ������ ���������� ������
		return msh::Mesh(vertices, indices, textures);
	}

	std::vector<msh::Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<msh::Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			// ���������, �� ���� �� �������� ��������� �����, � ���� - ��, �� ���������� �������� ����� �������� � ��������� � ��������� ��������
			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // �������� � ��� �� ����� � ����� ��� ���������, ��������� � ��������� (�����������)
					break;
				}
			}
			if (!skip)
			{   // ���� �������� ��� �� ���� ���������, �� ��������� �
				msh::Texture texture;
				texture.id = TextureFromFile(str.C_Str(), directory);
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
				textures_loaded.push_back(texture); // ��������� �������� � ������� � ��� ������������ ����������, ��� ����� ����������, ��� � ��� �� �������� ��� ������������� ��������� �������
			}
		}
		return textures;
	}

	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
	{
		std::string filename = std::string(path);
		filename = directory + '/' + filename;

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}

}