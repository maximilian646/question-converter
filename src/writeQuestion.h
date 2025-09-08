#include "question.h"
#include <fstream>
#include <filesystem>
#include <format>

#define HEADER "<?xml version=\"1.0\" encoding=\"utf-8\"?><!DOCTYPE questestinterop SYSTEM \"ims_qtiasiv1p2p1.dtd\">"
#define BEGIN "<questestinterop>"
#define END "</questestinterop>"


std::string getMetaData(Question& question);
std::string answerPresentation(std::string answer, int numAnswer);
std::string answerPointsSC(double points, int numAnswer);
std::string answerPointsMC(double points, int numAnswer);
std::string getPresentation(Question& question);
std::string getResprocessing(Question& question);
std::string getQuestionString(std::vector<Question>& questions);
void writeQuestions(std::vector<Question>& questions, const std::string& fileName);


