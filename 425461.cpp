#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <conio.h>

using namespace std;

class CourseType {
private:
    string courseName;
    string courseNo;
    char courseGrade;
    int courseCredit;

public:
    CourseType() : courseName(""), courseNo(""), courseGrade('F'), courseCredit(0) {}
    ~CourseType() {}

    void setCourseInfo(string courseName, string courseNo, char courseGrade, int courseCredit) {
        this->courseName = courseName;
        this->courseNo = courseNo;
        this->courseGrade = courseGrade;
        this->courseCredit = courseCredit;
    }

    void print(ostream& os) { 
        os << setw(10) << courseNo << setw(20) << courseName
           << setw(10) << courseCredit << setw(10) << courseGrade << endl;
    }

    int getCredit() { return courseCredit; }
    char getGrade() { return courseGrade; }
};

class PersonType {
protected:
    string studentName;
    string studentSurname;

public:
    PersonType(string name = "", string surname = "") {
        studentName = name;
        studentSurname = surname;
    }
};

class StudentType : public PersonType {
private:
    int studentId;
    int numberOfCourses;
    char istuitionPaid;
    CourseType* course; 

public:
    StudentType() : PersonType("", ""), studentId(0), numberOfCourses(0), istuitionPaid('N'), course(nullptr) {}
    ~StudentType() {
        delete[] course; 
    }

    void print(ostream& os) {
        os << "Student Name: " << studentName << " " << studentSurname << endl;
        os << "Student ID: " << studentId << endl;
        os << "Tuition Paid: " << (istuitionPaid == 'Y' ? "Yes" : "No") << endl;

        if (istuitionPaid == 'Y') {
            os << setw(10) << "Course No" << setw(20) << "Course Name"
               << setw(10) << "Credits" << setw(10) << "Grade" << endl;
            for (int i = 0; i < numberOfCourses; i++) {
                course[i].print(os);
            }
            os << "GPA: " << fixed << setprecision(2) << calculateGPA() << endl; 
        } else {
            os << "Tuition not paid. Grades withheld." << endl;
        }
    }

    void setInfo(int studentId, int numberOfCourses, char istuitionPaid, string studentName, string studentSurname) {
        this->studentId = studentId;
        this->numberOfCourses = numberOfCourses;
        this->istuitionPaid = istuitionPaid;
        this->studentName = studentName;
        this->studentSurname = studentSurname;

        if (numberOfCourses > 0) {
            course = new CourseType[numberOfCourses]; 
        }
    }

    void setCourseInfo(int index, string courseName, string courseNo, char courseGrade, int courseCredit) {
        if (index >= 0 && index < numberOfCourses) {
            course[index].setCourseInfo(courseName, courseNo, courseGrade, courseCredit);
        }
    }

    // Yeni Fonksiyon: GPA Hesaplama
    double calculateGPA() {
        int totalCredits = 0;
        double totalPoints = 0.0;

        for (int i = 0; i < numberOfCourses; i++) {
            int credit = course[i].getCredit();
            char grade = course[i].getGrade();
            double gradePoint = 0.0;

            // Harf notlarını puanlara çevir
            switch (grade) {
                case 'A': gradePoint = 4.0; break;
                case 'B': gradePoint = 3.0; break;
                case 'C': gradePoint = 2.0; break;
                case 'D': gradePoint = 1.0; break;
                case 'F': gradePoint = 0.0; break;
                default: gradePoint = 0.0; break;
            }

            totalCredits += credit;
            totalPoints += gradePoint * credit;
        }

        return (totalCredits > 0) ? (totalPoints / totalCredits) : 0.0;
    }
    
    void printf(const string& fileName, StudentType*& students) {
    int numStudents, tuitionFeePerCredit;
    ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        cerr << "Dosya açılamadı: " << fileName << endl;
        return;
    }

    inputFile >> numStudents >> tuitionFeePerCredit;

    students = new StudentType[numStudents];

    string studentName, studentSurname;
    int studentId, numberOfCourses;
    char istuitionPaid;
    string courseName, courseNo;
    int courseCredit;
    char courseGrade;

    for (int i = 0; i < numStudents; i++) {
        inputFile >> studentName >> studentSurname >> studentId >> istuitionPaid >> numberOfCourses;

        students[i].setInfo(studentId, numberOfCourses, istuitionPaid, studentName, studentSurname);

        for (int j = 0; j < numberOfCourses; j++) {
            inputFile >> courseName >> courseNo >> courseCredit >> courseGrade;
            students[i].setCourseInfo(j, courseName, courseNo, courseGrade, courseCredit);
        }
    }

    inputFile.close();
}
};


int main() {
    int numStudents, tuitionFeePerCredit;
    StudentType* students = nullptr;

    students->printf("input.txt", students);

    if (!students) {
        cerr << "Öğrenci verileri yüklenemedi!" << endl;
        return 1;
    }

    ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        cerr << "Output dosyası oluşturulamadı!" << endl;
        delete[] students;
        return 1;
    }

    for (int i = 0; i < numStudents; i++) {
        students[i].print(cout);      
        students[i].print(outputFile);
        cout << endl;
        outputFile << endl;
    }

    outputFile.close();

    delete[] students;

    return 0;
}
