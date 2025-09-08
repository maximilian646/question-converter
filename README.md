# ILIAS Question Converter

This little tool was inspired by *Kaffedraches Tiqi*. It is mainly a C++ rewrite with some added things a thougth would be nice.

> Create an ILIAS question pool from a text file (because doing this in ILIAS is annoying) - *Kaffedrache*

It enables you to write a text file and with some commands create a ILIAS question pool file.

## How to use

1. Write Questions

	Write a `.txt` file containing the questions (you can do this in the same directory as the converter executable to make things easier).

2. Convert Questions
   
	Open the termial in the same folder as the executable. To run the conversion type `./converter input_file_name` (on Linux) or `.\converter.exe input_file_name` (on Windows). The programm should throw an appropriate error if things go wrong.

	You can also add the following flags:
	+ `-r` or `--raw` to display the raw input text divided into questions
	+ `-q` or `--objects` to display the generated question objects (good for knowing what the program does) 
	+ `-n` or `--nowrite` to supress the generation of a `.xml` file

	Example: `./main input.txt -q -r`

3. Import into ILIAS

	Go to your ILIAS course and create a new test or question pool. If that is done, to into the question section and click import. Select the generated `.xml` file and upload. Select all the quesitions you want to use and continue.

## Formatting of the `.txt` file

+ Meta Data:

	Each question has to start with a `[q]`. After that the question type is declared (`[s]` for single choice, `[m]` for multiple choice, `[o]` for open questions and `[g]` for gap/cloze questions). You can add the maximum points with e.g. `[3]`. Note that this is an optional parameter. If the points are not declared, **1** point is assumed. Just add the question title at the end.

	Example: `[q][s][4] Question 1`

+ Question Text:

	Add the question text in a new line. There can be various new lines but no double new lines. Add a double new line at the end.

+ For single and multiple choice questions: Add answers

	Place your answers here. A line for each answer. Correct answers are prefixed by a `_` and wrong ones by `-`.

+ For open questions:

	Simply start the next question.

+ For gap questions:

	Write your gap text. You can add as many spaces and new lines as you want. A gap is started by `[gap]` and ended by `[\gap]`. Between these you can place your answers. If there is only one answer a open gap is assumed. If there are multiple options, you get a choosing gap with the first answer assumed correct. These answers are seperated by a `;`.

+ Points:
	
	Points are automaticlly divided among all answers. If there are numbers like 1/3rd. The last number will be the missing amound up to the maximum. E.g. 0.33, 0.33 and 0.34 (no, i have not found a better solution).

## Example

```
[q][s][2] Question 1
Which of the following answers are correct?


- incorrect answer
_ correct answer
- another incorrect answer
```

Note: the file name will be `input_file_name.qti.xml` because the ILIAS of my institution requires "qti" somewhere in the file name (i really don't know why).
