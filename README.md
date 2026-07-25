# GitHub 협업 가이드 (GitHub Desktop)

이 프로젝트는 **GitHub Desktop**을 이용하여 협업합니다.

Git 명령어를 직접 사용할 필요는 없으며, 대부분의 작업은 GitHub Desktop에서 가능합니다.

---

# 1. 브랜치(Branch) 규칙

브랜치는 다음과 같이 사용합니다.

```
main
└── develop
    ├── feature/player
    ├── feature/enemy
    ├── feature/ui
    └── feature/...
```

## main

배포 가능한 가장 안정적인 버전입니다.

**직접 작업하지 않습니다.**

---

## develop

현재 개발 중인 버전입니다.

모든 기능은 최종적으로 develop에 합쳐집니다.

직접 개발하지 않는 것을 권장합니다.

---

## feature 브랜치

각자 자신의 작업 브랜치를 만들어 사용합니다.

예시

```
feature/player
feature/ui
feature/sound
feature/stage
```

한 브랜드에는 하나의 기능만 개발합니다.

---

# 2. 작업 시작하기

매일 작업을 시작하기 전에 반드시 최신 프로젝트를 받아옵니다.

### ① GitHub Desktop 실행

### ② Current Branch를 develop으로 변경

### ③ 상단의

```
Fetch origin
```

버튼 클릭

변경 사항이 있다면

```
Pull origin
```

을 눌러 최신 내용을 받아옵니다.

---

### ④ 자신의 브랜치로 이동

예)

```
feature/player
```

---

### ⑤ 최신 develop 내용을 가져오기

메뉴

```
Branch
→ Update from develop
```

또는

```
Branch
→ Merge into Current Branch
```

를 이용하여 develop의 최신 내용을 가져옵니다.

항상 최신 develop 기준으로 작업해야 충돌을 줄일 수 있습니다.

---

# 3. 작업하기

평소처럼 Visual Studio에서 개발합니다.

작업이 끝났다면

GitHub Desktop의

```
Changes
```

탭에서 변경된 파일을 확인합니다.

---

# 4. Commit 하기

Summary에는

무엇을 수정했는지 간단히 작성합니다.

좋은 예

```
Add player movement

Fix collision bug

Implement inventory UI

Add enemy attack
```

나쁜 예

```
update

asdf

123
```

Summary 작성 후

```
Commit to feature/...
```

를 클릭합니다.

---

# 5. Push 하기

Commit이 끝나면

상단의

```
Push origin
```

버튼을 클릭합니다.

이제 GitHub에 업로드됩니다.

---

# 6. Pull Request 만들기

기능 개발이 완료되면

```
Create Pull Request
```

를 클릭합니다.

PR에는

- 무엇을 수정했는지
- 테스트 여부
- 특이사항

정도를 적어줍니다.

예시

```
Player 이동 구현

- WASD 이동
- Shift 달리기
- 점프 추가

테스트 완료
```

---

# 7. Merge

리뷰가 끝나면

feature 브랜치를

develop으로 Merge합니다.

Merge는 담당자만 진행합니다.

---

# 8. 절대로 하면 안 되는 것

## main에서 작업

❌ 금지

항상 feature 브랜치에서 작업합니다.

---

## develop에서 계속 작업

가능하면 하지 않습니다.

feature 브랜치를 새로 만들어 작업합니다.

---

## 다른 사람 브랜치에서 작업

자신의 브랜치만 수정합니다.

---

## Debug, Release 폴더 추가

빌드 결과물은 업로드하지 않습니다.

Git이 자동으로 제외합니다.

---

## .vs 폴더 추가

절대 업로드하지 않습니다.

Visual Studio 설정 파일입니다.

---

# 9. 충돌(Conflict)이 발생했을 때

같은 파일을 여러 사람이 수정하면 충돌이 발생할 수 있습니다.

혼자 해결하려 하지 말고

**반드시 팀원과 상의 후 해결합니다.**

충돌이 발생하면

```
Accept Yours

Accept Theirs
```

를 무조건 누르지 않습니다.

잘못 누르면 상대 코드가 사라질 수 있습니다.

---

# 10. 좋은 협업 습관

작업 시작 전

```
Pull
```

↓

작업

↓

Commit

↓

Push

↓

Pull Request

이 순서를 항상 지켜주세요.

---

# 11. 역할 분담

충돌을 줄이기 위해 가능하면 담당 기능을 나누어 작업합니다.

예시

| 팀원 | 담당 |
|------|------|
| A | Player |
| B | Enemy |
| C | UI |
| D | Sound |
| E | Stage |

가능하면 같은 cpp 파일을 여러 사람이 동시에 수정하지 않습니다.

---

# 12. Commit 메시지 예시

```
Add player movement

Fix animation bug

Implement inventory

Update boss AI

Refactor collision system
```

---

# 13. 작업이 끝났다면

✔ Commit 완료

✔ Push 완료

✔ Pull Request 생성

이 세 가지를 반드시 확인합니다.

---

# 협업 체크리스트

작업 시작 전

- [ ] develop Pull 완료
- [ ] 자신의 feature 브랜치 확인
- [ ] develop 최신 내용 Merge 완료

작업 후

- [ ] 변경 파일 확인
- [ ] Commit 완료
- [ ] Push 완료
- [ ] Pull Request 생성

---

모든 팀원은 **main 브랜치에서는 작업하지 않습니다.**

항상 자신의 **feature 브랜치**에서 개발 후 **develop으로 Pull Request**를 보내는 것을 원칙으로 합니다.
