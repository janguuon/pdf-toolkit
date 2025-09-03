# PDF Toolkit

MuPDF 라이브러리를 사용한 고성능 PDF 조작 도구입니다.

## 기능

- **PDF 병합**: 여러 PDF 파일을 하나로 병합
- **페이지 삭제**: PDF에서 특정 페이지 또는 페이지 범위 삭제
- **PDF 분할**: PDF를 각 페이지별로 개별 파일로 분할
- **파일 정보**: PDF 파일의 페이지 수 및 유효성 확인

## 시스템 요구사항

- Windows 10/11
- Visual Studio 2019 이상
- MuPDF 1.27 라이브러리

## 빌드 방법

1. Visual Studio에서 `pdf-toolkit.sln` 열기
2. 플랫폼을 `x64`로 설정
3. 구성을 `Debug` 또는 `Release`로 설정
4. 솔루션 빌드 (Ctrl+Shift+B)

## 사용법

### 명령행 인터페이스

```bash
# PDF 병합
pdf-toolkit merge <출력파일> <입력파일1> <입력파일2> [입력파일3...]

# 페이지 삭제
pdf-toolkit delete <입력파일> <출력파일> <시작페이지> [끝페이지]

# PDF 분할
pdf-toolkit split <입력파일> <출력접두사>

# 파일 정보 확인
pdf-toolkit info <입력파일>
```

### 사용 예제

```bash
# 여러 PDF 파일을 하나로 병합
pdf-toolkit merge output.pdf file1.pdf file2.pdf file3.pdf

# PDF에서 5-10페이지 삭제
pdf-toolkit delete input.pdf output.pdf 5 10

# PDF에서 단일 페이지(5페이지) 삭제
pdf-toolkit delete input.pdf output.pdf 5

# PDF를 각 페이지별로 분할
pdf-toolkit split input.pdf output

# PDF 파일 정보 확인
pdf-toolkit info input.pdf
```

## 프로젝트 구조

```
pdf-toolkit/
├── pdf-toolkit/
│   ├── src/
│   │   ├── main.cpp           # 메인 프로그램
│   │   ├── PDFToolkit.h       # 헤더 파일
│   │   └── PDFToolkit.cpp     # 구현 파일
│   └── pdf-toolkit.vcxproj    # Visual Studio 프로젝트 파일
├── testpdf/                   # 테스트용 PDF 파일들
├── thirdparty/                # 외부 라이브러리 (MuPDF)
├── build/                     # 빌드 출력 디렉토리
├── pdf-toolkit.sln            # Visual Studio 솔루션 파일
├── .gitignore                 # Git 무시 파일 목록
└── README.md                  # 프로젝트 설명서
```

## 주요 클래스

### PDFToolkit

PDF 조작을 위한 메인 클래스입니다.

#### 주요 메서드

- `bool mergePDFs(const std::vector<std::string>& inputFiles, const std::string& outputFile)`
  - 여러 PDF 파일을 하나로 병합

- `bool deletePages(const std::string& inputFile, const std::string& outputFile, int startPage, int endPage = -1)`
  - 특정 페이지 또는 페이지 범위 삭제

- `bool splitPDF(const std::string& inputFile, const std::string& outputPrefix)`
  - PDF를 각 페이지별로 분할

- `int getPageCount(const std::string& filePath)`
  - PDF 파일의 페이지 수 반환

- `bool isValidPDF(const std::string& filePath)`
  - PDF 파일 유효성 검사

## 테스트 결과

### 테스트 환경
- Windows 11
- Visual Studio 2019
- x64 Debug 빌드

## 기술적 특징

- **고성능**: MuPDF 라이브러리 기반으로 빠른 처리
- **안정성**: 전역 컨텍스트 관리로 메모리 안전성 보장
- **에러 처리**: 강건한 에러 처리 및 리소스 관리
- **한글 지원**: CP949 인코딩으로 한글 메시지 지원

## 라이선스

이 프로젝트는 MuPDF 라이브러리를 사용합니다. MuPDF의 라이선스 조건을 준수해야 합니다.

## 기여

버그 리포트나 기능 개선 제안은 GitHub Issues를 통해 제출해 주세요.

## 개발자

개발 및 테스트 완료된 안정적인 PDF 조작 도구입니다.
