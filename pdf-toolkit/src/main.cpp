#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include "PDFToolkit.h"

void printUsage() {
	std::wcout << L"PDF Toolkit - MuPDF ��� PDF ���� ����" << std::endl;
	std::wcout << L"����:" << std::endl;
	std::wcout << L"  pdf-toolkit merge <�������> <�Է�����1> <�Է�����2> [�Է�����3...]" << std::endl;
	std::wcout << L"  pdf-toolkit delete <�Է�����> <�������> <����������> [��������]" << std::endl;
	std::wcout << L"  pdf-toolkit split <�Է�����> <������λ�>" << std::endl;
	std::wcout << L"  pdf-toolkit info <�Է�����>" << std::endl;
	std::wcout << L"" << std::endl;
	std::wcout << L"����:" << std::endl;
	std::wcout << L"  pdf-toolkit merge output.pdf file1.pdf file2.pdf file3.pdf" << std::endl;
	std::wcout << L"  pdf-toolkit delete input.pdf output.pdf 5 10" << std::endl;
	std::wcout << L"  pdf-toolkit split input.pdf output" << std::endl;
	std::wcout << L"  pdf-toolkit info input.pdf" << std::endl;
}

int main(int argc, char** argv) {
	// �ѱ� ����� ���� �ܼ� ����
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
				std::wcout << L"���� ���: �ּ� 3���� �μ��� �ʿ��մϴ� (�������, �Է�����1, �Է�����2)" << std::endl;
				return 1;
			}

			std::string outputFile = argv[2];
			std::vector<std::string> inputFiles;
			for (int i = 3; i < argc; i++) {
				inputFiles.push_back(argv[i]);
			}

			if (toolkit.mergePDFs(inputFiles, outputFile)) {
				std::wcout << L"���� ����!" << std::endl;
			}
			else {
				std::wcout << L"���� ����!" << std::endl;
				return 1;
			}
		}
		else if (command == "delete") {
			if (argc < 5) {
				std::wcout << L"���� ���: �ּ� 4���� �μ��� �ʿ��մϴ� (�Է�����, �������, ����������, [��������])" << std::endl;
				return 1;
			}

			std::string inputFile = argv[2];
			std::string outputFile = argv[3];
			int startPage = std::stoi(argv[4]);
			int endPage = (argc > 5) ? std::stoi(argv[5]) : startPage;

			if (toolkit.deletePages(inputFile, outputFile, startPage, endPage)) {
				std::wcout << L"������ ���� ����!" << std::endl;
			}
			else {
				std::wcout << L"������ ���� ����!" << std::endl;
				return 1;
			}
		}
		else if (command == "split") {
			if (argc < 4) {
				std::wcout << L"���� ���: 2���� �μ��� �ʿ��մϴ� (�Է�����, ������λ�)" << std::endl;
				return 1;
			}

			std::string inputFile = argv[2];
			std::string outputPrefix = argv[3];

			if (toolkit.splitPDF(inputFile, outputPrefix)) {
				std::wcout << L"���� ����!" << std::endl;
			}
			else {
				std::wcout << L"���� ����!" << std::endl;
				return 1;
			}
		}
		else if (command == "info") {
			if (argc < 3) {
				std::wcout << L"���� ���: 1���� �μ��� �ʿ��մϴ� (�Է�����)" << std::endl;
				return 1;
			}

			std::string inputFile = argv[2];

			if (toolkit.isValidPDF(inputFile)) {
				int pageCount = toolkit.getPageCount(inputFile);
				std::wcout << L"����: " << inputFile.c_str() << std::endl;
				std::wcout << L"������ ��: " << pageCount << std::endl;
				std::wcout << L"��ȿ�� PDF �����Դϴ�." << std::endl;
			}
			else {
				std::wcout << L"��ȿ���� ���� PDF �����Դϴ�: " << inputFile.c_str() << std::endl;
				return 1;
			}
		}
		else {
			std::wcout << L"�� �� ���� ��ɾ�: " << command.c_str() << std::endl;
			printUsage();
			return 1;
		}
	}
	catch (const std::exception& e) {
		std::wcout << L"���� �߻�: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
