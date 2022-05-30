#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

class Subject {
public:
    string name;
    string type; //전공,교양 구분
    int score; //원점수
    int semester; //학기( 1 / 2 )
    string grade; //학점
    void Grading();
    float getGrade() {
        if (grade == "F") return 0.0;

        else {
            float temp = 69 - grade[0];//4,3,2,1을 결정
            if (grade[1] == '+') temp += 0.5;//n.5점 or n.0점 결정
            return temp;
        }
    }
};

void Subject::Grading() {
    if (score >= 95)
        grade = "A+"; //4.5
    else if (score >= 90)
        grade = "A0"; //4.0
    else if (score >= 85)
        grade = "B+"; //3.5
    else if (score >= 80)
        grade = "B0"; //3.0
    else if (score >= 75)
        grade = "C+"; //2.5
    else if (score >= 70)
        grade = "C0"; //2.0
    else if (score >= 65)
        grade = "D+"; //1.0
    else if (score >= 60)
        grade = "D0"; //0.5
    else
        grade = "F"; // 0.0
}

class Student {
public:
    string name;

    Subject* sbj;
    int sbjSize;
    float totalGrade;
    int rank = 1;
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

    string temp;
    int semester;

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
    {
        cout << "파일을 찾지 못했습니다..." << endl;
    }

    do {
        cout << "학기를 입력해주세요(1 or 2) >> ";
        cin >> semester;
        if (semester != 1 && semester != 2) { cout << "다시 입력해주세요... " << endl; }
    } while (semester != 1 && semester != 2);

    for (int i = 1; i < content.size(); i++)
    {
        student[i - 1].name = content[i][1];
        student[i - 1].sbjSize = content[i].size() - 2;
        student[i - 1].sbj = new Subject[content[i].size() - 2];

        for (int j = 2; j < content[i].size(); j++) {
            if (content[0][j][0] != '_') { 
                student[i - 1].sbj[j - 2].type = "교양"; 
                student[i - 1].sbj[j - 2].name = content[0][j];
            }
            else {
                student[i - 1].sbj[j - 2].type = "전공"; 
                temp = content[0][j].substr(1, content[0][j].length() - 1);
                student[i - 1].sbj[j - 2].name = temp;
            }
            
            student[i - 1].sbj[j - 2].semester = semester;
            student[i - 1].sbj[j - 2].score = stoi(content[i][j]);
            student[i - 1].sbj[j - 2].Grading();
            student[i - 1].totalGrade += student[i - 1].sbj[j - 2].getGrade();
        }
    }
    file.close();
    cout << "파일 읽기 완료 ... " << endl;
}

void getRank(Student student[15]) {

    for (int i = 0; i < 14; i++) //석차 구하기
    {
        for (int j = i; j < 15; j++)
        {
            if (student[i].totalGrade == student[j].totalGrade)
            {
                continue;
            }
            else if (student[i].totalGrade < student[j].totalGrade)
                student[i].rank++;
            else
                student[j].rank++;
        }
    }
    return;
}

void insertHeap(Student** h, Student* student, int& size) {
    int i;
    i = ++size;
    while ((i != 1) && (student->rank < h[i / 2]->rank)) {
        h[i] = h[i / 2];
        i /= 2;
    }
    h[i] = student;
}

Student* deleteHeap(Student** h, int& size) {
    int parent, child;
    Student* item, * temp;

    item = h[1];
    temp = h[--size];
    parent = 1; child = 2;

    while (child <= size) {
        if ((child < size) && (h[child]->rank > h[child + 1]->rank)) child++;

        if (temp->rank <= h[child]->rank) { break; }
        else {
            h[parent] = h[child];
            parent = child;
            child = child * 2;
        }
    }
    h[parent] = temp;
    return item;
}

int main() {
    Student student[15];

    Student *heap[16]; //등수를 기준으로 정렬하고 출력할 최소 힙
    int heap_size = 0;

    int cmd = 0;
    string studentName; 
    

    do {
        printMenu();
        cout << "메뉴 선택 >> ";
        cin >> cmd;
        switch (cmd) {
        case 1:
            cout << "1. 성적 입력" << endl;
            cout << "학생 명단을 불러오는 중입니다... " << endl;

            addfstudent(student);

            break;
        case 2:
            cout << "2. 개인 성적 출력" << endl;

            cout << "학생 이름을 입력해주세요 >> ";
            cin >> studentName;

            cout << "이름  ";
            for (int i = 0; i < student[0].sbjSize; i++) {
                cout << student[0].sbj[i].name << "          ";
            }
            cout << endl;
            for (int i = 0; i < 15; i++) {
                if (student[i].name == studentName) {
                    cout << student[i].name << " : ";
                    for (int j = 0; j < student[i].sbjSize; j++) {
                        cout << student[i].sbj[j].score << "\t\t";
                    }
                    cout << endl;
                }
            }
            cout << endl;
            break;
        case 3:
            cout << "3. 전체 성적 출력" << endl;

            cout << "이름  ";
            for (int i = 0; i < student[0].sbjSize; i++) {
                cout << student[0].sbj[i].name << "          ";
            }
            cout << endl;
            for (int i = 0; i < 15; i++) {
                cout << student[i].name << " : ";
                for (int j = 0; j < student[i].sbjSize; j++) {
                    cout << student[i].sbj[j].score << "\t\t";
                }
                cout << endl;
            }
            cout << endl;
            break;
        case 4:
            cout << "4. 학기 별 등수 출력" << endl;

            for (int i = 0; i < 15; i++) {

            }
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
}
