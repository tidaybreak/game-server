/*
 * Utility_Func.cpp
 *
 *  Created on: Oct 16, 2012
 *      Author: "Ti"
 */

#include "Utility_Func.h"
#include "Record_Client.h"
#include "custom_define.h"
#include <openssl/md5.h>

void Utility_Func::get_no_repeat_uint(const uint32_t max_size, const uint32_t generate_nums, UInt_Vec &vec) {
	vec.clear();
	if (max_size < 1 || generate_nums > max_size) {
		MSG_USER_TRACE("param error.");
		return;
	}

	UInt_Vec nums_seq;
	nums_seq.resize(max_size);
	for (uint32_t i = 0; i < max_size; ++i) {
		nums_seq[i] = i;
	}

	uint32_t last_index = nums_seq.size() - 1;
	for(uint32_t i = 0; i < generate_nums; ++i)
	{
		uint32_t random_index = rand() % (max_size - i);
		vec.push_back(nums_seq[random_index]);
		nums_seq[random_index] = nums_seq[last_index];
		--last_index;
	}
}

/// 返回大于1.5 * num的第一个素数
size_t Utility_Func::get_hash_table_size(unsigned int num) {
	size_t return_num = 1.5 * num, i, j;
	while (1) {
		/// 素数判断
		j = (size_t)sqrt(return_num++);
		for (i = 2; i < j; ++i) {
			if (return_num % i == 0)
				break;
		}
		if (i == j)
			return return_num;
	}
	return 0;
}

int Utility_Func::lottery_one(const Int_Vec &weight) {
	if (weight.empty()) {
		return 0;
	}
	int total_weight = 0;
	int size = weight.size();
	for (int i = 0; i < size; ++i) {
		total_weight += weight[i];
	}

	if (total_weight == 0) {
		return random() % weight.size();
	}

	int random_nums = random() % total_weight;
	int weight_counter = 0;

	int i = 0;
	for (; i < size; ++i) {
		weight_counter += weight[i];
		if (random_nums < weight_counter) {
			break;
		}
	}
	return i;
}

int Utility_Func::lottery_one_element(const Int_Vec &int_vec) {
	uint32_t size = int_vec.size();
	if (size == 0) {
		return 0;
	}
	uint32_t index = random() % size;
	return int_vec[index];
}

double Utility_Func::lottery_one_element(const Double_Vec &double_vec) {
	uint32_t size = double_vec.size();
	if (size == 0) {
		return 0.0;
	}
	uint32_t index = random() % size;
	return double_vec[index];
}

int Utility_Func::validate_md5(const char *key, const char *account, const char *time, const char *flag) {
    //static const char *key = "123!@#qwe";

    char mine_src[256 + 1], mine_md5[256 + 1];
    memset(mine_src, 0x00, 256 + 1);
    memset(mine_md5, 0x00, 256 + 1);

    snprintf(mine_src, sizeof(mine_src), "%s%s%s", account, time, key);
    const unsigned char *tmp_md5 = MD5((const unsigned char *) mine_src, strlen(mine_src), 0);

    for (uint i = 0; i < 16; i++) {
        sprintf(&mine_md5[i * 2], "%.2x", tmp_md5[i]);
    }

    return strncmp(flag, mine_md5, strlen(flag));
}

/*
 * 按权重值取随机值 wight = [权值1,权值2,权值3,权值n] 权值>=0
 * 成功返回wight下标,失败返回-1
 */
int Utility_Func::get_rand_index(const Int_Vec &wight) {
	if (wight.empty())
		return -1;

	size_t wight_size = wight.size();
	int sum = 0;
	for (size_t i = 0; i < wight_size; ++i) {
		if (wight[i] < 0) {
			return -1;
		}
		sum += wight[i];
	}
	if (sum == 0) {
		return -1;
	}

	int rand = random() % sum;
	for (size_t i = 0; i < wight_size; ++i) {
		rand -= wight[i];
		if (rand < 0) {
			return i;
		}
	}

	return -1;
}

/*
 * 按权重值取随机值 wight = [权值1,权值2,权值3,权值n] 权值>=0
 * 成功返回wight下标,失败返回-1
 */
int Utility_Func::get_rand_index(const Json::Value &wight) {
	if (wight.empty())
		return -1;

	size_t wight_size = wight.size();
	int sum = 0;
	for (size_t i = 0; i < wight_size; ++i) {
		if (! wight[i].isDouble()) {
			return -1;
		}
		sum += wight[i].asDouble() * 10000;
	}
	if (sum == 0) {
		return -1;
	}

	int rand = random() % sum;
	for (size_t i = 0; i < wight_size; ++i) {
		rand -= wight[i].asDouble() * 10000;
		if (rand < 0) {
			return i;
		}
	}

	return -1;
}

int Utility_Func::get_rand_min_max(const int min, const int max){
	return random()%(max-min+1)+min;
}


