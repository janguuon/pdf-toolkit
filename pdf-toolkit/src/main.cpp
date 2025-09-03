#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include "PDFToolkit.h"

void printUsage() {
	std::wcout << L"PDF Toolkit - MuPDF 기반 PDF 조작 도구" << std::endl;
	std::wcout << L"사용법:" << std::endl;
	std::wcout << L"  pdf-toolkit merge <출력파일> <입력파일1> <입력파일2> [입력파일3...]" << std::endl;
	std::wcout << L"  pdf-toolkit delete <입력파일> <출력파일> <시작페이지> [끝페이지]" << std::endl;
	std::wcout << L"  pdf-toolkit split <입력파일> <출력접두사>" << std::endl;
	std::wcout << L"  pdf-toolkit info <입력파일>" << std::endl;
	std::wcout << L"" << std::endl;
	std::wcout << L"예제:" << std::endl;
	std::wcout << L"  pdf-toolkit merge output.pdf file1.pdf file2.pdf file3.pdf" << std::endl;
	std::wcout << L"  pdf-toolkit delete input.pdf output.pdf 5 10" << std::endl;
	std::wcout << L"  pdf-toolkit split input.pdf output" << std::endl;
	std::wcout << L"  pdf-toolkit info input.pdf" << std::endl;
}

int main(int argc, char** argv) {
	// 한글 출력을 위한 콘솔 설정
	SetConsoleOutputCP(CP_UTF8);
	std::wcout.imbue(std::locale(""));

	if (argc < 2) {
		printUsage();
		return 1;
	}

	try {
		PDFToolkit toolkit;
		std::string command = argv[1];

		if (command == "merge") {
			if (argc < 4) {
				std::wcout << L"병합 기능: 최소 3개의 인수가 필요합니다 (출력파일, 입력파일1, 입력파일2)" << std::endl;
				return 1;
			}

			std::string outputFile = argv[2];
			std::vector<std::string> inputFiles;
			for (int i = 3; i < argc; i++) {
				inputFiles.push_back(argv[i]);
			}

			if (toolkit.mergePDFs(inputFiles, outputFile)) {
				std::wcout << L"병합 성공!" << std::endl;
			}
			else {
				std::wcout << L"병합 실패!" << std::endl;
				return 1;
			}
		}
		else if (command == "delete") {
			if (argc < 5) {
				std::wcout << L"삭제 기능: 최소 4개의 인수가 필요합니다 (입력파일, 출력파일, 시작페이지, [끝페이지])" << std::endl;
				return 1;
			}

			std::string inputFile = argv[2];
			std::string outputFile = argv[3];
			int startPage = std::stoi(argv[4]);
			int endPage = (argc > 5) ? std::stoi(argv[5]) : startPage;

			if (toolkit.deletePages(inputFile, outputFile, startPage, endPage)) {
				std::wcout << L"페이지 삭제 성공!" << std::endl;
			}
			else {
				std::wcout << L"페이지 삭제 실패!" << std::endl;
				return 1;
			}
		}
		else if (command == "split") {
			if (argc < 4) {
				std::wcout << L"분할 기능: 2개의 인수가 필요합니다 (입력파일, 출력접두사)" << std::endl;
				return 1;
			}

			std::string inputFile = argv[2];
			std::string outputPrefix = argv[3];

			if (toolkit.splitPDF(inputFile, outputPrefix)) {
				std::wcout << L"분할 성공!" << std::endl;
			}
			else {
				std::wcout << L"분할 실패!" << std::endl;
				return 1;
			}
		}
		else if (command == "info") {
			if (argc < 3) {
				std::wcout << L"정보 기능: 1개의 인수가 필요합니다 (입력파일)" << std::endl;
				return 1;
			}

			std::string inputFile = argv[2];

			if (toolkit.isValidPDF(inputFile)) {
				int pageCount = toolkit.getPageCount(inputFile);
				std::wcout << L"파일: " << inputFile.c_str() << std::endl;
				std::wcout << L"페이지 수: " << pageCount << std::endl;
				std::wcout << L"유효한 PDF 파일입니다." << std::endl;
			}
			else {
				std::wcout << L"유효하지 않은 PDF 파일입니다: " << inputFile.c_str() << std::endl;
				return 1;
			}
		}
		else {
			std::wcout << L"알 수 없는 명령어: " << command.c_str() << std::endl;
			printUsage();
			return 1;
		}
	}
	catch (const std::exception& e) {
		std::wcout << L"오류 발생: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
