// coin.cpp : 定义 DLL 应用程序的导出函数。
//
//#define DLL_EXPORTS
#include "stdafx.h"
#include "coin.h"
#include <omp.h>
#include <queue>
#include <vector>
#define MAX(a,b) (a>=b?a:b)
#define MIN(a,b) (a<=b?a:b)

using namespace std;

void list(vector<int> sta, vector<int> cha, int val) {
	queue<int> que;
	int left, right, mid, tmp[2];
	int i, j;
	que.push(0);
	que.push(val);

	//非递归快排
	while (que.size() > 0) {
		left = que.front();
		que.pop();
		right = que.front();
		que.pop();
		if (left >= right - 1) { continue; }	//子数组长度为零，无须排序
		mid = (left + right) / 2;
		if (sta[mid] > MIN(sta[left], sta[right - 1]) && sta[mid] < MAX(sta[left], sta[right - 1])) {
			tmp[0] = sta[mid];
			tmp[1] = cha[mid];
			sta[mid] = sta[right - 1];
			cha[mid] = cha[right - 1];
			sta[right - 1] = tmp[0];
			cha[right - 1] = tmp[1];
		}
		else {
			tmp[0] = sta[right - 1];
			tmp[1] = cha[right - 1];
		}
		for (i = left, j = right - 1; i < j; i++) {
			if (sta[i] <= tmp[0]) { continue; }
			else {
				sta[j] = sta[i];
				cha[j] = cha[i];
				for (j--; i < j; j--) {
					if (sta[j] > tmp[0]) { continue; }
					else {
						sta[i] = sta[j];
						cha[i] = cha[j];
						i++;
						break;
					}
				}
			}
		}
		sta[i] = tmp[0];
		cha[i] = tmp[1];
		que.push(left);
		que.push(i);
		que.push(i + 1);
		que.push(right);
	}
}

void fdelay(vector<int> stamp, vector<int> channel, int value, int delay[8]) {
	int i, t;
	#pragma omp parallel private(t)
	{
		#pragma omp for
		for (i = 0; i < value; i++) {
			t = delay[channel[i]];
			stamp[i] += t;
		}
	}
}

static int coin[6] = { 0 };

int* ffcoin(int *stamp, int *channel, int value, int delay[8], int bin) {
	if (value <= 1) { exit(0); }
	const int v = value;
	vector<int> sta(value);
	vector<int> cha(value);
	int i, j;

	//拷贝实验数据为vector类型
	#pragma omp parallel for num_threads(4)
	{
		for (i = 0; i < value; i++) {
			if (channel[i] >= 8 || channel[i] < 0) { exit(0); }
			sta[i] = stamp[i];
			cha[i] = channel[i];
		}
	}

	//对应上面两个函数
	fdelay(sta, cha, value, delay);
	list(sta, cha, value);

	//统计各个通道间计数
	int encode = 0;
	for (i = 0; i < value-1; i++) {
		for (j = i + 1; j < value; j++) {
			if ((sta[j] - sta[i]) <= bin) {
				encode = cha[i] * cha[i] + cha[j] * cha[j];
				switch (encode){
				case 1:	//0, 1
					coin[0]++; break;
				case 4:	//0, 2
					coin[1]++; break;
				case 9:	//0, 3
					coin[2]++; break;
				case 5:	//1, 2
					coin[3]++; break;
				case 10://1, 3
					coin[4]++; break;
				case 13://2, 3
					coin[5]++; break;
				default:
					break;
				}
			}
		}
		//cha[i] = 255;
	}
	return coin;
}




