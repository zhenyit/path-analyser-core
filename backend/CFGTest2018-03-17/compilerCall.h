#ifndef AUTO_SOFTWARE_TEST_COMPILER_CALL_H_
#define AUTO_SOFTWARE_TEST_COMPILER_CALL_H_


#include<string>
class CompilerCall
{
private:
	std::string m_sFileName;
public:
	CompilerCall();
	CompilerCall(std::string file);
	~CompilerCall();

	void call();
};
#endif // !AUTO_SOFTWARE_TEST_COMPILER_CALL_H_
