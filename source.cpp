#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#define shift_time 7//7時間で休憩か退勤
int main(void) {
	//input data
	std::vector<std::string> name;
	std::vector<int> kibou_temp_person;
	std::vector<std::vector<int>> kibou_temp_day;
	std::vector<std::vector<std::vector<int>>> kibou;
	std::vector<int> skill = { 3,3,3,3,3 };
	//open kibou file
	std::string str_line_buf;
	std::string str_conma_buf;
	std::string str_minus_buf;
	std::string str_plus_buf;
	int kibou_start, kibou_end;
	std::string input_path = "C:\\Users\\/*ファイルのパスを指定*/shift_kibou.csv";
	std::ifstream ifs(input_path);
	getline(ifs, str_line_buf);//skip head
	getline(ifs, str_line_buf);
	std::istringstream i_stream(str_line_buf);
	getline(i_stream, str_conma_buf, ',');
	const int member = std::stoi(str_conma_buf);
	getline(i_stream, str_conma_buf, ',');
	const int days = std::stoi(str_conma_buf);
	getline(ifs, str_line_buf);//skip day
	while (getline(ifs, str_line_buf)) {
		std::istringstream i_stream(str_line_buf);
		getline(i_stream, str_conma_buf, ',');
		name.push_back(str_conma_buf);
		while (getline(i_stream, str_conma_buf, ',')) {
			// kibou書き込む
			std::istringstream i_stream(str_conma_buf);
			if (str_conma_buf.size() != 0) {
				if (str_conma_buf.find("+") == -1) {
					getline(i_stream, str_minus_buf, '-');
					kibou_start = std::stoi(str_minus_buf);
					getline(i_stream, str_minus_buf, '-');
					kibou_end = std::stoi(str_minus_buf);
					for (int i = 0; i < 24; i++) {
						if (i >= kibou_start - 1 && i < kibou_end - 1) {
							kibou_temp_person.push_back(1);
						}
						else {
							kibou_temp_person.push_back(0);
						}
					}
					if (kibou_temp_person.size() != 0) {
						kibou_temp_day.push_back(kibou_temp_person);
						kibou_temp_person.clear();
					}

				}
			}
		}
		if (kibou_temp_day.size() >= member) {
			kibou.push_back(kibou_temp_day);
		}
	}//end reading kibou

	std::vector<std::vector<std::vector<int>>> shift_kettei;
	std::vector<std::vector<int>> shift_kettei_day;
	for (int i = 0; i < days; i++) {
		for (int j = 0; j < member; j++) {
			shift_kettei_day.push_back({ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 });
		}
		shift_kettei.push_back(shift_kettei_day);
	}
	std::vector<std::vector<int>> shift_assigned;
	for (int i = 0; i < days; i++) {
		shift_assigned.push_back({ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 });
	}

	int shift_needed[2/*days*/][24] = { { 0,0,0,0,0,0,0,0,3,3,4,4,4,2,2,2,2,4,4,4,4,0,0,0 }, { 0,0,0,0,0,0,0,0,3,3,4,4,4,2,2,2,2,4,4,4,4,0,0,0 } };
	//make shift
	int start, count;
	for (int i = 0; i < days; i++) {
		for (int j = 0; j < 24; j++) {
			start = rand() % member;
			count = 0;
			while (shift_assigned[i][j] < shift_needed[i][j]) {
				if (kibou[i][start][j] == 1) {
					shift_kettei[i][start][j] = 1;
					shift_assigned[i][j]++;
					for (int k = 1; k <= shift_time; k++) {//なるべく連続になるよう調整
						if (j + k >= 24) {
							break;
						}
						if (kibou[i][start][j + k] == 1 && shift_assigned[i][j + k] < shift_needed[i][j + k]) {
							shift_kettei[i][start][j + k] = 1;
							shift_assigned[i][j + k]++;
						}
						else {
							break;
						}
					}
				}
				if (start == member - 1) {
					start = 0;
				}
				else {
					start++;
				}
				count++;
				if (count > member) {
					break;
				}

			}
		}
	}
	//output
	putchar(' ');
	for (int i = 1; i <= 24; i++) {
		printf("%3d", i);
	}
	putchar('\n');
	for (int i = 0; i < days; i++) {
		for (int j = 0; j < member; j++) {
			std::cout << name[j];
			for (int k = 0; k < 24; k++) {
				printf("%3d", shift_kettei[i][j][k]);
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	bool is_first;
	for (int i = 0; i < days; i++) {
		for (int j = 0; j < member; j++) {
			std::cout << name[j];
			is_first = 1;
			for (int k = 0; k < 24; k++) {
				if (shift_kettei[i][j][k] == 1) {
					if (is_first == 0) {
						std::cout << "+";
					}
					std::cout << k+1 << "-";
					for (k; k < 24; k++) {
						if (shift_kettei[i][j][k] != 1) {
							std::cout << k + 1;
							is_first = 0;
							break;
						}
					}
				}
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	return 0;
}
