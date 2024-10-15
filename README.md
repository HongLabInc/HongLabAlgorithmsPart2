# [홍정모 연구소](https://honglab.co.kr/)

- 온라인 강의노트 같은 2차 저작물에는 참고자료에 "[홍정모 연구소](https://honglab.co.kr/)"를 꼭 적어주세요.
- 다른 학생들의 공부를 위해서 실습 문제의 정답이나 풀이를 온라인에 공개하지 마세요.

## 알고리듬 압축코스 파트2

### 선수 과목
- 프로그래밍 입문은 **[홍정모의 파이썬 추월코스](https://honglab.co.kr/courses/python)**
- 코테 공부용 C++ 문법은 **[C++ 핵심정리(무료)](https://honglab.co.kr/courses/cppsummary)**
- 기초적인 자료구조와 프로그래밍 연습은 **[자료구조 압축코스](https://honglab.co.kr/courses/data-structures)**
- **[알고리듬 압축코스 파트1](https://honglab.co.kr/courses/algorithms)**

## 예제 사용 방법
- Visual Studio 2022 설정 제공 (HongLabAlgorithmsPart2.sln 파일 실행)
- Visual Studio Code는 VSCode 폴더 사용 ([VSCode 사용법](https://youtu.be/uDq7woPOZ_A?si=3qoGEBENHcFFOttB))
- 맥에서 VSCode 사용 시에는 tasks.json에 "-std=c++17" 과 "${fileDirname}/*.cpp" 추가해주세요. ([샘플](https://github.com/HongLabInc/HongLabCppSummary/blob/main/.vscode_mac/tasks.json))

## OpenCV 설치 안내
- 챕터 22. 계산 기하 부터는 OpenCV가 필요합니다. 아래 설치 과정을 참고하세요.
- OpenCV는 원래 컴퓨터 비전 라이브러리인데, 여기서는 공간 분할 알고리듬을 가시화는 용도로만 사용합니다.
- OpenCV를 선택한 이유는 윈도우와 맥 모두 호환성 관리가 잘 되고 있고 AI 분야에서도 많이 사용되기 때문입니다.
- 윈도우즈에서 [vcpkg](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started-vs?pivots=shell-powershell)를 이용한 설치
  - 터미널(CMD, 파워쉘 등도 가능)에서 아래 순서로 설치하시면 Visual Studio 2022에서 알아서 OpenCV를 찾아서 사용합니다.
```
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg.exe install opencv:x64-windows
.\vcpkg.exe integrate install
```

- 맥OS에서 [Homebrew](https://brew.sh/)를 이용한 설치
  - 터미널에서 아래 순서로 설치하고 VS의 settings.json 수정
  - 폴더 이름은 설치되는 OpenCV 버전에 따라 달라질 수 있습니다. 아래 예시에서는 4.9.0_12 버전입니다.
```
brew install pkg-config
brew install vcpkg
settings.json에 아래와 같이 OpenCV 경로 추가
    "C_Cpp_Runner.linkerArgs": ["-L/opt/homebrew/Cellar/opencv/4.9.0_12/lib -lopencv_core -lopencv_highgui -lopencv_imgproc"],
    "C_Cpp_Runner.includePaths": [
    "/opt/homebrew/Cellar/opencv/4.9.0_12/include/opencv4"],
```

- 참고로 맥에서 XCode 사용할 때도 비슷하게 경로를 설정해주시면 됩니다.
  - Search Paths에서 Header Search Paths, Library Search Paths 설정
  - Linking - General -> Other Linker Flags 설정


## 기타 주의 사항
- 사용자 이름이나 폴더에 한글 사용 금지 (시간 낭비 방지 차원입니다.)

