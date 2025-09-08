#ifndef READQUESTION_H
#define READQUESTION_H

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

#include "question.h"
#include "flags.h"

void eraseWS(std::string& str);
void eraseTWS(std::string& str);
void replaceEscape(std::string& str);
std::string readInput(std::string fileName);
int getQuestionType(std::string rawText);
std::vector<std::string> rawQuestionText(std::string rawText);
std::string getQuestionText(std::string questionText);
std::string getQuestionTextOpen(std::string questionText);
std::string getQuestionName(std::string questionText);
std::vector<std::string> getAnswers(std::string questionText);
std::vector<std::string> getGapText(std::string questionText);
std::vector<double> getPointsMC(std::string questionText);
std::vector<double> getPointsSC(std::string questionText);
std::vector<double> getPointsOpen(std::string questionText);
std::vector<double> getPointsGap(std::string questionText);
std::vector<Question> getQuestions(std::string fileName, Settings& settings);

#endif
