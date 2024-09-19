#include <iostream>
#include <iomanip>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main(int argc, const char * argv[]) {

	json question = {
		{"question",{
						{
							{"id",0},
							{"choice_type","single"},
							{"grading",{
										   {"max_grade",10},
										   {"half_grade",5}
									   }},
							{"standard_answer",{1}},
							{"description","this is a test for choice judger"},
							{"choice",{
										  {
											  {"id",0},
											  {"description","nope"}
										  },
										  {
											  {"id",1},
											  {"description","B"}
										  },
										  {
											  {"id",2},
											  {"descrition","C"}
										  },
										  {
											  {"id",3},
											  {"description","D"}
										  }
									  }
							}
						},
						{
							{"id",1},
							{"choice_type","double"},
							{"grading",{
										   {"max_grade",10},
										   {"half_grade",5}
									   }},
							{"standard_answer",{1, 2}},
							{"description","this is a test for choice judger"},
							{"choice",{
										  {
											  {"id",0},
											  {"description","nope"}
										  },
										  {
											  {"id",1},
											  {"description","B"}
										  },
										  {
											  {"id",2},
											  {"descrition","C"}
										  },
										  {
											  {"id",3},
											  {"description","D"}
										  }
									  }
							}
						},
						{
							{"id",2},
							{"choice_type","multi"},
							{"grading",{
										   {"max_grade",10},
										   {"half_grade",5}
									   }},
							{"standard_answer",{1,2}},
							{"description","this is a test for choice judger"},
							{"choice",{
										  {
											  {"id",0},
											  {"description","nope"}
										  },
										  {
											  {"id",1},
											  {"description","B"}
										  },
										  {
											  {"id",2},
											  {"descrition","C"}
										  },
										  {
											  {"id",3},
											  {"description","D"}
										  }
									  }
							}
						}
					}
		},
		{"description","this is choice questions!"}
	};
	json answer = {
		{
			{"question_id",1},
			{"choice_id",{1}}
		},
		{
			{"question_id",0},
			{"choice_id",{1}}
		},
		{
			{"question_id",2},
			{"choice_id",{1,2,3}}
		}
	};

	std::cout << std::setw(4) << question << std::endl;
	std::cout << std::setw(4) << answer << std::endl;

	return 0;
}

