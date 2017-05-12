//custom_types.h

enum EnResults{
	T1Result = 10,
	T2Result,
	T3Result,
	L1Result = 20,
	L2Result,
	L3Result,
	SecurityDanger=29,
	FloodDanger=30
};

//typedef enum EnResults Results;

//Results ResultID;
typedef void(*fp)(unsigned char);

struct cmd_st{
	unsigned char set; 
	unsigned char value;
  fp function;
};

typedef struct cmd_st CommandStruct;

//EOF
