#include "question.h"

Question::Question(std::string questionName, int questionType, std::string questionText)
	: m_questionName(questionName), 
	m_questionType(questionType), 
	m_questionText(questionText)
{}	
//simple constructor

Question::Question(std::string questionName, int questionType, std::string questionText, std::vector<double> points)
	: m_questionName(questionName), 
	m_questionType(questionType), 
	m_questionText(questionText),  
	m_points(points)
{}
//constructor open questions

Question::Question(std::string questionName, int questionType, std::string questionText, std::vector<std::string> answers, std::vector<double> points)
	: m_questionName(questionName), 
	m_questionType(questionType), 
	m_questionText(questionText), 
	m_answers(answers), 
	m_points(points)
{}
// constructor sc, mc and gap


std::ostream& operator<<(std::ostream& os, const Question& obj){

	os << std::string("m_questionName: ") << obj.m_questionName 
	<< std::string("\nm_questionType: ") << std::to_string(obj.m_questionType)
	<< std::string("\nm_questionText: ") << obj.m_questionText;

	return os;	
};	

void Question::printAnswers(){

	for(auto i : m_answers){
		std::cout << i << std::endl;
	}	
};

void Question::printPoints(){

	for(auto i : m_points){
		std::cout << i << std::endl;
	}	
};

std::string Question::name(){
	return m_questionName;
};

int Question::type(){
	return m_questionType;
};

std::string Question::text(){
	return m_questionText;
};

std::vector<std::string> Question::answers(){
	return m_answers;
};

std::vector<double> Question::points(){
	return m_points;
};





