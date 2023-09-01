#include "Models/ModelManager.h"
#include <stdexcept>
#include <algorithm>

bool ModelManager::cachedModelName(const std::string& modelName)
{
	std::scoped_lock lk(_mtx);
	if (auto iter = _modelCache.find(modelName); iter == _modelCache.end()) {
		_modelCache[modelName] = nullptr;
		return true;
	}

	return false;
}

void ModelManager::loadModelObj(const std::string& pathModel, const std::string& modelName, unsigned int flag)
{
	std::scoped_lock lk(_mtx);
	auto iter = _modelCache.find(modelName);
	if (iter == _modelCache.end()) {
		std::string msg = "Model was not precached! " + modelName;
		throw std::runtime_error{ msg.c_str() };
	}

	iter->second = loadModel(pathModel, flag);
	_modelNames.push_back(modelName);
}

AssimpModel* ModelManager::getModel(const std::string& modelName)
{
	std::scoped_lock lk(_mtx);
	if (auto iter = _modelCache.find(modelName); iter != _modelCache.end()) {
		return iter->second.get();
	}

	return nullptr;
}

void ModelManager::deleteModel(const std::string& modelName)
{
	std::scoped_lock lk(_mtx);
	_modelCache.erase(modelName);
	_modelNames.erase(std::remove_if(_modelNames.begin(), _modelNames.end(), [&modelName](const auto& item) {
		return item == modelName;
		}), _modelNames.end());
}

void ModelManager::clearModelCache() {
	std::scoped_lock lk(_mtx);
	_modelCache.clear();
}

bool ModelManager::hasModel(const std::string& modelName) const
{
	std::scoped_lock lk(_mtx);
	return _modelCache.count(modelName);
}

const std::vector<std::string>& ModelManager::getModelsMenu() const {
	return _modelNames;
}

size_t ModelManager::size() const {
	std::scoped_lock lk(_mtx);
	return _modelCache.size();
}

bool ModelManager::empty() const {
	std::scoped_lock lk(_mtx);
	return _modelCache.empty();
}