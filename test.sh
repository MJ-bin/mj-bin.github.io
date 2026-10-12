#!/usr/bin/env bash
set -euo pipefail

cd -- "$(dirname -- "${BASH_SOURCE[0]}")"

if ! command -v ruby >/dev/null || ! command -v bundle >/dev/null; then
  printf 'Ruby와 Bundler가 필요합니다. 설치 후 다시 실행해주세요.\n' >&2
  exit 1
fi

PORT="${PORT:-4000}"
if [[ ! "$PORT" =~ ^[0-9]{1,5}$ ]] || (( 10#$PORT < 1 || 10#$PORT > 65535 )); then
  printf 'PORT는 1~65535 사이의 숫자여야 합니다.\n' >&2
  exit 1
fi
PORT="$((10#$PORT))"

if ! bundle check >/dev/null 2>&1; then
  printf '미리보기에 필요한 gem을 설치합니다...\n'
  bundle config set --local path vendor/bundle
  bundle install
fi

printf '\n로컬 미리보기 URL: http://localhost:%s/\n' "$PORT"
printf '빌드가 완료되면 접속하세요. 파일을 저장하면 자동으로 반영됩니다.\n'
printf '종료: Ctrl+C\n\n'

export JEKYLL_ENV=development
exec bundle exec jekyll serve --host 127.0.0.1 --port "$PORT" --baseurl "" --future --drafts
