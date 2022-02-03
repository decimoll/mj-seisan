#include <time.h>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>

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
std::vector<int> qijias;

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
    std::cout << "プレイヤー4人の名前: ";
    for (int i=0; i<4; i++) {
        std::cin >> names[i]; 
    }
    for (int i=0; i<4; i++) {
        std::cout << i + 1 << ": " << names[i] << std::endl;
    }
    puts("");
}

int add(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    return add(a, -b);
}

std::vector<int> eval(std::vector<std::string> &v) {
    std::vector<int> v2;
    for (const std::string& s : v) {
        int tmp[2] = {0};
        bool flg = 0;
        bool aflg = 0;
        bool sflg = 0;
        for (int i = 0; i < s.size(); i++) {
            if (s[i] == '+') {
                if (aflg) {
                    tmp[flg] = add(tmp[!flg], tmp[flg]);
                }
                else if (sflg) {
                    tmp[flg] = sub(tmp[!flg], tmp[flg]);
                }
                aflg = 1;
                sflg = 0;
                tmp[!flg] = 0;
                flg = !flg;
            }
            else if (s[i] == '-') {
                if (aflg) {
                    tmp[flg] = add(tmp[!flg], tmp[flg]);
                }
                else if (sflg) {
                    tmp[flg] = sub(tmp[!flg], tmp[flg]);
                }
                aflg = 0;
                sflg = 1;
                tmp[!flg] = 0;
                flg = !flg;
            }
            else if (s[i] - '0' < 0 || 9 < s[i] - '0') {
                std::cout << "エラー: 数値と+-のみ入力可" << std::endl;
                return std::vector<int>(4, 0);
            }
            else {
                tmp[flg] = tmp[flg] * 10 + (s[i] - '0');
            }
        }
        if (aflg) {
            tmp[flg] = add(tmp[!flg], tmp[flg]);
        }
        else if (sflg) {
            tmp[flg] = sub(tmp[!flg], tmp[flg]);
        }
        v2.push_back(tmp[flg]);
    }
    return v2;
}

void get_current_raw_scores()
{
    // よくわからないがこのgetchar()がないとエラー表示のifに行く
    getchar();
    while (1) {
        std::vector<std::string> tmp;
        std::string str, s;
        std::cout << "点数 / 100: ";
        std::getline(std::cin,str);
        std::stringstream ss{str};
        while (getline(ss, s, ' ')){
            tmp.push_back(s);
        }
        if (tmp.size() != 4) {
            printf("エラー: 入力が４つではありません。\n");
            continue;
        }
        std::vector<int> tmp_int = eval(tmp);

        for (int i = 0; i < 4; i++) {
            current_raw_scores[i] = static_cast<double>(tmp_int[i]);
        }

        int sum_raw_scores = 0;
        for (int i=0; i<4; i++) {
            sum_raw_scores += current_raw_scores[i];
        }
        if (sum_raw_scores != 1000) {
            printf("エラー: Σ=%d≠1000\n", sum_raw_scores);
        }
        else {
            break;
        }
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
    if (current_raw_scores[0] == -65536) {
        for (int i = 0; i < 4; i++) {
            total_scores[i] = 0;
        }
    }
    else {
        // その半荘のスコア計算
        for (int i = qijias.back() - 1; i < qijias.back() + 3; i++) {
            current_raw_scores[i % 4] += 0.1*(4 - i);
        }
        rank(current_raw_scores);
        for (int i = qijias.back() - 1; i < qijias.back() + 3; i++) {
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
            scoress[scoress.size() - 1][i] += umaoka[(int)(ranking[i] - 1)];
        }

        for (int i = 0; i < 4; i++) {
            total_scores[i] += scoress[scoress.size() - 1][i];
        }
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

// プラス、プラマイの記号を足すついでに小数点第二位以下を切る
std::string add_pm(double j)
{
    std::string s = "";
    if (j == 0) {
        s = "±";
    }
    else if (j > 0) {
        s = "+";
    }
    char str[24];
    sprintf(str, "%.1f", j);
    return s + std::string(str);
}
void output()
{
    printf("\n====================================\n");
    std::cout << "開始時刻: " << format_time(start_ut);
    std::cout << "現在時刻: " << format_time(time(NULL));
    time_t past_time = time(NULL) - start_ut;
    int past_time_h = past_time / 3600;
    int past_time_m = past_time % 3600 / 60;
    int past_time_s = past_time % 60;
    printf("経過時間: %02d:%02d:%02d\n", past_time_h, past_time_m, past_time_s);
    printf("料金総和: %d 円\n", fee);
    std::cout << "ウマ: 5-10\nオカあり\n原点: 30000点" << std::endl;

    printf("====================================\n");
    std::cout << "    |";
    for (int i = 0; i < 4; i++) {
        std::cout << std::setw(7) << names[i];
        if (i < 3) {
            std::cout << "|";
        }
    }
    for (int i = 0; i < scoress.size(); i++) {
        if (i == 0) {
            printf("\n====‡=======‡=======‡=======‡=======\n");
        }
        else {
            printf("\n----+-------+-------+-------+-------\n");
        }
        printf(" %02d |", i + 1);
        for (int j = 0; j < 4; j++) {
            printf("%7d", raw_scoress[i][j]*100);
            if (j < 3) {
                std::cout << "|";
            }
        }
        puts("");
        printf("    |");
        for (int j = 0; j < 4; j++) {
            printf("%7s", add_pm(scoress[i][j]).c_str());
            if (j < 3) {
                std::cout << "|";
            }
        }
    }

    printf("\n====‡=======‡=======‡=======‡=======\n");
    std::cout << " 計 |";
    for (int i = 0; i < 4; i++) {
        printf("%7s", add_pm(total_scores[i]).c_str());
        if (i < 3) {
            std::cout << "|";
        }
    }
    std::cout << std::endl;

    std::cout << "順位|";
    for (int i = 0; i < 4; i++) {
        std::cout << std::setw(5) << ranking[i];
        switch(ranking[i]) {
        case 1:
            std::cout << "st";
            break;
        case 2:
            std::cout << "nd";
            break;
        case 3:
            std::cout << "rd";
            break;
        case 4:
            std::cout << "th";
            break;
        }
        if (i < 3) {
            std::cout << "|";
        }
    }
    std::cout << std::endl;

    std::cout << "料金|";
    for (int i = 0; i < 4; i++) {
        std::cout << std::setw(7) << fees[i];
        if (i < 3) {
            std::cout << "|";
        }
    }
    std::cout << std::endl;
    printf("====================================\n\n");
}

void del()
{
    for (int i = 0; i < 4; i++) {
        total_scores[i] -= scoress.back()[i];
    }
    if (raw_scoress.size() > 1) {
        for (int i = 0; i < 4; i++) {
            total_scores[i] -= scoress[scoress.size() - 2][i];
        }
        for (int i = 0; i < 4; i++) {
            current_raw_scores[i] = static_cast<double>(raw_scoress[raw_scoress.size() - 2][i]);
        }
        for (int i = 0; i < 2; i++) {
            raw_scoress.pop_back();
            scoress.pop_back();
            qijias.pop_back();
        }
    }
    else {
        current_raw_scores[0] = -65536;
        raw_scoress.pop_back();
        scoress.pop_back();
        qijias.pop_back();
    }

}

void get_qijia_no()
{
    while (1) {
        std::cout << "起家の番号: ";
        char tmp;
        std::cin >> tmp;
        if (tmp == 'd') {
            if (raw_scoress.size() > 0) {
                del();
                calc();
                output();
            }
            else {
                printf("エラー: 削除できるデータがありません。\n");
            }
            continue;
        }
        int tmp_int = static_cast<int>(tmp - '0');
        if (tmp_int > 0 && tmp_int < 5) {
            qijias.push_back(tmp_int);
            break;
        }
        else {
            std::cout << "1 <= 起家番号 <= 4\n";
        }
    }
}

int main()
{
    std::cout << "RETURNキーを押して開始";
    getchar();

    start_ut = time(NULL);
    std::cout << "開始時刻: " << format_time(start_ut);

    get_names();
    while(1) {
        get_qijia_no();
        get_current_raw_scores();
        calc();
        output();
    }

    return 0;
}
