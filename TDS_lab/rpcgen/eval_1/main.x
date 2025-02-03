program STRING_OPS_PROG {
    version STRING_OPS_VERS {
        string REVERSE_STRING(string) = 1;
        string TO_UPPERCASE(string) = 2;
        bool IS_PALINDROME(string) = 3;
    } = 1;
} = 0x20000001;
