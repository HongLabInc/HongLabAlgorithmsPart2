import json
import os

# 자료구조 압축코스에서 사용했던 것의 차이
# - 명사 뿐 아니라 모든 단어
# - ascii로 이루어진 단어만 저장

f = open('EDMTDictionary.json', encoding = "utf8")
data = json.load(f)
f.close()

count = 0
f = open("eng_dic.txt", "wt", encoding="ascii")
for item in data:
    if item["word"].isascii():
        f.write(item["word"].lower().encode("ascii", "ignore").decode("ascii") + "\n")
        f.write(item["description"].encode("ascii", "ignore").decode("ascii") + "\n")
        count += 1
    else:
        print("Skip: ", item["word"].lower(), item["description"])

print(count)

f.close()
