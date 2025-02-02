struct ArrayData{
	int arraySize;
	int array[100];
};

program AVG_PROG{
    version AVG_VERS{
        int calAvg(ArrayData) = 1;
    } = 1;
} = 0x4562877;
