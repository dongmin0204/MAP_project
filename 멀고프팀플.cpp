#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Subject {
public:
    string name;
    string type; //전공,교양 구분
    int score; //원점수
    int semester; //학기
    char grade; //학점
    void Grading();
};

void Subject::Grading() {
    if (score >= 90)
        grade = 'A';
    else if (score < 90 && score >= 80)
        grade = 'B';
    else if (score < 80 && score >= 70)
        grade = 'C';
    else if (score < 70 && score >= 60)
        grade = 'D';
    else
        grade = 'F';
}

class Student {
public:
    string name;
    Subject* sbj;
    int sbjSize;
};

void printMenu(void) {
    cout << "------------------------------" << endl;
    cout << "1. 성적 입력" << endl;
    cout << "2. 개인 성적 출력" << endl;
    cout << "3. 전체 성적 출력" << endl;
    cout << "4. 학기 별 등수 출력" << endl;
    cout << "5. 과목별 상위 3명 출력" << endl;
    cout << "6. 종료" << endl;
    cout << "------------------------------" << endl;
}

void addfstudent(Student student[15]) {
    vector<vector<string>> content;
    vector<string> row;
    string line, word;

    fstream file("name.CSV", ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            row.clear();

            stringstream str(line);

            while (getline(str, word, ','))
                row.push_back(word);
            content.push_back(row);
        }
    }
    else
        cout << "파일을 찾지 못했습니다..." << endl;

    for (int i = 1; i < content.size(); i++)
    {
        student[i - 1].name = content[i][1];
        student[i - 1].sbjSize = content[i].size() - 2;
        student[i - 1].sbj = new Subject[content[i].size() - 2];
        
        for (int j = 2; j < content[i].size(); j++) {
            student[i - 1].sbj[j-2].name = content[0][j];
            student[i - 1].sbj[j-2].score = stoi(content[i][j]);
        }
    }
    file.close();
}

int main() {
    Student student[15];
    int cmd = 0;

    do {
        printMenu();
        cout << "메뉴 선택 >> ";
        cin >> cmd;
        switch (cmd) {
        case 1:
            cout << "1. 성적 입력" << endl;
            cout << "학생 명단을 불러오는 중입니다... " << endl;
            addfstudent(student);

            cout << "이름  ";

            for (int i = 0; i < student[0].sbjSize; i++) {
                cout << student[0].sbj[i].name << " ";
            }
            cout << endl;

            for (int i = 0; i < 15; i++) { 
                cout << student[i].name << " : ";
                for (int j = 0; j < student[i].sbjSize; j++) {
                    cout << student[i].sbj[j].score << "  ";
                }
                cout << endl;
            }
            cout << endl;
            break;
        case 2:
            cout << "2. 개인 성적 출력" << endl;
            break;
        case 3:
            cout << "3. 전체 성적 출력" << endl;
            break;
        case 4:
            cout << "4. 학기 별 등수 출력" << endl;
            break;
        case 5:
            cout << "5. 과목별 상위 3명 출력" << endl;
            break;
        case 6:
            break;
        default:
            cout << "입력 오류 ! 다시 입력해주세요..." << endl;
            break;
        }
    } while (cmd != 6);
    cout << "프로그램을 종료합니다." << endl;

    return 0;
