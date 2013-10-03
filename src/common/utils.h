// File: utils.h
// Author: Karl Moritz Hermann (mail@karlmoritz.com)
// Created: 30-01-2013
// Last Update: Thu 03 Oct 2013 11:47:39 AM BST

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

#ifndef UTILS_H_ODQEYJVF
#define UTILS_H_ODQEYJVF


#include "shared_defs.h"
#include "models.h"

void dumpModel(Model& model, int k);
void printSentence(const Dictionary& dict, const TrainingInstance &sent);

// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}


#endif /* end of include guard: UTILS_H_ODQEYJVF */
