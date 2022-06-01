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
    int sbjrnak = 1; //과목 석차 초기화
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
    if (file.is_open()) {
        while (getline(file, line)) {
            row.clear();

            stringstream str(line);

            while (getline(str, word, ','))
                row.push_back(word);
            content.push_back(row);
        }
    }
    else {
        cout << "파일을 찾지 못했습니다..." << endl;
        is_exist = false;
        return;
    }

    //불러온 파일 내용을 student, subject 객체에 할당하기
    for (int i = 1; i < content.size(); i++) {
        student[i - 1].name = content[i][1];
        student[i - 1].sbjSize = content[i].size() - 2;
        student[i - 1].sbj = new Subject[content[i].size() - 2];

        for (int j = 2; j < content[i].size(); j++) { //전공,교양 구분
            if (content[0][j][0] != '_') {
                student[i - 1].sbj[j - 2].type = "교양";
                student[i - 1].sbj[j - 2].name = content[0][j].substr(0, content[0][j].length() - 1);
                student[i - 1].sbj[j - 2].semester = stoi(content[0][j].substr(content[0][j].length() - 1, content[0][j].length())); //학기 구분
            }
            else {
                student[i - 1].sbj[j - 2].type = "전공";
                student[i - 1].sbj[j - 2].name = content[0][j].substr(1, content[0][j].length() - 2);
                student[i - 1].sbj[j - 2].semester = stoi(content[0][j].substr(content[0][j].length() - 1, content[0][j].length())); //학기 구분
            }

            student[i - 1].sbj[j - 2].score = stoi(content[i][j]); //원점수 할당
            student[i - 1].sbj[j - 2].Grading(); //학점으로 변환

            if (student[i - 1].sbj[j - 2].semester == 1) //1학기
                student[i - 1].totalGrade_1 += student[i - 1].sbj[j - 2].grade;
            else if (student[i - 1].sbj[j - 2].semester == 2) //2학기
                student[i - 1].totalGrade_2 += student[i - 1].sbj[j - 2].grade;
        }
    }

    for (int i = 0; i < 14; i++) { //석차구하기
        for (int j = i; j < 15; j++) {
            //1학기
            if (student[i].totalGrade_1 == student[j].totalGrade_1) {
                continue;
            }
            else if (student[i].totalGrade_1 < student[j].totalGrade_1) {
                student[i].rank_1 += 1;
            }
            else {
                student[j].rank_1 += 1;
            }
            //2학기
            if (student[i].totalGrade_2 == student[j].totalGrade_2) {
                continue;
            }
            else if (student[i].totalGrade_2 < student[j].totalGrade_2) {
                student[i].rank_2 += 1;
            }
            else {
                student[j].rank_2 += 1;
            }
        }
    }

    for (int i = 0; i < student[0].sbjSize; i++) { //과목별 석차 계산
        for (int j = 0; j < 14; j++) {
            for (int k = j; k < 15; k++) {
                if (student[j].sbj[i].score == student[k].sbj[i].score) {
                    continue;
                }
                else if (student[j].sbj[i].score < student[k].sbj[i].score) {
                    student[j].sbj[i].sbjrnak += 1;
                }
                else {
                    student[k].sbj[i].sbjrnak += 1;
                }
            }
        }
    }

    file.close();
    is_exist = true;
    cout << "파일 읽기 완료 ... " << endl;
}

//정렬된 Student 배열을 반환하는 함수
Student** sortStudent(Student* student, int semester) {
    Student** temp;
    Student* Swaptemp;
    temp = new Student * [15]; //포인터 배열 동적 할당

    //temp에 student 객체 포인터 삽입
    for (int i = 0; i < 15; i++) {
        temp[i] = (student + i);
    }

    //temp에 student 객체를 순위에 따라 정렬
    for (int i = 14; i > 0; i--) {
        for (int j = 0; j < i; j++) {
            if (semester == 1) {
                if (temp[j]->rank_1 > temp[j + 1]->rank_1) {
                    Swaptemp = temp[j];
                    temp[j] = temp[j + 1];
                    temp[j + 1] = Swaptemp;
                }
            }
            else {
                if (temp[j]->rank_2 > temp[j + 1]->rank_2) {
                    Swaptemp = temp[j];
                    temp[j] = temp[j + 1];
                    temp[j + 1] = Swaptemp;
                }
            }
        }
    }
    return temp;
}

Student** sortStudent(Student* student, string sbjName, int& index) {
    Student** temp;
    Student* Swaptemp;
    temp = new Student* [15]; //포인터 배열 동적 할당

    //temp에 student 객체 포인터 삽입
    for (int i = 0; i < 15; i++) {
        temp[i] = (student + i);
    }

    //원하는 과목 인덱스 찾기
    for (int i = 0; i < temp[0]->sbjSize; i++) {
        if (temp[0]->sbj[i].name == sbjName) {
            index = i;
            break;
        }
    }

    //temp에 student 객체를 순위에 따라 정렬
    for (int i = 14; i > 0; i--) {
        for (int j = 0; j < i; j++) {
            if (temp[j]->sbj[index].sbjrnak > temp[j + 1]->sbj[index].sbjrnak) {
                Swaptemp = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = Swaptemp;
            }
        }
    }
    return temp;

}


int main() {
    Student student[15];
    bool is_file_exist = false;
    int cmd = 0; //메뉴 선택 변수
    int choicesbj = 0; //과목 선택 변수
    int namecheck = 0; //이름 확인 변수
    string studentName;
    string sbjName;

    //백동민 추가로 사용한 변수 
    Student** temp;
    int index;

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
                cout << "------------------------------" << endl;
                cout << "1. 전체 과목 석차" << endl;
                cout << "2. 전공 과목 석차" << endl;
                cout << "3. 전체 성적 출력" << endl;
                cout << "------------------------------" << endl;
                cout << ">> ";
                cin >> choicesbj;
                if (choicesbj != 1 && choicesbj != 2 && choicesbj != 3)
                    cout << "입력 오류 ! 다시 입력해주세요..." << endl;
            } while (choicesbj != 1 && choicesbj != 2 && choicesbj != 3);

            if (choicesbj == 1) {
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

                if (namecheck == 1) {
                    cout << "과목:\t";
                    for (int i = 0; i < student[0].sbjSize; i++) {
                        cout << student[0].sbj[i].name << "\t";
                    }
                    cout << endl;
                    cout << "구분:\t";
                    for (int i = 0; i < student[0].sbjSize; i++) {
                        cout << student[0].sbj[i].type << "\t";
                    }
                    cout << endl;
                    cout << "학기:\t";
                    for (int i = 0; i < student[0].sbjSize; i++) {
                        cout << student[0].sbj[i].semester << "\t";
                    }
                    cout << endl;

                    for (int i = 0; i < 15; i++) {
                        if (student[i].name == studentName) {
                            cout << student[i].name << ": ";
                            for (int j = 0; j < student[i].sbjSize; j++) {
                                cout << student[i].sbj[j].sbjrnak << "\t";
                            }
                        }
                    }
                    cout << endl;
                }
                cout << endl;
            }

            else if (choicesbj == 2) {
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

                if (namecheck == 1) {
                    cout << "과목:\t";
                    for (int i = 0; i < student[0].sbjSize; i++) {
                        if (student[0].sbj[i].type == "전공")
                            cout << student[0].sbj[i].name << "\t";
                    }
                    cout << endl;
                    cout << "학기:\t";
                    for (int i = 0; i < student[0].sbjSize; i++) {
                        if (student[0].sbj[i].type == "전공")
                            cout << student[0].sbj[i].semester << "\t";
                    }
                    cout << endl;

                    for (int i = 0; i < 15; i++) {
                        if (student[i].name == studentName) {
                            cout << student[i].name << ": ";
                            for (int j = 0; j < student[i].sbjSize; j++) {
                                if (student[i].sbj[j].type == "전공")
                                    cout << student[i].sbj[j].sbjrnak << "\t";
                            }
                        }
                    }
                    cout << endl;
                }
                cout << endl;
            }

            else if (choicesbj == 3) {
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

                if (namecheck == 1) {
                    cout << "과목:\t";
                    for (int i = 0; i < student[0].sbjSize; i++) {
                        cout << student[0].sbj[i].name << "\t";
                    }
                    cout << "1학기\t2학기\t총평점" << endl;
                    cout << "구분:\t";
                    for (int i = 0; i < student[0].sbjSize; i++) {
                        cout << student[0].sbj[i].type << "\t";
                    }
                    cout << endl;
                    cout << "학기:\t";
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
                            cout << (student[i].totalGrade_1) / 5.0 << "\t" << (student[i].totalGrade_2) / 5.0 << "\t";
                            cout << (student[i].totalGrade_1 + student[i].totalGrade_2) / 10.0 << endl;
                        }
                    }
                    cout << endl;
                }
            }

            break;

        case 3:
            cout << "3. 전체 성적 출력" << endl;

            if (!(is_file_exist)) {
                cout << "학생 명단이 존재하지 않습니다..." << endl;
                continue;
            }

            do {
                cout << "------------------------------" << endl;
                cout << "1. 전체 과목 석차" << endl;
                cout << "2. 전공 과목 석차" << endl;
                cout << "------------------------------" << endl;
                cout << ">> ";
                cin >> choicesbj;
                if (choicesbj != 1 && choicesbj != 2)
                    cout << "입력 오류 ! 다시 입력해주세요..." << endl;
            } while (choicesbj != 1 && choicesbj != 2);

            if (choicesbj == 1) {
                cout << "과목:\t";
                for (int i = 0; i < student[0].sbjSize; i++) {
                    cout << student[0].sbj[i].name << "\t";
                }
                cout << endl;
                cout << "구분:\t";
                for (int i = 0; i < student[0].sbjSize; i++) {
                    cout << student[0].sbj[i].type << "\t";
                }
                cout << endl;
                cout << "학기:\t";
                for (int i = 0; i < student[0].sbjSize; i++) {
                    cout << student[0].sbj[i].semester << "\t";
                }
                cout << endl;

                for (int i = 0; i < 15; i++) {
                    cout << student[i].name << ": ";
                    for (int j = 0; j < student[i].sbjSize; j++) {
                        cout << student[i].sbj[j].sbjrnak << "\t";
                    }
                    cout << endl;
                }
                cout << endl;
            }

            else if (choicesbj == 2) {
                cout << "과목:\t";
                for (int i = 0; i < student[0].sbjSize; i++) {
                    if (student[0].sbj[i].type == "전공")
                        cout << student[0].sbj[i].name << "\t";
                }
                cout << endl;
                cout << "학기:\t";
                for (int i = 0; i < student[0].sbjSize; i++) {
                    if (student[0].sbj[i].type == "전공")
                        cout << student[0].sbj[i].semester << "\t";
                }
                cout << endl;

                for (int i = 0; i < 15; i++) {
                    cout << student[i].name << ": ";
                    for (int j = 0; j < student[i].sbjSize; j++) {
                        if (student[i].sbj[j].type == "전공")
                            cout << student[i].sbj[j].sbjrnak << "\t";
                    }
                    cout << endl;
                }
                cout << endl;
            }
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
                {
                    cout << student[0].sbj[i].name << "\t";
                }
                else
                    continue;
            }
            cout << "등수" << endl;

            temp = sortStudent(student, semester); //학기당 과목수 만큼 할당해서 정렬하기 위한 포인터 배열 생성


            for (int i = 0; i < 15; i++) {
                cout << temp[i]->name << ": ";
                for (int j = 0; j < temp[i]->sbjSize; j++) {
                    if (temp[i]->sbj[j].semester == semester)
                        cout << temp[i]->sbj[j].score << "\t";
                    else
                        continue;
                }
                if (semester == 1)
                    cout << temp[i]->rank_1 << endl;
                else if (semester == 2)
                    cout << temp[i]->rank_2 << endl;
            }

            break;
        case 5: //미완
            cout << "5. 과목별 상위 3명 출력" << endl;

            if (!(is_file_exist)) {
                cout << "학생 명단이 존재하지 않습니다..." << endl;
                continue;
            }
            
            namecheck = 0;
            do {
                cout << "과목 이름을 입력해주세요 >> ";
                cin >> sbjName;
                for (int i = 0; i < student[0].sbjSize; i++) {
                    if (student[0].sbj[i].name == sbjName) {
                        namecheck = 1;
                        break;
                    }
                }
                if (namecheck == 0) {
                    cout << "다시 입력해주세요 ..." << endl;
                }
            } while (namecheck == 0);

            temp = sortStudent(student, sbjName, index);

            cout << "이름\t" << sbjName << "\t등수" << endl;

            for (int i = 0; i < 3; i++) {
                cout << temp[i]->name << ": ";
                cout << temp[i]->sbj[index].score << "\t";
                cout << temp[i]->sbj[index].sbjrnak << endl;
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
