#!/usr/bin/python3
import unicodedata
import datetime
import subprocess

def rank(arr):
    ranking = []
    for i in range(len(arr)):
        cnt = 0
        for j in range(len(arr)):
            if i == j:
                continue
            if arr[i] >= arr[j]:
                cnt += 1
        ranking.append((cnt-len(arr))*(-1))
    return ranking

def length(text):
    count = 0
    for c in text:
        if unicodedata.east_asian_width(c) in 'FMA':
            count += 2
        else:
            count += 1
    return count

def calc_fee():
    td = datetime.datetime.now() - dt_start
    return 330 * int((td / datetime.timedelta(minutes=30) + 1))

def draw():
    print()
    print('-----------------------------------')
    print('開始時間:', str(dt_start).split('.')[0])
    print('ウマ 5-10')
    print('オカあり')
    print('原点30000点')
    print(' 回  ', end='')
    for i in range(len(names)):
        print(names[i].center(7), end='')
    print()
    for i in range(len(scoress)):
        print(' ' + str(i+1).zfill(2) + ' ', end='')
        for j in raw_scoress[i]:
            print(str(j * 100).center(8), end='')
        print()
        print(' ' * 4, end='')
        for j in scoress[i]:
            if j < 0:
                s = ''
            elif j > 0:
                s = '+'
            else:
                s = '±'
            s += str(j)
            print(s.center(8), end='')
        print()
    print(' 計 ', end='')
    for i in total_scores:
        if i < 0:
            s = ''
        elif i > 0:
            s = '+'
        else:
            s = '±'
        s += str(i)
        print(s.center(8), end='')
    print()
    print('順位', end='')
    for i in ranking:
        print(str(i).center(8), end='')
    print()
    print('料金', end='')
    for i in fees:
        print(str(round(i)).center(8), end='')
    print('\n料金総和: %d円' % fee)
    print('-----------------------------------')
    print()

raw_scoress = []
scoress = []
total_scores = [0, 0, 0, 0]
ranking = [0, 0, 0, 0]
fees = [0, 0, 0, 0]

# ウマとオカ
umaoka = [20+10, 5, -5, -10]
origin = 30 # 原点/1000

input('RETURNキーを押して開始: ')

dt_start = datetime.datetime.now()
print('開始時間:', str(dt_start).split('.')[0])

while True:
    names = input('４人の名前: ').split()
    if len(names) == 4:
        break
    else:
        print('エラー: 4人入力してください。')

for i in range(4):
    print('%d: %s' % (i, names[i]))

while True:
    while True:
        try:
            tmp = input('起家の番号: ')
            if tmp == 'd':
                if len(raw_scoress) >= 1:
                    del raw_scoress[-1]
                    del scoress[-1]
                    print('削除しました。')
                    continue
                else:
                    print('エラー: 削除するデータがありません。')
            qijia = int(tmp)
            break
        except ValueError:
            print('エラー: 1つの数値を入力してください。')
    raw_scores_tmp = []
    while True:
        raw_scores_tmp_tmp = [i for i in input('点数 / 100: ').split()]
        if len(raw_scores_tmp_tmp) != 4:
            print('エラー: 入力が４つではありません。')
            continue

        try: 
            raw_scores_tmp = [int(eval(i)) for i in raw_scores_tmp_tmp]
        except SyntaxError:
            print('eval() SyntaxError: 数値または計算式を入力してください。')
            continue
        except TypeError:
            print('TypeError')
            continue
        except ValueError:
            print('ValueError')
            continue

        if sum(raw_scores_tmp) == 1000:
            break
        else:
            print('エラー: Σ=%d≠10万点' % sum(raw_scores_tmp))
            continue

    global total_scores
    raw_scoress.append(raw_scores_tmp)
    scoress.append([i / 10 for i in raw_scoress[-1]])
    for i in range(qijia - 1, qijia + 3):
        raw_scores_tmp[i % 4] += 0.1*(4 - i)
    ranking = rank(raw_scores_tmp)
    for i in range(qijia - 1, qijia + 3):
        raw_scores_tmp[i % 4] -= 0.1*(4 - i)
        raw_scores_tmp[i % 4] = int(raw_scores_tmp[i % 4])
    for i in range(4):
        scoress[-1][i] = raw_scores_tmp[i] / 10 - origin
    for i in range(4):
        scoress[-1][i] += umaoka[int(ranking[i] - 1)]
    """
    max_scores_index = [i for i in range(len(scoress[-1])) if scoress[-1][i] == max(scoress[-1])]
    max_scores_index = [i + 4 for i in max_scores_index if i < qijia]
    scoress[-1][min(max_scores_index) % 4] += oka"""
    scoress[-1] = [round(i, 1) for i in scoress[-1]]
    for i in range(4):
        total_scores[i] += scoress[-1][i]
    total_scores = [round(i, 1) for i in total_scores]
    ranking = rank(total_scores)
    fee = calc_fee()
    for i in range(4):
        if ranking[i] == 1:
            fees[i] = 0
        elif ranking[i] == 2:
            fees[i] = fee * 0.5 / 4
        elif ranking[i] == 3:
            fees[i] = fee * 1.5 / 4
        else:
            fees[i] = fee * 2 / 4
    draw()

