/*
 *	TEST CASE O1:
 *	FUNCTION: speedmeter
 *	PURPOSE:
 */
#define COUNTER_CALL_MAX 0xFFFFFFF
#define MAX_STEPS 	(10)
const can_msg step_data[MAX_STEPS] = {
		{(short)0x101, (char)8, {0,0,0,0,0,0,0,0,0,0}, 0 },
		{(short)0x101, (char)8, {0,0,0,0,0,0,0,0,0,0}, 0 },
		{(short)0x101, (char)8, {0,0,0,0,0,0,0,0,0,0}, 0 },
		{(short)0x101, (char)8, {0,0,0,0,0,0,0,0,0,0}, 0 },
		{(short)0x101, (char)8, {0,0,0,0,0,0,0,0,0,0}, 0 },
		{(short)0x101, (char)8, {0,0,0,0,0,0,0,0,0,0}, 0 },
		{(short)0x101, (char)8, {0,0,0,0,0,0,0,0,0,0}, 0 },
		{(short)0x101, (char)8, {0,0,0,0,0,0,0,0,0,0}, 0 },
		{(short)0x101, (char)8, {0,0,0,0,0,0,0,0,0,0}, 0 },
		{(short)0x101, (char)8, {0,0,0,0,0,0,0,0,0,0}, 0 },
};
