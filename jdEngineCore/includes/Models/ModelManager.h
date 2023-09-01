#pragma once
#include "ECS/components/AssimpModel.h"
#include "Models/Model.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>

class ModelManager final
{
public:
	ModelManager() = default;
	ModelManager(const ModelManager&) = delete;
	void operator=(const ModelManager&) = delete;

	bool cachedModelName(const std::string& modelName);
	void loadModelObj(const std::string& pathModel, const std::string& modelName, unsigned int flag =
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace |
		aiProcess_GenSmoothNormals);
	AssimpModel* getModel(const std::string& modelName);
	void deleteModel(const std::string& modelName);
	void clearModelCache();
	bool hasModel(const std::string& modelName) const;
	const std::vector<std::string>& getModelsMenu() const;
	size_t size() const;
	bool empty() const;

private:
	std::vector<std::string> _modelNames;
	std::unordered_map<std::string, std::unique_ptr<AssimpModel>> _modelCache;
	mutable std::mutex _mtx;
};