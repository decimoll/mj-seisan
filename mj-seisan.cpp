#include <bits/types/time_t.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

// 今の半荘の素点
std::vector<double> current_raw_scores(4);
// プレーヤ名
std::string names[4];
// 今までの全ての素点
std::vector<std::vector<int>> raw_scoress;
// 今までの全てのスコア
std::vector<std::vector<double>> scoress;
// 今までのスコアの総和
double total_scores[4] = {};
// 現在順位
int ranking[4];
// 現在料金
int fees[4];
int fee;
int umaoka[4] = {20+10, 5, -5, -10};
int origin = 30;
time_t start_ut;
int qijia;

//降順ランキング
void rank(std::vector<double> arr)
{
    for (int i = 0; i < 4; i++) {
        int cnt = 0;
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                continue;
            }
            if (arr[i] >= arr[j]) {
                cnt++;
            }
        }
        ranking[i] = 4 - cnt;
    }
}
// 降順ランキング
void rank(double *arr)
{
    for (int i = 0; i < 4; i++) {
        int cnt = 0;
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                continue;
            }
            if (arr[i] >= arr[j]) {
                cnt++;
            }
        }
        ranking[i] = 4 - cnt;
    }
}

std::string format_time(time_t t_t)
{
    struct tm *t;
    t = localtime(&t_t);
    char t_char[24];
    sprintf(t_char, "%04d-%02d-%02d %02d:%02d:%02d\n", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    std::string t_str(t_char);

    return t_str;
}

void get_names()
{
    std::cout << "プレーヤー4人の名前: ";
    for (int i=0; i<4; i++) {
        std::cin >> names[i]; 
    }
    for (int i=0; i<4; i++) {
        std::cout << i + 1 << ": " << names[i] << std::endl;
    }
}

void get_qijia_no()
{
    std::cout << "起家の番号: ";
    char tmp;
    std::cin >> tmp;
    if (tmp == 'd') {
       std::cout << "削除する予定"; 
    }
    qijia = (int)(tmp - '0');
    if (qijia <= 0 || qijia >= 5) {
        std::cout << "不正な起家番号";
    }
}

void get_current_raw_scores()
{
    std::string current_raw_scores_tmp[4];
    std::cout << "点数 / 100: ";
    for (int i=0; i<4; i++) {
        std::cin >> current_raw_scores_tmp[i];
    }
    //eval()
    for (int i=0; i<4; i++) {
        current_raw_scores[i] = std::stoi(current_raw_scores_tmp[i]);
    }
    int sum_raw_scores = 0;
    for (int i=0; i<4; i++) {
        sum_raw_scores += current_raw_scores[i];
    }
    if (sum_raw_scores != 1000) {

    }
}

int calc_fee()
{
    // 実際には最初の1時間が660円で延長30分毎330円なり
    time_t current_ut = time(NULL);
    return 330 * (int)((current_ut - start_ut) / 60 / 30 + 1);
}

void calc()
{
    for (int i = qijia - 1; i < qijia + 3; i++) {
        current_raw_scores[i % 4] += 0.1*(4 - i);
    }
    rank(current_raw_scores);
    for (int i = qijia - 1; i < qijia + 3; i++) {
        current_raw_scores[i % 4] -= 0.1*(4 - i);
        current_raw_scores[i % 4] = (int)(current_raw_scores[i % 4]);
    }


    scoress.push_back(current_raw_scores);

    std::vector<int> tmp(4, 0);
    raw_scoress.push_back(tmp);
    for (int i = 0; i < 4; i++) {
        raw_scoress[raw_scoress.size() - 1][i] = (int)current_raw_scores[i];
    }

    for (int i = 0; i < 4; i++) {
        scoress[scoress.size() - 1][i] = current_raw_scores[i] / 10 - origin;
        std::cout << current_raw_scores[i] << "a";
        std::cout << scoress[scoress.size() - 1][i] << "b";
        scoress[scoress.size() - 1][i] += umaoka[(int)(ranking[i] - 1)];
        std::cout << scoress[scoress.size() - 1][i] << "c";
        std::cout << umaoka[i] << "d";
        std::cout << ranking[i] << "f";
    }


    for(int i = 0; i < 4; i++) {
        //std::cout << scoress[scoress.size()-1][i];
    }

    for (int i = 0; i < 4; i++) {
        total_scores[i] += scoress[scoress.size() - 1][i];
    }
    rank(total_scores);
    fee = calc_fee();
    for (int i = 0; i < 4; i++) {
        switch(ranking[i]) {
        case 1:
            fees[i] = 0;
            break;
        case 2:
            fees[i] = fee * 0.5 / 4;
            break;
        case 3:
            fees[i] = fee * 1.5 / 4;
            break;
        case 4:
            fees[i] = fee * 2 / 4;
            break;
        }
    }
}

std::string add_pm(double j)
{
    std::string s;
    if (j == 0) {
        s = "±";
    }
    else if (j > 0) {
        s = "+";
    }
    return s + std::to_string(j);
}
void output()
{
    std::cout << std::endl << "------------------" << std::endl;
    std::cout << "開始時間: " << format_time(start_ut);
    std::cout << "現在時刻: " << format_time(time(NULL));
    std::cout << "経過時間: " << format_time(time(NULL)-start_ut);
    std::cout << "ウマ: 5-10\nオカあり\n原点: 30000点" << std::endl;
    std::cout << " 回 ";
    for (int i = 0; i < 4; i++) {
        std::cout << names[i] << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < scoress.size(); i++) {
        std::cout << i + 1 << " ";
        for (int j : raw_scoress[i]) {
            std::cout << j*100 << " ";
        }
        std::cout << std::endl;
        for (double j : scoress[i]) {
            std::cout << add_pm(j) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << " 計 ";
    for (double i : total_scores) {
        std::cout << add_pm(i) << " ";
    }
    std::cout << std::endl;

    std::cout << "順位";
    for (double i : ranking) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "料金";
    for (double i : fees) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "料金総和: " << fee << std::endl;
    std::cout << "---------------" << std::endl << std::endl;
}

int main()
{
    std::cout << "RETURNキーを押して開始";
    getchar();

    start_ut = time(NULL);
    std::cout << "開始時間: " << format_time(start_ut) << std::endl;

    get_names();
    while(1) {
        get_qijia_no();
        get_current_raw_scores();
        calc();
        output();
    }

    return 0;
}
