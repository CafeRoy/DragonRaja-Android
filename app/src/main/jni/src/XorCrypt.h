#pragma once
#include <random>

class XorCrypt {
private:
	std::int32_t value;
	std::int32_t key;	
	std::int32_t checksum;
	// 移除 static constexpr std::int32_t MIX_CONSTANT = 0xDEADBEEF;
	std::int32_t salt;	// 【強化點 1】新增：動態鹽值 (Salt)
	bool init_falg;
public:
	// 函式簽名保持不變！
	XorCrypt(); 
	void PutEncrypted(int subj);
	int GetDecrypted();
	void Init(); 
private:
	void UpdateChecksum();
	static std::int32_t GenerateRandomKey();
};