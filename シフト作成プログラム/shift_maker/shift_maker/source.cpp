#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#define shift_time 7//7時間で休憩か退勤

int main(void) {
	//データ保存用変数
	std::vector<std::string> name;//名前
	std::vector<std::vector<int>> shift_kettei_day;//日ごとのシフト決定
	//std::vector<std::vector<std::vector<bool>>> shift_kettei;//全体のシフト決定
	std::vector<std::vector<int>> shift_assigned;//現時点でのシフト割り当て人数
	std::vector<std::vector<int>> shift_needed;//時間、日ごとの必要人数


	//読み込み用変数
	std::vector<int> kibou_temp_person;//vector代入用変数
	std::vector<std::vector<int>> kibou_temp_day;//日ごとのデータ代入用変数
	std::string str_line_buf;//行単位のデータを保存
	std::string str_conma_buf;//コンマ区切りのデータを保存
	std::string str_minus_buf;//マイナス区切りのデータを保存
	int kibou_start, kibou_end;//希望の開始時間、終了時間を保存
	std::string input_path;
	int count_day = 0, count_person = 0;

	//ファイルのパスを読み込み
	std::cout << "ファイルのパスを入力してください。(\"\"で囲まない)" << std::endl;
	std::cin >> input_path;
	std::ifstream ifs(input_path);
	if (!ifs) {
		std::cout << "ファイルが見つかりません";
		return 1;
	}
	getline(ifs, str_line_buf);//1行目のデータは不要なためそのまま廃棄
	getline(ifs, str_line_buf);//2行目読み込み
	std::istringstream i_stream(str_line_buf);
	getline(i_stream, str_conma_buf, ',');
	const int member = std::stoi(str_conma_buf);//人数読み込み
	getline(i_stream, str_conma_buf, ',');
	const int days = std::stoi(str_conma_buf);//日数読み込み
	getline(ifs, str_line_buf);//3行目のデータは現在は不要なため廃棄

	//初期化
	std::vector<std::vector<std::vector<int>>> shift_kettei(days, std::vector<std::vector<int>>(member, std::vector<int>(24, 0)));
	std::vector<std::vector<std::vector<int>>> kibou(days, std::vector<std::vector<int>>(member, std::vector<int>(24, 0)));
	
	for (int i = 0; i < days; i++) {
		shift_assigned.push_back({ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 });
	}

	//仮で以下の値を代入
	for (int i = 0; i < days; i++) {
		shift_needed.push_back({ 0,0,0,0,0,0,0,0,3,3,4,4,4,2,2,2,2,4,4,4,4,0,0,0 });
	}

	//4行目以降名前とシフト希望読み込み
	while (getline(ifs, str_line_buf)) {
		std::istringstream i_stream(str_line_buf);
		getline(i_stream, str_conma_buf, ',');
		name.push_back(str_conma_buf);
		while (getline(i_stream, str_conma_buf, ',')) {
			std::istringstream i_stream(str_conma_buf);
			if (str_conma_buf.size() != 0) {
				if (str_conma_buf.find("+") == -1) {//複数時間希望した場合に対応するよう機能拡張用if　未実装
					getline(i_stream, str_minus_buf, '-');
					kibou_start = std::stoi(str_minus_buf);
					getline(i_stream, str_minus_buf, '-');
					kibou_end = std::stoi(str_minus_buf);
					for (int i = 0; i < 24; i++) {
						if (i >= kibou_start - 1 && i < kibou_end - 1) {
							kibou_temp_person.push_back(1);
							kibou[count_day][count_person][i] = 1;
						}
						else {
							kibou_temp_person.push_back(0);
							kibou[count_day][count_person][i] = 0;

						}
					}
					if (kibou_temp_person.size() != 0) {
						kibou_temp_day.push_back(kibou_temp_person);
						kibou_temp_person.clear();
						count_day++;
						if (count_day >= days) {
							break;
						}
					}

				}
			}
		}
		if (count_day >= days) {
			kibou.push_back(kibou_temp_day);
			kibou_temp_day.clear();
			count_person++;
			count_day = 0;
		}
	}
	
	//シフト作成開始
	int start, count;
	for (int i = 0; i < days; i++) {
		for (int j = 0; j < 24; j++) {
			start = rand() % member;//実際に使うときはrandの初期化が必要
			count = 0;
			while (shift_assigned[i][j] < shift_needed[i][j]&& count < member) {
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
							kibou[i][start][j + k] = 0;
						}
						else {
							break;
						}
						if (k == shift_time) {
							kibou[i][start][j + k + 1] = 0;
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

			}
		}
	}
	//出力確認
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
	system("PAUSE");
	return 0;
}