# GitHub Blog

이 저장소는 `Jekyll`과 `jekyll-theme-chirpy`를 이용해 운영하는 개인 `github.io` 블로그다.

## 개요

이 블로그는 정적 사이트다. 글 원본은 저장소 안의 Markdown 파일로 관리하고, Jekyll이 이를 HTML로 빌드한 뒤 GitHub Pages가 배포한다.

흐름은 다음과 같다.

1. `_posts` 아래에 Markdown 글을 추가하거나 수정한다.
2. 이미지와 표지 파일을 `assets/img/` 아래에 정리한다.
3. GitHub에 `commit`과 `push`를 하면 GitHub Actions가 사이트를 빌드하고 배포한다.
4. 배포된 결과가 `https://<username>.github.io` 형태의 도메인에서 제공된다.

## 주요 파일

- `_config.yml`: 블로그 전역 설정
- `_posts/`: 게시글 Markdown 원본
- `_tabs/`: About, Archives, Categories, Tags 페이지
- `assets/`: 정적 파일
- `templates/post-skeleton.md`: 새 글 작성 템플릿
- `post.c`: 게시글 생성과 삭제를 돕는 CLI 도구
- `.github/workflows/pages-deploy.yml`: GitHub Pages 배포 workflow

## `post.c` 사용법

먼저 C 소스를 컴파일한다.

```bash
gcc -Wall -Wextra -pedantic -std=c11 post.c -o post
```

이후 `post` 실행파일로 새 글을 만들거나 삭제할 수 있다.

새 글 생성:

```bash
./post new
```

이 명령은 다음을 수행한다.

- `_posts/YYYY-MM-DD-new-post.md` 또는 중복 시 번호가 붙은 Markdown 파일 생성
- `templates/post-skeleton.md`를 복사
- `date:` 항목을 현재 시각으로 자동 치환
- `assets/img/covers/<post-name>/` 디렉터리 생성
- `assets/img/posts/<post-name>/` 디렉터리 생성

예:

```text
_posts/2026-03-12-new-post.md
assets/img/covers/2026-03-12-new-post/
assets/img/posts/2026-03-12-new-post/
```

글 삭제:

```bash
./post delete 1
```

이 명령은 오늘 날짜 기준으로 아래 파일과 디렉터리를 삭제한다.

```text
_posts/YYYY-MM-DD-new-post-1.md
assets/img/covers/YYYY-MM-DD-new-post-1/
assets/img/posts/YYYY-MM-DD-new-post-1/
```

`delete 0`은 suffix가 없는 기본 파일을 대상으로 한다.

이 도구는 글 파일과 이미지 디렉터리를 빠르게 만드는 보조 도구다. 실제 공개 반영은 `push` 이후 GitHub Actions가 수행한다.

## 이미지 경로 규칙

- 프로필 이미지: `assets/img/avatar/`
- 글 표지 이미지: `assets/img/covers/<post-name>/`
- 글 본문 이미지: `assets/img/posts/<post-name>/`

Markdown 본문에서는 절대 경로처럼 아래와 같이 참조한다.

```md
![example](/assets/img/posts/2026-03-12-new-post/sample.png)
```

표지 이미지는 front matter에서 지정한다.

```yaml
image:
  path: /assets/img/covers/2026-03-12-new-post/cover.png
  alt: "cover image"
```

## 배포 원리

이 저장소는 블로그 서비스의 DB를 쓰는 구조가 아니라, Git 저장소 자체가 블로그 원본이다.

- 글은 Markdown 파일이다.
- `post.c`와 `templates/`는 글 생성을 돕는 관리 도구이며, 사이트 빌드 결과에는 포함되지 않는다.
- Jekyll이 Markdown과 테마를 HTML로 변환한다.
- GitHub Actions가 Jekyll 빌드를 수행하고 GitHub Pages가 결과물을 호스팅한다.
- 따라서 방문자는 글을 읽기만 하고, 글 작성과 수정은 저장소 변경으로 이루어진다.

즉 이 블로그는 "레포지토리 기반 정적 블로그"라고 보면 된다.

## 운영 방식

이 저장소는 `_site`를 직접 배포하는 방식이 아니다. GitHub에 올리는 것은 소스 파일이고, 배포는 workflow가 자동으로 처리한다.

- GitHub에 올리는 것: `_posts`, `_tabs`, `assets`, `_config.yml`, `post.c`, `templates`, workflow 파일
- GitHub에 올리지 않는 것: `_site`, `.jekyll-cache`, `post` 실행파일

한 번 저장소와 Pages 설정이 끝나면, 이후에는 저장소의 소스 파일만 수정하고 `push`하는 방식으로 블로그를 관리하면 된다.
