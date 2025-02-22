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
#ifndef CONFIG_NODE_BASE_H
#define CONFIG_NODE_BASE_H

#include <memory>
#include <string>
#include <Journal.h>
using namespace std;

/**
 * @brief 用于判断是不是内部的config node，插件不需要用到
 * Irrelevant for config node plugin
 */
enum innerConfigNode {
    DATE_CONFIG = 1,
    TAG_CONFIG,
    LEVEL_CONFIG,
};

/**
 * @brief config node的接口类
 * 
 * interface class of config node 
 */
class ConfigNodeBase
{
public:
    ~ConfigNodeBase() = default;

    virtual uint32_t apiVersion() = 0;

    virtual string getKey() = 0;
    virtual string getDefaultValue() = 0;
    /**
     * @brief preprocess()是要求用户输入jrounal之前的处理
     * postprocess()是用户输入完journal之后的处理
     * 
     * preprocess() will run before user input journal
     * postprocess() process after user finish input journal
     */
    virtual void preprocess(shared_ptr<Journal> journal) = 0;
    virtual void postprocess(shared_ptr<Journal> journal) = 0;

    /**
     * @brief 这两个函数不是通用的函数，是专门针对程序自带的几个config node
     * The tow function is no a general function. It is for the config node, that
     * build in jrme
     */
    virtual int32_t innerConfigType()  {return 0;};
    virtual void setDate(time_t stamp) {};
};
#endif