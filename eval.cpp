#include <iostream>
#include <sstream>
#include <vector>

int add(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    return add(a, -b);
}

int main()
{   
    std::vector<std::string> v;
    std::string str, s;

    std::cout << "input: ";
    std::getline(std::cin,str);

    std::stringstream ss{str};

    while ( getline(ss, s, ' ') ){
        v.push_back(s);
    }
    
    for (const std::string& s : v){
        std::cout << s << std::endl;
    }
    
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
            else if ( s[i] - '0' < 0 || 9 < s[i] - '0') {
                std::cout << "invalid input" << std::endl;
                return 1;
            }
            std::cout << s[i] << std::endl;
            tmp[flg] = tmp[flg] * 10 + (s[i] - '0');
        }
        if (aflg) {
            tmp[flg] = add(tmp[!flg], tmp[flg]);
        }
        else if (sflg) {
            tmp[flg] = sub(tmp[!flg], tmp[flg]);
        }
        v2.push_back(tmp[flg]);
    }
    
    for (int i : v2){
        std::cout << i << std::endl;
    }

    return 0;
}
