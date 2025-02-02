struct numbers{

	int a;
	int b;
};

program CALC_PROG{
    version CALC_VERS{

        /*  function go in here sames as C Lanaguage */
        double add(numbers) = 1;
        double sub(numbers) = 2;
        double multiply(numbers) = 3;
        double divide(numbers) = 4;


    } = 1;
} = 0x1234567;
