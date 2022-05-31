#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <Windows.h>

using namespace std;

class Subject { //과목 클래스
public:
    string name; //과목이름
    string type; //전공,교양 구분
    int score; //원점수
    int semester; //학기( 1 / 2 )
    float grade; //학점
    void Grading(); //학점 계산 함수
};

void Subject::Grading() {
    if (score >= 95)
        grade = 4.5; //A+
    else if (score >= 90)
        grade = 4.0; //A0
    else if (score >= 85)
        grade = 3.5; //B+
    else if (score >= 80)
        grade = 3.0; //B0
    else if (score >= 75)
        grade = 2.5; //C+
    else if (score >= 70)
        grade = 2.0; //C0;
    else if (score >= 65)
        grade = 1.5; //D+
    else if (score >= 60)
        grade = 1.0; //D0
    else
        grade = 0.0; //F
}

class Student {
public:
    string name; //학생이름
    Subject* sbj; //과목 객체
    int sbjSize; //과목 크기
    float totalGrade_1 = 0; //1학기 학점
    float totalGrade_2 = 0; //2학기 학점
    int rank_1 = 1; //1학기 석차 초기화
    int rank_2 = 1; //2학기 석차 초기화
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

void addfstudent(Student student[15], bool& is_exist) {
    vector<vector<string>> content;
    vector<string> row;
    string line, word;

    //파일 불러오기
    fstream file("name.CSV", ios::in);
    if (file.is_open()){
        while (getline(file, line)){
            row.clear();

            stringstream str(line);

            while (getline(str, word, ','))
                row.push_back(word);
            content.push_back(row);
        }
    }
    else{
        cout << "파일을 찾지 못했습니다..." << endl;
        is_exist = false;
        return;
    }

    //불러온 파일 내용을 student, subject 객체에 할당하기
    for (int i = 1; i < content.size(); i++){
        student[i - 1].name = content[i][1];
        student[i - 1].sbjSize = content[i].size() - 2;
        student[i - 1].sbj = new Subject[content[i].size() - 2];

        for (int j = 2; j < content[i].size(); j++){ //전공,교양 구분
            if (content[0][j][0] != '_'){
                student[i - 1].sbj[j - 2].type = "교양";
                student[i - 1].sbj[j - 2].name = content[0][j].substr(0, content[0][j].length() - 1);
                student[i - 1].sbj[j - 2].semester = stoi(content[0][j].substr(content[0][j].length()-1,content[0][j].length())); //학기 구분
            }
            else {
                student[i - 1].sbj[j - 2].type = "전공";
                student[i - 1].sbj[j - 2].name = content[0][j].substr(1, content[0][j].length() - 2);
                student[i - 1].sbj[j - 2].semester = stoi(content[0][j].substr(content[0][j].length() - 1, content[0][j].length())); //학기 구분
            }

            student[i - 1].sbj[j - 2].score = stoi(content[i][j]); //원점수 할당
            student[i - 1].sbj[j - 2].Grading(); //학점으로 변환
    
            if(student[i - 1].sbj[j - 2].semester == 1) //1학기
                student[i - 1].totalGrade_1 += student[i - 1].sbj[j - 2].grade;
            else if(student[i - 1].sbj[j - 2].semester == 2) //2학기
                student[i - 1].totalGrade_2 += student[i - 1].sbj[j - 2].grade;
        }
    }

    for (int i = 0; i < 14; i++){ //석차구하기
        for (int j = i; j < 15; j++){
            //1학기
            if (student[i].totalGrade_1 == student[j].totalGrade_1){
                continue;
            }
            else if (student[i].totalGrade_1 < student[j].totalGrade_1){
                student[i].rank_1 += 1;
            }
            else{
                student[j].rank_1 += 1;
            }
            //2학기
            if (student[i].totalGrade_2 == student[j].totalGrade_2){
                continue;
            }
            else if (student[i].totalGrade_2 < student[j].totalGrade_2){
                student[i].rank_2 += 1;
            }
            else {
                student[j].rank_2 += 1;
            }
        }
    }

    file.close();
    is_exist = true;
    cout << "파일 읽기 완료 ... " << endl;
}

int main() {
    Student student[15];
    bool is_file_exist = false;
    int cmd = 0; //메뉴 선택 변수
    int namecheck = 0;
    string studentName;

    do {
        printMenu();
        cout << "메뉴 선택 >> ";
        cin >> cmd;
        switch (cmd) {

        case 1:
            cout << "1. 성적 입력" << endl;

            cout << "학생 명단을 불러오는 중입니다... " << endl;
            addfstudent(student, is_file_exist);
            cout << endl;
            break;

        case 2:
            cout << "2. 개인 성적 출력" << endl;

            if (!(is_file_exist)) {
                cout << "학생 명단이 존재하지 않습니다..." << endl;
                continue;
            }

            do {
                cout << "학생 이름을 입력해주세요 >> ";
                cin >> studentName;
                for (int i = 0; i < 15; i++) {
                    if (student[i].name == studentName) {
                        namecheck = 1;
                        break;
                    }
                }
                if (namecheck == 0) {
                    cout << "다시 입력해주세요 ..." << endl;
                }
            } while (namecheck == 0);

            cout << "과목:\t";
            for (int i = 0; i < student[0].sbjSize; i++) {
                cout << student[0].sbj[i].name << "\t";
            }
            cout << "1학기\t2학기" << endl;
            cout << "구분:\t";
            for (int i = 0; i < student[0].sbjSize; i++) {
                cout << student[0].sbj[i].type << "\t";
            }
            cout << endl;
            cout << "학기\t";
            for (int i = 0; i < student[0].sbjSize; i++) {
                cout << student[0].sbj[i].semester << "\t";
            }
            cout << endl;
            
            for (int i = 0; i < 15; i++) {
                if (student[i].name == studentName) {
                    cout << student[i].name << ": ";
                    for (int j = 0; j < student[i].sbjSize; j++) {
                        cout << student[i].sbj[j].score << "\t";
                    }
                    cout << (student[i].totalGrade_1) / 5.0 << "\t" << (student[i].totalGrade_2) / 5.0 << endl;
                }
            }
            cout << endl;
            break;

        case 3:
            cout << "3. 전체 성적 출력" << endl;

            if (!(is_file_exist)) {
                cout << "학생 명단이 존재하지 않습니다..." << endl;
                continue;
            }

            cout << "과목:\t";
            for (int i = 0; i < student[0].sbjSize; i++) {
                cout << student[0].sbj[i].name << "\t";
            }
            cout << "1학기\t2학기" << endl;
            cout << "구분:\t";
            for (int i = 0; i < student[0].sbjSize; i++) {
                cout << student[0].sbj[i].type << "\t";
            }
            cout << endl;
            cout << "학기\t";
            for (int i = 0; i < student[0].sbjSize; i++) {
                cout << student[0].sbj[i].semester << "\t";
            }
            cout << endl;

            for (int i = 0; i < 15; i++) {
                cout << student[i].name << ": ";
                for (int j = 0; j < student[i].sbjSize; j++) {
                    cout << student[i].sbj[j].score << "\t";
                }
                cout << (student[i].totalGrade_1)/5.0 << "\t" << (student[i].totalGrade_2)/5.0 << endl;
            }
            cout << endl;
            break;

        case 4:
            cout << "4. 학기 별 등수 출력" << endl;

            if (!(is_file_exist)) {
                cout << "학생 명단이 존재하지 않습니다..." << endl;
                continue;
            }

            int semester;
            do {
                cout << "학기를 입력해주세요(1 or 2) >> ";
                cin >> semester;
                if (semester != 1 && semester != 2) { cout << "다시 입력해주세요... " << endl; }
            } while (semester != 1 && semester != 2);

            cout << "이름\t";
            for (int i = 0; i < student[0].sbjSize; i++) {
                if (student[0].sbj[i].semester == semester)
                    cout << student[0].sbj[i].name << "\t";
                else
                    continue;
            }
            cout << "등수" << endl;
            for (int i = 0; i < 15; i++) {
                cout << student[i].name << ": ";
                for (int j = 0; j < student[i].sbjSize; j++) {
                    if (student[0].sbj[i].semester == semester)
                        cout << student[0].sbj[i].score << "\t";
                    else
                        continue;
                }
                if (semester == 1) { 
                    cout << student[i].rank_1;
                }
                else if (semester == 2) {
                    cout << student[i].rank_2;
                }
                cout << endl;
            }
            cout << endl;

            break;
        case 5: //미완
            cout << "5. 과목별 상위 3명 출력" << endl;

            if (!(is_file_exist)) {
                cout << "학생 명단이 존재하지 않습니다..." << endl;
                continue;
            }

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
