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
#include <fstream>
#include <gtest/gtest.h>
#include <memory>
#include <stdint.h>
#include <stdio.h>
#include <string>

#include "Journal.h"
#include "JournalIOBase.h"
#include "JournalIOFactory.h"

using namespace std;


TEST(markdown, read)
{
    ifstream journalStore;
    journalStore.open("read1Store.txt");
    ASSERT_TRUE(journalStore.is_open()) << "read1Store.txt open failed";
    shared_ptr<JournalIOBase> journalIO = JournalIOFactory().getJournalIO("read1.md");
    ASSERT_TRUE(journalIO) << "creat journal faild!";
    ASSERT_TRUE(journalIO->open("read1.md")) << "MdJournalIO open failed";
    journalIO->setReadMod();
    for (size_t i = 0; i < 3; i++)
    {
        shared_ptr<Journal> journal = journalIO->readJournal();
        ASSERT_TRUE(journal!=nullptr) << "journalIO read fail";
        
        string lineBuffer;
        Journal jStandard;
        getline(journalStore, lineBuffer, '#');
        jStandard.setTitle(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard.setConfig(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard.setContent(lineBuffer);
        ASSERT_TRUE(*journal == jStandard) << "fail in loop " << to_string(i) << "\n" << journal->toString() << jStandard.toString();
    }
}

TEST(markdown, write)
{
    ifstream journalStore;
    journalStore.open("read1Store.txt");
    ASSERT_TRUE(journalStore.is_open()) << "read1Store.txt open failed when verfy write";
    
    shared_ptr<JournalIOBase> journalIO = JournalIOFactory().getJournalIO("read1.md");
    ASSERT_TRUE(journalIO) << "creat journal faild!";
    string fileToWrite = string("test.txt");
    remove(fileToWrite.c_str());
    ASSERT_TRUE(journalIO->open(fileToWrite)) << "MdJournalIO open failed";
    bool ret = journalIO->setWriteMode();
    for (size_t i = 0; i < 3; i++)
    {
        string lineBuffer;
        shared_ptr<Journal> jStandard = shared_ptr<Journal>(new Journal);
        getline(journalStore, lineBuffer, '#');
        jStandard->setTitle(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard->setConfig(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard->setContent(lineBuffer);
        
        bool writeRet = journalIO->writeJournal(jStandard);
        ASSERT_TRUE(writeRet) << "journalIO wirte file:" << fileToWrite << " failed";
    }
    
    /* verfy writen file */
    journalStore.seekg(0, ios::beg);
    ASSERT_TRUE(journalStore.is_open()) << "read1Store.txt open failed when verfy write";
    ASSERT_TRUE(journalIO->open(fileToWrite)) << "MdJournalIO open failed";
    journalIO->setReadMod();
    for (size_t i = 0; i < 3; i++)
    {
        shared_ptr<Journal> journal = journalIO->readJournal();
        ASSERT_TRUE(journal!=nullptr) << "journalIO read fail when verfy write";
        
        string lineBuffer;
        Journal jStandard;
        getline(journalStore, lineBuffer, '#');
        jStandard.setTitle(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard.setConfig(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard.setContent(lineBuffer);
        
        if (!(*journal == jStandard))
        {
            printf("%s",journal->toString().c_str());
            printf("%s",jStandard.toString().c_str());
        }
        ASSERT_TRUE(*journal == jStandard) << " verfy write fail in loop " << to_string(i);
    }
}

TEST(txt, read)
{
    ifstream journalStore;
    journalStore.open("read1Store.txt");
    ASSERT_TRUE(journalStore.is_open()) << "read1Store.txt open failed";
    shared_ptr<JournalIOBase> journalIO = JournalIOFactory().getJournalIO("read1.txt");
    ASSERT_TRUE(journalIO) << "creat journal faild!";
    ASSERT_TRUE(journalIO->open("read1.txt")) << "TxtJournalIO open failed";
    journalIO->setReadMod();
    for (size_t i = 0; i < 3; i++)
    {
        shared_ptr<Journal> journal = journalIO->readJournal();
        ASSERT_TRUE(journal!=nullptr) << "journalIO read fail";
        
        string lineBuffer;
        Journal jStandard;
        getline(journalStore, lineBuffer, '#');
        jStandard.setTitle(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard.setConfig(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard.setContent(lineBuffer);
        
        ASSERT_TRUE(*journal == jStandard) << "fail in loop " << to_string(i);
    }
}

TEST(txt, write)
{
    ifstream journalStore;
    journalStore.open("read1Store.txt");
    ASSERT_TRUE(journalStore.is_open()) << "read1Store.txt open failed when verfy write";
    
    string fileToWrite = string("test.txt");
    remove(fileToWrite.c_str());
    shared_ptr<JournalIOBase> journalIO = JournalIOFactory().getJournalIO("read1.txt");
    ASSERT_TRUE(journalIO) << "creat journal faild!";
    ASSERT_TRUE(journalIO->open(fileToWrite)) << "TxtJournalIO open failed";
    bool ret = journalIO->setWriteMode();
    for (size_t i = 0; i < 3; i++)
    {
        string lineBuffer;
        shared_ptr<Journal> jStandard = shared_ptr<Journal>(new Journal);
        getline(journalStore, lineBuffer, '#');
        jStandard->setTitle(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard->setConfig(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard->setContent(lineBuffer);
        
        bool writeRet = journalIO->writeJournal(jStandard);
        ASSERT_TRUE(writeRet) << "journalIO wirte file:" << fileToWrite << " failed";
    }
    
    /* verfy writen file */
    journalStore.seekg(0, ios::beg);
    ASSERT_TRUE(journalStore.is_open()) << "read1Store.txt open failed when verfy write";
    journalIO->open(fileToWrite);
    journalIO->setReadMod();
    for (size_t i = 0; i < 3; i++)
    {
        shared_ptr<Journal> journal = journalIO->readJournal();
        ASSERT_TRUE(journal!=nullptr) << "journalIO read fail when verfy write";
        
        string lineBuffer;
        Journal jStandard;
        getline(journalStore, lineBuffer, '#');
        jStandard.setTitle(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard.setConfig(lineBuffer);
        getline(journalStore, lineBuffer, '#');
        jStandard.setContent(lineBuffer);
        
        ASSERT_TRUE(*journal == jStandard) << " verfy write fail in loop " << to_string(i);
    }
}

int main(int argc, char* argv[]) {
    
    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS(); 

    
    return ret;
}