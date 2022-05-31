#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <Windows.h>

using namespace std;

class Subject {
public:
    string name;
    string type; //����,���� ����
    int score; //������
    int semester; //�б�( 1 / 2 )
    string grade; //����
    void Grading();
    float getGrade() {
        if (grade == "F") return 0.0;

        else {
            float temp = float(69 - grade[0]);//4,3,2,1�� ����
            if (grade[1] == '+') temp += 0.5;//n.5�� or n.0�� ����
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
    int rank;

    Student() {
        rank = 1;
    }
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

void addfstudent(Student student[15],bool& is_exist) {
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
        cout << "������ ã�� ���߽��ϴ�..." << endl; 
        is_exist = false;
        return;
    }

    do {
        cout << "�б⸦ �Է����ּ���(1 or 2) >> ";
        cin >> semester;
        if (semester != 1 && semester != 2) { cout << "�ٽ� �Է����ּ���... " << endl; }
    } while (semester != 1 && semester != 2);

    for (int i = 1; i < content.size(); i++)
    {
        student[i - 1].name = content[i][1];
        student[i - 1].sbjSize = content[i].size() - 2;
        student[i - 1].sbj = new Subject[content[i].size() - 2];

        for (int j = 2; j < content[i].size(); j++) {
            if (content[0][j][0] != '_') {
                student[i - 1].sbj[j - 2].type = "����";
                student[i - 1].sbj[j - 2].name = content[0][j];
            }
            else {
                student[i - 1].sbj[j - 2].type = "����";
                temp = content[0][j].substr(1, content[0][j].length() - 1);
                student[i - 1].sbj[j - 2].name = temp;
            }

            student[i - 1].sbj[j - 2].semester = semester;
            student[i - 1].sbj[j - 2].score = stoi(content[i][j]);
            student[i - 1].sbj[j - 2].Grading();
            student[i - 1].totalGrade += student[i - 1].sbj[j - 2].getGrade();
        }
    }

    for (int i = 0; i < 14; i++) //���� ���ϱ�
    {
        for (int j = i; j < 15; j++)
        {
            if (student[i].totalGrade == student[j].totalGrade)
            {
                continue;
            }
            else if (student[i].totalGrade < student[j].totalGrade)
                student[i].rank += 1;
            else
                student[j].rank += 1;
        }
    }

    file.close();
    is_exist = true;
    cout << "���� �б� �Ϸ� ... " << endl;
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

    Student* heap[16]; //����� �������� �����ϰ� ����� �ּ� ��
    int heap_size = 0;

    bool is_file_exist = false;

    int cmd = 0;
    string studentName;


    do {
        printMenu();
        cout << "�޴� ���� >> ";
        cin >> cmd;
        switch (cmd) {
        case 1:
            cout << "1. ���� �Է�" << endl;
            cout << "�л� ����� �ҷ����� ���Դϴ�... " << endl;

            addfstudent(student, is_file_exist);

            break;
        case 2:
            cout << "2. ���� ���� ���" << endl;

            if (!(is_file_exist)) { 
                cout << "�л� ����� �������� �ʽ��ϴ�..." << endl; 
                continue; 
            }

            cout << "�л� �̸��� �Է����ּ��� >> ";
            cin >> studentName;

            cout << "�̸�  ";
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
            cout << "3. ��ü ���� ���" << endl;

            if (!(is_file_exist)) { 
                cout << "�л� ����� �������� �ʽ��ϴ�..." << endl; 
                continue; 
            }

            cout << "�̸�  ";
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

            if (!(is_file_exist)) { 
                cout << "�л� ����� �������� �ʽ��ϴ�..." << endl; 
                continue; 
            }

            for (int i = 0; i < 15; i++) {
                insertHeap(heap, student+i, heap_size);
            }

            cout << "�̸�  ";
            for (int i = 0; i < heap[1]->sbjSize; i++) {
                cout << heap[1]->sbj[i].name << "          ";
            }
            cout << "���" << endl;
            for (int i = 1; i < 16; i++) {
                cout << heap[i]->name << " : ";
                for (int j = 0; j < heap[i]->sbjSize; j++) {
                    cout << heap[i]->sbj[j].score << "\t\t";
                }
                cout << heap[i]->rank << endl;
            }
            cout << endl;

            break;
        case 5:
            cout << "5. ���� ���� 3�� ���" << endl;

            if (!(is_file_exist)) { 
                cout << "�л� ����� �������� �ʽ��ϴ�..." << endl; 
                continue; 
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