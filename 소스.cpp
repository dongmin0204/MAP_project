#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <Windows.h>

using namespace std;

class Subject { //���� Ŭ����
public:
    string name; //�����̸�
    string type; //����,���� ����
    int score; //������
    int semester; //�б�( 1 / 2 )
    float grade; //����
    int sbjrnak = 1; //���� ���� �ʱ�ȭ
    void Grading(); //���� ��� �Լ�
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
    string name; //�л��̸�
    Subject* sbj; //���� ��ü
    int sbjSize; //���� ũ��
    float totalGrade_1 = 0; //1�б� ����
    float totalGrade_2 = 0; //2�б� ����
    int rank_1 = 1; //1�б� ���� �ʱ�ȭ
    int rank_2 = 1; //2�б� ���� �ʱ�ȭ
};

void printMenu(void) {
    cout << "------------------------------" << endl;
    cout << "1. ���� �Է�" << endl;
    cout << "2. ���� ���� ���" << endl;
    cout << "3. ��ü ���� ���" << endl;
    cout << "4. �б� �� ��� ���" << endl;
    cout << "5. ���� ���� 3�� ���" << endl;
    cout << "6. ����" << endl;
    cout << "------------------------------" << endl;
}

void addfstudent(Student student[15], bool& is_exist) {
    vector<vector<string>> content;
    vector<string> row;
    string line, word;

    //���� �ҷ�����
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
        cout << "������ ã�� ���߽��ϴ�..." << endl;
        is_exist = false;
        return;
    }

    //�ҷ��� ���� ������ student, subject ��ü�� �Ҵ��ϱ�
    for (int i = 1; i < content.size(); i++) {
        student[i - 1].name = content[i][1];
        student[i - 1].sbjSize = content[i].size() - 2;
        student[i - 1].sbj = new Subject[content[i].size() - 2];

        for (int j = 2; j < content[i].size(); j++) { //����,���� ����
            if (content[0][j][0] != '_') {
                student[i - 1].sbj[j - 2].type = "����";
                student[i - 1].sbj[j - 2].name = content[0][j].substr(0, content[0][j].length() - 1);
                student[i - 1].sbj[j - 2].semester = stoi(content[0][j].substr(content[0][j].length() - 1, content[0][j].length())); //�б� ����
            }
            else {
                student[i - 1].sbj[j - 2].type = "����";
                student[i - 1].sbj[j - 2].name = content[0][j].substr(1, content[0][j].length() - 2);
                student[i - 1].sbj[j - 2].semester = stoi(content[0][j].substr(content[0][j].length() - 1, content[0][j].length())); //�б� ����
            }

            student[i - 1].sbj[j - 2].score = stoi(content[i][j]); //������ �Ҵ�
            student[i - 1].sbj[j - 2].Grading(); //�������� ��ȯ

            if (student[i - 1].sbj[j - 2].semester == 1) //1�б�
                student[i - 1].totalGrade_1 += student[i - 1].sbj[j - 2].grade;
            else if (student[i - 1].sbj[j - 2].semester == 2) //2�б�
                student[i - 1].totalGrade_2 += student[i - 1].sbj[j - 2].grade;
        }
    }

    for (int i = 0; i < 14; i++) { //�������ϱ�
        for (int j = i; j < 15; j++) {
            //1�б�
            if (student[i].totalGrade_1 == student[j].totalGrade_1) {
                continue;
            }
            else if (student[i].totalGrade_1 < student[j].totalGrade_1) {
                student[i].rank_1 += 1;
            }
            else {
                student[j].rank_1 += 1;
            }
            //2�б�
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

    file.close();
    is_exist = true;
    cout << "���� �б� �Ϸ� ... " << endl;
}
void pStudentSwap(Student* &a, Student* &b) {
    Student* &temp = a;
    a = b; 
    b = temp;
}
//���ĵ� Student �迭�� ��ȯ�ϴ� �Լ�
Student** sortStudent(Student* student, int semester) {
    Student** temp;
    Student* Swaptemp;
    temp = new Student*[15]; //������ �迭 ���� �Ҵ�

    //temp�� student ��ü ������ ����
    for (int i = 0; i < 15; i++) {
        temp[i] = (student + i);
    }

    //temp�� student ��ü�� ������ ���� ����
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


int main() {
    Student student[15];
    bool is_file_exist = false;
    int cmd = 0; //�޴� ���� ����
    int choicesbj = 0; //���� ���� ����
    int namecheck = 0; //�̸� Ȯ�� ����
    string studentName;

    //�鵿�� �߰��� ����� ���� 
    int semester_size = 0;
    Student** temp;

    do {
        printMenu();
        cout << "�޴� ���� >> ";
        cin >> cmd;
        switch (cmd) {

        case 1:
            cout << "1. ���� �Է�" << endl;

            cout << "�л� ����� �ҷ����� ���Դϴ�... " << endl;
            addfstudent(student, is_file_exist);
            cout << endl;
            break;

        case 2:
            cout << "2. ���� ���� ���" << endl;

            if (!(is_file_exist)) {
                cout << "�л� ����� �������� �ʽ��ϴ�..." << endl;
                continue;
            }

            do {
                cout << "------------------------------" << endl;
                cout << "1. ��ü ���� ����" << endl;
                cout << "2. ���� ���� ����" << endl;
                cout << "3. ��ü ���� ���" << endl;
                cout << "------------------------------" << endl;
                cout << ">> ";
                cin >> choicesbj;
                if (choicesbj != 1 && choicesbj != 2 && choicesbj != 3)
                    cout << "�Է� ���� ! �ٽ� �Է����ּ���..." << endl;
            } while (choicesbj != 1 && choicesbj != 2 && choicesbj != 3);

            if (choicesbj == 1) {
                do {
                    cout << "�л� �̸��� �Է����ּ��� >> ";
                    cin >> studentName;
                    for (int i = 0; i < 15; i++) {
                        if (student[i].name == studentName) {
                            namecheck = 1;
                            break;
                        }
                    }
                    if (namecheck == 0) {
                        cout << "�ٽ� �Է����ּ��� ..." << endl;
                    }
                } while (namecheck == 0);

                if (namecheck == 1) {
                    cout << "����:\t";
                    for (int i = 0; i < student[0].sbjSize; i++) {
                        cout << student[0].sbj[i].name << "\t";
                    }
                    cout << endl;
                    cout << "����:\t";
                    for (int i = 0; i < student[0].sbjSize; i++) {
                        cout << student[0].sbj[i].type << "\t";
                    }
                    cout << endl;
                    cout << "�б�:\t";
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
                    cout << "�л� �̸��� �Է����ּ��� >> ";
                    cin >> studentName;
                    for (int i = 0; i < 15; i++) {
                        if (student[i].name == studentName) {
                            namecheck = 1;
                            break;
                        }
                    }
                    if (namecheck == 0) {
                        cout << "�ٽ� �Է����ּ��� ..." << endl;
                    }
                } while (namecheck == 0);

                if (namecheck == 1) {
                    cout << "����:\t";
                    for (int i = 0; i < student[0].sbjSize; i++) {
                        if (student[0].sbj[i].type == "����")
                            cout << student[0].sbj[i].name << "\t";
                    }
                    cout << endl;
                    cout << "�б�:\t";
                    for (int i = 0; i < student[0].sbjSize; i++) {
                        if (student[0].sbj[i].type == "����")
                            cout << student[0].sbj[i].semester << "\t";
                    }
                    cout << endl;

                    for (int i = 0; i < 15; i++) {
                        if (student[i].name == studentName) {
                            cout << student[i].name << ": ";
                            for (int j = 0; j < student[i].sbjSize; j++) {
                                if (student[i].sbj[j].type == "����")
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
                    cout << "�л� �̸��� �Է����ּ��� >> ";
                    cin >> studentName;
                    for (int i = 0; i < 15; i++) {
                        if (student[i].name == studentName) {
                            namecheck = 1;
                            break;
                        }
                    }
                    if (namecheck == 0) {
                        cout << "�ٽ� �Է����ּ��� ..." << endl;
                    }
                } while (namecheck == 0);

                if (namecheck == 1) {
                    cout << "����:\t";
                    for (int i = 0; i < student[0].sbjSize; i++) {
                        cout << student[0].sbj[i].name << "\t";
                    }
                    cout << "1�б�\t2�б�\t������" << endl;
                    cout << "����:\t";
                    for (int i = 0; i < student[0].sbjSize; i++) {
                        cout << student[0].sbj[i].type << "\t";
                    }
                    cout << endl;
                    cout << "�б�:\t";
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
            cout << "3. ��ü ���� ���" << endl;

            if (!(is_file_exist)) {
                cout << "�л� ����� �������� �ʽ��ϴ�..." << endl;
                continue;
            }

            do {
                cout << "------------------------------" << endl;
                cout << "1. ��ü ���� ����" << endl;
                cout << "2. ���� ���� ����" << endl;
                cout << "------------------------------" << endl;
                cout << ">> ";
                cin >> choicesbj;
                if (choicesbj != 1 && choicesbj != 2)
                    cout << "�Է� ���� ! �ٽ� �Է����ּ���..." << endl;
            } while (choicesbj != 1 && choicesbj != 2);

            if (choicesbj == 1) {
                cout << "����:\t";
                for (int i = 0; i < student[0].sbjSize; i++) {
                    cout << student[0].sbj[i].name << "\t";
                }
                cout << endl;
                cout << "����:\t";
                for (int i = 0; i < student[0].sbjSize; i++) {
                    cout << student[0].sbj[i].type << "\t";
                }
                cout << endl;
                cout << "�б�:\t";
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
                cout << "����:\t";
                for (int i = 0; i < student[0].sbjSize; i++) {
                    if (student[0].sbj[i].type == "����")
                        cout << student[0].sbj[i].name << "\t";
                }
                cout << endl;
                cout << "�б�:\t";
                for (int i = 0; i < student[0].sbjSize; i++) {
                    if (student[0].sbj[i].type == "����")
                        cout << student[0].sbj[i].semester << "\t";
                }
                cout << endl;

                for (int i = 0; i < 15; i++) {
                    cout << student[i].name << ": ";
                    for (int j = 0; j < student[i].sbjSize; j++) {
                        if (student[i].sbj[j].type == "����")
                            cout << student[i].sbj[j].sbjrnak << "\t";
                    }
                    cout << endl;
                }
                cout << endl;
            }
            break;

        case 4:
            cout << "4. �б� �� ��� ���" << endl;

            if (!(is_file_exist)) {
                cout << "�л� ����� �������� �ʽ��ϴ�..." << endl;
                continue;
            }

            int semester;
            do {
                cout << "�б⸦ �Է����ּ���(1 or 2) >> ";
                cin >> semester;
                if (semester != 1 && semester != 2) { cout << "�ٽ� �Է����ּ���... " << endl; }
            } while (semester != 1 && semester != 2);

            cout << "�̸�\t";
            for (int i = 0; i < student[0].sbjSize; i++) {
                if (student[0].sbj[i].semester == semester)
                {
                    cout << student[0].sbj[i].name << "\t";
                    semester_size++;
                }
                else
                    continue;
            }
            cout << "���" << endl;

            temp = sortStudent(student, semester); //�б�� ����� ��ŭ �Ҵ��ؼ� �����ϱ� ���� ������ �迭 ����
            
            
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
        case 5: //�̿�
            cout << "5. ���� ���� 3�� ���" << endl;

            if (!(is_file_exist)) {
                cout << "�л� ����� �������� �ʽ��ϴ�..." << endl;
                continue;
            }


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
        case 6:
            break;
        default:
            cout << "�Է� ���� ! �ٽ� �Է����ּ���..." << endl;
            break;
        }
    } while (cmd != 6);
    cout << "���α׷��� �����մϴ�." << endl;

    return 0;
}
