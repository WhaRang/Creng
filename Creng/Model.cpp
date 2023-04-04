#include "Model.h"

Model::Model(const std::string& modelName, bool alpha) : modelName(modelName), alpha(alpha) {
}

Model::~Model() {
	ClearModel();
}

bool Model::LoadModel(const std::string& fileName) {

	this->modelName = modelName;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene) {
		printf("Model '%p' failed to load: %p", &fileName, importer.GetErrorString());
		return false;
	}

	LoadNode(scene->mRootNode, scene);
	LoadMaterials(scene);
}

void Model::RenderModel() {

	for (size_t i = 0; i < meshes.size(); i++) {
		
		unsigned int materialIndex = meshToTex[i];
		if (materialIndex < textures.size() && textures[materialIndex]) {
			textures[materialIndex]->UseTexture();
		}

		meshes[i]->RenderMesh();
	}
}

void Model::ClearModel() {

	for (size_t i = 0; i < meshes.size(); i++) {
		if (meshes[i]) {
			delete meshes[i];
			meshes[i] = nullptr;
		}
	}

	for (size_t i = 0; i < textures.size(); i++) {
		if (textures[i]) {
			delete textures[i];
			textures[i] = nullptr;
		}
	}
}

void Model::LoadNode(aiNode* node, const aiScene* scene) {

	for (size_t i = 0; i < node->mNumMeshes; i++) {
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++) {
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene) {

	std::vector<GLfloat> verticies;
	std::vector<unsigned int> indicies;

	for (size_t i = 0; i < mesh->mNumVertices; i++) {

		verticies.insert(verticies.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });

		if (mesh->mTextureCoords[0]) {
			verticies.insert(verticies.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else {
			verticies.insert(verticies.end(), { 0, 0 });
		}

		verticies.insert(verticies.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++) {

		aiFace face = mesh->mFaces[i];

		for (size_t j = 0; j < face.mNumIndices; j++) {
			indicies.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&verticies[0], &indicies[0], verticies.size(), indicies.size());
	meshes.push_back(newMesh);
	meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene* scene) {

	textures.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++) {

		aiMaterial* material = scene->mMaterials[i];
		textures[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE)) {

			aiString path;

			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {

				int idx = std::string(path.data).rfind("\\");
				std::string fileName = std::string(path.data).substr(idx + 1);
				std::string texPath = std::string("Textures/" + modelName + "/") + fileName;

				textures[i] = new Texture(texPath.c_str());
				if (!textures[i]->LoadTexture(alpha)) {

					printf("Failed to load texture at: %s\n", texPath);
					delete textures[i];
					textures[i] = nullptr;
				}
			}
		}

		if (!textures[i]) {
			textures[i] = new Texture("Textures/Default/Dirt.jpg");
			textures[i]->LoadTexture(false);
		}
	}
}
