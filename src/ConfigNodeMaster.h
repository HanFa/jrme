/*
Copyright 2022 Zorn Link

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef CONFIG_NODE_MASTER_H
#define CONFIG_NODE_MASTER_H

#include <list>
#include <memory>

#include "ConfigNodeBase.h"

/**
 * @brief 
 * 加载所有的config node，包括插件
 * 管理搜哟逇config node
 * 生成日记的config部分
 * 依次调用config node的preprocess()和postprecess()
 * 
 * Load all config node include pulgin.
 * ConfigNodeMaster manage all config node.
 * Generate config of a journal.
 * Call preprocess() and postprocess() one by one.
 * 
*/
class ConfigNodeMaster
{
private:
    list<shared_ptr<ConfigNodeBase>> mNodeList;

public:
    ConfigNodeMaster();
    ~ConfigNodeMaster() = default;

    size_t nodeSize();

    /**
     * @brief 载入一个config node插件
     * add plug in node to ConfigNodeMaster
     * 
     * @param name plugin node
     * @return true load plugin success
     * @return false load plugin faild
     */
    bool addPluginNode(string name);
    bool addPluginNode(list<string> nameList) {return true;};

    /**
     * @brief 根据载入的config node生成日记中的config部分
     * Generate config in journal by loaded config node
     * 
     * @return string 
     */
    string genConfig();
    /**
     * @brief 依次运行每个载入的config node的preprocess()或者postprocess()
     * Run preprocess() or postprocess() one by one
     */
    void preprocess(shared_ptr<Journal> journal);
    void postprocess(shared_ptr<Journal> journal);

    /**
     * @brief 为 date config node设置时间
     * set date for date config node.
     */
    void setDate(time_t stamp);
};

#endif