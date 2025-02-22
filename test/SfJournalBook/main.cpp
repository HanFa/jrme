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
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <syscall.h>

using namespace std;

#include "SfJournalBook.h"
#include "Utils.h"

#include "debug_print.h"

TEST(SFJournalBook, total)
{
    ifstream journalStore;
    journalStore.open("loop1Insert.txt");
    ASSERT_TRUE(journalStore.is_open()) << "loop1Insert.txt open failed";
    system("cp loop1.txt test.txt");
    SfJournalBook journalBook;
    ASSERT_TRUE(journalBook.open("test.txt")) << "journal book open filed";
    ASSERT_EQ(journalBook.size(), 3) << "journal on journal book test.txt not eq 3";

    string lineBuffer;
    shared_ptr<Journal> jInsert = shared_ptr<Journal>(new Journal);
    getline(journalStore, lineBuffer, '#');
    jInsert->setTitle(lineBuffer);
    getline(journalStore, lineBuffer, '#');
    jInsert->setConfig(lineBuffer);
    getline(journalStore, lineBuffer, '#');
    jInsert->setContent(lineBuffer);
    
    journalBook.push_back(jInsert);
    ASSERT_EQ(journalBook.size(), 4) << "journal on SfJournalBook not eq 4 after insert";

    journalBook.order();
    ASSERT_TRUE(journalBook.save()) << "journal book save fail";
    journalBook.close();
    
    //verfy
    SfJournalBook journalBookV;
    ASSERT_TRUE(journalBookV.open("test.txt"))  << "journal book open filed while verfy";
    ASSERT_EQ(journalBookV.size(), 4) << "journal on journal book test.txt not eq 4 while verfy";
    bool inorder = true;
    size_t loopCount = journalBookV.size();
    time_t stampBefor = getStampFormConfig(journalBookV.at(0)->getConfig());
    time_t stampNow;
    for (size_t i = 1; i < loopCount; i++)
    {
        stampNow = getStampFormConfig(journalBookV.at(i)->getConfig());
        if (stampBefor<stampNow)
        {
            inorder = false;
            break;
        }
        stampBefor = stampNow;
    }
    ASSERT_TRUE(inorder) << "journal no in order";
}


int main(int argc, char* argv[]) {
    
    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS(); 
    return ret;
}