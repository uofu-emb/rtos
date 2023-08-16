void map(?)
{
}

int add5(int x)
{
    return x + 5;
}

void example(void)
{
    int input[5] = {0, 1, 2, 3};
    int output[5];
    map(&input, &output, 4, add5);
}
