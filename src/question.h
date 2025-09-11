#ifndef QUESTION_H
#define QUESTION_H

#include <iostream>
#include <string>
#include <vector>

enum QuestionType{
	SINGLE_CHOICE,
	MULTIPLE_CHOICE,
	OPEN_QUESTION,
	GAP_QUESTION
};

class Question{
	public:
		Question(std::string questionName, int questionType, std::string questionText);
		//simple constructor, no longer used

		Question(std::string questionName, int questionType, std::string questionText, std::vector<double> points);
		//constructor open questions

		Question(std::string questionName, int questionType, std::string questionText, std::vector<std::string> answers, std::vector<double> points);
		//constructor sc, mc and gap


		friend std::ostream& operator<<(std::ostream& os, const Question& obj);

		void printAnswers() const;
		void printPoints() const;
		std::string name() const;
		int type() const;
		std::string text() const;
		std::vector<std::string> answers() const;
		std::vector<double> points() const;
		
		


	private:
		//member variables private?
		const std::string m_questionName;
		const int m_questionType;
		const std::string m_questionText;
		const std::vector<double> m_points;
		const std::vector<std::string> m_answers;
		
		
};

#endif
